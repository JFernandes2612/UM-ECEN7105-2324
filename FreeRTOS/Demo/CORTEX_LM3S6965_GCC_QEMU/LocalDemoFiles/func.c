/*
 * func.c
 *
 *  Created on: 27 Oct 2023
 *      Author: jelao
 */

#include <math.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "func.h"
#include "globals.h"

double mysin(double a)
{
	return a - a*a*a / 6.0 + a*a*a*a*a / 120.0 - a*a*a*a*a*a*a / 5040.0 + a*a*a*a*a*a*a*a*a / 362880.0;
}

static TaskHandle_t xHandleFunc = NULL;

void FuncTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;
	const TickType_t xDefaultFrequency = pdMS_TO_TICKS(66);

	int a, b, c;

	if (xQueueReceive(funcQueue, &a, portMAX_DELAY ) == pdPASS) {
		funcState = B;
	}

	if (xQueueReceive(funcQueue, &b, portMAX_DELAY ) == pdPASS) {
		funcState = C;
	}

	if (xQueueReceive(funcQueue, &c, portMAX_DELAY ) == pdPASS) {
		funcState = DONE;
	}


	double ad = (double)a, bd = (double)b, cd = (double)c;
	int i = 0;
	funcFreq = (char)(21.0 / fabs(ad));
	while (i < funcFreq)
	{
		double v = ((double)i/(double)funcFreq) * 3.14159;
		double finalV;
		finalV = mysin(v) * bd * 5.0;

		if (finalV > 0.0)
			func[i] = fabs(finalV - (double)((char)finalV)) >= 0.5 ? -((char)finalV + 1) : -(char)finalV;
		else
			func[i] = fabs(finalV - (double)((char)finalV)) >= 0.5 ? ((char)finalV - 1) : (char)finalV;



		i++;
	}

	i = 0;

	while (i < funcFreq)
	{
		func[i + funcFreq] = -func[i];
		i++;
	}

	i = 0;

	while (i < funcFreq * 2)
	{
		func[i] -= 5 * cd;
		i++;
	}

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xDefaultFrequency );

		if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
			funcStartIndex++;
			funcStartIndex %= (funcFreq * 2);
			xSemaphoreGive(displayS);
		}
	}
}

void startFunc()
{
	funcStartIndex = 0;
	funcState = A;
	funcQueue = xQueueCreate( 3, sizeof( int ) );
	xTaskCreate( FuncTask, "Func", 100, NULL, tskIDLE_PRIORITY + 2, &xHandleFunc );
}

void stopFunc()
{
	vQueueDelete(funcQueue);
	vTaskDelete( xHandleFunc );
}
