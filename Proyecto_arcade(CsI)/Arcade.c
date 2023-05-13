
#include "arcade.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Arcade': Sample thread
 *---------------------------------------------------------------------------*/

void Arcade(void const *argument);                              // thread function
osThreadId tid_Arcade;                                          // thread id
osThreadDef(Arcade, osPriorityNormal, 1, 0);                    // thread object

int menu = 1;
int mini_juegos = 0;
int perfil = 0;
int puntuacion = 0;

int Init_Arcade(void) {
	tid_Arcade = osThreadCreate(osThread(Arcade), NULL);
	if(!tid_Arcade) return(-1);

	estados = PUNTOS;

	return(0);
}

void Arcade(void const *argument) {
	while(1) {
		if(menu) {
			apagar_leds();
			escribirLCDMenu();

			osEvent evt = osSignalWait(0, osWaitForever);

			if(evt.status == osEventSignal) {

				switch(evt.value.signals) {

					case PULSACION_DOWN:

						if(estados == PUNTOS)
							estados = MINIJUEGOS;
						else if(estados == MINIJUEGOS)
							estados = NUEVO_PERFIL;

						escribirLCDMenu();

						break;

					case PULSACION_UP:
						if(estados == NUEVO_PERFIL)
							estados = MINIJUEGOS;
						else if(estados == MINIJUEGOS)
							estados = PUNTOS;

						escribirLCDMenu();

						break;

					case PULSACION_CENTER:

						switch(estados) {
							case PUNTOS:
								menu = 0;
								puntuacion = 1;
								puntos();
								break;

							case MINIJUEGOS:
								menu = 0;
								mini_juegos = 1;
								minijuegos();
								break;

							case NUEVO_PERFIL:
								menu = 0;
								perfil = 1;
								nuevo_perfil();
								break;
						}	//Cierre switch estados

						break;

				}//Cierre switch pulsación

			}//Cierre if

			osThreadYield();                                            // suspend thread

		}//Cierre if menu

	}//Cierre while

}//Cierre hilo



//Aqui le metia un switch
void escribirLCDMenu(void) {

	char linea[25];
	if(estados == PUNTOS) {
		sprintf(linea, "-> PUNTOS");
		EscribirFrase_L1(linea);
		sprintf(linea, "  MINIJUEGOS");
		EscribirFrase_L2(linea);
	} else if(estados == MINIJUEGOS) {
		sprintf(linea, "-> MINIJUEGOS");
		EscribirFrase_L1(linea);
		sprintf(linea, "  NUEVO PERFIL");
		EscribirFrase_L2(linea);
	} else if(estados == NUEVO_PERFIL) {
		sprintf(linea, "-> NUEVO PERFIL");
		EscribirFrase_L1(linea);
		EscribirFrase_L2("");
	}


}


