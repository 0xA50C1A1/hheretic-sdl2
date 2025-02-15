//**************************************************************************
//**
//** OGL_TEX.C
//**
//**************************************************************************

// HEADER FILES ------------------------------------------------------------

#include "h2stdinc.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "doomdef.h"
#include "r_local.h"
#include "ogl_def.h"

// MACROS ------------------------------------------------------------------

// TYPES -------------------------------------------------------------------

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

// EXTERNAL DATA DECLARATIONS ----------------------------------------------

extern int		numtextures;
extern texture_t	**textures;

extern int		maxTexSize;	/* Maximum supported texture size */
extern int		ratioLimit;
extern int		test3dfx;

// PUBLIC DATA DEFINITIONS -------------------------------------------------

int			pallump;
int			mipmapping = 0;
int			linearRaw = 0;

// Properties of the current texture.
float			texw = 1, texh = 1;
int			texmask = 0;
GLuint			curtex = 0;
byte			topLineRGB[3];

texsize_t		*lumptexsizes;	/* Sizes for all the lumps */
unsigned short		*spriteheights;

// PRIVATE DATA DEFINITIONS ------------------------------------------------

// Texture names for all lumps. Although the list will contain names for
// ALL lumps, only the graphics entries that aren't flats, wall textures or
// sprites are used.

static GLuint		*lumptexnames, *lumptexnames2;
				/* Support for two parts */

static int		*rawlumps, numrawlumps;
				/* Raw screen lumps (just lump numbers) */

static GLuint		*flattexnames, *texnames, *spritenames;
static char		*texmasked;	/* 1 if the texture is masked. */

static GLuint		dltexname;	/* Name of the dynamic light texture */

static int		glmode[6] =	/* Indexed by 'mipmapping' */
{
					GL_NEAREST,
					GL_LINEAR,
					GL_NEAREST_MIPMAP_NEAREST,
					GL_LINEAR_MIPMAP_NEAREST,
					GL_NEAREST_MIPMAP_LINEAR,
					GL_LINEAR_MIPMAP_LINEAR
};

// CODE --------------------------------------------------------------------


int FindNextPower2(int num)
{
	int cumul;
	for (cumul = 1; num > cumul; cumul *= 2)
		;
	return cumul;
}

float NextPower2Ratio(int num)
{
	return num / (float)FindNextPower2(num);
}

void OGL_TexInit(void)
{
	// Allocate memory for the flat texture numbers.
	flattexnames = (GLuint *) Z_Malloc(numflats * sizeof(GLuint), PU_STATIC, NULL);
	memset(flattexnames, 0, numflats * sizeof(GLuint));

	texnames = (GLuint *) Z_Malloc(numtextures * sizeof(GLuint), PU_STATIC, NULL);
	memset(texnames, 0, numtextures * sizeof(GLuint));

	texmasked = (char *) Z_Malloc(numtextures, PU_STATIC, NULL);
	memset(texmasked, 0, numtextures);

	// Sprites.
	spritenames = (GLuint *) Z_Malloc(numspritelumps * sizeof(GLuint), PU_STATIC, NULL);
	memset(spritenames, 0, numspritelumps * sizeof(GLuint));
	spriteheights = (unsigned short *) Z_Malloc(numspritelumps * sizeof(short), PU_STATIC, NULL);
	memset(spriteheights, 0, numspritelumps * sizeof(short));

	// Standard lump textures (raw images and other gfx).
	// First parts.
	lumptexnames = (GLuint *) Z_Malloc(numlumps * sizeof(GLuint), PU_STATIC, NULL);
	memset(lumptexnames, 0, numlumps * sizeof(GLuint));
	// Second parts.
	lumptexnames2 = (GLuint *) Z_Malloc(numlumps * sizeof(GLuint), PU_STATIC, NULL);
	memset(lumptexnames2, 0, numlumps * sizeof(GLuint));
	// Size data.
	lumptexsizes = (texsize_t *) Z_Malloc(numlumps * sizeof(texsize_t), PU_STATIC, NULL);
	memset(lumptexsizes, 0, numlumps * sizeof(texsize_t));

	// Raw screen lump book keeping.
	rawlumps = 0;
	numrawlumps = 0;

	// The dynamic light map.
	dltexname = 0;

	// The palette lump, for color information (really?).
	pallump = W_GetNumForName("PLAYPAL");
}

