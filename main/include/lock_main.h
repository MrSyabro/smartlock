#include <stdint.h>

#ifndef LOCK_MAIN
#define LOCK_MAIN

#define CARD_ACCESSED   BIT0;
#define CARD_BLOCKED    BIT1;
#define TBUTTON_PRESSED BIT2;
#define MQTT_OPEN       BIT3;
#define MQTT_CLOSE      BIT4;

static EventGroupHandle_t lock_event_group;

#endif
