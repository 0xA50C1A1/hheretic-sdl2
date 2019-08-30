
// DoomDef.h
// $Revision$
// $Date$

#ifndef __DOOMDEF__
#define __DOOMDEF__

/* if rangecheck is undefined, most parameter
 * validation debugging code will not be compiled
 */
#ifndef NORANGECHECKING
#define RANGECHECK	1
#endif

#define __STRINGIFY(x) #x
#define STRINGIFY(x) __STRINGIFY(x)

#define VERSION			130
#define VERSION_TEXT		"v1.3"

#define VERSION_MAJ	0
#define VERSION_MIN	2
#define VERSION_PATCH	4
#define HHERETIC_VERSION	"v" STRINGIFY(VERSION_MAJ) "." STRINGIFY(VERSION_MIN) "." STRINGIFY(VERSION_PATCH)

#if defined(__linux)
#define VERSION_PLATFORM "Linux"
#elif defined (__FreeBSD__)
#define VERSION_PLATFORM "FreeBSD"
#elif defined (_WIN32)
#define VERSION_PLATFORM "Windows"
#else
#define VERSION_PLATFORM "Unknown"
#endif

/* compatibility definitions: */
#if defined(_WIN32) && !defined(F_OK)
/* values for the mode argument of access(). MS does not define them.
   these aren't in h2stdinc.h, because not all files include io.h or
   unistd.h */
#define	R_OK	4		/* Test for read permission.  */
#define	W_OK	2		/* Test for write permission.  */
#define	X_OK	1		/* Test for execute permission.  */
#define	F_OK	0		/* Test for existence.  */
#endif

/* max length of a filesystem pathname	*/
#define	MAX_OSPATH		256

#define	DATA_ENVVAR		"HERETIC_DATA"
#define	H_USERDIR		".hheretic"

/* path to the user directory with a trailing
 * directory separator character. initialized
 * to DUMMY_BASEPATH string, which is "./" or
 * empty string "", so that it will have no
 * effect when user directories are disabled.
 */
extern	const char		*basePath;
#define	DUMMY_BASEPATH		""

#define	SAVEGAMENAME		"hticsav"
#define	CONFIG_FILE_NAME	"heretic.cfg"

/* the directory that holds development maps */
/* for the -wart # # command. */
#define	DEVMAPDIR		"/heretic/data/"


#ifdef __WATCOMC__
#include <malloc.h>
#define strcasecmp strcmpi
#define strncasecmp strnicmp
#endif

/* all external data is defined here */
#include "doomdata.h"

/* all important printed strings */
#include "dstrings.h"

/* header generated by multigen utility */
#include "info.h"

extern	byte	*destview, *destscreen;	/* PC direct to screen pointers */

/* most key data are simple ascii (uppercased) */
#define	KEY_TAB			9
#define	KEY_ENTER		13
#define	KEY_ESCAPE		27
#define	KEY_SPACE		32	/* 0x20 */
#define	KEY_MINUS		45	/* 0x2D */

#define	KEY_FIVE		53	/* 0x35 */
#define	KEY_SIX			54	/* 0x36 */
#define	KEY_SEVEN		55	/* 0x37 */
#define	KEY_EIGHT		56	/* 0x38 */
#define	KEY_NINE		57	/* 0x39 */
#define	KEY_ZERO		48	/* 0x30 */

#define	KEY_EQUALS		61	/* 0x3D */

/* These added by S.A. */
#define KEY_LEFTBRACKET		91
#define KEY_RIGHTBRACKET	93
#define KEY_BACKQUOTE		96
#define KEY_QUOTEDBL		34
#define KEY_QUOTE		39
#define KEY_SEMICOLON		59
#define KEY_PERIOD		46
#define KEY_COMMA		44
#define KEY_SLASH		47

#define	KEY_BACKSLASH		92	/* 0x5C */

#define	KEY_BACKSPACE		127	/* 0x7F */

#define	KEY_UPARROW		128	/* 0x80 */
#define	KEY_DOWNARROW		129
#define	KEY_LEFTARROW		130
#define	KEY_RIGHTARROW		131

#define	KEY_ALT			132
#define	KEY_LALT		KEY_ALT
#define	KEY_RALT		KEY_ALT

#define	KEY_CTRL		133
#define	KEY_LCTRL		KEY_CTRL
#define	KEY_RCTRL		KEY_CTRL

#define	KEY_SHIFT		134
#define	KEY_LSHIFT		KEY_SHIFT
#define	KEY_RSHIFT		KEY_SHIFT

#define	KEY_F1			135
#define	KEY_F2			136
#define	KEY_F3			137
#define	KEY_F4			138
#define	KEY_F5			139
#define	KEY_F6			140
#define	KEY_F7			141
#define	KEY_F8			142
#define	KEY_F9			143
#define	KEY_F10			144
#define	KEY_F11			145
#define	KEY_F12			146
#define	KEY_INS			147
#define	KEY_DEL			148
#define	KEY_PGDN		149
#define	KEY_PGUP		150
#define	KEY_HOME		151
#define	KEY_END			152

#define KEY_PAUSE		255	/* 0xFF */

/* mouse buttons */
#define	KEY_MOUSE1		200	/* 0xC8 */
#define	KEY_MOUSE2		201	/* right mouse button			*/
#define	KEY_MOUSE3		202	/* middle mouse button			*/
#define	KEY_MWHEELUP		203	/* wheel-up as a virtual button		*/
#define	KEY_MWHEELDOWN		204	/* wheel-down as a virtual button	*/
#define	KEY_MOUSE4		205	/* thumb buttons			*/
#define	KEY_MOUSE5		206	/* thumb buttons			*/

