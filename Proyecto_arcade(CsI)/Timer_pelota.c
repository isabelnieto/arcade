
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
uint8_t accionPelota;
uint8_t paradaPelota = 0;



/*----- One-Shoot Timer Example -----*/
static void Timer1_Callback(void const *arg);                   // prototype for timer callback function

osTimerId id1;                                           // timer id
static uint32_t  exec1;                                         // argument for the timer call back function
static osTimerDef(Timer1, Timer1_Callback);                     // define timers

// One-Shoot Timer Function
static void Timer1_Callback(void const *arg) {
	paradaPelota = 0;
}


/*----- Periodic Timer Example -----*/
static void Timer2_Callback(void const *arg);                   // prototype for timer callback function

osTimerId id2;                                           // timer id
static uint32_t  exec2;                                         // argument for the timer call back function
static osTimerDef(Timer2, Timer2_Callback);

// Periodic Timer Example
static void Timer2_Callback(void const *arg) {
	accionPelota = 1;
}


// Example: Create and Start timers
void Init_TimersPelota(void) {
	osStatus status;                                              // function return status

	// Create one-shoot timer
	exec1 = 1;
	id1 = osTimerCreate(osTimer(Timer1), osTimerOnce, &exec1);
	if(id1 != NULL) {     // One-shot timer created
		// start timer with delay 100ms
		//status = osTimerStart (id1, 500);    //Tiempo a la que salta
		if(status != osOK) {
			// Timer could not be started
		}
	}

	// Create periodic timer
	exec2 = 2;
	id2 = osTimerCreate(osTimer(Timer2), osTimerPeriodic, &exec2);
	if(id2 != NULL) {     // Periodic timer created
		// start timer with periodic 1000ms interval
		// status = osTimerStart (id2, 1000);
		if(status != osOK) {
			// Timer could not be started
		}
	}
}
