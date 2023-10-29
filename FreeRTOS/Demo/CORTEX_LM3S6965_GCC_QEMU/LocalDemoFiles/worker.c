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

static TaskHandle_t xHandleWorker1 = NULL;
static TaskHandle_t xHandleWorker2 = NULL;
static TaskHandle_t xHandleWorker3 = NULL;

void WorkerTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;
	const TickType_t secondFrequency = pdMS_TO_TICKS(1000);

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
		int x;
		if (xQueueReceive(workersQueue, &x, portMAX_DELAY ) == pdPASS)
		{
			if (x != 0)
			{
				*timer = x;
				xLastWakeTime = xTaskGetTickCount();
			}
		}

		while (*timer > 0) {
			xTaskDelayUntil( &xLastWakeTime, secondFrequency );
			*timer -= 1;
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