/* joystick buttons */
#define	KEY_JOY1		207
#define	KEY_JOY2		208
#define	KEY_JOY3		209
#define	KEY_JOY4		210

/* aux keys, for multi-buttoned joysticks */
#define	KEY_AUX1		211
#define	KEY_AUX2		212
#define	KEY_AUX3		213
#define	KEY_AUX4		214
#define	KEY_AUX5		215
#define	KEY_AUX6		216
#define	KEY_AUX7		217
#define	KEY_AUX8		218
#define	KEY_AUX9		219
#define	KEY_AUX10		220
#define	KEY_AUX11		221
#define	KEY_AUX12		222
#define	KEY_AUX13		223
#define	KEY_AUX14		224
#define	KEY_AUX15		225
#define	KEY_AUX16		226
#define	KEY_AUX17		227
#define	KEY_AUX18		228
#define	KEY_AUX19		229
#define	KEY_AUX20		230
#define	KEY_AUX21		231
#define	KEY_AUX22		232
#define	KEY_AUX23		233
#define	KEY_AUX24		234
#define	KEY_AUX25		235
#define	KEY_AUX26		236
#define	KEY_AUX27		237
#define	KEY_AUX28		238
#define	KEY_AUX29		239
#define	KEY_AUX30		240
#define	KEY_AUX31		241
#define	KEY_AUX32		242

#define	MAXKEYS				256


#define	FINEANGLES		8192
#define	FINEMASK		(FINEANGLES - 1)
#define	ANGLETOFINESHIFT	19	/* 0x100000000 to 0x2000 */

/*
===============================================================================

						GLOBAL TYPES

===============================================================================
*/

#define NUMARTIFCTS	28
#define MAXPLAYERS	4
#define TICRATE		35		/* number of tics / second */
#define TICSPERSEC	35

#define ANGLE_1		0x01000000
#define ANGLE_45	0x20000000
#define ANGLE_90	0x40000000
#define ANGLE_180	0x80000000
#define ANGLE_MAX	0xffffffff

#define	ANG45		0x20000000
#define	ANG90		0x40000000
#define	ANG180		0x80000000
#define	ANG270		0xc0000000

typedef unsigned angle_t;

typedef enum
{
	sk_baby,
	sk_easy,
	sk_medium,
	sk_hard,
	sk_nightmare
} skill_t;

typedef enum
{
	ev_keydown,
	ev_keyup,
	ev_mouse,
	ev_joystick
} evtype_t;

typedef struct
{
	evtype_t	type;
	int		data1;		/* keys / mouse/joystick buttons */
	int		data2;		/* mouse/joystick x move */
	int		data3;		/* mouse/joystick y move */
} event_t;

typedef struct
{
	signed char	forwardmove;	/* *2048 for move */
	signed char	sidemove;	/* *2048 for move */
	short		angleturn;	/* <<16 for angle delta */
	short		consistancy;	/* checks for net game */
	byte		chatchar;
	byte		buttons;
	byte		lookfly;	/* look/fly up/down/centering */
	byte		arti;		/* artitype_t to use */
} ticcmd_t;

#define	BT_ATTACK		1
#define	BT_USE			2
#define	BT_CHANGE		4	/* if true, the next 3 bits hold weapon num */
#define	BT_WEAPONMASK	(8 + 16 + 32)
#define	BT_WEAPONSHIFT		3

#define BT_SPECIAL		128	/* game events, not really buttons */
#define	BTS_SAVEMASK	(4 + 8 + 16)
#define	BTS_SAVESHIFT		2
#define	BT_SPECIALMASK		3
#define	BTS_PAUSE		1	/* pause the game */
#define	BTS_SAVEGAME		2	/* save the game at each console */
/* savegame slot numbers occupy the second byte of buttons */

typedef enum
{
	GS_LEVEL,
	GS_INTERMISSION,
	GS_FINALE,
	GS_DEMOSCREEN
} gamestate_t;

typedef enum
{
	ga_nothing,
	ga_loadlevel,
	ga_newgame,
	ga_loadgame,
	ga_savegame,
	ga_playdemo,
	ga_completed,
	ga_victory,
	ga_worlddone,
	ga_screenshot
} gameaction_t;

typedef enum
{
	wipe_0,
	wipe_1,
	wipe_2,
	wipe_3,
	wipe_4,
	NUMWIPES,
	wipe_random
} wipe_t;

typedef struct
{
	const char	*name;
	int	*location;
	int	defaultvalue;
	int	minvalue;
	int	maxvalue;
} default_t;

typedef struct
{
	const char	*name;
	char	*location;	/* pointer to an 80 char array, null terminated */
	char	*defaultvalue;	/* backup of the default value. malloc'ed at init */
} default_str_t;

/*
===============================================================================

							MAPOBJ DATA

===============================================================================
*/

/* think_t is a function pointer to a routine to handle an actor */
typedef void (*think_t) ();

typedef struct thinker_s
{
	struct thinker_s	*prev, *next;
	think_t		function;
} thinker_t;

struct player_s;

