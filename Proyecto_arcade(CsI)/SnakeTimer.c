
#include "cmsis_os.h"
#include "Signals.h"
/*****************************************************************************
 *                           TIMER HORA
 *****************************************************************************/

extern osThreadId tid_Snake;

//Variables y funciones para la creación del timer
static void Timer_Hora_Callback(void const *arg);
osTimerId tid_timerSnake;
static uint32_t  exec;
static osTimerDef(Timer3, Timer_Hora_Callback);

static void Timer_Hora_Callback(void const *arg) {
	osSignalSet(tid_Snake, SIGNAL_MOVERSE);
}



void Init_SnakeTimer(void) {
	//osStatus status;
	exec = 1;
	tid_timerSnake = osTimerCreate(osTimer(Timer3), osTimerPeriodic, &exec);
	if(tid_timerSnake != NULL) {    																					// Timer periódico creado
		//status = osTimerStart(tid_timerSnake, 500);
		//if(status != osOK) {
		//}
	}
}
