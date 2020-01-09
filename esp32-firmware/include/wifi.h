/*
 * wifi.h
 * author(s): khuynh
 * description: 
 */

#ifndef WIFI_H
#define WIFI_H

#include "esp_wifi.h"
#include "esp_err.h"

/*
 * Configure wifi ssid, password, and hostname here
 * set CONFIG_WIFI_SSID to your network SSID
 * set CONFIG_WIFI_PASSWORD to your network password
 * 
 * set ESP_HOSTNAME to the hostname you want your esp32 device to have
 * set ESP_INSTANCE_NAME to a short description of the device
 */
#define CONFIG_WIFI_SSID "vietfi"
#define CONFIG_WIFI_PASSWORD "nezuko95"
#define ESP_HOSTNAME "kesp32-01"
#define ESP_INSTANCE_NAME "kenny's esp32"

/*
 * wifi_event_handler
 * 
 * wifi events:
 *      SYSTEM_EVENT_STA_START
 *      SYSTEM_EVENT_STA_GOT_IP
 *      SYSTEM_EVENT_STA_DISCONNECTED
 * 
 */
esp_err_t wifi_event_handler(void *ctx, system_event_t *event);

/*
 * wifi_init
 * initializes wifi event handler. configure wifi ssid and password using
 * CONFIG_WIFI_SSID and CONFIG_WIFI_PASSWORD macros
 */
void wifi_init(void);

/*
 * start_mdns_service
 * sets hostname and instance name of device. configure using the 
 *  ESP_HOSTNAME and ESP_INSTANCE_NAME macros
 */
void start_mdns_service(void);

/* 
 * _http_event_handler
 * handles http events
//  */
// esp_err_t _http_event_handler(esp_http_client_event_t *evt);

#endif