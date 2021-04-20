/******************************************************************************
* | File      	:   GUI_Paint.c
* | Author      :   Waveshare electronics
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*                   rectangle, solid circle hollow circle.
* | Info        :
*   Achieve display characters: Display a single character, string, number
*   Achieve time display: adaptive size display time minutes and seconds
*----------------
* |	This version:   V3.2
* | Date        :   2020-07-23
* | Info        :
* -----------------------------------------------------------------------------
* V3.2(2020-07-23):
* 1. Change: paint_set_scale(UBYTE scale)
*			Add scale 7 for 5.65f e-Parper
* 2. Change: paint_set_pixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)  
* 			Add the branch for scale 7
* 3. Change: paint_clear(UWORD Color)
*			Add the branch for scale 7
*
* V3.1(2019-10-10):
* 1. Add gray level
*   PAINT Add Scale
* 2. Add void paint_set_scale(UBYTE scale);
*
* V3.0(2019-04-18):
* 1.Change: 
*    paint_draw_point(..., DOT_STYLE DOT_STYLE)
* => paint_draw_point(..., DOT_STYLE Dot_Style)
*    paint_draw_line(..., LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
* => paint_draw_line(..., DOT_PIXEL Line_width, LINE_STYLE Line_Style)
*    paint_draw_rectangle(..., DRAW_FILL Filled, DOT_PIXEL Dot_Pixel)
* => paint_draw_rectangle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
*    paint_draw_circle(..., DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel)
* => paint_draw_circle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Filll)
*
* -----------------------------------------------------------------------------
* V2.0(2018-11-15):
* 1.add: paint_new_image()
*    Create an image's properties
* 2.add: paint_select_image()
*    Select the picture to be drawn
* 3.add: paint_set_rotate()
*    Set the direction of the cache    
* 4.add: Paint_RotateImage() 
*    Can flip the picture, Support 0-360 degrees, 
*    but only 90.180.270 rotation is better
* 4.add: paint_set_mirroring() 
*    Can Mirroring the picture, horizontal, vertical, origin
* 5.add: Paint_DrawString_CN() 
*    Can display Chinese(GB1312)   
*
* ----------------------------------------------------------------------------- 
* V1.0(2018-07-17):
*   Create library
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
******************************************************************************/
#include "gui_paint.h"
#include "../main.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <stdio.h>
#include <math.h>

PAINT Paint;

