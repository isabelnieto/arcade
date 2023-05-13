
#include "Zumbador.h"

/*****************************************************************************
 *                           			ZUMBADOR
 *****************************************************************************/

t_zumEstado zumEstado;


void encenderZumbador(void) {
	LPC_PWM1->MR1 = 8000;
}


void apagarZumbador(void) {
	LPC_PWM1->MR1 = 10001;
}
void Init_PWM(void) {
	LPC_SC->PCONP |= 1 << 6;	//Se habilita PWM1

	LPC_PINCON->PINSEL4 |= 1 << 0;
	LPC_PWM1->TCR	= (1 << 0) | (1 << 2);
	LPC_PWM1->PR = 0;
	LPC_PWM1->MCR = 1 << 1;
	LPC_PWM1->MR0	= 10000;

	LPC_PWM1->MR1 = 10001;

	LPC_PWM1->LER |= 1 << 1;		//Se actualizan los valores de los nuevos registros del MR
	LPC_PWM1->PCR |= 1 << 9;
}
