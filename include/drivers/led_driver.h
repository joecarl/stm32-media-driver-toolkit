#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

void Init_Leds(void);

void LED_On(uint8_t led);

void LED_Off(uint8_t led);

void LED_Toggle(uint8_t led);

#endif