void OGL_SetPaletteLump(const char *palname)
{
	pallump = W_GetNumForName(palname);
}

void OGL_TexReset(void)
{
	glDeleteTextures(numflats, flattexnames);
	memset(flattexnames, 0, numflats * sizeof(GLuint));

	glDeleteTextures(numtextures, texnames);
	memset(texnames, 0, numtextures * sizeof(GLuint));

	memset(texmasked, 0, numtextures);

	glDeleteTextures(numspritelumps, spritenames);
	memset(spritenames, 0, numspritelumps * sizeof(GLuint));
	memset(spriteheights, 0, numspritelumps * sizeof(short));

	glDeleteTextures(1, &dltexname);
	dltexname = 0;

	// Normal patch/raw image textures aren't deleted here (see below).
}

void OGL_ResetLumpTexData(void)
{
	glDeleteTextures(numlumps, lumptexnames);
	glDeleteTextures(numlumps, lumptexnames2);
	memset(lumptexnames, 0, numlumps * sizeof(GLuint));
	memset(lumptexnames2, 0, numlumps * sizeof(GLuint));
	memset(lumptexsizes, 0, numlumps * sizeof(texsize_t));

	// Free the raw lumps book keeping table.
	free(rawlumps);
	rawlumps = NULL;
	numrawlumps = 0;
}

// Binds the texture if necessary.
void OGL_BindTexture(GLuint texname)
{
	if (curtex != texname)
	{
		glBindTexture(GL_TEXTURE_2D, texname);
		curtex = texname;
	}
}

void PalToRGB(byte *palidx, byte *rgb)
{
	int	i;

	for (i = 0; i < 3; i++)
	{
		*(rgb + i) = gammatable[usegamma][*(palidx + i)];
	}
}

void PalIdxToRGB(byte *pal, int idx, byte *rgb)
{
	PalToRGB(pal + idx*3, rgb);
}

unsigned int OGL_BindTexFlat(int lump)
{
	byte *flatptr;
	byte *palette;
	byte *rgbflat;
	GLuint name;
	int	i;

	if (lumpinfo[lump].size < 4096)
	{
		printf ("Invalid texture lump (%s,%d). Using texture 0\n",
						lumpinfo[lump].name, lump);
		OGL_BindTexture(0);
		return 0;
	}

	flatptr = (byte *) W_CacheLumpNum(lump, PU_STATIC);
	palette = (byte *) W_CacheLumpNum(pallump = W_GetNumForName("PLAYPAL"), PU_CACHE);
	rgbflat = (byte *) malloc (3 * lumpinfo[lump].size);

	OGL_DEBUG("OGL_SetFlat: Loading flat %d.\n", idx);
	// Convert the data to RGB.
	for (i = 0; i < lumpinfo[lump].size; i++)
		PalIdxToRGB(palette, flatptr[i], rgbflat + i*3);

	// Generate and bind the texture.
	glGenTextures(1, &name);//flattexnames+idx);
	glBindTexture(GL_TEXTURE_2D, name);//flattexnames[idx]);

	// Set the texture properties.
	if (test3dfx)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbflat);
	}
	else
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, rgbflat);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmapping]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Change the tag.
	Z_ChangeTag(flatptr, PU_CACHE);
	free (rgbflat);
	return name;
}

// Returns the OpenGL name of the texture.
unsigned int OGL_PrepareFlat(int idx)
{
	idx = flattranslation[idx];
	if (!flattexnames[idx])
	{
		// The flat isn't yet bound with OpenGL.
		flattexnames[idx] = OGL_BindTexFlat(firstflat + idx);
	}
	texw = texh = 64;
	texmask = 0;
	return flattexnames[idx];
}

void OGL_SetFlat(int idx)
{
	glBindTexture(GL_TEXTURE_2D, OGL_PrepareFlat(idx));
}

