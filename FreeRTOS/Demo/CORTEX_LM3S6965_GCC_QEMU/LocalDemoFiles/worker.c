/*
 * worker.c
 *
 *  Created on: 4 Oct 2023
 *      Author: jelao
 */

#include "worker.h"
#include "string.h"

#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "globals.h"

TaskHandle_t xHandleWorker1 = NULL;
TaskHandle_t xHandleWorker2 = NULL;
TaskHandle_t xHandleWorker3 = NULL;

void WorkerTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;
	const TickType_t xDefaultFrequency = pdMS_TO_TICKS(50);
	const TickType_t secondFrequency = pdMS_TO_TICKS(1000);

	int working = 0;

	unsigned int* timer;
	char taskName[12];
	getTaskName(taskName);
	if (strcmp(taskName, "Worker1") == 0)
	{
		timer = &current_timer_1;
	}
	else if (strcmp(taskName, "Worker2") == 0)
	{
		timer = &current_timer_2;
	}
	else if (strcmp(taskName, "Worker3") == 0)
	{
		timer = &current_timer_3;
	}

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xDefaultFrequency );

		if (!working) {
			int x;
			if (xQueueReceive(workersQueue, &x, ( TickType_t ) 0 ) == pdPASS)
			{
				if (x != 0)
				{
					*timer = x;
					working = 1;
					xLastWakeTime = xTaskGetTickCount();
				}
			}
		}

		while (working) {
			xTaskDelayUntil( &xLastWakeTime, secondFrequency );
			*timer -= 1;
			if (*timer <= 0)
				working = 0;
		}
	}
}

void initWorkers()
{
	current_timer_1 = 0;
	current_timer_2 = 0;
	current_timer_3 = 0;

	workersQueue = xQueueCreate( 10, sizeof( unsigned int ) );

	xTaskCreate( WorkerTask, "Worker1", 60, NULL, tskIDLE_PRIORITY + 2, &xHandleWorker1 );
	xTaskCreate( WorkerTask, "Worker2", 60, NULL, tskIDLE_PRIORITY + 2, &xHandleWorker2 );
	xTaskCreate( WorkerTask, "Worker3", 60, NULL, tskIDLE_PRIORITY + 2, &xHandleWorker3 );
}

void stopWorkers()
{
	vTaskDelete( xHandleWorker1 );
	vTaskDelete( xHandleWorker2 );
	vTaskDelete( xHandleWorker3 );

	vQueueDelete(workersQueue);
}
