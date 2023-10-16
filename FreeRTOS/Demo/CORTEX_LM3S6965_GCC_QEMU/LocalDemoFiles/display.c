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

void printScreanType()
{
	long int i = 0;
	// Write line to line
	if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
		while (serialBuffer[i])
		{
			OSRAM128x64x4StringDraw(serialBuffer + i, 0, i/21 * 8, 0xF, 1);
			i += 21;
		}
		xSemaphoreGive(displayS);
	}
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
	}
}


void printWorkers()
{
	OSRAM128x64x4StringDraw("Workers", 0, 0, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 1", 65, 8, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 2", 65, 28, 0xF, 1);

	OSRAM128x64x4StringDraw("Worker 3", 65, 48, 0xF, 1);

	if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
		OSRAM128x64x4StringDraw(serialBuffer, 10, 32, 0xF, 1);
		xSemaphoreGive(displayS);
	}

	char s[4];

	sprintf(s, "%d", current_timer_1);
	OSRAM128x64x4StringDraw(s, 65, 16, 0xF, 1);

	sprintf(s, "%d", current_timer_2);
	OSRAM128x64x4StringDraw(s, 65, 36, 0xF, 1);

	sprintf(s, "%d", current_timer_3);
	OSRAM128x64x4StringDraw(s, 65, 56, 0xF, 1);

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
		OSRAM128x64x4PixelDraw(snake.s[i].x % OSRAM128x64x4WIDTH, snake.s[i].y % OSRAM128x64x4HEIGHT, 0xA, 1);
		i--;
	}
	OSRAM128x64x4PixelDraw(snake.s[0].x % OSRAM128x64x4WIDTH, snake.s[0].y % OSRAM128x64x4HEIGHT, 0xB, 1);

	OSRAM128x64x4PixelDraw(snake.food.x, snake.food.y, 0xF, 1);

	OSRAM128x64x4RectangleDraw(SNAKEGAME_START_X, SNAKEGAME_START_Y, SNAKEGAME_WIDTH, SNAKEGAME_HEIGHT, 0x1, 1);

	sprintf(score, "Score-%d", snake.p);
	OSRAM128x64x4StringDraw(score, 0, 0, 0xF, 1);
}
