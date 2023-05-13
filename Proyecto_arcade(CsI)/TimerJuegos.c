
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "signals.h"

extern osThreadId tid_Arcade;

/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/


/*----- One-Shoot Timer Example -----*/
static void Timer_10seg_Callback(void const *arg);                   // prototype for timer callback function

osTimerId id_10seg;                                      // timer id
static uint32_t  exec1;                                         // argument for the timer call back function
static osTimerDef(Timer_10seg, Timer_10seg_Callback);                     // define timers

// One-Shoot Timer Function
static void Timer_10seg_Callback(void const *arg) {
	// add user code here

	osSignalSet(tid_Arcade, SEG10);

}

// Example: Create and Start timers
void Init_Timers_minijuegos(void) {

	// Create one-shoot timer
	exec1 = 1;
	id_10seg = osTimerCreate(osTimer(Timer_10seg), osTimerOnce, &exec1);


}
