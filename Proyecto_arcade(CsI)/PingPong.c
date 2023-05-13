/**
* @author Isabel Nieto Petinal
* @date 20-12-2020
*/

#include "PingPong.h"

/*****************************************************************************
 *                          PING PONG
 *****************************************************************************/

/**
**Inicializaci�n del hilo del juego Ping Pong
  **/
int Init_PingPong(void) {
	Init_Pot();
	osSignalSet(tid_Pot, SIGNAL_ENCENDER_POTENCIOMETRO);
	tid_PingPong = osThreadCreate(osThread(PingPong), NULL);
	if(!tid_PingPong) return(-1);
	return(0);
}

/**
**Funci�n principal:
  -Calculo de la posi�n de la pelota relativa al resto de elementos del
juego, como las barras o bordes, actuando de manera correspondiente.
    -Si la pelota toca los l�mites superiores se su movimiento variar�
   con respecto al eje Y
    -Al tocar con alguna de las varras la pelota variar� hacia la
   direcci�n de x opuesta y pudiendo haber variado en la direcci�n Y si
   al colisionar con la barra, esta se estaba moviendo.
    -Al pasar las barras el marcador aumenta de forma correspondiente
  -Calculo de lad posiciones de las barras y su movimiento seg�n los
  valores obtenido por los potenci�metos, mo pudiendo exceder los l�mites
  de la pantalla
  -Representaci�n de los puntos de manera correspondiente
  -Representaci�n del juego
  -Capacidad de encender y apagar el juego seg�n el valor de "juego_activo"

  **/
void PingPong(void const *argument) {

	uint8_t p_x = 64; //Posici�n x de la pelota
	int8_t mas_x = 3; //Aumento x de la pelota
	uint8_t p_y = 16; //Posici�n y de la pelota
	int8_t mas_y = 0; //Aumento y de la pelota
	uint8_t p_b1 = 12; //Posici�n y de la barra 1
	uint8_t p_b2 = 12; //Posici�n y de la barra 2
	uint8_t p_bx1 = 12; //Posici�n x de la barra 1
	uint8_t p_bx2 = 119; //Posici�n x de la barra 2

	uint8_t Puntos_partida = 10; //Puntaci�n a la que se temina la partida




	juego_activo = 1; //Posici�n x de la pelota

	score1 = 0; //Puntuaci�n del Jugador 1
	score2 = 0; //Puntuaci�n del Jugador 2
	uint16_t vel = 40;
	osTimerStart(id2, vel);
	paradaPelota = 0;

	while(true) {
		osSignalWait(J2_ACCION, 20);
		osSignalWait(J1_ACCION, 20);

		//Caulculo de fin de partida
		if(score1 == Puntos_partida || score2 == Puntos_partida) {
			osSignalSet(tid_Arcade, GAME_OVER);
			osSignalSet(tid_Pot, SIGNAL_APAGAR_POTENCIOMETRO);
			osThreadTerminate(tid_Pot);
			osTimerStop(id1);
			osTimerStop(id2);
			apagarZumbador();
		}


		//Calculo de si la pelota ha chocado con las barras
		if((p_x <= p_bx1 && p_y <= p_b1 + 2 && p_y >= (p_b1 - 12)) && mas_x < 0) {
			mas_x = -mas_x;
			mas_y = mas_y + estadoJ1;
		}

		if((p_x >= p_bx2 && p_y <= p_b2 + 2 && p_y >= (p_b2 - 12)) && mas_x > 0) {
			mas_x = -mas_x;
			mas_y = mas_y + estadoJ2;
		}

		//Accion si se ha marcado gol
		if(p_x < 8 || p_x > 123) {
			encenderZumbador();
			if(p_x <= 7) {
				score2++;
			} else {
				score1++;
			}
			osTimerStart(id1, 750);
			paradaPelota = 1;
			p_x = 64;
			p_y = 16;
			mas_y = 0;
			mas_x = (score1 < score2) ? 3 : -3; //La pelota empezar� hacia el jugador con m�s puntos
		}
		//Calculo evitar que la pelota salga por los extremos superior e inferior
		if(p_y < 4 && mas_y < 0) {
			mas_y = -mas_y;
		} else if(p_y >= 30 && mas_y > 0) {
			mas_y = -mas_y;
		}

		//Calculo de cada barra
		p_b2 = p_b2 + estadoJ2;
		p_b1 = p_b1 + estadoJ1;

		//Calculo evitar salirse del cuadrado cada barra
		if(p_b1 < 12) {
			p_b1 = 12;
		} else if(p_b1 > 30) {
			p_b1 = 30;
		}

		if(p_b2 < 12) {
			p_b2 = 12;
		} else if(p_b2 > 30) {
			p_b2 = 30;
		}

		//Cambiar la posic�n de la pelota cada vez que se activa el timer
		if((accionPelota == 1) && (paradaPelota == 0)) {
			accionPelota = 0;
			p_x = p_x + mas_x;
			p_y = p_y + mas_y;
			apagarZumbador();
		}
		// Mostrar el juego
		pintarPong(p_x, p_y, p_b1, p_b2, score1, score2);
	}
}
