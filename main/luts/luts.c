#include "luts.h"
#include "utility/edp_2in9b_v3.h"

#define LUT_FOR_VCOM                                0x20 
#define LUT_WHITE_TO_WHITE                          0x21
#define LUT_BLACK_TO_WHITE                          0x22
#define LUT_WHITE_TO_BLACK                          0x23
#define LUT_BLACK_TO_BLACK                          0x24

/**
 *  @brief: set the look-up table
 */

const UBYTE lut_vcom0[] =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,        
0x00, 0x17, 0x17, 0x00, 0x00, 0x02,        
0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,        
0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
};

const UBYTE lut_vcom0_quick[] =
{
0x00, 0x0E, 0x00, 0x00, 0x00, 0x01,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
};


const UBYTE lut_ww[] =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const UBYTE lut_ww_quick[] =
{
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //2
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //3
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //4
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //5
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //6

//original     
0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //3
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //4
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //5
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //6
};


const UBYTE lut_bw[] =
{
0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,        
0x80, 0x01, 0x00, 0x00, 0x00, 0x00,        
};

const UBYTE lut_bw_quick[] =
{
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //2
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //3
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //4
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //5
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //6
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //7
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //8
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //9
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //10

//original 

0xA0, 0x0E, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //3
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //4
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //5
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //6
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //7        
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //8       
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //9       
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //10       
};


const UBYTE lut_bb[] =
{
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const UBYTE lut_bb_quick[] =
{ 
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //2
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //3
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //4
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //5
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //6

//original   
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //3
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //4
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //5
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //6
};


const UBYTE lut_wb[] =
{
0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const UBYTE lut_wb_quick[] =
{
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //2
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //3
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //4
0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //5
//0x00, 0x01, 0x00, 0x00, 0x00, 0x01, //6

//original 
0x50, 0x0E, 0x00, 0x00, 0x00, 0x01, //1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //3
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //4
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //5
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //6
};

void edp_set_lut(void) {
    unsigned int count;     
    epd_2in9b_v3_send_command(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(lut_vcom0[count]);
    }
    
    epd_2in9b_v3_send_command(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_ww[count]);
    }   
    
    epd_2in9b_v3_send_command(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 60; count++) {
        epd_2in9b_v3_send_data(lut_bw[count]);
    } 

    epd_2in9b_v3_send_command(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_wb[count]);
    } 

    epd_2in9b_v3_send_command(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_bb[count]);
    } 
}

void edp_set_reduced_lut(void) {
    unsigned int count;     
    epd_2in9b_v3_send_command(LUT_FOR_VCOM);                            //vcom
    for(count = 0; count < 60; count++) {
        // epd_2in9b_v3_send_data(lut_vcom0_quick[count]);
        epd_2in9b_v3_send_data(0x00);
    }
    
    epd_2in9b_v3_send_command(LUT_WHITE_TO_WHITE);                      //ww --
    for(count = 0; count < 36; count++) {
        epd_2in9b_v3_send_data(lut_ww_quick[count]);
        // epd_2in9b_v3_send_data(0x00);
    }   
    
    epd_2in9b_v3_send_command(LUT_BLACK_TO_WHITE);                      //bw r
    for(count = 0; count < 60; count++) {
        // epd_2in9b_v3_send_data(lut_bw_quick[count]);
        epd_2in9b_v3_send_data(0x00);
    } 

    epd_2in9b_v3_send_command(LUT_WHITE_TO_BLACK);                      //wb w
    for(count = 0; count < 36; count++) {
        // epd_2in9b_v3_send_data(lut_wb_quick[count]);
        epd_2in9b_v3_send_data(0x00);
    } 

    epd_2in9b_v3_send_command(LUT_BLACK_TO_BLACK);                      //bb b
    for(count = 0; count < 36; count++) {
        // epd_2in9b_v3_send_data(lut_bb_quick[count]);
        epd_2in9b_v3_send_data(0x00);
    } 
}