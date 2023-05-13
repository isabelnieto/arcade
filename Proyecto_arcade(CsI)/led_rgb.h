#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "cmsis_os.h"

#define PORT_RGB 2

#define PIN_RED    3 //p23
#define PIN_GREEN  2 //024
#define PIN_BLUE   1 //p25

void Init_RGB(void);
void encender_led(int pin_color);
void apagar_leds(void);

#endif

