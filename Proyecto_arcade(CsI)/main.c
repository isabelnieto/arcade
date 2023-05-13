/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "led_rgb.h"


extern void LCD_Initialize(void);
extern int Init_Rebotes(void);
extern int Init_Arcade(void);
extern void InitRandomTimer(void);
extern void Init_TimersPelota(void);
extern void Init_PWM(void);
extern void 	Init_SnakeTimer(void);


/*
 * main: initialize and start the system
 */
int main(void) {
	osKernelInitialize();                     // initialize CMSIS-RTOS

	// initialize peripherals here
	Init_SnakeTimer();
	Init_Rebotes();
	Init_RGB();
	apagar_leds();
	LCD_Initialize();
	Init_Arcade();
	InitRandomTimer();
	Init_TimersPelota();
	Init_PWM();

	osKernelStart();                          // start thread execution
}
