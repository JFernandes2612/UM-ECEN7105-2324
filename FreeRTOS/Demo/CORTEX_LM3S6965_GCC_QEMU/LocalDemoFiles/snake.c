/*
 * snake.c
 *
 *  Created on: 15 Oct 2023
 *      Author: jelao
 */
#include "FreeRTOS.h"
#include "task.h"

#include "snake.h"
#include "globals.h"

TaskHandle_t xHandleSnake = NULL;

void initSnake()
{
	snake.s[0].x = 64;
	snake.s[0].y = 32;
	snake.p = 0;
	snake.d = S_RIGHT;
	snake.food.x = xTaskGetTickCount() % SNAKEGAME_WIDTH + SNAKEGAME_START_X;
	snake.food.y = xTaskGetTickCount() % SNAKEGAME_HEIGHT + SNAKEGAME_START_Y;

	// Top priority
	xTaskCreate( SnakeTask, "Snake", 60, NULL, tskIDLE_PRIORITY, &xHandleSnake );
}

void SnakeTask( void *pvParameters )
{
	( void ) pvParameters;

	TickType_t xLastWakeTime;

	const TickType_t xDefaultFrequency = pdMS_TO_TICKS(50);

	xLastWakeTime = xTaskGetTickCount();

	for (;;)
	{
		// Wait for the next cycle.
		xTaskDelayUntil( &xLastWakeTime, xDefaultFrequency );

		unsigned char px = snake.s[0].x;
		unsigned char py = snake.s[0].y;

		snake.ps[0].x = snake.s[0].x;
		snake.ps[0].y = snake.s[0].y;

		switch (snake.d)
		{
		case S_UP:
			snake.s[0].y--;
			break;
		case S_DOWN:
			snake.s[0].y++;
			break;
		case S_RIGHT:
			snake.s[0].x++;
			break;
		case S_LEFT:
			snake.s[0].x--;
			break;
		}


		snake.s[0].x %=	(SNAKEGAME_WIDTH + SNAKEGAME_START_X);
		snake.s[0].y %= (SNAKEGAME_HEIGHT + SNAKEGAME_START_Y);

		if (snake.s[0].x == 0 && snake.d == S_RIGHT)
			snake.s[0].x = SNAKEGAME_START_X;
		else if (snake.s[0].x == SNAKEGAME_START_X - 1 && snake.d == S_LEFT)
			snake.s[0].x = SNAKEGAME_START_X + SNAKEGAME_WIDTH - 1;

		if (snake.s[0].y == 0 && snake.d == S_DOWN)
			snake.s[0].y = SNAKEGAME_START_Y;
		if (snake.s[0].y == SNAKEGAME_START_Y - 1 && snake.d == S_UP)
			snake.s[0].y = SNAKEGAME_START_Y + SNAKEGAME_HEIGHT - 1;

		if (snake.s[0].x == snake.food.x && snake.s[0].y == snake.food.y)
		{
			snake.p++;
			snake.s[snake.p].x = px;
			snake.s[snake.p].x = py;

			snake.food.x = xTaskGetTickCount() % SNAKEGAME_WIDTH + SNAKEGAME_START_X;
			snake.food.y = xTaskGetTickCount() % SNAKEGAME_HEIGHT + SNAKEGAME_START_Y;
		}

		unsigned int i = 1;

		while (i <= snake.p)
		{
			snake.ps[i].x = snake.s[i].x;
			snake.ps[i].y = snake.s[i].y;

			snake.s[i].x = snake.ps[i-1].x;
			snake.s[i].y = snake.ps[i-1].y;

			if (snake.s[i].x == snake.s[0].x && snake.s[i].y == snake.s[0].y)
				snake.p = 0;

			i++;
		}
	}
}

void stopSnake()
{
	vTaskDelete( xHandleSnake );
}
