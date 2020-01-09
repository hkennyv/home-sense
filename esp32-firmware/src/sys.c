/*
 * sys.c
 * author(s): khuynh
 * description: 
 */
#include <stdio.h>

#include "esp_system.h"
#include "esp_spi_flash.h"

#include "sys.h"

void sys_print_info(void)
{
    printf("Hello world!\n");
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
           CHIP_NAME,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}