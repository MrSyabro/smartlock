#include <stdint.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lock_main.h"
#include "lock_access.h"
#include "lock_connect.h"
#include "lock_hardware.h"

static const char *TAG = "Lock_Main";

void app_main()
{
	ESP_LOGI(TAG, "Startup..");
	ESP_LOGI(TAG, "Free memory: %d bytes", esp_get_free_heap_size());
	ESP_LOGI(TAG, "IDF version: %s", esp_get_idf_version());

	/* MAIN CODE */
	
	lock_stop = 1; // Условие всех циклов

	lock_event_group = xEventGroupCreate();
	
	ESP_LOGI(TAG, "Start initializing...");

	ESP_ERROR_CHECK(Lock_Access_Init ());
	ESP_ERROR_CHECK(Lock_Latch_Init ());
	ESP_ERROR_CHECK(Lock_TButton_Init ());
}
