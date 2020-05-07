#include "lock_main.h"
#include "lock_hardware.h"

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
	xEventGroupSetBits(lock_event_group, LATCH_OPEN);

	return ESP_OK;
}

esp_err_t Lock_Latch_close ()
{
	ESP_LOGD(TAG, "Latch close");
	gpio_set_level(LOCK_LATCH_GPIO, 0);
	xEventGroupClearBits(lock_event_group, LATCH_OPEN);

	return ESP_OK;
}

void lock_latch_task(void* params)
{
	ESP_LOGD(TAG, "Start Latch task..");
	while (!lock_stop) {
		xEventGroupWaitBits(lock_event_group, CARD_ACCESS, true, true, portMAX_DELAY);
		Lock_Latch_open ();
		vTaskDelay(LOCK_LATCH_DELAY / portTICK_RATE_MS);
		Lock_Latch_close ();
	}

	ESP_LOGD(TAG, "Stop Latch task");
	vTaskDelete(NULL);
}

esp_err_t Lock_Latch_Init()
{
	ESP_LOGD(TAG, "Init start..");

	Lock_Latch_init_gpio();

	xTaskCreate(lock_latch_task, "lock_latch_task", configMINIMAL_STACK_SIZE * 2, NULL, 4, NULL);
	ESP_LOGI(TAG, "Init end");

	return ESP_OK;
}
