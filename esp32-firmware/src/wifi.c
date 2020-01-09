/*
 * wifi.c
 * author(s): khuynh
 * description: 
 */

#include "wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "mdns.h"
#include "freertos/event_groups.h"

static const char *TAG = "[WIFI]";
static EventGroupHandle_t wifi_event_group;
const static int CONNECTED_BIT = BIT0;

/* WIFI connection and mdns service functions */
esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "Wifi connected...\n");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "Wifi got ip...\n");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "Wifi disconnected, attempting to reconnect...\n");
        break;
    default:
        break;
    }
    return ESP_OK;
}

void wifi_init(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_LOGI(TAG, "start the WIFI SSID:[%s]", CONFIG_WIFI_SSID);
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err)
    {
        ESP_LOGI(TAG, "MDNS Init failed: %d\n", err);
        return;
    }

    //set hostname
    mdns_hostname_set(ESP_HOSTNAME);
    //set default instance

    mdns_instance_name_set(ESP_INSTANCE_NAME);
    ESP_LOGI(TAG, "Hostname set: %s\n", ESP_HOSTNAME);
    ESP_LOGI(TAG, "Instance name set: %s\n", ESP_INSTANCE_NAME);
}
