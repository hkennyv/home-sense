/* Hello World Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "bme280.h"
#include "gpio.h"
#include "i2c.h"
#include "sys.h"
#include "wifi.h"

void app_main(void)
{
    sys_print_info();

    // init
    gpio_config_init();
    nvs_flash_init();
    wifi_init();
    start_mdns_service();

    ESP_ERROR_CHECK(i2c_master_init());

    uint8_t toggle = 0;
    // for (int i = 10; i >= 0; i--)
    for (;;)
    {
        // printf("Restarting in %d seconds...\n", i);
        toggle ^= 0x01;
        printf("howdy!\n");
        gpio_set_level(GPIO_OUTPUT_1, toggle);
        gpio_set_level(GPIO_OUTPUT_0, !toggle);
        bme280_read_id();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
