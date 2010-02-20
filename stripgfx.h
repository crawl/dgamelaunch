#ifndef INCLUDED_stripgfx_h
#define INCLUDED_stripgfx_h

#define NO_GRAPHICS      1
#define DEC_GRAPHICS     2
#define IBM_GRAPHICS     3
#define UNICODE_GRAPHICS 4

void populate_gfx_array (int gfxset);
unsigned char strip_gfx (unsigned char inchar);
void buffer_strip_gfx (char *buffer, int *len);

#endif /* !INCLUDED_stripgfx_h */
