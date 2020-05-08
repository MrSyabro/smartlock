#include "lock_main.h"
#include "driver/gpio.h"

#ifndef LOCK_HARDWARE_H
#define LOCK_HARDWARE_H

#define LOCK_LATCH_GPIO       5
#define LOCK_LATCH_GPIO_SEL   (1ULL<<LOCK_LATCH_GPIO)

#define LOCK_TBUTTON_GPIO     4
#define LOCK_TBUTTON_GPIO_SEL (1ULL<<LOCK_TBUTTON_GPIO)

#define LOCK_LATCH_DELAY      1000

esp_err_t Lock_Latch_Init ();
esp_err_t Lock_TButton_Init ();

esp_err_t Lock_Latch_open();
esp_err_t Lock_Latch_close();

#endif
