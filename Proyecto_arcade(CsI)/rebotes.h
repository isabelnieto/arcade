#ifndef REBOTES_H_INCLUDED
#define REBOTES_H_INCLUDED

#include "cmsis_os.h"                                           // CMSIS RTOS header file

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"
#include "signals.h"

#define PUERTO_INT   				   	0

#define LINEA_INT_UP				  	23 //(DIP15)
#define LINEA_INT_DOWN					17 //(DIP12)
#define LINEA_INT_LEFT					15 //(DIP13)
#define LINEA_INT_RIGHT					24 //(DIP16)
#define LINEA_INT_CENTER				16 //(DIP14)

extern osThreadId tid_Arcade; // Se env�a al aut�mata una se�al inform�ndole de que bot�n del joystick ha sido pulsado

typedef enum {Subida, Bajada} tipoPulsacion; // declaraci�n de las opciones del switch
tipoPulsacion pulsacion = Subida;

void EINT3_IRQHandler(void);

#endif

