#include "lock_main.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

#ifndef LOCK_WIFI_H
#define LOCK_WIFI_H

EventGroupHandle_t wifi_event_group;
#define CONNECTED_BIT BIT0

esp_err_t Lock_WiFi_Init ();
esp_err_t Lock_MQTT_Init ();

#endif