typedef struct mobj_s
{
	thinker_t		thinker;	/* thinker links */

	/* info for drawing */
	fixed_t			x, y, z;
	struct mobj_s	*snext, *sprev;		/* links in sector (if needed) */
	angle_t			angle;
	spritenum_t		sprite;		/* used to find patch_t and flip value */
	int			frame;		/* might be ord with FF_FULLBRIGHT */

	/* interaction info */
	struct mobj_s	*bnext, *bprev;		/* links in blocks (if needed) */
	struct subsector_s	*subsector;
	fixed_t			floorz, ceilingz;	/* closest together of contacted secs */
	fixed_t			radius, height;		/* for movement checking */
	fixed_t			momx, momy, momz;	/* momentums */
	int			validcount;	/* if == validcount, already checked */
	mobjtype_t		type;
	mobjinfo_t		*info;		/* &mobjinfo[mobj->type] */
	int			tics;		/* state tic counter */
	state_t			*state;
	int			damage;		/* For missiles */
	int			flags;
	int			flags2;		/* Heretic flags */

	/* be doubly careful with these two: they may be used to store
	 * a state or the address of an mobj_t or player_t structure!!!
	 */
	intptr_t		special1;	/* Special info */
	intptr_t		special2;	/* Special info */

	int			health;
	int			movedir;	/* 0-7 */
	int			movecount;	/* when 0, select a new dir */
	struct mobj_s		*target;	/* thing being chased/attacked (or NULL)
						   also the originator for missiles */
	int			reactiontime;	/* if non 0, don't attack yet
						   used by player to freeze a bit after
						   teleporting */
	int			threshold;	/* if > 0, the target will be chased
						   no matter what (even if shot) */
	struct player_s		*player;	/* only valid if type == MT_PLAYER */
	int			lastlook;	/* player number last looked for */

	mapthing_t		spawnpoint;	/* for nightmare respawn */
} mobj_t;

/* each sector has a degenmobj_t in it's center for sound origin purposes */
typedef struct
{
	thinker_t		thinker;	/* not used for anything. */
	fixed_t			x, y, z;
} degenmobj_t;

/* Most damage defined using HITDICE */
#define HITDICE(a)	((1 + (P_Random() & 7)) * (a))

/* frame flags */
#define	FF_FULLBRIGHT	0x8000		/* flag in thing->frame */
#define FF_FRAMEMASK	0x7fff

/* --- mobj.flags --- */
#define	MF_SPECIAL		1	/* call P_SpecialThing when touched */
#define	MF_SOLID		2
#define	MF_SHOOTABLE		4
#define	MF_NOSECTOR		8	/* don't use the sector links (invisible but touchable) */
#define	MF_NOBLOCKMAP		16	/* don't use the blocklinks (inert but displayable) */
#define	MF_AMBUSH		32
#define	MF_JUSTHIT		64	/* try to attack right back */
#define	MF_JUSTATTACKED		128	/* take at least one step before attacking */
#define	MF_SPAWNCEILING		256	/* hang from ceiling instead of floor */
#define	MF_NOGRAVITY		512	/* don't apply gravity every tic */

/* movement flags */
#define	MF_DROPOFF		0x400		/* allow jumps from high places */
#define	MF_PICKUP		0x800		/* for players to pick up items */
#define	MF_NOCLIP		0x1000		/* player cheat */
#define	MF_SLIDE		0x2000		/* keep info about sliding along walls */
#define	MF_FLOAT		0x4000		/* allow moves to any height, no gravity */
#define	MF_TELEPORT		0x8000		/* don't cross lines or look at heights */
#define MF_MISSILE		0x10000		/* don't hit same species, explode on block */

#define	MF_DROPPED		0x20000		/* dropped by a demon, not level spawned */
#define	MF_SHADOW		0x40000		/* use fuzzy draw (shadow demons / invis) */
#define	MF_NOBLOOD		0x80000		/* don't bleed when shot (use puff) */
#define	MF_CORPSE		0x100000	/* don't stop moving halfway off a step */
#define	MF_INFLOAT		0x200000	/* floating to a height for a move, don't */
							/* auto float to target's height. */

#define	MF_COUNTKILL		0x400000	/* count towards intermission kill total */
#define	MF_COUNTITEM		0x800000	/* count towards intermission item total */

#define	MF_SKULLFLY		0x1000000	/* skull in flight */
#define	MF_NOTDMATCH		0x2000000	/* don't spawn in death match (key cards) */

#define	MF_TRANSLATION		0xc000000	/* if 0x4 0x8 or 0xc, use a translation */

#define	MF_TRANSSHIFT		26		/* table for player colormaps */

/* --- mobj.flags2 --- */

#define MF2_LOGRAV		0x00000001	/* alternate gravity setting */
#define MF2_WINDTHRUST		0x00000002	/* gets pushed around by the wind specials */
#define MF2_FLOORBOUNCE		0x00000004	/* bounces off the floor */
#define MF2_THRUGHOST		0x00000008	/* missile will pass through ghosts */
#define MF2_FLY			0x00000010	/* fly mode is active */
#define MF2_FOOTCLIP		0x00000020	/* if feet are allowed to be clipped */
#define MF2_SPAWNFLOAT		0x00000040	/* spawn random float z */
#define MF2_NOTELEPORT		0x00000080	/* does not teleport */
#define MF2_RIP			0x00000100	/* missile rips through solid targets */
#define MF2_PUSHABLE		0x00000200	/* can be pushed by other moving mobjs */
#define MF2_SLIDE		0x00000400	/* slides against walls */
#define MF2_ONMOBJ		0x00000800	/* mobj is resting on top of another mobj */
#define MF2_PASSMOBJ		0x00001000	/* Enable z block checking.  If on, */
							/* this flag will allow the mobj to */
							/* pass over/under other mobjs. */
#define MF2_CANNOTPUSH		0x00002000	/* cannot push other pushable mobjs */
#define MF2_FEETARECLIPPED	0x00004000	/* a mobj's feet are now being cut */
#define MF2_BOSS		0x00008000	/* mobj is a major boss */
#define MF2_FIREDAMAGE		0x00010000	/* does fire damage */
#define MF2_NODMGTHRUST		0x00020000	/* does not thrust target when damaging */
#define MF2_TELESTOMP		0x00040000	/* mobj can stomp another */
#define MF2_FLOATBOB		0x00080000	/* use float bobbing z movement */
#define MF2_DONTDRAW		0X00100000	/* don't generate a vissprite */

