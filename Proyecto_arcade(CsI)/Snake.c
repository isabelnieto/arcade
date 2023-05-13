/**
* @author Isabel Nieto Petinal
* @date 20-12-2020
*/

#include "Snake.h"

/*****************************************************************************
 *                           JUEGO DE LA SERPIENTE
 *****************************************************************************/

/**
* Inicialización del juego de la Serpiente. El juego consiste en comer todas las
* manzanas posibles sin chocharse contra las paredes del LCD o contra uno mismo
*
* @return 0 si se ha realizado con éxito la creación del hilo.
*        -1 si ha habido algún error en la creación del hilo.
*/
t_posicion serpiente[30];
t_posicion manzanas[5];
t_posicion nextPos;

uint8_t longSerpiente;
uint8_t nManzanas;
uint8_t velocidad;
uint8_t points;

bool gameOver;

/**
* Inicializar el hilo que maneja el juego de la serpiente.
*
*/
int Init_Snake(void) {
	longSerpiente = 4;
	nManzanas = 0;
	velocidad = 1;
	points = 0;
	gameOver = false;
	direccionSiguiente = DERECHA;
	Init_SnakeArray();
	osTimerStart(tid_timerSnake,500);

	//Inicializar Thread
	tid_Snake = osThreadCreate(osThread(Snake), NULL);
	if(!tid_Snake) return(-1);
	return(0);
}

/**
* Hilo que maneja el jueg de la serpiente.
*
*
*/
void Snake(void const *argument) {
	osEvent evt;
	colocarManzana();

	while(1) {
		evt = osSignalWait(0, osWaitForever);
		switch(evt.value.signals) {
			case PULSACION_RIGHT:
				if(direccionActual != IZQUIERDA)
					direccionSiguiente = DERECHA;
				break;

			case PULSACION_LEFT:
				if(direccionActual != DERECHA)
					direccionSiguiente = IZQUIERDA;
				break;

			case PULSACION_UP:
				if(direccionActual != ABAJO)
					direccionSiguiente = ARRIBA;
				break;

			case PULSACION_DOWN:
				if(direccionActual != ARRIBA)
					direccionSiguiente = ABAJO;
				break;

			case SIGNAL_MOVERSE:
				moverse();
				break;
		}
		osThreadYield();    // suspend thread
	}
}
/**
* Funcion encargada de incializar el autómata
*
*
*/
void Init_SnakeArray() {
	serpiente[0].x = 10;
	serpiente[0].y = 4;
	nextPos.x = 10;
	nextPos.y = 4;

	for(uint8_t i = 1; i < longSerpiente; i++) {
		serpiente[i].x = serpiente[i - 1].x - 1;
		serpiente[i].y = serpiente[i - 1].y;
	}
}

/*
*Funcion encargada de moverse. Se activa cada vez que se recibe
*un mensaje del timer.
*
*/
void moverse() {
	if(posicionCorrectaMapa()) {
		moverNextPos();
		if(noEsSerpiente()) {
			if(hayManzana()) {
				osTimerStop(tid_timerSnake);
				if(longSerpiente < 25) longSerpiente++;
				points++;
				nManzanas--;
				colocarManzana();
				if(velocidad < 24)velocidad++;
				osTimerStart(tid_timerSnake, 500 - 20 * velocidad);
			}
			moverSerpiente();
			pintarTablero();

		} else
			gameOver = true;
	} else {
		gameOver = true;
	}
	if(gameOver) {
		osTimerStop(tid_timerSnake);
		osSignalSet(tid_Arcade, GAME_OVER);
	}


}

