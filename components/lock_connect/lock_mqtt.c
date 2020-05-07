#include "lock_connect.h"
#include "lock_main.h"

static const char *TAG = "Lock_MQTT";

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
	client = event->client;
	int msg_id;
	// your_context_t *context = event->context;
	switch (event->event_id) {
		case MQTT_EVENT_CONNECTED:
			ESP_LOGD(TAG, "MQTT_EVENT_CONNECTED");

			msg_id = esp_mqtt_client_subscribe(client, "/door/lock/hand", 0);
			ESP_LOGD(TAG, "sent subscribe successful, msg_id=%d", msg_id);
		break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
		break;
		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGD(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
		break;
		case MQTT_EVENT_UNSUBSCRIBED:
			ESP_LOGD(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
		break;
		case MQTT_EVENT_PUBLISHED:
			ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
		break;
		case MQTT_EVENT_DATA:
			ESP_LOGD(TAG, "MQTT_EVENT_DATA");
			printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
			printf("DATA=%.*s\r\n", event->data_len, event->data);
			if (!memcmp(event->topic, "/door/lock/hand", event->topic_len))
			{
				if (!memcmp(event->data, "open", event->data_len))
				{
					ESP_LOGD(TAG, "Send open latch");
					xEventGroupSetBits(lock_event_group, LATCH_OPEN);
				}else if (!memcmp(event->data, "close", event->data_len)){
					ESP_LOGD(TAG, "Send close latch");
					// TODO: Добавить возможность принудительно закрыть замок
				}
			}
		break;
		case MQTT_EVENT_ERROR:
			ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
		break;
	}
	return ESP_OK;
}

void mqtt_app_start (void)
{
  	xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
	esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .event_handle = mqtt_event_handler,
        // .user_context = (void *)your_context
#if CONFIG_BROKER_AUTH
        .username = CONFIG_BROKER_USER,
        .password = CONFIG_BROKER_PASS,
#endif
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}

void lock_mqtt_task (void* params)
{
	int group_bits;

	while (!lock_stop) {
		group_bits = xEventGroupWaitBits(lock_event_group, LATCH_OPEN | CARD_BLOCKED | BELL, false, true, portMAX_DELAY);
		// TODO: оставлю на будущее..
	}

	ESP_LOGD(TAG, "Stop mqtt task");
	vTaskDelete(NULL);
}

esp_err_t Lock_MQTT_Init ()
{
	ESP_LOGD(TAG, "Init start..");
	mqtt_app_start ();
	//xTaskCreate (lock_mqtt_task, "lock_mqtt_task", configMINIMAL_STACK_SIZE * 2, NULL, 5, NULL);
	ESP_LOGI(TAG, "Init end");
	return ESP_OK;
}
