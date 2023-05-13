#ifndef SNAKE_H
#define SNAKE_H

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"
#include "cmsis_os.h"

//Librerías propias
#include "Signals.h"
#include "lcd.h"

//INICIALIZACIÓN DEL HILO
int Init_Snake(void);
void Snake(void const *argument);
osThreadDef(Snake, osPriorityNormal, 1, 0);
osThreadId tid_Snake;

void moverSerpiente(void);
bool posicionCorrectaMapa(void);
bool noEsSerpiente(void);
bool comerManzana(void);
void colocarManzana(void);
void InitRandomTimer(void);
void moverNextPos(void);
void moverse(void);
bool hayManzana(void);
t_posicion getRandomPos(void);
uint8_t puntuacionSnake(void);
void Init_SnakeArray(void);

/*-----------------------ESTADOS-------------------------*/
typedef enum {DERECHA, IZQUIERDA, ARRIBA, ABAJO} t_direccion;
t_direccion direccionActual;
t_direccion direccionSiguiente;

//TIMER HORA
extern 	osTimerId tid_timerSnake;
extern 	void Init_SnakeTimer(void);
extern osThreadId tid_Arcade;

void pintarTablero(void);

#endif
