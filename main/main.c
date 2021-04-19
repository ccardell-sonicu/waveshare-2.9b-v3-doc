#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "fonts/custom_fonts.h"
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

extern sFONTCUSTOM aclonica_regular_48;
extern sFONTCUSTOM dejaVu_sans_mono_48;
extern sFONTCUSTOM dejaVu_sans_mono_16;
extern sFONTCUSTOM dejaVu_sans_mono_12;
extern PAINT Paint;

unsigned char red_image_data[4736] = {0};
unsigned char black_image_data[4736] = {0};

void app_main(void)
{
    vTaskDelay(5 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "Init GPIOs\n");
    epd_gpio_config();

    // ESP_LOGI(TAG, "Init screen\n");
    epd_2in9b_v3_init();

    // ESP_LOGI(TAG, "Clear screen\n");
    edp_set_lut();
    epd_2in9b_v3_clear();

    /* Test code starts here */

    // Write hello world to screen
    paint_new_image(black_image_data, 296, 128, ROTATE_0, BLACK);  
    
    paint_draw_string(Paint.Width / 2 - 7 * 8 - 4, 10, "00:12:34:56:78:90", &dejaVu_sans_mono_12, BLACK, WHITE);
    paint_draw_line(10, 14, Paint.Width - 1, 14, BLACK, 1, LINE_STYLE_SOLID);

    //left and right min max in series
    paint_draw_line(Paint.Width / 2, 19, Paint.Width / 2, Paint.Height, BLACK, 1, LINE_STYLE_SOLID); // dotted line to separate sides
    paint_draw_string((Paint.Width / 4) - (7 * 9) - 4, Paint.Height * 3 / 4 + 12, "min: 24 C max: 56 C", &dejaVu_sans_mono_12, BLACK, WHITE);
    paint_draw_string((Paint.Width * 3 / 4) - (7 * 9) - 4,  Paint.Height * 3 / 4 + 12, "min: 25 C max: 54 C", &dejaVu_sans_mono_12, BLACK, WHITE);
  
    // // left min max stacked
    // paint_draw_string((Paint.Width / 4) - (7 * 4) - 4,  Paint.Height * 3 / 4 + 6, "min: 25 C", &Font12, BLACK, WHITE);
    // paint_draw_string((Paint.Width / 4) - (7 * 4) - 4,  Paint.Height * 3 / 4 + 12 + 6, "max: 54 C", &Font12, BLACK, WHITE);

    // //right min max stacked
    // paint_draw_string((Paint.Width * 3 / 4) - (7 * 4) - 4,  Paint.Height * 3 / 4 + 6, "min: 25 C", &Font12, BLACK, WHITE);
    // paint_draw_string((Paint.Width * 3 / 4) - (7 * 4) - 4,  Paint.Height * 3 / 4 + 12 + 6, "max: 54 C", &Font12, BLACK, WHITE);

    // humidity
    paint_draw_string((Paint.Width / 4) - (11 * 3), Paint.Height / 2 + 20, "50% RH", &dejaVu_sans_mono_16, BLACK, WHITE);
    paint_draw_string((Paint.Width * 3 / 4) - (11 * 3), Paint.Height / 2 + 20, "49% RH", &dejaVu_sans_mono_16, BLACK, WHITE);

    // tempature
    paint_draw_string((Paint.Width / 4) - (17 * 3), Paint.Height / 2, "26 C", &dejaVu_sans_mono_48, BLACK, WHITE);
    paint_draw_circle(100, 38, 7, BLACK, 2, DRAW_FILL_EMPTY);

    paint_select_image(red_image_data);
    paint_draw_string((Paint.Width * 3 / 4) - (17 * 3), Paint.Height / 2, "54 C", &dejaVu_sans_mono_48, RED, WHITE);
    paint_draw_circle((Paint.Width / 2) + 100, 38, 7, BLACK, 2, DRAW_FILL_EMPTY);

    // load image to screen
    epd_2in9b_v3_display(black_image_data, red_image_data);

    /* Test code ends here */

    // int i = 0;
    for (;;) {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
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