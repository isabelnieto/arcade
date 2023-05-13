#ifndef ZUMBADOR_H
#define	ZUMBADOR_H

#include "LPC17xx.h"

typedef enum {ZUM_APAGADO, ZUM_ENCENDIDO} t_zumEstado;

void 	Init_Zumbador(void);

void 	encenderZumbador(void);
void 	apagarZumbador(void);

int 	conocerZumEstado(void);
#endif
