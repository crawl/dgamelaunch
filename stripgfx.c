/* Parts taken from drawing.c, nethack source */
/* Copyright (c) NetHack Development Team 1992.			  */
/* NetHack may be freely redistributed.  See license for details. */

#include <string.h>
#include "stripgfx.h"

#define MAXPCHARS 92

unsigned char gfx_map[256];
unsigned int state = 0;

extern int stripped;

static unsigned char no_graphics[MAXPCHARS] = {
    '#', '*', '.', ',', '\'', '+', '^', '>', '<',  // wall, stairs up
    '_', '\\', '}', '{', '8', '~', '~',            // altar, item detect
    '0', ')', '[', '/', '%', '?', '=', '!', '(',   // orb, missile
    ':', '|', '}', '%', '$', '"', '#',             // book, cloud
};

static unsigned char ibm_graphics[MAXPCHARS] = {
    177, 176, 249, 250, 0, 254, 0, 0, 0,
    220, 239, 244, 247, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static unsigned char dec_graphics[MAXPCHARS] = {
    225, 224, 254, 0, 0, 238, 0, 0, 0,
    251, 182, 167, 187, 0, 171, 168,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};

static
struct {
    const char *sequence;
    int len;
    char ascii;
} utf8_mappings[] = {
    { "\xe2\x96\x92", 0, '#' },
    { "\xe2\x96\x91", 0, '*' },
    { "\xc2\xb7",     0, '.' },
    { "\xe2\x97\xa6", 0, ',' },
    { "\xe2\x97\xbc", 0, '+' },
    { "\xe2\x88\xa9", 0, '\\' },
    { "\xe2\x8c\xa0", 0, '}' },
    { "\xe2\x89\x88", 0, '{' },
};

#define ASIZE(x) (sizeof(x) / sizeof(*x))

void
populate_gfx_array (int gfxset)
{
  int i;

  state = 0;

  memset (gfx_map, 0, 256);

  if (gfxset == NO_GRAPHICS)
    return;

  if (gfxset == UNICODE_GRAPHICS)
  {
    for (i = 0; i < ASIZE(utf8_mappings); ++i)
      utf8_mappings[i].len = strlen(utf8_mappings[i].sequence);
    return;
  }

  for (i = 0; i < MAXPCHARS; i++)
    {
      if ((gfxset == DEC_GRAPHICS) && (dec_graphics[i]))
        {
          const int dec_ch = dec_graphics[i];
          const int shifted = dec_ch >= 0xE0? dec_ch & 0x7F : dec_ch;
          if (!gfx_map[shifted])
            gfx_map[shifted] = no_graphics[i];
        }
      if ((gfxset == IBM_GRAPHICS) && (ibm_graphics[i]))
        gfx_map[ibm_graphics[i]] = no_graphics[i];
    }

  if (gfxset == IBM_GRAPHICS)
    state = -1;

  /*
     endwin();
     for (i=0;i<256;i++) {
     printf("%X:%X ",i,gfx_map[i]);
     }
     exit(1);  */
}

unsigned char
strip_gfx (unsigned char inchar)
{
  if ((inchar == 0x0E) && (state == 0))
    {
      state = 1;
      return 0x00;
    }

  if ((inchar == 0x0F) && (state == 1))
    {
      state = 0;
      return inchar;
    }

  if ((inchar == 0x1B) && (state == 1))
    {
      state = 0;
      return inchar;
    }

  if (gfx_map[inchar] && (state == 1 || state == -1))
    {
      return gfx_map[inchar];
    }

  return inchar;
}

inline static void
buffer_strip_mapping(char *buffer, int *len,
                     const char *seq, int slen, char ascii)
{
    char *s = buffer;
    char *find;
    while ((find = strstr(s, seq)) && (find < buffer + *len))
    {
        memmove(find + 1, find + slen, *len - (find - buffer) - slen);
        *len -= slen - 1;
        *find = ascii;
        s = find + 1;
    }
}

static void
buffer_strip_unicode(char *buffer, int *len)
{
    unsigned i;
    for (i = 0; i < ASIZE(utf8_mappings); ++i)
        buffer_strip_mapping(buffer, len,
                             utf8_mappings[i].sequence,
                             utf8_mappings[i].len,
                             utf8_mappings[i].ascii);
}

void
buffer_strip_gfx (char *buffer, int *len)
{
    int i;
    if (stripped == UNICODE_GRAPHICS)
        buffer_strip_unicode(buffer, len);
    else
    {
        for (i = 0; i < *len; ++i)
            buffer[i] = strip_gfx(buffer[i]);
    }
}
