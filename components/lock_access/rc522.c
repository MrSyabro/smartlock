#include "lock_main.h"
#include "lock_access.h"
#include "rc522.h"

/* TODO: Сделать нормальное хранение разрешенных к открытию карт..			*/
/* {0x7b, 0x1d, 0xc3, 0x1b, 0xbe} // ID карты, которой все можно			*/

static const char *TAG = "Lock_RC522";
static const uint8_t UID[MFRC522_MAX_LEN] = {0x7b, 0x1d, 0xc3, 0x1b, 0xbe, 0}; // ID карты, которой все можно

esp_err_t print_chip_version ()
{
	char *name = heap_caps_calloc(17, sizeof(char), MALLOC_CAP_8BIT);
	//memset(&name, 0x0, sizeof(name));
	// Считываем версия прошивки чипа и проверяем соединение
	uint32_t ver = MFRC522_ReadRegister(MFRC522_REG_VERSION);
	switch(ver) {
		case 0x88: name = "clone"; break;
		case 0x90: name = "v0.0"; break;
		case 0x91: name = "v1.0"; break;
		case 0x92: name = "v2.0"; break;
		case 0x12: name = "counterfeit chip"; break;
		default:   name = "Chip not found"; ver = 0;
	}
	
	ESP_LOGI(TAG, "RC522 version: %s (0x%0x)", name, ver);
	
	if (ver == 0) return ESP_FAIL;
	return ESP_OK;
}

void lock_access_rc522_task (void *pvParameter)
{
	ESP_LOGD(TAG, "Start task..");
	// Ципл проверки карты и вывода ID, если карта считана
	uint8_t uid[MFRC522_MAX_LEN];
	uint8_t status;
	while (!lock_stop){
		status = MFRC522_Check(&uid); // Проверяем карту
		
		if (status == MI_OK){
			if (memcmp(UID, uid, MFRC522_MAX_LEN) == 0) {
				// Кагбэ доступ получен.
				ESP_LOGD(TAG, "UID done. Open");
				xEventGroupSetBits(lock_event_group, CARD_ACCESS);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}else{
				ESP_LOGD(TAG, "UID fail");
				xEventGroupSetBits(lock_event_group, CARD_BLOCKED);
			}
		}
		
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	
	vTaskDelete(NULL);
}

esp_err_t Lock_Access_Init ()
{
	MFRC522_Init ();
	
	if (print_chip_version () != ESP_OK) return ESP_FAIL;
	
	xTaskCreate (lock_access_rc522_task, "lock_access_rc522_task", configMINIMAL_STACK_SIZE * 2, NULL, 5, NULL);
	
	return ESP_OK;
}
