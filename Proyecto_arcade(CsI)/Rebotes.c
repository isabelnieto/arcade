#include "rebotes.h"

/*----------------------------------------------------------------------------
 *      Thread 2 'Thread_Name': Rebotes thread
 *---------------------------------------------------------------------------*/

void Rebotes(void const *argument);                              // thread function
osThreadId tid_Rebotes;                                  				 // thread id
osThreadDef(Rebotes, osPriorityNormal, 1, 0);                    // thread object

/*
 * Inicialización del Joystick y del hilo de pulsaciones (para el control de rebotes)
 * @retun 0 en caso de crease correctamente, -1 en caso contrario.
 */
int Init_Rebotes(void) {

	//Configuaración de los pines que vamos a usar (PIN_PINMODE_PULLDOWN: trabajan en pull down, están conectados a 3.3V en el otro extremo)
	PIN_Configure(PUERTO_INT, LINEA_INT_UP, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_INT, LINEA_INT_DOWN, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_INT, LINEA_INT_LEFT, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_INT, LINEA_INT_RIGHT, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_INT, LINEA_INT_CENTER, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	LPC_GPIOINT -> IO0IntEnR = 1 << LINEA_INT_UP | 1 << LINEA_INT_DOWN | 1 << LINEA_INT_LEFT | 1 << LINEA_INT_RIGHT | 1 << LINEA_INT_CENTER;

	tid_Rebotes = osThreadCreate(osThread(Rebotes), NULL);
	NVIC_EnableIRQ(EINT3_IRQn);

	if(!tid_Rebotes) return(-1);

	return(0);
}

/*
 * Función del hilo de rebotes. Recive las señales de que se ha pulsado cualquier botón del joystick, controla los rebotes y reenvia la señal al acelerómetro.
 */
void Rebotes(void const *argument) {

	osEvent event;

	while(1) {

		switch(pulsacion) {

			case Subida:

				event = osSignalWait(0, osWaitForever);

				osSignalSet(tid_Arcade, event.value.signals);  //Se reenvía la señal de que algún pulsador ha sido utilizado y el autómata gestiona cual.

				osDelay(200);

				LPC_GPIOINT -> IO0IntEnF = 1 << LINEA_INT_UP | 1 << LINEA_INT_DOWN | 1 << LINEA_INT_LEFT | 1 << LINEA_INT_RIGHT | 1 << LINEA_INT_CENTER;

				pulsacion = Bajada;

				break;

			default: // Bajada:

				event = osSignalWait(PULSACION_BAJADA, osWaitForever);

				osDelay(200);

				LPC_GPIOINT -> IO0IntEnR = 1 << LINEA_INT_UP | 1 << LINEA_INT_DOWN | 1 << LINEA_INT_LEFT | 1 << LINEA_INT_RIGHT | 1 << LINEA_INT_CENTER;

				pulsacion = Subida;

		}

		osThreadYield();                                            // suspend thread
	}

}

/*
 * Función que está a la espera de alguna pulsación y envia la señal al hilo que gestiona los rebotes. Envía tanto las señales de subida como las de bajada.
 */
void EINT3_IRQHandler(void) {

	if(LPC_GPIOINT ->IO0IntStatR & 1 << LINEA_INT_RIGHT) {

		osSignalSet(tid_Rebotes, PULSACION_RIGHT);

		LPC_GPIOINT -> IO0IntClr = 1 << LINEA_INT_RIGHT;

	} else if(LPC_GPIOINT ->IO0IntStatR & 1 << LINEA_INT_LEFT) {

		osSignalSet(tid_Rebotes, PULSACION_LEFT);

		LPC_GPIOINT -> IO0IntClr = 1 << LINEA_INT_LEFT;
	} else if(LPC_GPIOINT ->IO0IntStatR & 1 << LINEA_INT_UP) {

		osSignalSet(tid_Rebotes, PULSACION_UP);

		LPC_GPIOINT ->IO0IntClr = 1 << LINEA_INT_UP;

	} else if(LPC_GPIOINT ->IO0IntStatR & 1 << LINEA_INT_DOWN) {

		osSignalSet(tid_Rebotes, PULSACION_DOWN);

		LPC_GPIOINT ->IO0IntClr = 1 << LINEA_INT_DOWN;

	} else if(LPC_GPIOINT ->IO0IntStatR & 1 << LINEA_INT_CENTER) {

		osSignalSet(tid_Rebotes, PULSACION_CENTER);

		LPC_GPIOINT ->IO0IntClr = 1 << LINEA_INT_CENTER;

	} else if(LPC_GPIOINT -> IO0IntStatF) { //Bajada

		osSignalSet(tid_Rebotes, PULSACION_BAJADA);

		LPC_GPIOINT -> IO0IntClr = 1 << LINEA_INT_UP | 1 << LINEA_INT_DOWN | 1 << LINEA_INT_LEFT | 1 << LINEA_INT_RIGHT | 1 << LINEA_INT_CENTER;

	}

	LPC_GPIOINT -> IO0IntEnF = 0;
	LPC_GPIOINT -> IO0IntEnR = 0;

}
