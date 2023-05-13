#include "Potenciometro.h"
/*****************************************************************************
 *                           POTENCIOMETRO
 *****************************************************************************/

void Potenciometro(void const *argument);                              // thread function
osThreadDef(Potenciometro, osPriorityNormal, 1, 0);                    // thread object
osThreadId tid_Pot;

uint16_t  resADCY1;
uint16_t  resADCY2;
int8_t  estadoJ1;
int8_t  estadoJ2;
typedef enum {POT_OFF, POT_ON} t_estadoPot;
t_estadoPot estadoPot;


/**
**Inicialización del los potenciómetos.
  - Asignación del los pines y puertos correpondientes los
potenciómetros usados para el minijuego colocados en los pines 19 y 20
  - Inicialización del hilo correspondiente a los potenciómetros
**/
int Init_Pot(void) {
	estadoPot = POT_OFF;

	LPC_SC->PCONP |= (1 << 12);              // Habilitamos el reloj para el controlador del ADC
	LPC_PINCON->PINSEL3 |= (1 << 28) | (1 << 29) | (1 << 30) | (1U << 31);
	LPC_ADC->ADCR |= (1 << 21) | (10 << 8);  // Ponemos el reloj y el encendido del módulo ADC

	resADCY2 = 0;

	GPIO_SetDir(PUERTODER, PIN_POTX_2, GPIO_DIR_INPUT);
	GPIO_SetDir(PUERTODER, PIN_POTY_2, GPIO_DIR_INPUT);

	//Inicializar el hilo
	tid_Pot = osThreadCreate(osThread(Potenciometro), NULL);
	if(!tid_Pot) return(-1);

	return(0);
}


/**
**Función principal:
  - Manejo de los dos estados del potenciómetro ON / OFF
  - Multiplexación de los potenciómetros para obtener infoemación de los dos
  - Transformacón de la información obtenida de los potenciómetos en un valor
adecuado para la variaón de las barras del juego
  **/
void Potenciometro(void const *argument) {
	osEvent evt;
	while(1) {


		switch(estadoPot) {
			case POT_OFF:
				osSignalWait(SIGNAL_ENCENDER_POTENCIOMETRO, osWaitForever);
				estadoPot = POT_ON;
				break;

			default: //POT_ON
				evt = osSignalWait(SIGNAL_APAGAR_POTENCIOMETRO, 50);     // Esperar 300 segundos. Si se ha recibido
				if(evt.status == osEventSignal) estadoPot = POT_OFF;			// la señal de apagar el potenciometro, cambiar de estado.

				LPC_ADC->ADCR |= 1 << 24;

				LPC_ADC -> ADCR |= (1 << 5); // Activamos potenciómetro de la derecha
				LPC_ADC -> ADCR = (LPC_ADC -> ADCR) & ~(1 << 4);  // Desactivamos potenciómetro de la izquierda
				osDelay(1);
				LPC_ADC->ADCR |= (1 << 24);            // Comenzamos conversión ADC
				while(LPC_ADC->ADGDR & 0 << 31);       // Aquí decimos que mientras ese bit sea un 0 esperamos
				resADCY2 = (LPC_ADC->ADGDR >> 4) & 0xfff;   // Leemos el resultado de 12 bits


				LPC_ADC->ADCR |= (1 << 4);  // Activamos potenciómetro de la izquierda
				LPC_ADC -> ADCR = (LPC_ADC -> ADCR) & ~(1 << 5);  // Desactivamos potenciómetro de la derecha
				osDelay(1);
				LPC_ADC->ADCR |= (1 << 24);            // Comenzamos conversión ADC
				while(LPC_ADC->ADGDR & 0 << 31);  // Aquí decimos que mientras ese bit sea un 0 esperamos
				resADCY1 = (LPC_ADC->ADGDR >> 4) & 0xfff;   // Leemos el resultado de 12 bits

				ejeY1();
				ejeY2();
				break;
		}


		osThreadYield();                                            // suspend thread
	}

}

/**
**Converión del Eje 2:
  - Transformación de la información obtenida por el potenciómetro 2 resADCY2
en unos valores adecuados para la alteración de las barras en el juego.
Estos valores son 1, 2, -1, -2 y se almacenan en la variable globarl estadoJ2
  **/
void ejeY2() {
	if(resADCY2 < 5) resADCY2 = 5;
	resADCY2 = (resADCY2 - 5) * 2.5;

	if(resADCY2 > 9600 && resADCY2 < 10130) {
		estadoJ2 = 0;
	}
	if(resADCY2 < 9600) {
		osSignalSet(tid_PingPong, J2_ACCION);
		if(resADCY2 > 8000) {
			estadoJ2 = -1;
		} else {
			estadoJ2 = -2;
		}
	}
	if(resADCY2 > 10130) {
		osSignalSet(tid_PingPong, J2_ACCION);
		if(resADCY2 < 10160) {
			estadoJ2 = 1;
		} else {
			estadoJ2 = 2;
		}
	}
}

/**
**Converión del Eje 1:
  - Transformación de la información obtenida por el potenciómetro 1 resADCY1
en unos valores adecuados para la alteración de las barras en el juego.
Estos valores son 1, 2, -1, -2 y se almacenan en la variable globarl estadoJ1
  **/
void ejeY1() {
	if(resADCY1 < 5) resADCY1 = 5;
	resADCY1 = (resADCY1 - 5) * 2.5;

	if(resADCY1 > 10130 && resADCY1 < 10160) {
		estadoJ1 = 0;
	}
	if(resADCY1 < 10130) {
		osSignalSet(tid_PingPong, J1_ACCION);
		if(resADCY1 > 10000) {
			estadoJ1 = -1;
		} else {
			estadoJ1 = -2;
		}
	}
	if(resADCY1 > 10160) {
		osSignalSet(tid_PingPong, J1_ACCION);
		if(resADCY1 < 10214) {
			estadoJ1 = 1;
		} else {
			estadoJ1 = 2;
		}
	}
}
