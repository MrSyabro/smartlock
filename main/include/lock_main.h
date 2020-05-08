#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#ifndef LOCK_MAIN
#define LOCK_MAIN

#define LATCH_OPEN      BIT0
#define BELL            BIT1
#define CARD_BLOCKED    BIT2
#define CARD_ACCESS     BIT3
#define MQTT_ACCESS     BIT4

EventGroupHandle_t lock_event_group;

static uint8_t lock_stop;

esp_mqtt_client_handle_t client;

#endif