/*============================================================================*/

typedef enum
{
	PST_LIVE,		/* playing */
	PST_DEAD,		/* dead on the ground */
	PST_REBORN		/* ready to restart */
} playerstate_t;

/* psprites are scaled shapes directly on the view screen
 * coordinates are given for a 320*200 view screen
 */
typedef enum
{
	ps_weapon,
	ps_flash,
	NUMPSPRITES
} psprnum_t;

typedef struct
{
	state_t	*state;		/* a NULL state means not active */
	int		tics;
	fixed_t	sx, sy;
} pspdef_t;

typedef enum
{
	key_yellow,
	key_green,
	key_blue,
	NUMKEYS
} keytype_t;

typedef enum
{
	wp_staff,
	wp_goldwand,
	wp_crossbow,
	wp_blaster,
	wp_skullrod,
	wp_phoenixrod,
	wp_mace,
	wp_gauntlets,
	wp_beak,
	NUMWEAPONS,
	wp_nochange
} weapontype_t;

#define	AMMO_GWND_WIMPY	10
#define	AMMO_GWND_HEFTY	50
#define	AMMO_CBOW_WIMPY	5
#define	AMMO_CBOW_HEFTY	20
#define	AMMO_BLSR_WIMPY	10
#define	AMMO_BLSR_HEFTY	25
#define	AMMO_SKRD_WIMPY	20
#define	AMMO_SKRD_HEFTY	100
#define	AMMO_PHRD_WIMPY	1
#define	AMMO_PHRD_HEFTY	10
#define	AMMO_MACE_WIMPY	20
#define	AMMO_MACE_HEFTY	100

typedef enum
{
	am_goldwand,
	am_crossbow,
	am_blaster,
	am_skullrod,
	am_phoenixrod,
	am_mace,
	NUMAMMO,
	am_noammo	/* staff, gauntlets */
} ammotype_t;

typedef struct
{
	ammotype_t	ammo;
	int	upstate;
	int	downstate;
	int	readystate;
	int	atkstate;
	int	holdatkstate;
	int	flashstate;
} weaponinfo_t;

extern	weaponinfo_t	wpnlev1info[NUMWEAPONS];
extern	weaponinfo_t	wpnlev2info[NUMWEAPONS];

typedef enum
{
	arti_none,
	arti_invulnerability,
	arti_invisibility,
	arti_health,
	arti_superhealth,
	arti_tomeofpower,
	arti_torch,
	arti_firebomb,
	arti_egg,
	arti_fly,
	arti_teleport,
	NUMARTIFACTS
} artitype_t;

typedef enum
{
	pw_None,
	pw_invulnerability,
	pw_invisibility,
	pw_allmap,
	pw_infrared,
	pw_weaponlevel2,
	pw_flight,
	pw_shield,
	pw_health2,
	NUMPOWERS
} powertype_t;

#define	INVULNTICS	(30 * 35)
#define	INVISTICS	(60 * 35)
#define	INFRATICS	(120* 35)
#define	IRONTICS	(60 * 35)
#define WPNLEV2TICS	(40 * 35)
#define FLIGHTTICS	(60 * 35)

#define CHICKENTICS	(40 * 35)

#define MESSAGETICS	( 4 * 35)
#define BLINKTHRESHOLD	( 4 * 32)

#define NUMINVENTORYSLOTS	14
typedef struct
{
	int	type;
	int	count;
} inventory_t;

/*
================
player_t
================
*/
typedef struct player_s
{
	mobj_t		*mo;
	playerstate_t	playerstate;
	ticcmd_t	cmd;

	fixed_t		viewz;			/* focal origin above r.z */
	fixed_t		viewheight;		/* base height above floor for viewz */
	fixed_t		deltaviewheight;	/* squat speed */
	fixed_t		bob;			/* bounded/scaled total momentum */

	int		flyheight;
	int		lookdir;
	boolean		centering;
	int		health;			/* only used between levels, mo->health */
								/* is used during levels */
	int		armorpoints, armortype;	/* armor type is 0-2 */

	inventory_t	inventory[NUMINVENTORYSLOTS];
	artitype_t	readyArtifact;
	int		artifactCount;
	int		inventorySlotNum;
	int		powers[NUMPOWERS];
	boolean		keys[NUMKEYS];
	boolean		backpack;
	signed int	frags[MAXPLAYERS];	/* kills of other players */
	weapontype_t	readyweapon;
	weapontype_t	pendingweapon;		/* wp_nochange if not changing */
	boolean		weaponowned[NUMWEAPONS];
	int		ammo[NUMAMMO];
	int		maxammo[NUMAMMO];
	int		attackdown, usedown;	/* true if button down last tic */
	int		cheats;			/* bit flags */

	int		refire;			/* refired shots are less accurate */

	int		killcount, itemcount, secretcount;	/* for intermission */
	const char	*message;		/* hint messages */
	int		messageTics;		/* counter for showing messages */
	int		damagecount, bonuscount;/* for screen flashing */
	int		flamecount;		/* for flame thrower duration */
	mobj_t		*attacker;		/* who did damage (NULL for floors) */
	int		extralight;		/* so gun flashes light up areas */
	int		fixedcolormap;		/* can be set to REDCOLORMAP, etc */
	int		colormap;		/* 0-3 for which color to draw player */
	pspdef_t	psprites[NUMPSPRITES];	/* view sprites (gun, etc) */
	boolean		didsecret;		/* true if secret level has been done */
	int		chickenTics;		/* player is a chicken if > 0 */
	int		chickenPeck;		/* chicken peck countdown */
	mobj_t		*rain1;			/* active rain maker 1 */
	mobj_t		*rain2;			/* active rain maker 2 */
} player_t;

