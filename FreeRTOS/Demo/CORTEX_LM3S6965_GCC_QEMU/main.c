/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_uart.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib.h"
#include "osram128x64x4.h"
#include "uart.h"


/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
tick hook. */
#define mainCHECK_DELAY						( ( TickType_t ) 5000 / portTICK_PERIOD_MS )

/* Task stack sizes. */
#define STOCK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 40 )

/* The period of the system clock in nano seconds.  This is used to calculate
the jitter time in nano seconds. */
#define mainNS_PER_CLOCK					( ( uint32_t ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define ulSSI_FREQUENCY						( 3500000UL )

/*-----------------------------------------------------------*/

/*
 * Display Task
 */
static void OLEDTask( void *pvParameters );

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
	xTaskCreate( OLEDTask, "OLED", STOCK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate( SerialTask, "Serial", STOCK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );


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
}

void vApplicationTickHook( void )
{

}
/*-----------------------------------------------------------*/

void SerialTask( void *pvParameters )
{
	/* Prevent warnings about unused parameters. */
	( void ) pvParameters;

	static char c;
	unsigned long i = 0;

	// Continuous read and print back to serial
	while (1)
	{
		if (UARTCharsAvail(UART0_BASE)) {
			c = UARTCharGet(UART0_BASE);
			UARTCharPut(UART0_BASE, c);

			OSRAM128x64x4StringDraw(&c, 6*(i % 21), i/21 * 8, 0xF, 0);
			i++;
		}
	}
}

void OLEDTask( void *pvParameters )
{
/* Functions to access the OLED.  The one used depends on the dev kit
being used. */
void ( *vOLEDInit )( uint32_t ) = NULL;
	/* Prevent warnings about unused parameters. */
	( void ) pvParameters;

	/* Map the OLED access functions to the driver functions that are appropriate
	for the evaluation kit being used. */
	configASSERT( ( HWREG( SYSCTL_DID1 ) & SYSCTL_DID1_PRTNO_MASK ) == SYSCTL_DID1_PRTNO_6965 );
	vOLEDInit = OSRAM128x64x4Init;

	/* Initialise the OLED */
	vOLEDInit( ulSSI_FREQUENCY );
	vTaskDelete(NULL);
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
