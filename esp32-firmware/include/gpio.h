/*
 * gpio.h
 * author(s): khuynh
 * description: 
 */

#ifndef GPIO_H
#define GPIO_H

/*
 * define GPIO pins
 * see: https://github.com/espressif/esp-idf/blob/a20d02b7f196c407bc9f39b781e31a0a4f665968/examples/peripherals/gpio/main/gpio_example_main.c
 */
#define GPIO_OUTPUT_0 18
#define GPIO_OUTPUT_1 2
#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_OUTPUT_0) | (1ULL << GPIO_OUTPUT_1))

/*
 * gpio_config_init
 */
void gpio_config_init();

#endif