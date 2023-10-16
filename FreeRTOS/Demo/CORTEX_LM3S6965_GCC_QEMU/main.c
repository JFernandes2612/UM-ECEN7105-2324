/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_uart.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib.h"
#include "uart.h"

#include "globals.h"
#include "osram128x64x4.h"
#include "serial.h"
#include "display.h"

/*
 * Display Refresh Task
 */
static void RefreshDisplayTask( void *pvParameters );

/*
 * Serial Task
 */
static void SerialTask( void *pvParameters );

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware( void );

/*
 * Hook functions that can get called by the kernel.
 */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );

/*-----------------------------------------------------------*/

int main( void )
{
	prvSetupHardware();

	/* Start the tasks defined within this file/specific to this demo. */
	xTaskCreate( SerialTask, "Serial", STOCK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( RefreshDisplayTask, "RefreshDisplay", STOCK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);


	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	task. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void prvSetupHardware( void )
{
    /* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
    a workaround to allow the PLL to operate reliably. */
    if( DEVICE_IS_REVA2 )
    {
        SysCtlLDOSet( SYSCTL_LDO_2_75V );
    }

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );

	// Enable UART
	SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );
	UARTEnable( UART0_BASE );
	cleanSerial();

	/* Map the OLED access functions to the driver functions that are appropriate
	for the evaluation kit being used. */
	configASSERT( ( HWREG( SYSCTL_DID1 ) & SYSCTL_DID1_PRTNO_MASK ) == SYSCTL_DID1_PRTNO_6965 );

	/* Initialise the OLED */
	OSRAM128x64x4Init( ulSSI_FREQUENCY );
}

void vApplicationTickHook( void )
{

}
/*-----------------------------------------------------------*/

void SerialTask( void *pvParameters )
{
	/* Prevent warnings about unused parameters. */
	( void ) pvParameters;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(1);

	xLastWakeTime = xTaskGetTickCount();

	// Continuous read and print back to serial
	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xFrequency );

		if (state == SCREEN_TYPE)
		{
			defaultSerial();
		}
		else if (state == MOUSE)
		{
			moveMouse();
		}
		else if (state == MENU)
		{
			interactMenu();
		}
		else if (state == WORKERS)
		{
			interactWorkers();
		}
		else if (state == SNAKE)
		{
			interactSnakeGame();
		}
	}
}

void RefreshDisplayTask( void *pvParameters )
{
	/* Prevent warnings about unused parameters. */
	( void ) pvParameters;

	{
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = pdMS_TO_TICKS(16); // ~60fps

		// Initialise the xLastWakeTime variable with the current time.
		xLastWakeTime = xTaskGetTickCount();

		displayS = xSemaphoreCreateMutex();

		for (;;)
		{
			// Wait for the next cycle.
			xTaskDelayUntil( &xLastWakeTime, xFrequency );

			if (state == SCREEN_TYPE)
			{
				printScreanType();
			}
			else if (state == MOUSE)
			{
				printMouse();
			}
			else if (state == MENU)
			{
				printMenu();
			}
			else if (state == WORKERS)
			{
				printWorkers();
			}
			else if (state == SNAKE)
			{
				drawSnakeGame();
			}

			OSRAM128x64x4SwapBuffer();
		}
	}
}


/*-----------------------------------------------------------*/

volatile char *pcOverflowedTask = NULL; /* Prevent task name being optimised away. */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pxTask;
	pcOverflowedTask = pcTaskName;
	vAssertCalled( __FILE__, __LINE__ );
	for( ;; );
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char *pcFile, uint32_t ulLine )
{
volatile uint32_t ulSetTo1InDebuggerToExit = 0;

	taskENTER_CRITICAL();
	{
		while( ulSetTo1InDebuggerToExit == 0 )
		{
			/* Nothing to do here.  Set the loop variable to a non zero value in
			the debugger to step out of this function to the point that caused
			the assertion. */
			( void ) pcFile;
			( void ) ulLine;
		}
	}
	taskEXIT_CRITICAL();
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/*-----------------------------------------------------------*/

char * _sbrk_r (struct _reent *r, int incr)
{
	/* Just to keep the linker quiet. */
	( void ) r;
	( void ) incr;

	/* Check this function is never called by forcing an assert() if it is. */
	configASSERT( incr == -1 );

	return NULL;
}
