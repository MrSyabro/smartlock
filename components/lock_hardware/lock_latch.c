#include <stdint.h>
#include "lock_main.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

static const char *TAG = "Lock_Latch";

esp_err_t Lock_Latch_init_gpio ()
{
	ESP_LOGD(TAG, "Init gpio");

	gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = LOCK_LATCH_GPIO_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

	return ESP_OK;
}

esp_err_t Lock_Latch_open ()
{
	ESP_LOGD(TAG, "Latch open");
	gpio_set_level(LOCK_LATCH_GPIO, 1);
	return ESP_OK;
}

esp_err_t Lock_Latch_close ()
{
	ESP_LOGD(TAG, "Latch close");
	gpio_set_level(LOCK_LATCH_GPIO, 0);
	return ESP_OK;
}

esp_err_t lock_latch_task(void* params)
{
	ESP_LOGD(TAG, "Start Latch task..");
	uint8_t group_bits;
	while (lock_stop){
		group_bits = xEventGroupWaitBits(lock_event_group, OPEN_LATCH, true, true, portMAX_DELAY);
		if ((group_bits & OPEN_LATCH) == OPEN_LATCH)
		{
			Lock_Latch_open ();
			vTaskDelay(LATCH_OPEN_DELAY / portTICK_RATE_MS);
			Lock_Latch_close ();
		}
	}

	ESP_LOGD(TAG, "Stop Latch task");
	vTaskDelete(NULL);
}

esp_err_t Lock_Latch_Init()
{
	ESP_LOGD(TAG, "Init start..");

	Lock_Latch_init_gpio();

	ESP_LOGI(TAG, "Init end");

	xTaskCreate(lock_latch_task, "lock_latch_task", configMINIMAL_STACK_SIZE * 1, NULL, 5, NULL);

	return ESP_OK;
}
