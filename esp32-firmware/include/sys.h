#ifndef SYS_H
#define SYS_H

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif

/*
 * sys_print_info
 * 
 * prints system information
 */
void sys_print_info(void);

#endif