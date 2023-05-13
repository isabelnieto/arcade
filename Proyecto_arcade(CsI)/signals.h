#ifndef SIGNALS_H_INCLUDED
#define SIGNALS_H_INCLUDED

/*
 * En este fichero se encuentran declaradas todas las señalaes que se intercambianbian entre todos los hilos y timers con la finalidad no repetir
 * identificadores de señal dirigidas al mismo hilo.
 */

//JOYSTICK

#define PULSACION_RIGHT 0x0001      //se manda al autómata y a rebotes
#define PULSACION_LEFT 0x0002       //se manda al autómata y a rebotes
#define PULSACION_UP 0x0004         //se manda al autómata y a rebotes
#define PULSACION_DOWN 0x0008       //se manda al autómata y a rebotes
#define PULSACION_CENTER 0x0010     //se manda al autómata y a rebotes
#define PULSACION_BAJADA 0x0020     //se manda al autómata y a rebotes

//JOYSTICK POTENCIOMENTROS EXTRA

#define J2_ACCION 				0x1000
#define J1_ACCION 				0x2000
#define SIGNAL_APAGAR_POTENCIOMETRO 	0x4000
#define SIGNAL_ENCENDER_POTENCIOMETRO 0x8000

//SNAKE
#define SIGNAL_MOVERSE 	0x0100
#define GAME_ON 				0x0200
#define GAME_OVER 			0X0400


#endif

