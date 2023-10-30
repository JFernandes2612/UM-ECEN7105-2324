/*
 * display.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include <stdio.h>

#include <globals.h>
#include "display.h"
#include "serial.h"
#include "osram128x64x4.h"
#include "snake.h"
#include "video.h"
#include "globals.h"

void printScreanType()
{
	unsigned int i = 0;
	// Write line to line
	while (i < 169)
	{
		OSRAM128x64x4StringDraw(serialBuffer + i, 0, i/21 * 8, 0xF, 1);
		i += 21;
	}

	OSRAM128x64x4RectangleDraw(serialBufferIndex%21 * 6, (serialBufferIndex/21 + 1) * 7 + serialBufferIndex/21, 5, 1, flash_cursor, 1);
}

void printMouse()
{
	OSRAM128x64x4PixelDraw(mouse.x % OSRAM128x64x4WIDTH, mouse.y % OSRAM128x64x4HEIGHT, 0xF, 1);
}

void printMenu()
{
	OSRAM128x64x4StringDraw("Main Menu", 0, 0, 0xF, 1);

	switch (menu)
	{
		case SCREEN_TYPE_SELECTION:
			OSRAM128x64x4StringDraw("Screen Type Mode", 0, 15, 0xF, 1);
			break;
		case MOUSE_SELECTION:
			OSRAM128x64x4StringDraw("Mouse Mode", 0, 15, 0xF, 1);
			break;
		case WORKERS_SELECTION:
			OSRAM128x64x4StringDraw("Workers Mode", 0, 15, 0xF, 1);
			break;
		case SNAKE_SELECTION:
			OSRAM128x64x4StringDraw("Snake Game", 0, 15, 0xF, 1);
			break;
		case VIDEO_SELECTION:
			OSRAM128x64x4StringDraw("Video", 0, 15, 0xF, 1);
			break;
		case SIN_SELECTION:
			OSRAM128x64x4StringDraw("Sin Function Plot", 0, 15, 0xF, 1);
			break;
	}

	char currentTime[7];
	sprintf(currentTime, "%02d:%02d", menuClock/60, menuClock%60);
	OSRAM128x64x4StringDraw(currentTime, 0, 56, 0xA, 1);
}


void printWorkers()
{
	OSRAM128x64x4StringDraw("Workers", 0, 0, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 1", 65, 8, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 2", 65, 28, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 3", 65, 48, 0xF, 1);

	OSRAM128x64x4StringDraw(serialBuffer, 10, 32, 0xF, 1);

	char s[4];

	sprintf(s, "%d", current_timer_1);
	OSRAM128x64x4StringDraw(s, 65, 16, 0xF, 1);

	sprintf(s, "%d", current_timer_2);
	OSRAM128x64x4StringDraw(s, 65, 36, 0xF, 1);

	sprintf(s, "%d", current_timer_3);
	OSRAM128x64x4StringDraw(s, 65, 56, 0xF, 1);

	OSRAM128x64x4RectangleDraw(serialBufferIndex * 6 + 10, 32 + 7, 5, 1, flash_cursor, 1);

	char nextUp[9];
	unsigned int nextUpVal;
	if (xQueuePeek(workersQueue, &nextUpVal, ( TickType_t ) 0) == pdPASS)
	{
		sprintf(nextUp, "Next-%d", nextUpVal);
		OSRAM128x64x4StringDraw(nextUp, 0, 12, 0xF, 1);
	}
}

void drawSnakeGame()
{
	unsigned int i = snake.p;

	char score[9];

	while (i != 0)
	{
		OSRAM128x64x4PixelDraw(snake.s[i].x % OSRAM128x64x4WIDTH, snake.s[i].y % OSRAM128x64x4HEIGHT, 0x8, 1);
		i--;
	}
	OSRAM128x64x4PixelDraw(snake.s[0].x % OSRAM128x64x4WIDTH, snake.s[0].y % OSRAM128x64x4HEIGHT, 0xB, 1);

	OSRAM128x64x4PixelDraw(snake.food.x, snake.food.y, 0xF, 1);

	OSRAM128x64x4RectangleDraw(SNAKEGAME_START_X, SNAKEGAME_START_Y, SNAKEGAME_WIDTH, SNAKEGAME_HEIGHT, 0x1, 1);

	sprintf(score, "Score-%d", snake.p);
	OSRAM128x64x4StringDraw(score, 0, 0, 0xF, 1);
}

void drawVideo()
{
	OSRAM128x64x4ImageDraw(video[frame/2], 34, 0, 20, 16, 1, 3);

	if (xSemaphoreTake(videoMutex, portMAX_DELAY ) == pdTRUE){
		frame++;
		frame %= totalVideoFrames; // 2612 frames total
		xSemaphoreGive(videoMutex);
	}

	char currentTime[7];
	sprintf(currentTime, "%02d:%02d", frame/1800, (frame%1800)/30);
	OSRAM128x64x4StringDraw(currentTime, 0, 0, 0xF, 1);

	char finalTime[7];
	sprintf(finalTime, "%02d:%02d", totalVideoFrames/1800, (totalVideoFrames%1800)/30);
	OSRAM128x64x4StringDraw(finalTime, 95, 0, 0xF, 1);

	OSRAM128x64x4RectangleDraw(14, 54, frame * 100 / totalVideoFrames, 4, 0xF, 1);
	OSRAM128x64x4RectangleDraw(14 + frame * 100 / totalVideoFrames, 54, 1, 4, (frame * 100 * 15 / totalVideoFrames) % 15, 1);
}

void drawFunc()
{
	OSRAM128x64x4StringDraw("Sin(Ax) * B + C", 0, 0, 0xF, 1);

	if (funcState == A)
	{
		OSRAM128x64x4StringDraw("A:", 0, 8, 0xF, 1);
	}
	else if (funcState == B)
	{
		OSRAM128x64x4StringDraw("B:", 0, 8, 0xF, 1);
	}
	else if (funcState == C)
	{
		OSRAM128x64x4StringDraw("C:", 0, 8, 0xF, 1);
	}

	OSRAM128x64x4StringDraw(serialBuffer, 12, 8, 0xF, 1);

	if (funcState != DONE)
		OSRAM128x64x4RectangleDraw(serialBufferIndex * 6 + 12, 15, 5, 1, flash_cursor, 1);

	OSRAM128x64x4RectangleDraw(14, 35, 100, 1, 0x7, 1);
	OSRAM128x64x4RectangleDraw(64, 10, 1, 50, 0x7, 1);

	if (funcState == DONE)
	{
		char k = 0;
		while (k < 100)
		{
			OSRAM128x64x4PixelDraw(k + 14, func[(k + funcStartIndex)%(funcFreq * 2)] + 35, 0xF, 1);
			k++;
		}
	}
}

// Flash Cursor

static TaskHandle_t xHandleFlashCursor = NULL;

void FlashCursorTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;

	const TickType_t xDefaultFrequency = pdMS_TO_TICKS(25);

	xLastWakeTime = xTaskGetTickCount();

	unsigned char bounce = 1;

	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xDefaultFrequency );

		if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
			if (bounce)
			{
				if (flash_cursor == 12)
					bounce = 0;
				else
					flash_cursor += 1;;
			}
			else
			{
				if (flash_cursor == 0)
					bounce = 1;
				else
					flash_cursor -= 1;
			}
			xSemaphoreGive(displayS);
		}

	}
}

void startFlashCursor()
{
	xTaskCreate( FlashCursorTask, "FlashCursor", 60, NULL, tskIDLE_PRIORITY + 2, &xHandleFlashCursor );
}

void stopFlashCursor()
{
	vTaskDelete(xHandleFlashCursor);
}

void clockTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;

	const TickType_t xDefaultFrequency = pdMS_TO_TICKS(1000);

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xDefaultFrequency );

		if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
			menuClock++;
			xSemaphoreGive(displayS);
		}

	}
}