// Return GL_RGB or GL_RGBA.
static int DrawRealPatch(byte *rgbflat, byte *rgbaflat, byte *palette, int texwidth,
			 int texheight, int sz, patch_t *patch, boolean maskZero)
{
	column_t *column;
	byte *desttop1, *desttop2;
	byte *dest1, *dest2;
	const byte *end1, *end2;
	byte *source;
	int count, col, w, i;

	dest1 = NULL; dest2 = NULL;
	end1 = (rgbflat)  ? rgbflat  + sz * 3 - 3 : NULL;
	end2 = (rgbaflat) ? rgbaflat + sz * 4 - 4 : NULL;
	desttop1 = rgbflat;
	desttop2 = rgbaflat;
	w = SHORT(patch->width);
	for (col = 0; col < w; col++)
	{
		column = (column_t *)((byte *)patch + LONG(patch->columnofs[col]));
		// Step through the posts in a column
		while (column->topdelta != 0xff)
		{
			source = (byte *)column + 3;
			if (rgbflat)
				dest1 = desttop1 + column->topdelta*texwidth*3;
			if (rgbaflat)
				dest2 = desttop2 + column->topdelta*texwidth*4;
			count = column->length;
			while (count--)
			{
				int palidx = *source++;
				if (rgbflat)
				{
					if (dest1 > end1) break;
					if (!maskZero || palidx)
						PalIdxToRGB(palette, palidx, dest1);
					dest1 += texwidth*3;
				}
				if (rgbaflat)
				{
					if (dest2 > end2) break;
					if (!maskZero || palidx)
					{
						PalIdxToRGB(palette, palidx, dest2);
						*(dest2 + 3) = 0xff;	// This pixel is not clear.
					}
					dest2 += texwidth*4;
				}
			}
			column = (column_t *)((byte *)column + column->length + 4);
		}
		if (desttop1) desttop1 += 3;
		if (desttop2) desttop2 += 4;
	}
	// Scan through the RGBA buffer and check for sub-0xff alpha.
	if (rgbflat && rgbaflat)	// Which one is it?
	{
		for (i = 0; i < texwidth*texheight; i++)
		{
			if (*(rgbaflat + i*4 + 3) != 0xff)
				return GL_RGBA;
		}
	}
	if (rgbflat)
		return GL_RGB;
	return GL_RGBA;
}

// Returns the OpenGL texture name.
unsigned int OGL_PrepareTexture(int idx)
{
	if (idx == 0)
	{
		// No texture?
		texw = 1;
		texh = 1;
		texmask = 0;
		return 0;
	}
	idx = texturetranslation[idx];
	if (!texnames[idx])
	{
		// The texture must be given to OpenGL.
		int i, k, textype;
		texture_t *tex = textures[idx];
		byte *palette = (byte *) W_CacheLumpNum(pallump, PU_CACHE);
		byte *rgbflat = (byte *) malloc (3 * tex->width * tex->height);
		byte *colptr;

		if (tex->patchcount > 1)
		{
			textype = GL_RGB;
			for (i = 0; i < tex->width; i++)
			{
				colptr = R_GetColumn(idx, i);
				for (k = 0; k < tex->height; k++)
					PalIdxToRGB(palette, colptr[k], rgbflat + (k*tex->width + i)*3);
			}
		}
		else
		{
			// This texture has only only one patch. It might be masked.
			int texsize = tex->width * tex->height;
			byte *rgbaflat = (byte *) calloc (1, 4 * texsize);
			textype = DrawRealPatch(rgbflat, rgbaflat, palette, tex->width, tex->height, texsize,
						(patch_t *)W_CacheLumpNum(tex->patches[0].patch, PU_CACHE), false);
			if (textype == GL_RGBA)
			{
				free (rgbflat);
				rgbflat = rgbaflat;
			}
			else
			{
				free (rgbaflat);
			}
		}

		// Generate and bind the texture.
		glGenTextures(1, texnames + idx);
		glBindTexture(GL_TEXTURE_2D, texnames[idx]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, (textype == GL_RGB) ? 3 : 4, tex->width,
					tex->height, textype, GL_UNSIGNED_BYTE, rgbflat);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmapping]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (textype == GL_RGBA)
			texmasked[idx] = 1;	// Yes it is.

		free (rgbflat);
	}
	texw = textures[idx]->width;
	texh = textures[idx]->height;
	texmask = texmasked[idx];
	return texnames[idx];
}

void OGL_SetTexture(int idx)
{
	glBindTexture(GL_TEXTURE_2D, OGL_PrepareTexture(idx));
}