/**
* Devuelve si la posición siguiente de la serpiente va a 
* estar fuera de rango.
* @return bool - True si la posición del mapa es correcta 
*              - False si está fuera de límite
*/
bool posicionCorrectaMapa() {
	bool posicionCorrecta = true;

	switch(direccionSiguiente) {
		case DERECHA:
			if(nextPos.x >= 31) posicionCorrecta = false;
			break;

		case IZQUIERDA:
			if(nextPos.x == 0) posicionCorrecta = false;
			break;

		case ARRIBA:
			if(nextPos.y == 0) posicionCorrecta = false;
			break;

		case ABAJO:
			if(nextPos.y >= 7) posicionCorrecta = false;
			break;
	}

	return posicionCorrecta;
}

/**
* Funcion que indica si la posición actual es parte de
* la serpiente.
*
* @return true si no forma parte de la serpiente
*/
bool noEsSerpiente() {
	bool noEsSerpiente = true;
	for(uint8_t i = 0; i < longSerpiente; i++) {
		if(serpiente[i].x == nextPos.x && serpiente[i].y == nextPos.y)
			noEsSerpiente = false;
	}
	return noEsSerpiente;
}

/**
* Indica si en la posición actual hay una manzana.
*
* @return bool - True si hay una manzana, false si no la hay.
*/
bool hayManzana() {
	bool hayManzana = false;
	for(uint8_t i = 0; i < nManzanas; i++) {
		if(manzanas[i].x == nextPos.x && manzanas[i].y == nextPos.y) {
			hayManzana = true;
		}
	}
	return hayManzana;
}

/**
* Mueve de posición la posición actual por la siguiente. Es decir,
* se desplaza.
*
*/
void moverNextPos() {
	switch(direccionSiguiente) {
		case ARRIBA:
			nextPos.y--;
			break;
		case ABAJO:
			nextPos.y++;
			break;
		case IZQUIERDA:
			nextPos.x--;
			break;
		case DERECHA:
			nextPos.x++;
			break;
	}
	direccionActual = direccionSiguiente;
}

/**
* Mueve las posiciones del array que contiene las posiciones de la serpiente
* para guardar la posición acutal y eliminar la ultima posición del array.
*/
void moverSerpiente() {
	for(uint8_t i = longSerpiente - 1; 0 < i; i--) {
		serpiente[i] = serpiente[i - 1];
	}
	serpiente[0] = nextPos;
}

/**
* Pinta por el LCD la serpiente
*/
void pintarTablero() {
	pintarSerpiente(serpiente, manzanas, longSerpiente, nManzanas);
}

/**
* Pone una manzana en el tablero.
*
*/
void colocarManzana(void) {
	bool posicionBuena;
	t_posicion pos;
	do {
		posicionBuena = true;
		osDelay(1);
		pos = getRandomPos();
		for(uint8_t i = 0; i < longSerpiente; i++) {
			if(serpiente[i].x == pos.x && serpiente[i].y == pos.y)
				posicionBuena = false;
		}
	} while(!posicionBuena);

	manzanas[nManzanas].x = pos.x;
	manzanas[nManzanas].y = pos.y;
	nManzanas++;
}

/**
* Inicializa los registros necesarios para encender un timer. Se encargará de obtener
* un numero random.
*
*/
void InitRandomTimer() {

	LPC_SC->PCONP |= 1 << 1;		//Habilita el timer 0

	LPC_TIM0->TCR = 1; 				//Inicia el timer

	LPC_TIM0->MCR = 1 << 1; 		//El bit en la posición 1, al ponerlo a 1, resetea el contador (TC)
	//si coincide con el valor de MR0

	LPC_TIM0->MR0 = 0xFF;

	NVIC_EnableIRQ(TIMER0_IRQn);
}

/**
* Devuelve una posición aleatoria del tablero.
*
* @return pos - Posición aleatoria
*/
t_posicion getRandomPos() {
	uint32_t random = (LPC_TIM0->TC);
	t_posicion pos;
	pos.x = random & 0x1F;
	pos.y = (random & 0xE0) >> 5;
	return pos;
}

/**
* Devuelve la puntuación del juego.
*
*/
uint8_t puntuacionSnake() {
	return points;
}
