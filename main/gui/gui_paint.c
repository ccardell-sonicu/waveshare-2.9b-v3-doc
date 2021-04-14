#include "../main.h"
#include "../fonts/fonts.h"
#include "gui_paint.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> //memset()
#include <math.h>

PAINT Paint;


/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void paint_clear(UWORD Color)
{
    if(Paint.Scale == 2 || Paint.Scale == 4) {
		for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
			for (UWORD X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
				UDOUBLE Addr = X + Y*Paint.WidthByte;
				Paint.Image[Addr] = Color;
			}
		}
	}
	if(Paint.Scale == 7) {
		Color = (UBYTE)Color;
		UWORD Width = (Paint.WidthMemory * 3 % 8 == 0)? (Paint.WidthMemory * 3 / 8 ): (Paint.WidthMemory * 3 / 8 + 1);
		for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
			for (UWORD X = 0; X < Width; X++ ) {
				UDOUBLE Addr = X + Y * Width;
				if((X + Y * Width)%3 == 0) 
					Paint.Image[Addr] = ((Color<<5) | (Color<<2) | (Color>>1));							
				else if((X + Y * Width)%3 == 1) 
					Paint.Image[Addr] = ((Color<<7) | (Color<<4) | (Color<<1) | (Color>>2));				
				else if((X + Y * Width)%3 == 2)
					Paint.Image[Addr] = ((Color<<6) | (Color<<3) |  Color);				
			}
		}
	}
}

/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Painted colors
******************************************************************************/
void paint_set_pixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if(Xpoint > Paint.Width || Ypoint > Paint.Height){
        // Debug("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;
    switch(Paint.Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(Paint.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Paint.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Paint.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Paint.WidthMemory - X - 1;
        Y = Paint.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
        // Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    if(Paint.Scale == 2){
        // UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
        UDOUBLE Addr = Y / 8 + X * Paint.HeightByte;
        UBYTE Rdata = Paint.Image[Addr];
        if(Color == BLACK)
            // Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
            Paint.Image[Addr] = Rdata & ~(0x80 >> (Y % 8));
        else
            // Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
            Paint.Image[Addr] = Rdata | (0x80 >> (Y % 8));
    }else if(Paint.Scale == 4){
        UDOUBLE Addr = X / 4 + Y * Paint.WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = Paint.Image[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        Paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(Paint.Scale == 7){
			UWORD Width = Paint.WidthMemory*3%8 == 0 ? Paint.WidthMemory*3/8 : Paint.WidthMemory*3/8+1;
			UDOUBLE Addr = (Xpoint * 3) / 8 + Ypoint * Width;
			UBYTE shift, Rdata, Rdata2;
			shift = (Xpoint+Ypoint*Paint.HeightMemory) % 8;

			switch(shift) {
				case 0 :
					Rdata = Paint.Image[Addr] & 0x1f;
					Rdata = Rdata | ((Color << 5) & 0xe0);
					Paint.Image[Addr] = Rdata;
					break;
				case 1 :
					Rdata = Paint.Image[Addr] & 0xe3;
					Rdata = Rdata | ((Color << 2) & 0x1c);
					Paint.Image[Addr] = Rdata;
					break;
				case 2 :
					Rdata = Paint.Image[Addr] & 0xfc;
					Rdata2 = Paint.Image[Addr + 1] & 0x7f;
					Rdata = Rdata | ((Color >> 1) & 0x03);
					Rdata2 = Rdata2 | ((Color << 7) & 0x80);
					Paint.Image[Addr] = Rdata;
					Paint.Image[Addr + 1] = Rdata2;
					break;
				case 3 :
					Rdata = Paint.Image[Addr] & 0x8f;
					Rdata = Rdata | ((Color << 4) & 0x70);
					Paint.Image[Addr] = Rdata;
					break;
				case 4 :
					Rdata = Paint.Image[Addr] & 0xf1;
					Rdata = Rdata | ((Color << 1) & 0x0e);
					Paint.Image[Addr] = Rdata;
					break;
				case 5 :
					Rdata = Paint.Image[Addr] & 0xfe;
					Rdata2 = Paint.Image[Addr + 1] & 0x3f;
					Rdata = Rdata | ((Color >> 2) & 0x01);
					Rdata2 = Rdata2 | ((Color << 6) & 0xc0);
					Paint.Image[Addr] = Rdata;
					Paint.Image[Addr + 1] = Rdata2;
					break;
				case 6 :
					Rdata = Paint.Image[Addr] & 0xc7;
					Rdata = Rdata | ((Color << 3) & 0x38);
					Paint.Image[Addr] = Rdata;
					break;
				case 7 :
					Rdata = Paint.Image[Addr] & 0xf8;
					Rdata = Rdata | (Color & 0x07);
					Paint.Image[Addr] = Rdata;
					break;						
			}	
		}
}

/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void paint_draw_char(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                    sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Page, Column;

    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        // Debug("paint_draw_char Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        // printf("Page: %d - ", Page);

        for (Column = 0; Column < Font->Width; Column ++ ) {
            // printf("Column: %d, ", Column);
            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    paint_set_pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    paint_set_pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    paint_set_pixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
        // printf("\n");
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void paint_draw_string(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > Paint.Width || Ystart > Paint.Height) {
        // Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > Paint.Width ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > Paint.Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        paint_draw_char(Xpoint, Ypoint, * pString, Font, Color_Background, Color_Foreground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}