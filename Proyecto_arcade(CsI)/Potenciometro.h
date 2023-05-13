#ifndef JOY_H
#define JOY_H

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

#include <lpc17xx.h>
#include <stdio.h>
#include "Signals.h"

//Puertos Joytic Der(2)
#define PUERTODER 1
#define PIN_POTY_2 30   //PIN 19
#define PIN_POTX_2 31   //PIN 20

//Puertos Joytic IZ(1)
#define PUERTOIZ 0
#define PIN_POTY_1 25   //PIN17
#define PIN_POTX_1 26   //PIN18


extern osThreadId tid_PingPong;
extern int8_t estadoJ2; // A1 3,A2 2,A3 1,C 0,B1 -1,B2 -2,B3 -3
extern int8_t estadoJ1; // A1 3,A2 2,A3 1,C 0,B1 -1,B2 -2,B3 -3

//Funciones
void ejeY2(void);
void ejeY1(void);
#endif
