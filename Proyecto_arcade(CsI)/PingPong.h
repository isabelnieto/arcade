#ifndef PINGPONG_H
#define PINGPONG_H

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"
#include "cmsis_os.h"

//Librerías propias
#include "lcd.h"
#include "Potenciometro.h"
#include "zumbador.h"

//INICIALIZACIÓN DEL HILO
void PingPong(void const *argument);
osThreadDef(PingPong, osPriorityNormal, 1, 0);
osThreadId tid_PingPong;

/*-----------------------PERIFERICOS-------------------------*/
//POTENCIOMETRO
extern 	osThreadId tid_Pot;
extern  int8_t estadoJ2;
extern  int8_t estadoJ1;
extern int Init_Pot(void);

//PELOTA
uint8_t juego_activo;

//TIMERS
extern osTimerId id2;
extern osTimerId id1;
extern uint8_t accionPelota;
extern uint8_t paradaPelota;

//ARCADE
extern osThreadId tid_Arcade;

//PUNTUACIÓN
uint8_t score1 = 0; //Puntuación del Jugador 1
uint8_t score2 = 0; //Puntuación del Jugador 2

//ZUMBADOR
extern void encenderZumbador(void);
extern void apagarZumbador(void);

#endif