#define CF_NOCLIP		1
#define	CF_GODMODE		2
#define	CF_NOMOMENTUM		4	/* not really a cheat, just a debug aid */


#define	BACKUPTICS		12	/* CHANGED FROM 12 !?!? */

typedef struct
{
	unsigned int	checksum;		/* high bit is retransmit request */
	byte		retransmitfrom;		/* only valid if NCMD_RETRANSMIT */
	byte		starttic;
	byte		player, numtics;
	ticcmd_t	cmds[BACKUPTICS];
} doomdata_t;

typedef struct
{
	int32_t		id;
	short		intnum;		/* DOOM executes an int to execute commands */

/* communication between DOOM and the driver */
	short		command;	/* CMD_SEND or CMD_GET */
	short		remotenode;	/* dest for send, set by get (-1 = no packet) */
	short		datalength;	/* bytes in doomdata to be sent */

/* info common to all nodes */
	short		numnodes;	/* console is allways node 0 */
	short		ticdup;		/* 1 = no duplication, 2-5 = dup for slow nets */
	short		extratics;	/* 1 = send a backup tic in every packet */
	short		deathmatch;	/* 1 = deathmatch */
	short		savegame;	/* -1 = new game, 0-5 = load savegame */
	short		episode;	/* 1-3 */
	short		map;		/* 1-9 */
	short		skill;		/* 1-5 */

/* info specific to this node */
	short		consoleplayer;
	short		numplayers;
	short		angleoffset;	/* 1 = left, 0 = center, -1 = right */
	short		drone;		/* 1 = drone */

/* packet data to be sent */
	doomdata_t	data;
} doomcom_t;

#define	DOOMCOM_ID		0x12345678l

extern	doomcom_t		*doomcom;
extern	doomdata_t		*netbuffer;	/* points inside doomcom */

#define	MAXNETNODES		8		/* max computers in a game */

#define	CMD_SEND		1
#define	CMD_GET			2

#define	SBARHEIGHT		42		/* status bar height at bottom of screen */


/*
===============================================================================

					GLOBAL VARIABLES

===============================================================================
*/

#define TELEFOGHEIGHT		(32 * FRACUNIT)

#define MAXEVENTS		64

extern	event_t		events[MAXEVENTS];
extern	int		eventhead;
extern	int		eventtail;

extern	fixed_t		finesine[5*FINEANGLES/4];
extern	fixed_t		*finecosine;

extern	gameaction_t	gameaction;
extern	boolean		paused;
extern	boolean		shareware;	/* true if main WAD is the shareware version */
extern	boolean		ExtendedWAD;	/* true if main WAD is the extended version */
extern	boolean		nomonsters;	/* checkparm of -nomonsters */
extern	boolean		respawnparm;	/* checkparm of -respawn */
extern	boolean		debugmode;	/* checkparm of -debug */
extern	boolean		usergame;	/* ok to save / end game */
extern	boolean		ravpic;		/* checkparm of -ravpic */
extern	boolean		deathmatch;	/* only if started as net death */
extern	boolean		netgame;	/* only true if >1 player */

extern	boolean		playeringame[MAXPLAYERS];
extern	int		consoleplayer;	/* player taking events and displaying */
extern	int		displayplayer;
extern	int		viewangleoffset;/* ANG90 = left side, ANG270 = right */
extern	player_t	players[MAXPLAYERS];

extern	boolean		singletics;	/* debug flag to cancel adaptiveness */
extern	boolean		DebugSound;	/* debug flag for displaying sound info */

extern	int		maxammo[NUMAMMO];

extern	boolean		demoplayback;
extern	int		skytexture;

extern	gamestate_t	gamestate;
extern	skill_t		gameskill;
extern	boolean		respawnmonsters;
extern	int		gameepisode;
extern	int		gamemap;
extern 	int 		prevmap;
extern	int		totalkills, totalitems, totalsecret;	/* for intermission */
extern	int		levelstarttic;	/* gametic at level start */
extern	int		leveltime;	/* tics in game play for par */

extern	int		ticcount;

extern	ticcmd_t	netcmds[MAXPLAYERS][BACKUPTICS];
extern	int		ticdup;

extern	ticcmd_t	localcmds[BACKUPTICS];
extern	int		rndindex;
extern	int		gametic, maketic;
extern	int		nettics[MAXNETNODES];

extern	mapthing_t	*deathmatch_p;
extern	mapthing_t	deathmatchstarts[10];
extern	mapthing_t	playerstarts[MAXPLAYERS];

extern	int		viewwindowx;
extern	int		viewwindowy;
extern	int		viewwidth;
extern	int		scaledviewwidth;
extern	int		viewheight;

extern	int		mouseSensitivity;

extern	boolean		precache;	/* if true, load all graphics at level load */

extern	byte		*screen;	/* off screen work buffer, from V_video.c */

extern	boolean		singledemo;	/* quit after playing a demo from cmdline */

extern	FILE		*debugfile;
extern	int		bodyqueslot;
extern	skill_t		startskill;
extern	int		startepisode;
extern	int		startmap;
extern	boolean		autostart;


/*
===============================================================================

					GLOBAL FUNCTIONS

===============================================================================
*/


