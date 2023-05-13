#include "led_rgb.h"
#include "cmsis_os.h"        // CMSIS RTOS header file
#include "signals.h"

/*
 * Inicializaci�n del led RGB para los 3 colores
 */
void Init_RGB(void) {

	GPIO_SetDir(PORT_RGB, PIN_RED,   GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, PIN_GREEN, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PORT_RGB, PIN_BLUE,  GPIO_DIR_OUTPUT);

}

/*
 * Funci�n que enciende el led RGB en funci�n del color que le pasemso por par�metro
 */
void encender_led(int pin_color) {

	apagar_leds();
	GPIO_PinWrite(PORT_RGB, pin_color, 0); //Funciona a nivel bajo

}

/*
 * Funci�n que apaga todos los colores del RGB
 */
void apagar_leds(void) {

	GPIO_PinWrite(PORT_RGB, PIN_RED,   1);
	GPIO_PinWrite(PORT_RGB, PIN_GREEN, 1);
	GPIO_PinWrite(PORT_RGB, PIN_BLUE,  1);

}