/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void paint_new_image(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color)
{
    Paint.Image = NULL;
    Paint.Image = image;

    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;    
    Paint.WidthByte = (Width % 8 == 0) ? (Width / 8 ) : (Width / 8 + 1);
    Paint.HeightByte = (Height % 8 == 0) ? (Height / 8 ) : (Height / 8 + 1); //Height;    
    Paint.Rotate = Rotate;
    Paint.Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Paint.Width = Width;
        Paint.Height = Height;
    } else {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void paint_select_image(UBYTE *image)
{
    Paint.Image = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void paint_set_rotate(UWORD Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        Paint.Rotate = Rotate;
    } else {
        printf("rotate = 0, 90, 180, 270\r\n");
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void paint_set_mirroring(UBYTE mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        Paint.Mirror = mirror;
    } else {
        printf("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
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
        printf("Exceeding display boundaries\r\n");
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
        printf("Exceeding display boundaries\r\n");
        return;
    }
    
    UDOUBLE Addr = Y / 8 + X * Paint.HeightByte;
    UBYTE Rdata = Paint.Image[Addr];
    if(Color == BLACK) {
        Paint.Image[Addr] = Rdata | (0x80 >> (Y % 8));
    } else {
        Paint.Image[Addr] = Rdata & ~(0x80 >> (Y % 8));
    }
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void paint_clear(UWORD Color)
{
    for (UWORD Y = 0; Y < Paint.HeightByte; Y++) {
        for (UWORD X = 0; X < Paint.WidthByte; X++ ) {//8 pixel =  1 byte
            UDOUBLE Addr = X + Y*Paint.WidthByte;
            Paint.Image[Addr] = Color;
        }
    }
}

/******************************************************************************
function: Clear the color of a window
parameter:
    Xstart : x starting point
    Ystart : Y starting point
    Xend   : x end point
    Yend   : y end point
    Color  : Painted colors
******************************************************************************/
void paint_clear_windows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{
    UWORD X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {//8 pixel =  1 byte
            paint_set_pixel(X, Y, Color);
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void paint_draw_point(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("paint_draw_point Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                paint_set_pixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                paint_set_pixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void paint_draw_line(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        printf("paint_draw_line Input exceeds the normal display range\r\n");
        return;
    }

    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            paint_draw_point(Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            paint_draw_point(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void paint_draw_rectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        printf("Input exceeds the normal display range\r\n");
        return;
    }

    if (Draw_Fill) {
        UWORD Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            paint_draw_line(Xstart, Ypoint, Xend, Ypoint, Color , Line_width, LINE_STYLE_SOLID);
        }
    } else {
        paint_draw_line(Xstart, Ystart, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        paint_draw_line(Xstart, Ystart, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
        paint_draw_line(Xend, Yend, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        paint_draw_line(Xend, Yend, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void paint_draw_circle(UWORD X_Center, UWORD Y_Center, UWORD Radius, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height) {
        printf("paint_draw_circle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                paint_draw_point(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                paint_draw_point(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                paint_draw_point(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                paint_draw_point(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                paint_draw_point(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                paint_draw_point(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                paint_draw_point(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                paint_draw_point(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            paint_draw_point(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            paint_draw_point(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            paint_draw_point(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            paint_draw_point(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            paint_draw_point(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            paint_draw_point(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            paint_draw_point(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            paint_draw_point(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
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
void paint_draw_char(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char, sFONTCUSTOM* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Row, Column;

    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        printf("paint_draw_char Input exceeds the normal display range\r\n");
        return;
    }
    
    sGlyph glyph = Font->glyph_table[Acsii_Char - 32];

    int height = glyph.height;
    int width = glyph.width;
    int offset = glyph.bitmapOffset;

    const unsigned char *ptr = &Font->bitmap_table[offset];
    UWORD bit = 0;

    for (Row = 0; Row < height; Row ++ ) {
        for (Column = 0; Column < width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> bit)) {
                    paint_set_pixel(Xpoint + glyph.xOffset + Column , Ypoint + glyph.yOffset + Row, Color_Foreground);
                } 
            } else {
                if (*ptr & (0x80 >> bit)) {
                    paint_set_pixel(Xpoint + glyph.xOffset + Column, Ypoint + glyph.yOffset + Row, Color_Foreground);
                } else {
                    paint_set_pixel(Xpoint + glyph.xOffset + Column, Ypoint + glyph.yOffset + Row, Color_Background);
                }
            }

            //go to next bit
            bit++;

            //if next pit is out of range, reset to zero and use next byte
            if (bit == 8) {
                bit = 0;
                ptr++;
            }

        }// Write a line

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
void paint_draw_string(UWORD Xstart, UWORD Ystart, const char * pString, sFONTCUSTOM* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > Paint.Width || Ystart > Paint.Height) {
        printf("paint_draw_string Input exceeds the normal display range\r\n");
        return;
    }

    sGlyph glyph;
    // int font_height;
    // int font_width;

    while (* pString != '\0') {

        glyph = Font->glyph_table[* pString - 32];
        // font_height = glyph.height - glyph.yOffset;
        // font_width =  glyph.width + glyph.xOffset;

        // //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        // if ((Xpoint + font_width ) > Paint.Width ) {
        //     Xpoint = Xstart;
        //     Ypoint += font_height;
        // }

        // // If the Y direction is full, reposition to(Xstart, Ystart)
        // if ((Ypoint  + font_height ) > Paint.Height ) {
        //     Xpoint = Xstart;
        //     Ypoint = Ystart;
        // }

        paint_draw_char(Xpoint, Ypoint, * pString, Font, Color_Foreground, Color_Background);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += glyph.xAdvance;
    }
}

/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void paint_draw_bitmap(const unsigned char* image_buffer)
{
    UWORD x, y;
    UDOUBLE Addr = 0;

    for (y = 0; y < Paint.HeightByte; y++) {
        for (x = 0; x < Paint.WidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * Paint.WidthByte;
            Paint.Image[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

/******************************************************************************
function:	Display image
parameter:
    image            ：Image start address
    xStart           : X starting coordinates
    yStart           : Y starting coordinates
    xEnd             ：Image width
    yEnd             : Image height
******************************************************************************/
void paint_draw_image(const unsigned char *image_buffer, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image) 
{
    UWORD Row, Column;

    if (xStart > Paint.Width || yStart > Paint.Height) {
        printf("paint_draw_char Input exceeds the normal display range\r\n");
        return;
    }

    UWORD bit = 0;
    const unsigned char *ptr = image_buffer;

    for (Row = 0; Row < H_Image; Row ++ ) {
        for (Column = 0; Column < W_Image; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (*ptr & (0x80 >> bit)) {
                paint_set_pixel(xStart + Column , yStart + Row, BLACK);
            }

            //go to next bit
            bit++;

            //if next pit is out of range, reset to zero and use next byte
            if (bit == 8) {
                bit = 0;
                ptr++;
            }

        }// Write a line
    }// Write all
}