fixed_t	FixedMul (fixed_t a, fixed_t b);
fixed_t	FixedDiv (fixed_t a, fixed_t b);
fixed_t	FixedDiv2 (fixed_t a, fixed_t b);

#undef	_HAVE_FIXED_ASM

#if !defined(_DISABLE_ASM)
#if defined(__i386__) || defined(__386__) || defined(_M_IX86)
#if defined(__WATCOMC__)

#define	_HAVE_FIXED_ASM			1

#pragma aux FixedMul =			\
	"imul ebx",			\
	"shrd eax,edx,16"		\
	parm	[eax] [ebx]		\
	value	[eax]			\
	modify exact [eax edx]

#pragma aux FixedDiv2 =			\
	"cdq",				\
	"shld edx,eax,16",		\
	"sal eax,16",			\
	"idiv ebx"			\
	parm	[eax] [ebx]		\
	value	[eax]			\
	modify exact [eax edx]

#elif defined(__GNUC__)

#define	_HAVE_FIXED_ASM			1

# if defined(_INLINE_FIXED_ASM)
# if (__GNUC__ == 2) && (__GNUC_MINOR__ <= 91)
# define FixedMul(fa,fb) ({ int __value, __fb = (fb);	\
	__asm__("imul %%ebx; shrd $16,%%edx,%%eax"	\
		: "=a" (__value)			\
		: "0" (fa), "b" (__fb)			\
		: "eax", "edx" ); __value; })

# define FixedDiv2(fa,fb) ({ int __value;		\
	__asm__("cdq; shld $16,%%eax,%%edx; sall $16,%%eax; idiv %%ebx"	\
		: "=a" (__value)			\
		: "0" (fa), "b" (fb)			\
		: "eax", "edx" ); __value; })

# else	/* GCC > 2.91.x */
# define FixedMul(fa,fb) ({ int __value, __fb = (fb);	\
	__asm__("imul %%ebx; shrd $16,%%edx,%%eax"	\
		: "=a" (__value)			\
		: "0" (fa), "b" (__fb)			\
		: "edx" ); __value; })

# define FixedDiv2(fa,fb) ({ int __value;		\
	__asm__("cdq; shld $16,%%eax,%%edx; sall $16,%%eax; idiv %%ebx"	\
		: "=a" (__value)			\
		: "0" (fa), "b" (fb)			\
		: "edx" ); __value; })

# endif	/* GCC/EGCS versions */

# endif	/* _INLINE_FIXED_ASM */
#endif
#endif	/* X86 */
#endif	/* !_DISABLE_ASM */

#define FIX2FLT(x)	((float)((x)>>FRACBITS) + (float)((x)&(FRACUNIT-1)) / (float)(FRACUNIT))
#define Q_FIX2FLT(x)	((float)((x)>>FRACBITS))


int16_t ShortSwap(int16_t) __attribute__((__const__));
int32_t LongSwap (int32_t) __attribute__((__const__));

#if defined(__GNUC__)
static inline __attribute__((__const__)) int16_t _H2_SWAP16(int16_t x)
{
	return (int16_t) (((uint16_t)x << 8) | ((uint16_t)x >> 8));
}
static inline __attribute__((__const__)) int32_t _H2_SWAP32(int32_t x)
{
	return (int32_t) (((uint32_t)x << 24) | ((uint32_t)x >> 24) |
			  (((uint32_t)x & (uint32_t)0x0000ff00UL) << 8) |
			  (((uint32_t)x & (uint32_t)0x00ff0000UL) >> 8));
}
#endif	/* GCC */

/*
#ifdef __BIG_ENDIAN__
*/
#ifdef WORDS_BIGENDIAN
# ifdef __GNUC__
#  define SHORT(a)	_H2_SWAP16((a))
#  define LONG(a)	_H2_SWAP32((a))
# else
#  define SHORT(x)	ShortSwap((x))
#  define LONG(x)	LongSwap((x))
# endif
#else
#define SHORT(x)	(x)
#define LONG(x)		(x)
#endif

/* ---- READ_INT16/32 --- */

#define READ_INT16(b)	((b)[0] | ((b)[1] << 8))
#define READ_INT32(b)	((b)[0] | ((b)[1] << 8) | ((b)[2] << 16) | ((b)[3] << 24))
#define INCR_INT16(b)	(b)+=2
#define INCR_INT32(b)	(b)+=4


/* ----- MEMORY ZONE ---- */

/* tags < 100 are not overwritten until freed */
#define	PU_STATIC		1		/* static entire execution time */
#define	PU_SOUND		2		/* static while playing */
#define	PU_MUSIC		3		/* static while playing */
#define	PU_DAVE			4		/* anything else Dave wants static */
#define	PU_LEVEL		50		/* static until level exited */
#define	PU_LEVSPEC		51		/* a special thinker in a level */
/* tags >= 100 are purgable whenever needed */
#define	PU_PURGELEVEL		100
#define	PU_CACHE		101

#define	ZONEID		0x1d4a11

void Z_Init (void);
void *Z_Malloc (int size, int tag, void *ptr);
void Z_Free (void *ptr);
void Z_FreeTags (int lowtag, int hightag);
void Z_CheckHeap (void);
void Z_ChangeTag2 (void *ptr, int tag);
void Z_DumpHeap (int lowtag, int hightag);
void Z_FileDumpHeap (FILE *f);
int Z_FreeMemory (void);

extern	boolean		MallocFailureOk;

typedef struct memblock_s
{
	int			size;		/* including the header and possibly tiny fragments */
	void			**user;		/* NULL if a free block */
	int			tag;		/* purgelevel */
	int			id;		/* should be ZONEID */
	struct memblock_s	*next, *prev;
} memblock_t;

