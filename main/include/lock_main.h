#include <stdint.h>

#ifndef LOCK_MAIN
#define LOCK_MAIN

#define LATCH_OPEN      BIT0;
#define BELL            BIT1;
//#define TBUTTON_PRESSED BIT2;
//#define MQTT_OPEN       BIT3;
//#define MQTT_CLOSE      BIT4;

static EventGroupHandle_t lock_event_group;

static uint8_t lock_stop;

#endif