int LineAverageRGB(byte *imgdata, int components, int width, int line,
						byte *rgb, byte *palette)
{
	byte	*start = imgdata + components*width*line;
	int		i, c, count = 0;
	int		integerRGB[3] = { 0, 0, 0};

	for (i = 0; i < width; i++)
	{
		byte *pixpos = start + i*components;
		if (*(pixpos + 3) > 0)	// Not transparent?
		{
			count++;
			for (c = 0; c < 3; c++)
				integerRGB[c] += pixpos[c];
		}
	}
	// All transparent? Sorry...
	if (!count)
		return 0;

	// We're going to make it!
	for (c = 0; c < 3; c++)
		rgb[c] = integerRGB[c] / count;
	return 1;	// Successful.
}

void ImageAverageRGB(byte *imgdata, int components, int width, int height,
						byte *rgb, byte *palette)
{
	int	i, c, integerRGB[3] = { 0, 0, 0}, count = 0;

	for (i = 0; i < height; i++)
	{
		if (LineAverageRGB(imgdata, components, width, i, rgb, palette))
		{
			count++;
			for (c = 0; c < 3; c++)
				integerRGB[c] += rgb[c];
		}
	}
	if (count)	// If there were pixels...
	{
		for (c = 0; c < 3; c++)
			rgb[c] = integerRGB[c] / count;
	}
}

unsigned int OGL_PrepareSky(int idx, boolean zeroMask)
{
	if (idx != texturetranslation[idx])
		I_Error("Skytex: %d, translated: %d\n", idx, texturetranslation[idx]);

	idx = texturetranslation[idx];

	if (!texnames[idx])
	{
		int		i, k;
		byte		*palette = (byte *) W_CacheLumpNum(pallump, PU_CACHE);
		texture_t	*tex = textures[idx];
		int		textype, numpels = 256*256;	// 'Full' size.
		byte		*imgdata, *colptr;
		// Choose the right format.
		if (zeroMask)
		{
			textype = GL_RGBA;
			imgdata = (byte *) calloc (1, 4 * numpels);
		}
		else
		{
			textype = GL_RGB;
			imgdata = (byte *) calloc (1, 3 * numpels);
		}
		if (tex->patchcount > 1)
		{
			for (i = 0; i < tex->width; i++)
			{
				colptr = R_GetColumn(idx, i);
				for (k = 0; k < tex->height; k++)
				{
					if (textype == GL_RGB)
						PalIdxToRGB(palette, colptr[k], imgdata + (k*256 + i)*3);
					else if (textype == GL_RGBA && colptr[k])
					{
						byte *imgpos = imgdata + (k*256 + i)*4;
						PalIdxToRGB(palette, colptr[k], imgpos);
						*(imgpos + 3) = 0xff;	// Not transparent, this pixel.
					}
				}
			}
		}
		else
		{
			// This texture has only only one patch.
			if (textype == GL_RGB)
				DrawRealPatch(imgdata, NULL, palette, 256, tex->height, numpels,
					(patch_t *)W_CacheLumpNum(tex->patches[0].patch, PU_CACHE), false);
			else if (textype == GL_RGBA) // Mask out zeros.
				DrawRealPatch(NULL, imgdata, palette, 256, tex->height, numpels,
					(patch_t *)W_CacheLumpNum(tex->patches[0].patch, PU_CACHE), true);
		}
		if (textype == GL_RGBA)	// For masked data, calculate the alpha-fill color.
		{
			byte rgbAverage[3];
			ImageAverageRGB(imgdata, 4, 256, 200, rgbAverage, palette);
			// Fill all the transparent places.
			for (i = 0; i < 256*200; i++)
			{
				byte *pixel = imgdata + 4*i;
				if (!pixel[3])
					memcpy(pixel, rgbAverage, 3);
			}
		}
		// Calculate the topline RGB for sky top fadeouts.
		memset(topLineRGB, 0, 3);
		LineAverageRGB(imgdata, (textype == GL_RGB) ? 3 : 4, 256, 0, topLineRGB, palette);

		// Generate and bind the texture.
		glGenTextures(1, texnames + idx);
		glBindTexture(GL_TEXTURE_2D, texnames[idx]);
		if (test3dfx)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, (textype == GL_RGB) ? 3 : 4, 256, 256, 0, GL_RGB,
									GL_UNSIGNED_BYTE, imgdata);
		}
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, (textype == GL_RGB) ? 3 : 4, 256, 256, textype,
									GL_UNSIGNED_BYTE, imgdata);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmapping]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Do we have a masked texture?
		if (textype == GL_RGBA)
			texmasked[idx] = 1;
		else
			texmasked[idx] = 0;

		free (imgdata);
	}
	texw = textures[idx]->width;
	texh = textures[idx]->height;
	texmask = texmasked[idx];
	return texnames[idx];
}

