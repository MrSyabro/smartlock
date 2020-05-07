#include "lock_main.h"
#include "lock_hardware.h"

static const char *TAG = "Lock_TButton";

xQueueHandle gpio_evt_queue = NULL;

void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void lock_tbutton_gpio_task(void *arg)
{
    ESP_LOGD(TAG, "Start task");

	uint32_t io_num;

    while (!lock_stop) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == LOCK_TBUTTON_GPIO)
			{
				int msg_id;
				if (gpio_get_level(io_num)) msg_id = esp_mqtt_client_publish(client, "/door/bell", "1", 0, 0, 0);
				else msg_id = esp_mqtt_client_publish(client, "/door/bell", "0", 0, 0, 0);
				ESP_LOGD(TAG, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
				ESP_LOGD(TAG, "sent publish successful, msg_id=%d", msg_id);
			}
        }
    }

	vTaskDelete(NULL);
}

void Lock_TButton_init_gpio ()
{
	ESP_LOGD(TAG, "Init gpio");

	gpio_config_t io_conf;
	//interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = LOCK_TBUTTON_GPIO_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    gpio_set_intr_type(LOCK_TBUTTON_GPIO, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(lock_tbutton_gpio_task, "lock_tbutton_gpio_task", configMINIMAL_STACK_SIZE * 2, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(0);

    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(LOCK_TBUTTON_GPIO, gpio_isr_handler, (void *) LOCK_TBUTTON_GPIO);
}

esp_err_t Lock_TButton_Init ()
{
	ESP_LOGD(TAG, "Init start..");
	Lock_TButton_init_gpio ();
	ESP_LOGI(TAG, "Init end");

	return ESP_OK;
}
