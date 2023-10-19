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
#include "semphr.h"

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

// MUTEX

extern SemaphoreHandle_t displayS;

// STATE
enum State {
	MENU,
	SCREEN_TYPE,
	MOUSE,
	WORKERS,
	SNAKE,
};

extern enum State state;

// MOUSE
struct Pos {
	unsigned char x, y;
};

extern struct Pos mouse;

// SERIAL INPUT BUFFER
extern char serialBuffer[169];
extern unsigned int serialBufferIndex;

// MENU

enum MenuSelection {
	SCREEN_TYPE_SELECTION,
	MOUSE_SELECTION,
	WORKERS_SELECTION,
	SNAKE_SELECTION,
};

extern enum MenuSelection menu;

extern signed char numberOfMenuStates;

// WORKERS

extern QueueHandle_t workersQueue;

extern unsigned int current_timer_1;
extern unsigned int current_timer_2;
extern unsigned int current_timer_3;

// SNAKE

enum SnakeDir {
	S_UP,
	S_DOWN,
	S_RIGHT,
	S_LEFT
};

struct Snake {
	struct Pos s[128];
	struct Pos ps[128];
	unsigned int p;
	enum SnakeDir d;
	struct Pos food;
};

extern struct Snake snake;

#endif /* LOCALDEMOFILES_GLOBALS_H_ */
