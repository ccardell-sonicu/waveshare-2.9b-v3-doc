/*****************************************************************************
* | File      	:   EPD_2in9b_V3.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper b V3
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-04-12
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "edp_2in9b_v3.h"
#include "esp_log.h"
#include <stdio.h>

static const char* TAG = "edp_2in9b_v3";

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void epd_2in9b_v3_reset(void)
{
    gpio_set_level(EPD_RST_PIN, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    gpio_set_level(EPD_RST_PIN, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    gpio_set_level(EPD_RST_PIN, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void epd_2in9b_v3_send_read_command(UBYTE Reg)
{
    gpio_set_level(EPD_DC_PIN, 0);
    gpio_set_level(EPD_CS_PIN, 0);
    spi_write_byte(Reg);

    gpio_set_level(EPD_DC_PIN, 1);
    spi_read_byte();

    gpio_set_level(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void epd_2in9b_v3_send_command(UBYTE Reg)
{
    gpio_set_level(EPD_DC_PIN, 0);
    gpio_set_level(EPD_CS_PIN, 0);
    spi_write_byte(Reg);
    gpio_set_level(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void epd_2in9b_v3_send_data(UBYTE Data)
{
    gpio_set_level(EPD_DC_PIN, 1);
    gpio_set_level(EPD_CS_PIN, 0);
    spi_write_byte(Data);
    gpio_set_level(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void epd_2in9b_v3_read_busy(void)
{
    // Debug("e-Paper busy\r\n");
    UBYTE busy;
	do
	{
        // ESP_LOGI(TAG, "get status command sent\n");
		// epd_2in9b_v3_send_command(0x71); // get status
        vTaskDelay(20 / portTICK_PERIOD_MS);

		busy = gpio_get_level(EPD_BUSY_PIN);
		busy =!(busy & 0x01);        
	}
	while(busy); 
    // Debug("e-Paper busy release\r\n");
    vTaskDelay(200 / portTICK_PERIOD_MS);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void epd_2in9b_v3_init(void)
{
    epd_2in9b_v3_reset();

   //waiting for the electronic paper IC to release the idle signal
    // epd_2in9b_v3_send_command(0x05); //power on measure

    epd_2in9b_v3_send_command(0x01);//power setting
    epd_2in9b_v3_send_data (0x03); //VDS_EN, VDG_EN
    epd_2in9b_v3_send_data (0x00); //VCOM_HV, VGHL_LV
    epd_2in9b_v3_send_data (0x2b); //VDH
    epd_2in9b_v3_send_data (0x2b); //VDL
    epd_2in9b_v3_send_data (0x03); //VDHR

    epd_2in9b_v3_send_command(0x06); //booster soft start
    epd_2in9b_v3_send_data (0x17); 
    epd_2in9b_v3_send_data (0x17); 
    epd_2in9b_v3_send_data (0x17); 

    epd_2in9b_v3_send_command(0x04); //power on
    epd_2in9b_v3_read_busy();

    epd_2in9b_v3_send_command(0x00);//panel setting
    epd_2in9b_v3_send_data(0xBB);//128x296, LUT from REG，KW/R mode, gate scan up, source shift right, booster on, no RST

    // epd_2in9b_v3_send_command(0x30);//PLL control
    // epd_2in9b_v3_send_data (0x3C);

    epd_2in9b_v3_send_command(0x50);//VCOM and data interval
    epd_2in9b_v3_send_data(0xC7);   //WBmode:VBDF 17|D7 VBDW 97 VBDB 57 //This has to do with border data
                                    //WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
    edp_set_lut();
    epd_2in9b_v3_read_busy();


    // epd_2in9b_v3_send_read_command(0xA2);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void epd_2in9b_v3_clear(void)
{
    UWORD Width = (EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V3_HEIGHT;

    // send black data
    epd_2in9b_v3_send_command(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            epd_2in9b_v3_send_data(0xFF);
        }
    }

    // send red data
    epd_2in9b_v3_send_command(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            epd_2in9b_v3_send_data(0x00);
        }
    }
    
    epd_2in9b_v3_send_command(0x12); //refresh screen
    epd_2in9b_v3_read_busy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void epd_2in9b_v3_display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9B_V3_WIDTH % 8 == 0)? (EPD_2IN9B_V3_WIDTH / 8 ): (EPD_2IN9B_V3_WIDTH / 8 + 1);
    Height = EPD_2IN9B_V3_HEIGHT;

    epd_2in9b_v3_send_command(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            epd_2in9b_v3_send_data(blackimage[i + j * Width]);
        }
    }
    // epd_2in9b_v3_send_command(0x92);
    
    epd_2in9b_v3_send_command(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            epd_2in9b_v3_send_data(ryimage[i + j * Width]);
        }
    }
    // epd_2in9b_v3_send_command(0x92);

    epd_2in9b_v3_send_command(0x12);
    epd_2in9b_v3_read_busy();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void epd_2in9b_v3_sleep(void)
{
    epd_2in9b_v3_send_command(0x02); // POWER_OFF
    epd_2in9b_v3_read_busy();
    epd_2in9b_v3_send_command(0x07); // DEEP_SLEEP
    epd_2in9b_v3_send_data(0xA5); // check code
}
