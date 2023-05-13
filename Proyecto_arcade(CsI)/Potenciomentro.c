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
**Inicializaci�n del los potenci�metos.
  - Asignaci�n del los pines y puertos correpondientes los
potenci�metros usados para el minijuego colocados en los pines 19 y 20
  - Inicializaci�n del hilo correspondiente a los potenci�metros
**/
int Init_Pot(void) {
	estadoPot = POT_OFF;

	LPC_SC->PCONP |= (1 << 12);              // Habilitamos el reloj para el controlador del ADC
	LPC_PINCON->PINSEL3 |= (1 << 28) | (1 << 29) | (1 << 30) | (1U << 31);
	LPC_ADC->ADCR |= (1 << 21) | (10 << 8);  // Ponemos el reloj y el encendido del m�dulo ADC

	resADCY2 = 0;

	GPIO_SetDir(PUERTODER, PIN_POTX_2, GPIO_DIR_INPUT);
	GPIO_SetDir(PUERTODER, PIN_POTY_2, GPIO_DIR_INPUT);

	//Inicializar el hilo
	tid_Pot = osThreadCreate(osThread(Potenciometro), NULL);
	if(!tid_Pot) return(-1);

	return(0);
}


/**
**Funci�n principal:
  - Manejo de los dos estados del potenci�metro ON / OFF
  - Multiplexaci�n de los potenci�metros para obtener infoemaci�n de los dos
  - Transformac�n de la informaci�n obtenida de los potenci�metos en un valor
adecuado para la varia�n de las barras del juego
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
				if(evt.status == osEventSignal) estadoPot = POT_OFF;			// la se�al de apagar el potenciometro, cambiar de estado.

				LPC_ADC->ADCR |= 1 << 24;

				LPC_ADC -> ADCR |= (1 << 5); // Activamos potenci�metro de la derecha
				LPC_ADC -> ADCR = (LPC_ADC -> ADCR) & ~(1 << 4);  // Desactivamos potenci�metro de la izquierda
				osDelay(1);
				LPC_ADC->ADCR |= (1 << 24);            // Comenzamos conversi�n ADC
				while(LPC_ADC->ADGDR & 0 << 31);       // Aqu� decimos que mientras ese bit sea un 0 esperamos
				resADCY2 = (LPC_ADC->ADGDR >> 4) & 0xfff;   // Leemos el resultado de 12 bits


				LPC_ADC->ADCR |= (1 << 4);  // Activamos potenci�metro de la izquierda
				LPC_ADC -> ADCR = (LPC_ADC -> ADCR) & ~(1 << 5);  // Desactivamos potenci�metro de la derecha
				osDelay(1);
				LPC_ADC->ADCR |= (1 << 24);            // Comenzamos conversi�n ADC
				while(LPC_ADC->ADGDR & 0 << 31);  // Aqu� decimos que mientras ese bit sea un 0 esperamos
				resADCY1 = (LPC_ADC->ADGDR >> 4) & 0xfff;   // Leemos el resultado de 12 bits

				ejeY1();
				ejeY2();
				break;
		}


		osThreadYield();                                            // suspend thread
	}

}

/**
**Converi�n del Eje 2:
  - Transformaci�n de la informaci�n obtenida por el potenci�metro 2 resADCY2
en unos valores adecuados para la alteraci�n de las barras en el juego.
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
**Converi�n del Eje 1:
  - Transformaci�n de la informaci�n obtenida por el potenci�metro 1 resADCY1
en unos valores adecuados para la alteraci�n de las barras en el juego.
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
