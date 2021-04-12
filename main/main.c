#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "utility/edp_2in9b_v3.h"
#include "gui/gui_paint.h"
#include "esp_log.h"
#include "soc/io_mux_reg.h"

static const char* TAG = "main";


void app_main(void)
{

    vTaskDelay(500 / portTICK_PERIOD_MS);

    // initialize gpios
    ESP_LOGI(TAG, "Init GPIOs\n");
    epd_gpio_config();

    ESP_LOGI(TAG, "Init screen\n");
    epd_2in9b_v3_init();

    // ESP_LOGI(TAG, "Clear screen\n");
    // epd_2in9b_v3_clear();

    // printf("clearing to black\n");
    // paint_clear(BLACK);

    // printf("printing hello world\n");
    // paint_draw_string(10, 20, "hello world", &Font12, WHITE, BLACK);
    // printf("done\n");
    ESP_LOGI(TAG, "print screen\n");
    epd_2in9b_v3_display(NULL, NULL);


    // gpio_set_direction(EPD_SCK_PIN, GPIO_MODE_OUTPUT);
    // gpio_iomux_out(EPD_SCK_PIN, FUNC_MTCK_GPIO13, false);

    int i = 0;
    for (;;) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // gpio_set_level(EPD_SCK_PIN, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // gpio_set_level(EPD_SCK_PIN, 0);
        ESP_LOGI(TAG, "loop %d\n", i++);

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