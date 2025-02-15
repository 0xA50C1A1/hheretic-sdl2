// i_sound.c

#include "h2stdinc.h"
#include <math.h>	/* pow() */
#include <pthread.h>
#include "doomdef.h"
#include "sounds.h"
#include "i_sound.h"
#include "audio_plugin.h"


#define SAMPLE_FORMAT	FMT_S16
#define SAMPLE_ZERO	0
#define SAMPLE_RATE	11025	/* Hz */
#define SAMPLE_CHANNELS	2

#if 0
#define SAMPLE_TYPE	char
#else
#define SAMPLE_TYPE	short
#endif


/*
 *	SOUND HEADER & DATA
 */

int snd_Channels;
int snd_MaxVolume,		/* maximum volume for sound */
	snd_MusicVolume;	/* maximum volume for music */
boolean snd_MusicAvail,		/* whether music is available */
	snd_SfxAvail;		/* whether sfx are available. */

/*
 *	SOUND FX API
 */

typedef struct
{
	unsigned char	*begin;		/* pointers into Sample.firstSample */
	unsigned char	*end;

	SAMPLE_TYPE	*lvol_table;	/* point into vol_lookup */
	SAMPLE_TYPE	*rvol_table;

	unsigned int	pitch_step;
	unsigned int	step_remainder;	/* 0.16 bit remainder of last step. */

	int		pri;
	unsigned int	time;
} Channel;

typedef struct
{
/* Sample data is a lump from a wad: byteswap the a, freq
 * and the length fields before using them		*/
	short		a;		/* always 3	*/
	short		freq;		/* always 11025	*/
	int32_t		length;		/* sample length */
	unsigned char	firstSample;
} Sample;
COMPILE_TIME_ASSERT(Sample, offsetof(Sample,firstSample) == 8);


extern OutputPlugin *get_oplugin_info();
static OutputPlugin *audioPI = NULL;
static int	audio_exit_thread = 1;
static pthread_t	audio_thread;


#define CHAN_COUNT	8
static Channel	channel[CHAN_COUNT];

#define MAX_VOL		64	/* 64 keeps our table down to 16Kb */
static SAMPLE_TYPE	vol_lookup[MAX_VOL * 256];

static int	steptable[256];		/* Pitch to stepping lookup */

#define BUF_LEN		(256 * 2)


static void *audio_loop (void *arg)
{
	Channel* chan;
	Channel* cend;
	char buf[BUF_LEN];
	SAMPLE_TYPE *begin;
	SAMPLE_TYPE *end;
	unsigned int sample;
	register int dl;
	register int dr;

	end = (SAMPLE_TYPE *) (buf + BUF_LEN);
	cend = channel + CHAN_COUNT;

    while (! audio_exit_thread) {

	begin = (SAMPLE_TYPE *) buf;
	while (begin < end)
	{
	// Mix all the channels together.
		dl = SAMPLE_ZERO;
		dr = SAMPLE_ZERO;

		chan = channel;
		for ( ; chan < cend; chan++)
		{
			// Check channel, if active.
			if (chan->begin)
			{
				// Get the sample from the channel.
				sample = *chan->begin;

				// Adjust volume accordingly.
				dl += chan->lvol_table[sample];
				dr += chan->rvol_table[sample];

				// Increment sample pointer with pitch adjustment.
				chan->step_remainder += chan->pitch_step;
				chan->begin += chan->step_remainder >> 16;
				chan->step_remainder &= 65535;

				// Check whether we are done.
				if (chan->begin >= chan->end)
				{
					chan->begin = NULL;
				//	printf ("  channel done %d\n", chan);
				}
			}
		}

#if 0	/* SAMPLE_FORMAT */
		if (dl > 127)
			dl = 127;
		else if (dl < -128)
			dl = -128;
		if (dr > 127)
			dr = 127;
		else if (dr < -128)
			dr = -128;
#else
		if (dl > 0x7fff)
			dl = 0x7fff;
		else if (dl < -0x8000)
			dl = -0x8000;
		if (dr > 0x7fff)
			dr = 0x7fff;
		else if (dr < -0x8000)
			dr = -0x8000;
#endif

		*begin++ = dl;
		*begin++ = dr;
	}

	// This write is expected to block.
	audioPI->write_audio(buf, BUF_LEN);

    } /* end of the while(!audio_exit_thread) loop. */

	pthread_exit(NULL);
}


void I_SetSfxVolume(int volume)
{
}

// Gets lump nums of the named sound.  Returns pointer which will be
// passed to I_StartSound() when you want to start an SFX.  Must be
// sure to pass this to UngetSoundEffect() so that they can be
// freed!

int I_GetSfxLumpNum(sfxinfo_t *sound)
{
	if (sound->name[0] == 0)
		return 0;
	if (sound->link)
		sound = sound->link;
	return W_GetNumForName(sound->name);
}


// Id is unused.
// Data is a pointer to a Sample structure.
// Volume ranges from 0 to 127.
// Separation (orientation/stereo) ranges from 0 to 255.  128 is balanced.
// Pitch ranges from 0 to 255.  Normal is 128.
// Priority looks to be unused (always 0).