unsigned int OGL_PrepareSprite(int pnum)
{
	if (!spritenames[pnum])
	{
		// There's no name for this patch, load it in.
		patch_t *patch = (patch_t *) W_CacheLumpNum(firstspritelump + pnum, PU_CACHE);
		int	w = SHORT(patch->width),
			h = SHORT(patch->height);
		int	p2width = FindNextPower2(w),
			p2height = OGL_ValidTexHeight2(w, h);
		int	flatsize = p2width*p2height;
		byte	*rgbaflat = (byte *) calloc (1, 4* flatsize);

		OGL_DEBUG("orig: %d x %d => %d x %d\n", w, h, p2width, p2height);

		DrawRealPatch(NULL, rgbaflat, (byte *)W_CacheLumpNum(pallump,PU_CACHE),
						p2width, p2height, flatsize, patch, false);

		// Generate and bind the texture.
		glGenTextures(1, spritenames + pnum);
		glBindTexture(GL_TEXTURE_2D, spritenames[pnum]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, p2width, p2height, GL_RGBA,
						GL_UNSIGNED_BYTE, rgbaflat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmapping]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		spriteheights[pnum] = h;
		free (rgbaflat);
	}
	return spritenames[pnum];
}

void OGL_SetSprite(int pnum)
{
	OGL_BindTexture(OGL_PrepareSprite(pnum));
}

void OGL_NewRawLump(int lump)
{
	rawlumps = (int *) realloc (rawlumps, sizeof(int) * ++numrawlumps);
	rawlumps[numrawlumps - 1] = lump;
}

GLuint OGL_GetOtherPart(int lump)
{
	return lumptexnames2[lump];
}

// Part is either 1 or 2. Part 0 means only the left side is loaded.
// No splittex is created in that case. Once a raw image is loaded
// as part 0 it must be deleted before the other part is loaded at the
// next loading.
void OGL_SetRawImage(int lump, int part)
{
	if (part < 0 || part > 2)
		return;	// Check the part.

	if (!lumptexnames[lump])
	{
		// Load the raw texture data (320x200).
		// We'll create two textures (256x256 and 64x256).
		byte *raw = (byte *) W_CacheLumpNum(lump, PU_CACHE);
		byte *dat1 = (byte *) malloc (3*256*256);
		byte *dat2 = (byte *) malloc (3*64*256);
		byte *palette = (byte *) W_CacheLumpNum(pallump, PU_CACHE);
		int i, k;

		memset(dat1, 0, 3*256*256);	// Why must this be done?
		for (k = 0; k < 200; k++)
		{
			for (i = 0; i < 256; i++)
			{
				// We can't go over the end.
				if (k*320 + i > lumpinfo[lump].size - 1)
					break;
				// Part one.
				PalIdxToRGB(palette, *(raw + (k*320 + i)), dat1 + 3*(k*256 + i));
				if (i < 64 && part) // Part two?
					PalIdxToRGB(palette, *(raw + (k*320 + i + 256)), dat2 + 3*(k*64 + i));
			}
		}

		// Do a special fill for textures with h<200 (part 0).
		if (/*lumpinfo[lump].size/320 < 200 &&*/ !part)
		{
			int _lines = lumpinfo[lump].size / 320;
			// Copy the missing data from the beginning.
			memcpy(dat1 + _lines*256*3, dat1, 3*256*(256 - _lines));
		}

		// Generate and load the textures.
		glGenTextures(1, lumptexnames + lump);
		glBindTexture(GL_TEXTURE_2D, lumptexnames[lump]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, dat1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearRaw ? GL_LINEAR : GL_NEAREST);

		if (part)
		{
			// And the other part.
			glGenTextures(1, lumptexnames2 + lump);
			glBindTexture(GL_TEXTURE_2D, lumptexnames2[lump]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, dat2);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearRaw ? GL_LINEAR : GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

			// Add it to the list.
			OGL_NewRawLump(lump);
		}

		lumptexsizes[lump].w = 256;
		lumptexsizes[lump].w2 = 64;
		lumptexsizes[lump].h = 200;
		free (dat1);
		free (dat2);
	}
	// Bind the correct part.
	if (part <= 1)
		glBindTexture(GL_TEXTURE_2D, lumptexnames[lump]);
	if (part == 2)
		glBindTexture(GL_TEXTURE_2D, lumptexnames2[lump]);
	// We don't track the current texture with raw images.
	curtex = 0;
}

