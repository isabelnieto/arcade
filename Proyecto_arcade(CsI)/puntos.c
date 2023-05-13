#include "puntos.h"

extern int menu;
extern int puntuacion;

char usuarios [100][3];
int puntosUsuarios [100];
int posicionLibre = 0;
int posicion = 0;

void puntos(void) {

	if(posicionLibre == 0) {

		EscribirFrase_L1("No hay usuarios");
		EscribirFrase_L2("");

	} else if(posicionLibre == 1) { //En caso de que solo haya uno guardado

		pintar_puntos_LAST(0);

	} else {

		pintar_puntos_LCD(0);
		posicion ++;

	}

	while(puntuacion) {

		encender_led(PIN_GREEN);

		osEvent evtent = osSignalWait(0, osWaitForever);

		if(evtent.status == osEventSignal) {

			switch(evtent.value.signals) {

				case PULSACION_DOWN: //Avanzar en la lista de usuarios

					if((posicion + 1) < posicionLibre) {
						pintar_puntos_LCD(posicion);
						posicion ++;

					} else if((posicion + 1) == posicionLibre) {
						pintar_puntos_LAST(posicion);

					}

					break;

				case PULSACION_UP: // Retroceder en la lista de usuarios

					if(posicion != 0) {
						posicion --;
						pintar_puntos_LCD(posicion);

					}


					break;

				case PULSACION_LEFT: //Retroceder al menu principal

					//Reset de todas las variables
					menu = 1;
					puntuacion = 0;
					posicion = 0;

					break;
			}
		}
	}
}


void agregarPerfil(char nombre[]) {

	for(int i = 0; i < 3; i++) {
		usuarios [posicionLibre] [i] = nombre[i];
	}

	puntosUsuarios [posicionLibre] = 0;
	posicionLibre ++;

	//GUARDAR EN MEMORIA

}

void pintar_puntos_LCD(int posicionPintar) {
	char linea1[25];
	char linea2[25];
	char user1 [3];
	char user2 [3];

	for(int i = 0; i < 3; i++) {

		user1[i] = usuarios[posicionPintar][i];
		user2[i] = usuarios[posicionPintar + 1][i];

	}

	sprintf(linea1, "%3s -> %d", user1, puntosUsuarios [posicionPintar]);
	EscribirFrase_L1(linea1);
	sprintf(linea2, "%3s -> %d", user2, puntosUsuarios [posicionPintar + 1 ]);
	EscribirFrase_L2(linea2);

}

void pintar_puntos_LAST(int posicionPintar) {

	char linea1[25];

	char user1 [3];

	for(int i = 0; i < 3; i++) {
		user1[i] = usuarios[posicionPintar][i];
	}
	sprintf(linea1, "%3s -> %d", user1, puntosUsuarios [posicionPintar]);
	EscribirFrase_L1(linea1);
	EscribirFrase_L2(" ");
}

void sumar_puntos(int puntos, int posicion) {
	puntosUsuarios [posicion] = puntosUsuarios [posicion] + puntos;
}

