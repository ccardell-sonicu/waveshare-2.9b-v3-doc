
#ifndef _CUSTOM_FONTS_H_
#define _CUSTOM_FONTS_H_
#include <stdint.h>

typedef struct _tGlyph
{    
  int bitmapOffset; //location in the bitmap this character's data is
  int width; //width of the character in bit or pixels
  int height; //height of the character in bits or pixels
  int xAdvance; //number of bits to advance after printing character
  int xOffset; //number of bits or pixels of padding to the left of the character
  int yOffset; //number of bits or pixels of padding above the character (this is negative because of a different screen)
} sGlyph;

typedef struct _tFontCustom
{    
  const uint8_t *bitmap_table;
  const sGlyph *glyph_table;
} sFONTCUSTOM;

#endif /* _CUSTOM_FONTS_H_ */