// Copies a rectangular region of the source buffer to the destination
// buffer. Doesn't perform clipping, so be careful.
static void pixBlt(byte *src, int srcWidth, byte *dest, int destWidth, int pixelSize,
				int srcRegX, int srcRegY, int destRegX, int destRegY,
				int regWidth, int regHeight)
{
	int	y;	// Y in the copy region.

	for (y = 0; y < regHeight; y++)		// Copy line by line.
	{
		memcpy(dest + pixelSize * (destRegX + (y + destRegY) * destWidth),
			src + pixelSize * (srcRegX  + (y +  srcRegY) * srcWidth) ,
			pixelSize * regWidth);
	}
}

// Notices ratiolimit.
int OGL_ValidTexHeight2(int width, int height)
{
	int p2w, p2h = FindNextPower2(height);
	int minheight;

	if (!ratioLimit)
		return p2h;

	p2w = FindNextPower2(width);
	// Do we have to notice that the texture will be split?
	if (p2w > maxTexSize)
	{
		int	part2width = FindNextPower2(width - maxTexSize);
		int	height1 = maxTexSize/ratioLimit,
			height2 = part2width/ratioLimit;
		minheight = height1 > height2 ? height1 : height2;
	}
	else
		minheight = p2w/ratioLimit;
	// Do the test.
	if (minheight > p2h)
		return minheight;
	return p2h;
}

