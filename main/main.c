#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "main.h"
#include "utility/edp_2in9b_v3.h"
#include "gui/gui_paint.h"
#include "esp_log.h"
#include "soc/io_mux_reg.h"
#include "test_screens/test_screens.h"
#include "luts/luts.h"

static const char* TAG = "main";

// extern const unsigned char horizontal_quadrants[];
// extern const unsigned char vertical_quadrants[];
// extern const unsigned char four_quadrants[];
extern const unsigned char black_image[];
extern const unsigned char red_image[];
extern sFONT Font8;
extern sFONT Font12;
extern sFONT Font16;
extern sFONT Font20;
extern sFONT Font24;
extern PAINT Paint;

unsigned char red_image_data[4736] = {0};
unsigned char black_image_data[4736] = {0};

void app_main(void)
{
    vTaskDelay(50 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "Init GPIOs\n");
    epd_gpio_config();

    ESP_LOGI(TAG, "Init screen\n");
    epd_2in9b_v3_init();

    ESP_LOGI(TAG, "Clear screen\n");
    edp_set_lut();
    epd_2in9b_v3_clear();

    /* Test code starts here */

    // Write hello world to screen
    Paint.Image = black_image_data;
    Paint.Width = 296;
    Paint.Height = 128; 
    Paint.WidthMemory = Paint.Width;
    Paint.HeightMemory = Paint.Height; 
    Paint.Color =  BLACK;  
    Paint.Rotate = 0;
    Paint.Mirror = MIRROR_NONE; 
    Paint.WidthByte = Paint.Width / 8;
    Paint.HeightByte = Paint.Height / 8;   
    Paint.Scale = 2;

    //paint_draw_point(10, 10, WHITE, 1, DOT_FILL_AROUND);

    // paint_draw_string(10, 10, ".", &Font8, BLACK, WHITE);
    paint_draw_string(10, 10, "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ", &Font12, BLACK, WHITE);
    // paint_draw_string(10, 24, "0123456789", &Font16, BLACK, WHITE);
    //paint_draw_string(10, 42, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", &Font16, BLACK, WHITE);
    // paint_draw_string(10, 60, "0123456789", &Font20, BLACK, WHITE);
    // paint_draw_string(10, 82, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", &Font20, BLACK, WHITE);
    // paint_draw_string(10, 104, "0123456789", &Font24, BLACK, WHITE);

    paint_draw_line(10, 30, Paint.Width - 10, 30, BLACK, 2, LINE_STYLE_DOTTED);
    paint_draw_circle(50, 50, 10, BLACK, 2, DRAW_FILL_FULL);
    paint_draw_rectangle(70, 70, 90, 90, BLACK, 2, DRAW_FILL_FULL);

    Paint.Image = red_image_data;

    paint_draw_circle(200, 50, 10, BLACK, 2, DRAW_FILL_EMPTY);
    paint_draw_rectangle(200, 70, 270, 88, BLACK, 2, DRAW_FILL_EMPTY);

    paint_draw_line(10, 120, Paint.Width - 10, 90, BLACK, 2, LINE_STYLE_SOLID);


    // int j;
    // printf("\n\n\n");
    // for (j = 0; j < 4736; j++) {
    //     printf("0x%02hhX, ", Paint.Image[j]);
    //     if ((j + 1) % 16 == 0) printf("\n");
    // }
    // printf("\n\n\n");

    epd_2in9b_v3_display(black_image_data, red_image_data);


    // ESP_LOGI(TAG, "Print black white red image to screen\n");
    // epd_2in9b_v3_display(black_image, red_image);
    // vTaskDelay(500 / portTICK_PERIOD_MS);

    /* Test code ends here */

    int i = 0;
    for (;;) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // ESP_LOGI(TAG, "loop %d\n", i++);
        printf(".");
    }
}


void epd_gpio_config(void)
{
    // pinMode(EPD_BUSY_PIN,  INPUT);
    gpio_set_direction(EPD_BUSY_PIN, GPIO_MODE_INPUT);
    // pinMode(EPD_RST_PIN , OUTPUT);
    gpio_set_direction(EPD_RST_PIN, GPIO_MODE_OUTPUT);
    // pinMode(EPD_DC_PIN  , OUTPUT);
    gpio_set_direction(EPD_DC_PIN, GPIO_MODE_OUTPUT);

    
    // pinMode(EPD_MOSI_PIN, OUTPUT);
    gpio_set_direction(EPD_MOSI_PIN, GPIO_MODE_OUTPUT);
    gpio_iomux_out(EPD_MOSI_PIN, FUNC_MTMS_GPIO14, false);
    // pinMode(EPD_SCK_PIN, OUTPUT);
    gpio_set_direction(EPD_SCK_PIN, GPIO_MODE_OUTPUT);
    gpio_iomux_out(EPD_SCK_PIN, FUNC_MTCK_GPIO13, false);
    // pinMode(EPD_CS_PIN , OUTPUT);
    gpio_set_direction(EPD_CS_PIN, GPIO_MODE_OUTPUT);
    gpio_iomux_out(EPD_CS_PIN, FUNC_MTDO_GPIO15, false);

    // digitalWrite(EPD_CS_PIN , HIGH);
    gpio_set_level(EPD_CS_PIN, 1);
    // digitalWrite(EPD_SCK_PIN, LOW);
    gpio_set_level(EPD_SCK_PIN, 0);

}

/******************************************************************************
function:
			SPI read and write
******************************************************************************/
void spi_write_byte(UBYTE data)
{

    // digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);
    // gpio_set_level(EPD_CS_PIN, 0);


    for (int i = 0; i < 8; i++)
    {
        // if ((data & 0x80) == 0) digitalWrite(EPD_MOSI_PIN, GPIO_PIN_RESET); 
        if ((data & 0x80) == 0) gpio_set_level(EPD_MOSI_PIN, 0); 
        // else                    digitalWrite(EPD_MOSI_PIN, GPIO_PIN_SET);        
        else                    gpio_set_level(EPD_MOSI_PIN, 1);

        data <<= 1;
        // digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        gpio_set_level(EPD_SCK_PIN, 1);
        // digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
        gpio_set_level(EPD_SCK_PIN, 0);

    }

    // digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
    // gpio_set_level(EPD_CS_PIN, 1);

}

void spi_read_byte(void)
{
    UBYTE data;

    for (int j = 0; j < 0xFFF; j ++) 
    {
        data = 0;
        for (int i = 0; i < 8; i++)
        {
            // if ((data & 0x80) == 0) digitalWrite(EPD_MOSI_PIN, GPIO_PIN_RESET); 
            if (gpio_get_level(EPD_MOSI_PIN) == 0) 
                data |= 0;
            else                    
                data |= 1;
            
            data <<= 1;
        
            // digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
            gpio_set_level(EPD_SCK_PIN, 1);
            // digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
            gpio_set_level(EPD_SCK_PIN, 0);
        }
        printf("%X ", data);
    }
    printf("\n\n");
}