#include "minijuegos.h"
#include "puntos.h"

/*-----------------------MINIJUEGOS-------------------------*/
typedef enum {SERPIENTE, PINPONG} t_juegos;
t_juegos juegos;

extern int posicionLibre;
int posicionUser = 0;

extern int menu;
extern int mini_juegos;
int serp = 0;
int ping = 0;

int elegir = 0;

int jugador1 = 0;
int jugador2 = 0;
extern osThreadId tid_Snake;

void minijuegos(void) {

	listarMinijuegosLCD();

	while(mini_juegos) {

		osEvent evt = osSignalWait(0, osWaitForever);

		if(evt.status == osEventSignal) {

			switch(evt.value.signals) {

				case PULSACION_DOWN:

					if(juegos == SERPIENTE) {
						juegos = PINPONG;
						listarMinijuegosLCD();
					}

					break;

				case PULSACION_UP:
					if(juegos == PINPONG) {
						juegos = SERPIENTE;
						listarMinijuegosLCD();
					}
					break;

				case PULSACION_LEFT: //Retroceder al menu principal
					menu = 1;
					mini_juegos = 0;

					break;

				case PULSACION_CENTER:

					switch(juegos) {

						case SERPIENTE: //Poner variable para cuando estoy en este juego
							serp = 1;

							EscribirFrase_L1("    SERPINTE  ");
							EscribirFrase_L2("   Un jugador ");

							osDelay(1000);

							juego_Serpiente();


							break;

						case PINPONG: //Poner variable para cuando estoy en este juego
							ping = 1;

							EscribirFrase_L1("    PING-PONG  ");
							EscribirFrase_L2("   Dos jugador ");

							osDelay(1000);

							juego_PingPong();

							break;

					}//Cierre switch center

					break;

			}//Cierre switch pulsador

		}//Cierre if

	} //Cierre mini_juegos = 1

} // Cierre funcion minijuegos

void listarMinijuegosLCD(void) {

	encender_led(PIN_RED);
	char linea[25];
	if(juegos == SERPIENTE) {
		sprintf(linea, "-> Serpiente");
		EscribirFrase_L1(linea);
		sprintf(linea, "  Pin-pong");
		EscribirFrase_L2(linea);
	} else if(juegos == PINPONG) {
		sprintf(linea, "  Serpiente");
		EscribirFrase_L1(linea);
		sprintf(linea, "-> Pin-pong");
		EscribirFrase_L2(linea);
	}


}

/**
*
* @return devuelve la posicion del jugador elegido
*
*/
int elegirJugador(void) {

	int jugador = 0;

	elegir = 1;
	serp = 0;
	ping = 0;
	mini_juegos = 0;

	while(elegir == 1) {

		osEvent evtent = osSignalWait(0, osWaitForever);

		if(evtent.status == osEventSignal) {

			switch(evtent.value.signals) {

				case PULSACION_DOWN: //Avanzar en la lista de usuarios

					if((posicionUser + 1) < posicionLibre) {
						pintar_puntos_LCD(posicionUser);
						posicionUser ++;

					} else if((posicionUser + 1) == posicionLibre) {
						pintar_puntos_LAST(posicionUser);

					}

					break;

				case PULSACION_UP: // Retroceder en la lista de usuarios

					if(posicionUser != 0) {
						posicionUser --;
						pintar_puntos_LCD(posicionUser);

					}

					break;

				case PULSACION_LEFT: //Retroceder al menu

					//Reset de todas las variables
					mini_juegos = 1;
					elegir = 0;
					posicionUser = 0;
					jugador = 101; //max hay 100 jugadores (0-99), el 101 esta fuera de rango asi que es como no seleccionar ninguno

					break;

				case PULSACION_CENTER: //Eleccion del jugador
					jugador = posicionUser;

					if(juegos == SERPIENTE) {
						serp = 1;

					} else if(juegos == PINPONG) {
						ping = 1;

					}

					elegir = 0;
					posicionUser = 0;

					break;
			}
		}
	}

	return jugador;

}


void juego_Serpiente(void) {

  apagar_leds();
	EscribirFrase_L1("Eleccion del jugador:");
	EscribirFrase_L2("");
	osDelay(2000);

	if(posicionLibre == 0) {

		EscribirFrase_L1("No hay usuarios");
		EscribirFrase_L2("Cree un perfil antes");

	} else {

		if(posicionLibre == 1) { //En caso de que solo haya uno guardado
			pintar_puntos_LAST(0);

		} else {
			pintar_puntos_LCD(0);
			posicionUser ++;

		}

		jugador1 = elegirJugador();

		if(jugador1 != 101) {

			EscribirFrase_L1("   COMENZAMOS  ");
			EscribirFrase_L2("     Suerte    ");

			osDelay(1500);

			Init_Snake();
			osDelay(1500);
			bool gameOver = false;
			while(!gameOver) {
				osEvent evt = osSignalWait(0, osWaitForever);
				if(evt.status == osEventSignal) {
					if(evt.value.signals == GAME_OVER)
						gameOver = true;
					else
						osSignalSet(tid_Snake, evt.value.signals);
				}

			}
			puntuacionSnake();
			osThreadTerminate(tid_Snake);
			EscribirFrase_L1("   GAME OVER  ");
			EscribirFrase_L2("");
			sumar_puntos(puntuacionSnake(), jugador1);
			osDelay(2000);
			listarMinijuegosLCD();
			mini_juegos = 1;

		} else { // Cierre if el jugador no es nulo

			serp = 0;
			listarMinijuegosLCD();

		}
	}
}



void juego_PingPong(void) {

  apagar_leds();
	if(posicionLibre < 2) { // tengo menos de 2 usuarios

		EscribirFrase_L1("No hay 2 usuarios");
		EscribirFrase_L2("Cree perfiles antes");

	} else {

		EscribirFrase_L1("Elegir 1er de jugador:");
		EscribirFrase_L2("");
		osDelay(2000);

		pintar_puntos_LCD(0);
		posicionUser ++;

		jugador1 = elegirJugador();

		if(jugador1 != 101) {

			EscribirFrase_L1("Elegir 2nd jugador:");
			EscribirFrase_L2("");
			osDelay(2000);

			pintar_puntos_LCD(0);
			posicionUser ++;

			jugador2 = elegirJugador();

			if(jugador2 != 101) {

				EscribirFrase_L1("   COMENZAMOS  ");
				EscribirFrase_L2("     Suerte    ");
				Init_PingPong();
				osDelay(1500);
				osSignalWait(GAME_OVER, osWaitForever);

				osThreadTerminate(tid_PingPong);
				EscribirFrase_L1("   GAME OVER  ");
				EscribirFrase_L2("");
				sumar_puntos(score1, jugador1);
				sumar_puntos(score2, jugador2);
				osDelay(2000);
				listarMinijuegosLCD();
				mini_juegos = 1;

			} else { // Cierre if el jugador 2 no es nulo

				ping = 0;
				listarMinijuegosLCD();

			}

		} else { // Cierre if el jugador 1 no es nulo

			ping = 0;
			listarMinijuegosLCD();
		}
	}
}
