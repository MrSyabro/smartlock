#ifndef LOCK_HARDWARE
#define LOCK_HARDWARE

#define LOCK_LATCH_GPIO       2
#define LOCK_LATCH_GPIO_SEL   (1ULL<<LOCK_LATCH_GPIO)

#define LOCK_TBUTTON_GPIO     4
#define LOCK_TBUTTON_GPIO_SEL (1ULL<<LOCK_TBUTTON_GPIO)

esp_err_t Lock_Latch_Init ();
esp_err_t Lock_TButton_Init ();

#endif
