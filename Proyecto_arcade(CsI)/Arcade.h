#ifndef ARCADE_H
#define ARCADE_H

#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "LPC17xx.h"
#include "cmsis_os.h"
#include "signals.h"
#include "puntos.h"
#include "minijuegos.h"
#include "nuevo_perfil.h"
#include "lcd.h"

/*-----------------------ESTADOS-------------------------*/
typedef enum {PUNTOS, MINIJUEGOS, NUEVO_PERFIL} t_estado;
t_estado estados;


void escribirLCDMenu(void);


#endif