#define Z_ChangeTag(p,t)							\
{										\
	if (((memblock_t *)((byte *)((p)) - sizeof(memblock_t)))->id != ZONEID)	\
		I_Error("Z_CT at %s:%i", __FILE__, __LINE__);			\
	Z_ChangeTag2((p),(t));							\
};

/* ------- WADFILE ------- */
typedef struct
{
	char		name[8];
	int		handle, position, size;
} lumpinfo_t;
COMPILE_TIME_ASSERT(lumpinfo_t, sizeof(lumpinfo_t) == 20);

extern	lumpinfo_t	*lumpinfo;
extern	int		numlumps;
extern	const char	*waddir;

boolean W_IsWadPresent(const char *filename);
void W_InitMultipleFiles(const char **filenames);
int W_CheckNumForName(const char *name);
int W_GetNumForName(const char *name);
int W_LumpLength(int lump);
void W_ReadLump(int lump, void *dest);
void *W_CacheLumpNum(int lump, int tag);
void *W_CacheLumpName(const char *name, int tag);


/* ---------- BASE LEVEL ---------- */

void D_DoomMain(void);
/* not a globally visible function, just included for source reference
 * calls all startup code
 * parses command line options
 * if not overrided, calls N_AdvanceDemo
 */

void IncThermo(void);
void InitThermo(int max);
void tprintf(const char *string, int initflag);

void D_DoomLoop(void);
/* not a globally visible function, just included for source reference
 * called by D_DoomMain, never exits
 * manages timing and IO
 * calls all ?_Responder, ?_Ticker, and ?_Drawer functions
 * calls I_GetTime, I_StartFrame, and I_StartTic
 */

void D_PostEvent(event_t *ev);
/* called by IO functions when input is detected */

void NetUpdate (void);
/* create any new ticcmds and broadcast to other players */

void D_QuitNetGame (void);
/* broadcasts special packets to other players to notify of game exit */

void TryRunTics (void);

#if !(defined(__WATCOMC__) || defined(__DJGPP__) || defined(__DOS__) || \
      defined(_WIN32) || defined(_WIN64))
char *strupr (char *str);
char *strlwr (char *str);
int filelength(int handle);
#endif


/* --------- SYSTEM IO --------- */

#if 1
#define	SCREENWIDTH	320
#define	SCREENHEIGHT	200
#else
#define	SCREENWIDTH	560
#define	SCREENHEIGHT	375
#endif

byte *I_ZoneBase (int *size);
/* called by startup code to get the ammount of memory to malloc
 * for the zone management
 */

int I_GetTime (void);
/* called by D_DoomLoop
 * returns current time in tics
 */

void I_StartFrame (void);
/* called by D_DoomLoop
 * called before processing any tics in a frame (just after displaying a frame)
 * time consuming syncronous operations are performed here (joystick reading)
 * can call D_PostEvent
 */

void I_StartTic (void);
/* called by D_DoomLoop
 * called before processing each tic in a frame
 * quick syncronous operations are performed here
 * can call D_PostEvent
 *
 * asyncronous interrupt functions should maintain private ques that are
 * read by the syncronous functions to be converted into events
 */

void I_Init (void);
/* called by D_DoomMain
 * determines the hardware configuration and sets up the video mode
 */

void I_InitGraphics (void);

void I_InitNetwork (void);
void I_NetCmd (void);

void I_Error (const char *error, ...) __attribute__((__format__(__printf__,1,2), __noreturn__));
/* called by anything that can generate a terminal error
 * bad exit with diagnostic message
 */

void I_Quit (void) __attribute__((__noreturn__));
/* called by M_Responder when quit is selected
 * clean exit, displays sell blurb
 */

void I_SetPalette (byte *palette);
/* takes full 8 bit values */

void I_Update(void);
/* Copy buffer to video */

void I_WipeUpdate(wipe_t wipe);
/* Copy buffer to video with wipe effect */

void I_WaitVBL(int count);
/* wait for vertical retrace or pause a bit */

void I_BeginRead (void);
void I_EndRead (void);

byte *I_AllocLow (int length);
/* allocates from low memory under dos, just mallocs under unix */

extern	boolean		useexterndriver;

#if defined(__WATCOMC__) && defined(_DOS)
#define EBT_FIRE		1
#define EBT_OPENDOOR		2
#define EBT_SPEED		4
#define EBT_STRAFE		8
#define EBT_MAP			0x10
#define EBT_INVENTORYLEFT	0x20
#define EBT_INVENTORYRIGHT	0x40
#define EBT_USEARTIFACT		0x80
#define EBT_FLYDROP		0x100
#define EBT_CENTERVIEW		0x200
#define EBT_PAUSE		0x400
#define EBT_WEAPONCYCLE		0x800

typedef struct
{
	short		vector;		/* Interrupt vector */

	signed char	moveForward;	/* forward/backward (maxes at 50) */
	signed char	moveSideways;	/* strafe (maxes at 24) */
	short		angleTurn;	/* turning speed (640 [slow] 1280 [fast]) */
	short		angleHead;	/* head angle (+2080 [left] : 0 [center] : -2048 [right]) */
	signed char	pitch;		/* look up/down (-110 : +90) */
	signed char	flyDirection;	/* flyheight (+1/-1) */
	unsigned short	buttons;	/* EBT_* flags */
} externdata_t;

void I_Tactile (int on, int off, int total);
#endif	/* externdriver, DOS */


/* ---- GAME ---- */

void G_DeathMatchSpawnPlayer (int playernum);

void G_InitNew (skill_t skill, int episode, int map);

