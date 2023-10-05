/*
 * utils.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_GLOBALS_H_
#define LOCALDEMOFILES_GLOBALS_H_

#include "FreeRTOS.h"
#include "queue.h"

/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
tick hook. */
#define mainCHECK_DELAY						( ( TickType_t ) 5000 / portTICK_PERIOD_MS )

/* Task stack sizes. */
#define STOCK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 40 )

/* The period of the system clock in nano seconds.  This is used to calculate
the jitter time in nano seconds. */
#define mainNS_PER_CLOCK					( ( uint32_t ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

#define ulSSI_FREQUENCY						( 3500000UL )

/*-----------------------------------------------------------*/

// STATE
enum State {
	MENU,
	SCREEN_TYPE,
	MOUSE,
	WORKERS,
};

extern enum State state;

// MOUSE
struct Mouse {
	unsigned char x, y;
};

extern struct Mouse mouse;

// SERIAL INPUT BUFFER
extern char serialBuffer[1024];

// MENU

enum MenuSelection {
	SCREEN_TYPE_SELECTION,
	MOUSE_SELECTION,
	WORKERS_SELECTION,
};

extern enum MenuSelection menu;

extern signed char numberOfMenuStates;

// WORKERS

extern QueueHandle_t workersQueue;

extern unsigned int current_timer_1;
extern unsigned int current_timer_2;
extern unsigned int current_timer_3;
#endif /* LOCALDEMOFILES_GLOBALS_H_ */
