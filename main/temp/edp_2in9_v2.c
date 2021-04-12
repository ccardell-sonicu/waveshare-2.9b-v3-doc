/*****************************************************************************
* | File      	:  	EPD_2in9_V2.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-10-23
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
#include "edp_2in9_v2.h"
#include "esp_log.h"
#include <stdio.h>


UBYTE _WF_PARTIAL_2IN9[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0A,0x0,0x0,0x0,0x0,0x0,0x2,  
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x22,0x17,0x41,0xB0,0x32,0x36,
};

static const char* TAG = "edp_2in9_v2";

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void epd_2in9_v2_reset(void)
{
	ESP_LOGI(TAG, "reset pin 1");
    gpio_set_level(EPD_RST_PIN, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "reset pin 0");
    gpio_set_level(EPD_RST_PIN, 0);
    vTaskDelay(2 / portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "reset pin 1");
    gpio_set_level(EPD_RST_PIN, 1);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void epd_2in9_v2_send_command(UBYTE Reg)
{
    gpio_set_level(EPD_DC_PIN, 0);
    // gpio_set_level(EPD_CS_PIN, 0);
    spi_write_byte(Reg);
    // gpio_set_level(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void epd_2in9_v2_send_data(UBYTE Data)
{
    gpio_set_level(EPD_DC_PIN, 1);
    // gpio_set_level(EPD_CS_PIN, 0);
    spi_write_byte(Data);
    // gpio_set_level(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void epd_2in9_v2_read_busy(void)
{
    // Debug("e-Paper busy\r\n");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    while(gpio_get_level(EPD_BUSY_PIN) == 0) {      //LOW: idle, HIGH: busy
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    // Debug("e-Paper busy release\r\n");
}

static void epd_2in9_v2_lut(void)
{       
	UBYTE count;
	epd_2in9_v2_send_command(0x32);
	for(count=0; count<153; count++) 
		epd_2in9_v2_send_data(_WF_PARTIAL_2IN9[count]); 
	epd_2in9_v2_read_busy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void epd_2in9_v2_set_windows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    epd_2in9_v2_send_command(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    epd_2in9_v2_send_data((Xstart >> 3) & 0xFF);
    epd_2in9_v2_send_data((Xend >> 3) & 0xFF);

    epd_2in9_v2_send_command(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    epd_2in9_v2_send_data(Ystart & 0xFF);
    epd_2in9_v2_send_data((Ystart >> 8) & 0xFF);
    epd_2in9_v2_send_data(Yend & 0xFF);
    epd_2in9_v2_send_data((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void epd_2in9_v2_set_cursor(UWORD Xstart, UWORD Ystart)
{
    epd_2in9_v2_send_command(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    epd_2in9_v2_send_data(Xstart & 0xFF);

    epd_2in9_v2_send_command(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    epd_2in9_v2_send_data(Ystart & 0xFF);
    epd_2in9_v2_send_data((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void epd_2in9_v2_turn_on_display(void)
{
    epd_2in9_v2_send_command(0x22); // DISPLAY_UPDATE_CONTROL_2
    epd_2in9_v2_send_data(0xF7);
    epd_2in9_v2_send_command(0x20); // MASTER_ACTIVATION
    epd_2in9_v2_read_busy();
}

static void epd_2in9_v2_turn_on_display_partial(void)
{
    epd_2in9_v2_send_command(0x22); // DISPLAY_UPDATE_CONTROL_2
    epd_2in9_v2_send_data(0xFF);
    epd_2in9_v2_send_command(0x20); // MASTER_ACTIVATION
    epd_2in9_v2_read_busy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void epd_2in9_v2_init(void)
{
	ESP_LOGI(TAG, "reset 1");
    epd_2in9_v2_reset();
	ESP_LOGI(TAG, "read busy 1");
	epd_2in9_v2_read_busy();   
	
	ESP_LOGI(TAG, "send command 1");
	// epd_2in9_v2_send_command(0x12);  //SWRESET
	epd_2in9_v2_send_command(0x00);  //SWRESET
	ESP_LOGI(TAG, "read busy 2");
	epd_2in9_v2_read_busy();   

	// ESP_LOGI(TAG, "send command 2");
	// epd_2in9_v2_send_command(0x01); //Driver output control      
	// ESP_LOGI(TAG, "send data 1");
	// epd_2in9_v2_send_data(0x27);
	// ESP_LOGI(TAG, "send data 2");
	// epd_2in9_v2_send_data(0x01);
	// ESP_LOGI(TAG, "send data 3");
	// epd_2in9_v2_send_data(0x00);

	// ESP_LOGI(TAG, "send command 3");
	// epd_2in9_v2_send_command(0x11); //data entry mode       
	// ESP_LOGI(TAG, "send data 4");
	// epd_2in9_v2_send_data(0x03);

	// ESP_LOGI(TAG, "set windows");
	// epd_2in9_v2_set_windows(0, 0, EPD_2IN9_V2_WIDTH-1, EPD_2IN9_V2_HEIGHT-1);

	// ESP_LOGI(TAG, "send command 4");
	// epd_2in9_v2_send_command(0x21); //  Display update control
	// ESP_LOGI(TAG, "send data 5");
	// epd_2in9_v2_send_data(0x00);		
	// ESP_LOGI(TAG, "send data 6");
	// epd_2in9_v2_send_data(0x80);					

	// ESP_LOGI(TAG, "set cursor");
	// epd_2in9_v2_set_cursor(0, 0);
	// ESP_LOGI(TAG, "read busy 3");
	// epd_2in9_v2_read_busy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void epd_2in9_v2_clear(void)
{
	UWORD i;
	epd_2in9_v2_send_command(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		epd_2in9_v2_send_data(0x00);
	}
	epd_2in9_v2_turn_on_display();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void epd_2in9_v2_display(UBYTE *Image)
{
	UWORD i;	
	epd_2in9_v2_send_command(0x24);   //write RAM for black(0)/white (1)
	for(i=0;i<4736;i++)
	{
		epd_2in9_v2_send_data(Image[i]);
	}
	epd_2in9_v2_turn_on_display();	
}

void epd_2in9_v2_display_base(UBYTE *Image)
{
	UWORD i;   

	epd_2in9_v2_send_command(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		epd_2in9_v2_send_data(Image[i]);
	}
	epd_2in9_v2_send_command(0x26);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{               
		epd_2in9_v2_send_data(Image[i]);
	}
	epd_2in9_v2_turn_on_display();	
}

void epd_2in9_v2_display_partial(UBYTE *Image)
{
	UWORD i;

//Reset
    gpio_set_level(EPD_RST_PIN, 0);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    gpio_set_level(EPD_RST_PIN, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
		
	epd_2in9_v2_lut();
	epd_2in9_v2_send_command(0x37); 
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x00); 
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x40);  
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x00);   
	epd_2in9_v2_send_data(0x00);  
	epd_2in9_v2_send_data(0x00);

	epd_2in9_v2_send_command(0x3C); //BorderWavefrom
	epd_2in9_v2_send_data(0x80);	

	epd_2in9_v2_send_command(0x22); 
	epd_2in9_v2_send_data(0xC0);   
	epd_2in9_v2_send_command(0x20); 
	epd_2in9_v2_read_busy();  
	
	epd_2in9_v2_set_windows(0, 0, EPD_2IN9_V2_WIDTH-1, EPD_2IN9_V2_HEIGHT-1);
	epd_2in9_v2_set_cursor(0, 0);

	epd_2in9_v2_send_command(0x24);   //Write Black and White image to RAM
	for(i=0;i<4736;i++)
	{
		epd_2in9_v2_send_data(Image[i]);
	} 
	epd_2in9_v2_turn_on_display_partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void epd_2in9_v2_sleep(void)
{
    epd_2in9_v2_send_command(0x10);
    epd_2in9_v2_send_data(0x01);
    epd_2in9_v2_read_busy();
}