void G_DeferedInitNew (skill_t skill, int episode, int map);
/* can be called by the startup code or M_Responder
 * a normal game starts at map 1, but a warp test can start elsewhere
 */

void G_DeferedPlayDemo (const char *demo);

void G_LoadGame (int slot);
/* can be called by the startup code or M_Responder
 * calls P_SetupLevel or W_EnterWorld
 */

void G_DoLoadGame (void);

void G_SaveGame (int slot, const char *description);
/* called by M_Responder */

void G_RecordDemo (skill_t skill, int numplayers, int episode,
		   int map, const char *name);
/* only called by startup code */

void G_PlayDemo (const char *name);
void G_TimeDemo (const char *name);

void G_ExitLevel (void);
void G_SecretExitLevel (void);

void G_WorldDone (void);

void G_Ticker (void);
boolean G_Responder (event_t *ev);

void G_ScreenShot (void);


/* ------- SV_SAVE ------- */

#define SAVEVERSIONSIZE		16
#define SAVESTRINGSIZE		24

void SV_SaveGame(int slot, const char *description);
void SV_LoadGame(int slot);


/* ----- PLAY ----- */

void P_Ticker (void);
/* called by C_Ticker
 * can call G_PlayerExited
 * carries out all thinking of monsters and players
 */

void P_SetupLevel (int episode, int map, int playermask, skill_t skill);
/* called by W_Ticker */

void P_Init (void);
/* called by startup code */


/* ------- REFRESH ------- */

extern	boolean		setsizeneeded;

extern	boolean		BorderNeedRefresh;
extern	boolean		BorderTopRefresh;

extern	int		UpdateState;

/* define the different areas for the dirty map */
#define I_NOUPDATE	0
#define I_FULLVIEW	1
#define I_STATBAR	2
#define I_MESSAGES	4
#define I_FULLSCRN	8

void R_RenderPlayerView (player_t *player);
/* called by G_Drawer */

void R_Init (void);
/* called by startup code */

void R_DrawViewBorder (void);
void R_DrawTopBorder (void);
/* if the view size is not full screen, draws a border around it */

void R_SetViewSize (int blocks, int detail);
/* called by M_Responder */

int R_FlatNumForName (const char *name);

int R_TextureNumForName (const char *name);
int R_CheckTextureNumForName (const char *name);
/* called by P_Ticker for switches and animations
 * returns the texture number for the texture name
 */


/* ---- MISC ---- */
extern	const char	**myargv;
extern	int		myargc;

int M_CheckParm(const char *check);
/* returns the position of the given parameter in the arg list (0 if not found) */

boolean M_ValidEpisodeMap(int episode, int map);
/* returns true if the episode/map combo is valid for the current
 * game configuration
 */

void M_ExtractFileBase(const char *path, char *dest);

void M_ForceUppercase(char *text);
/* Changes a string to uppercase */

int M_Random (void);
/* returns a number from 0 to 255 */

int P_Random (void);
/* as M_Random, but used only by the play simulation */

void M_ClearRandom (void);
/* fix randoms for demos */

void M_FindResponseFile(void);

void M_ClearBox (fixed_t *box);
void M_AddToBox (fixed_t *box, fixed_t x, fixed_t y);
/* bounding box functions */

boolean M_WriteFile(char const *name, const void *source, int length);
int M_ReadFile(char const *name, void **buffer);

void M_ScreenShot (void);

void M_LoadDefaults(const char *fileName);

void M_SaveDefaults (void);

int M_DrawText (int x, int y, boolean direct, const char *string);


/* ---- Interlude (IN_lude.c) ---- */

extern	boolean		intermission;

void IN_Start(void);
void IN_Ticker(void);
void IN_Drawer(void);

/* ---- Finale (F_finale.c) ------ */

void F_Drawer(void);
void F_Ticker(void);
void F_StartFinale(void);


/* ---- Chat mode (CT_chat.c) ---- */

void CT_Init(void);
void CT_Drawer(void);
boolean CT_Responder(event_t *ev);
void CT_Ticker(void);
char CT_dequeueChatChar(void);

extern	char		chat_macros[10][80];
extern	boolean		chatmodeon;
extern	boolean		ultimatemsg;


/* ---- STATUS BAR (SB_bar.c) ---- */

extern	int		SB_state;
void SB_Init(void);
boolean SB_Responder(event_t *event);
void SB_Ticker(void);
void SB_Drawer(void);


/* ---- MENU (MN_menu.c) ---- */

void MN_Init(void);
void MN_ActivateMenu(void);
void MN_DeactivateMenu(void);
boolean MN_Responder(event_t *event);
void MN_Ticker(void);
void MN_Drawer(void);
void MN_DrTextA(const char *text, int x, int y);
int MN_TextAWidth(const char *text);
void MN_DrTextB(const char *text, int x, int y);
int MN_TextBWidth(const char *text);


/* --- AUTOMAP---- */

#define	AM_TRANSPARENT	1	/* compile time option. 0: old style map drawn	*/
				/* onto solid background.  1:  transparent map.	*/

extern	boolean		automapactive;


/* ---- VIDEO ---- */

extern	int		dirtybox[4];
extern	byte		gammatable[5][256];
extern	int		usegamma;

void V_Init(void); /* Allocates buffer screens, call before R_Init */
void V_DrawPatch(int x, int y, patch_t *patch);
void V_DrawFuzzPatch(int x, int y, patch_t *patch);
void V_DrawShadowedPatch(int x, int y, patch_t *patch);
void V_DrawRawScreen(byte *raw);

#include "sounds.h"

#endif	/* __DOOMDEF__ */