void OGL_SetPatch(int lump)	// No mipmaps are generated.
{
	if (!lumptexnames[lump])
	{
		// Load the patch.
		patch_t	*patch = (patch_t *) W_CacheLumpNum(lump, PU_CACHE);
		int	w = SHORT(patch->width),
			h = SHORT(patch->height);
		int	p2width = FindNextPower2(w),
			p2height = OGL_ValidTexHeight2(w, h);
		int	numpels = p2width*p2height;
		byte	*rgbflat  = (byte *) malloc (3 * numpels),
			*rgbaflat = (byte *) malloc (4 * numpels);
		int	ptype;

		memset(rgbaflat, 0, 4 * numpels);
		ptype = DrawRealPatch(rgbflat, rgbaflat, (byte *)W_CacheLumpNum(pallump,PU_CACHE),
							p2width, p2height, numpels, patch, false);

		// See if we have to split the patch into two parts.
		if (p2width > maxTexSize) // Nothing about the height...
		{
			// Notice: This is only vertical splitting, p2height 
			// applies to both parts. 
			// The width of the first part is maxTexSize.
			int part2width = FindNextPower2(w - maxTexSize);
			byte *tempbuff = (byte *) malloc (4 * maxTexSize * p2height);
			if (part2width > maxTexSize)
				I_Error("OGL_SetPatch: Too wide texture (really: %d, pow2: %d).\n", w, p2width);
			// We'll use a temporary buffer for doing to splitting.
			// First, part one.
			pixBlt(ptype == GL_RGB ? rgbflat : rgbaflat, p2width, tempbuff,
						maxTexSize, ptype == GL_RGB ? 3 : 4,
						0, 0, 0, 0, maxTexSize, p2height);
			// Generate a texture.
			glGenTextures(1, lumptexnames + lump);
			glBindTexture(GL_TEXTURE_2D, lumptexnames[lump]);
			// There won't be mipmapping versions.
			glTexImage2D(GL_TEXTURE_2D, 0, ptype == GL_RGB ? 3 : 4, maxTexSize, p2height,
								0, ptype, GL_UNSIGNED_BYTE, tempbuff);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			// Then part two.
			pixBlt(ptype == GL_RGB ? rgbflat : rgbaflat, p2width, tempbuff,
						part2width, ptype == GL_RGB ? 3 : 4,
						maxTexSize, 0, 0, 0, part2width, p2height);
			// Generate a texture.
			glGenTextures(1, lumptexnames2 + lump);
			glBindTexture(GL_TEXTURE_2D, lumptexnames2[lump]);
			// There won't be mipmapping versions.
			glTexImage2D(GL_TEXTURE_2D, 0, ptype == GL_RGB ? 3 : 4, part2width, p2height,
								0, ptype, GL_UNSIGNED_BYTE, tempbuff);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			OGL_BindTexture(lumptexnames[lump]);

			lumptexsizes[lump].w = maxTexSize;
			lumptexsizes[lump].w2 = w - maxTexSize;
			free (tempbuff);
		}
		else // We can use the normal one-part method.
		{
			// Generate a texture.
			glGenTextures(1, lumptexnames + lump);
			glBindTexture(GL_TEXTURE_2D, lumptexnames[lump]);
			// There won't be mipmapping versions.
			glTexImage2D(GL_TEXTURE_2D, 0, (ptype == GL_RGB) ? 3 : 4, p2width, p2height,
					0, ptype, GL_UNSIGNED_BYTE, (ptype == GL_RGB) ? rgbflat : rgbaflat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			lumptexsizes[lump].w = w;
			lumptexsizes[lump].w2 = 0;
		}
		// The rest of the size information.
		lumptexsizes[lump].h = h;
		lumptexsizes[lump].offx = -SHORT(patch->leftoffset);
		lumptexsizes[lump].offy = -SHORT(patch->topoffset);
		free (rgbflat);
		free (rgbaflat);
	}
	else
	{
		OGL_BindTexture(lumptexnames[lump]);
	}
	curtex = lumptexnames[lump];
}

#if 0
// Drawing polygons with an unset texture causes a segfault with Mesa 3.0.
// I guess Windows OpenGL handles this ok.
// I am disabling texturing when needed rather than binding an unset texture.
void OGL_SetNoTexture(void)
{
//	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	curtex = 0;
}
#endif

GLuint OGL_PrepareLightTexture(void)
{
	if (!dltexname)
	{
		// We need to generate the texture, I see.
		byte *image = (byte *) W_CacheLumpName("DLIGHT", PU_CACHE);
		if (!image)
			I_Error("OGL_SetLightTexture: no dlight texture.\n");
		// The dynamic light map is a 64x64 grayscale 8-bit image.
		glGenTextures(1, &dltexname);
		glBindTexture(GL_TEXTURE_2D, dltexname);

		glTexImage2D(GL_TEXTURE_2D, 0, 1, 64, 64, 0, GL_LUMINANCE,
						GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	// Set the info.
	texw = texh = 64;
	texmask = 0;
	return dltexname;
}

int OGL_GetLumpTexWidth(int lump)
{
	return lumptexsizes[lump].w;
}

int OGL_GetLumpTexHeight(int lump)
{
	return lumptexsizes[lump].h;
}

// Updates the textures, flats and sprites.
void OGL_UpdateTexParams(int mipmode)
{
	int	i;

	// Textures.
	for (i = 0; i < numtextures; i++)
	{
		if (texnames[i])	// Is the texture loaded?
		{
			glBindTexture(GL_TEXTURE_2D, texnames[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmode]);
		}
	}
	// Flats.
	for (i = 0; i < numflats; i++)
	{
		if (flattexnames[i])	// Is the texture loaded?
		{
			glBindTexture(GL_TEXTURE_2D, flattexnames[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmode]);
		}
	}
	// Sprites.
	for (i = 0; i < numspritelumps; i++)
	{
		if (spritenames[i])
		{
			glBindTexture(GL_TEXTURE_2D, spritenames[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glmode[mipmode]);
		}
	}
}

// Updates the raw screen smoothing (linear magnification).
// This is the main reason for having the rawlumps table.
void OGL_UpdateRawScreenParams(int smoothing)
{
	int		i;
	int		_glmode = (smoothing) ? GL_LINEAR : GL_NEAREST;

	for (i = 0; i < numrawlumps; i++)
	{
		// First part 1.
		glBindTexture(GL_TEXTURE_2D, lumptexnames[rawlumps[i]]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _glmode);
		// Then part 2.
		glBindTexture(GL_TEXTURE_2D, lumptexnames2[rawlumps[i]]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _glmode);
	}
}