int I_StartSound(int id, void *data, int vol, int sep, int pitch, int priority)
{
	// Relative time order to find oldest sound.
	static unsigned int soundTime = 0;
	int chanId;
	Sample *sample;
	Channel *chan;
	int oldest;
	int i;

	// Find an empty channel, the oldest playing channel, or default to 0.
	// Currently ignoring priority.

	chanId = 0;
	oldest = soundTime;
	for (i = 0; i < CHAN_COUNT; i++)
	{
		if (! channel[ i ].begin)
		{
			chanId = i;
			break;
		}
		if (channel[ i ].time < oldest)
		{
			chanId = i;
			oldest = channel[ i ].time;
		}
	}

	sample = (Sample *) data;
	chan = &channel[chanId];

	I_UpdateSoundParams(chanId + 1, vol, sep, pitch);

	// begin must be set last because the audio thread will access the channel
	// once it is non-zero.  Perhaps this should be protected by a mutex.
	chan->pri = priority;
	chan->time = soundTime;
	chan->end = &sample->firstSample + LONG(sample->length);
	chan->begin = &sample->firstSample;

	soundTime++;

#if 0
	printf ("I_StartSound %d: v:%d s:%d p:%d pri:%d | %d %d %d %d\n",
		id, vol, sep, pitch, priority,
		chanId, chan->pitch_step, SHORT(sample->a), SHORT(sample->freq));
#endif

	return chanId + 1;
}

void I_StopSound(int handle)
{
	handle--;
	handle &= 7;
	channel[handle].begin = NULL;
}

int I_SoundIsPlaying(int handle)
{
	handle--;
	handle &= 7;
	return (channel[ handle ].begin != NULL);
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	int lvol, rvol;
	Channel *chan;

	// Set left/right channel volume based on seperation.
	sep += 1;	// range 1 - 256
	lvol = vol - ((vol * sep * sep) >> 16);	// (256*256);
	sep = sep - 257;
	rvol = vol - ((vol * sep * sep) >> 16);

	// Sanity check, clamp volume.
	if (rvol < 0)
	{
	//	printf ("rvol out of bounds %d, id %d\n", rvol, handle);
		rvol = 0;
	}
	else if (rvol > 127)
	{
	//	printf ("rvol out of bounds %d, id %d\n", rvol, handle);
		rvol = 127;
	}

	if (lvol < 0)
	{
	//	printf ("lvol out of bounds %d, id %d\n", lvol, handle);
		lvol = 0;
	}
	else if (lvol > 127)
	{
	//	printf ("lvol out of bounds %d, id %d\n", lvol, handle);
		lvol = 127;
	}

	// Limit to MAX_VOL (64)
	lvol >>= 1;
	rvol >>= 1;

	handle--;
	handle &= 7;
	chan = &channel[handle];
	chan->pitch_step = steptable[pitch];
	chan->step_remainder = 0;
	chan->lvol_table = &vol_lookup[lvol * 256];
	chan->rvol_table = &vol_lookup[rvol * 256];
}


/*
 *	SOUND STARTUP STUFF
 */

// inits all sound stuff
void I_StartupSound (void)
{
	snd_SfxAvail = false;

	if (M_CheckParm("--nosound") || M_CheckParm("-s") || M_CheckParm("-nosound"))
	{
		printf("I_StartupSound: Sound Disabled.\n");
		return;
	}

	/* Using get_oplugin_info() from oss.c.  In the future this could
	   load from a real shared library plugin. */
	audioPI = get_oplugin_info();
	if (!audioPI) return;
	fprintf(stdout, "%s\n", audioPI->about());

	audioPI->init();
	if (audioPI->open_audio(SAMPLE_FORMAT, SAMPLE_RATE, SAMPLE_CHANNELS) < 0)
	{
		fprintf (stderr, "I_StartupSound: failed\n");
	}
	else
	{
		audio_exit_thread = 0;
		pthread_create(&audio_thread, NULL, audio_loop, NULL);
		fprintf (stdout, "I_StartupSound: success\n");
		snd_SfxAvail = true;
	}
}

// shuts down all sound stuff
void I_ShutdownSound (void)
{
	snd_SfxAvail = false;
	if (audioPI)
	{
		if (!audio_exit_thread)
		{
			audio_exit_thread = 1;
			pthread_join(audio_thread, NULL);
		}
		audioPI->close_audio();
		audioPI = NULL;
	}
}

void I_SetChannels(int channels)
{
	int v, j;
	int *steptablemid;

	// We always have CHAN_COUNT channels.

	for (j = 0; j < CHAN_COUNT; j++)
	{
		channel[j].begin = NULL;
		channel[j].end   = NULL;
		channel[j].time = 0;
	}

	// This table provides step widths for pitch parameters.
	steptablemid = steptable + 128;
	for (j = -128; j < 128; j++)
	{
		steptablemid[j] = (int) (pow(2.0, (j/64.0)) * 65536.0);
	}

	// Generate the volume lookup tables.
	for (v = 0; v < MAX_VOL; v++)
	{
		// Turn the unsigned samples into signed samples.
		for (j = 0; j < 256; j++)
		{
			#if 0	/* SAMPLE_FORMAT */
			vol_lookup[v*256+j] = (v * (j-128)) / (MAX_VOL-1);
			#else
			vol_lookup[v*256+j] = (v * (j-128) * 256) / (MAX_VOL-1);
			#endif
			//printf ("vol_lookup[%d*256+%d] = %d\n", v, j, vol_lookup[v*256+j]);
		}
	}
}


/*
 *	SONG API
 */

int I_RegisterSong(void *data, int siz)
{
	return 0;
}

int I_RegisterExternalSong(const char *nm)
{
	return 0;
}

void I_UnRegisterSong(int handle)
{
}

void I_PauseSong(int handle)
{
}

void I_ResumeSong(int handle)
{
}

void I_SetMusicVolume(int volume)
{
}

int I_QrySongPlaying(int handle)
{
	return 0;
}

// Stops a song.  MUST be called before I_UnregisterSong().
void I_StopSong(int handle)
{
}

void I_PlaySong(int handle, boolean looping)
{
}
