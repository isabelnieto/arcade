#ifndef PUNTOS_H_INCLUDED
#define PUNTOS_H_INCLUDED

#include "led_rgb.h"
#include "signals.h"
#include "lcd.h"


void puntos(void);
void agregarPerfil(char nombre[]);
void pintar_puntos_LCD(int posicion);
void pintar_puntos_LAST(int posicion);

#endif


