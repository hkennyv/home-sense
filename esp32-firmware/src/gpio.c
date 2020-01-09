/*
 * gpio.c
 * author(s): khuynh
 * description: 
 */

#include "gpio.h"
#include "driver/gpio.h"

void gpio_config_init()
{
    gpio_config_t io_conf;

    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    // bit mask of pins that you want to set
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    // disable pull-down mode and pull-up mode
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    // configure GPIO with io_conf
    gpio_config(&io_conf);

    // turn on blue LED on init success
    gpio_set_level(GPIO_OUTPUT_1, 1);
}
