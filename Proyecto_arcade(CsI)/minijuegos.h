#ifndef MINIJUEGOS_H
#define MINIJUEGOS_H

#include "led_rgb.h"
#include "lcd.h"
#include "signals.h"

//____________SNAKE______________
extern int Init_Snake(void);
extern uint8_t puntuacionSnake(void);

//____________PINGPONG_____________
extern osThreadId tid_PingPong;
extern int Init_PingPong(void);
extern uint8_t score1;
extern uint8_t score2;

//Puntuación
extern void sumar_puntos(int puntos, int posicion);


void minijuegos(void);
void listarMinijuegosLCD(void);
int elegirJugador(void);
void juego_Serpiente(void);
void juego_PingPong(void);





#endif

