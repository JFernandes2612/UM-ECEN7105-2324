/*
 * serial.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "serial.h"

#include "hw_memmap.h"
#include "hw_types.h"

#include "uart.h"

#include "globals.h"

#include "worker.h"
#include "snake.h"

void cleanSerial()
{
	while (UARTCharsAvail(UART0_BASE)) {
		UARTCharGet(UART0_BASE);
	}

	UARTCharPut(UART0_BASE, 0x1B);
	UARTCharPut(UART0_BASE, 0x5B);
	UARTCharPut(UART0_BASE, 0x32);
	UARTCharPut(UART0_BASE, 0x4A);

	UARTCharPut(UART0_BASE, 0x1B);
	UARTCharPut(UART0_BASE, 0x5B);
	UARTCharPut(UART0_BASE, 0x48);
}

void writeToSerialBuffer(long i, char v)
{
	if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
		serialBuffer[i] = v;
		xSemaphoreGive(displayS);
	}
}

void defaultSerial()
{
	static char c;
	static long i = 0;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x1B)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				while (i > 0)
				{
					serialBuffer[i] = 0;
					i--;
				}
				serialBuffer[i] = 0;
				cleanSerial();
				state = 0;
				xSemaphoreGive(displayS);
			}
			break;
		}

		UARTCharPut(UART0_BASE, c);

		if (c == 0x08)
		{
			if (i == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			i--;
			writeToSerialBuffer(i, 0);
		}
		else
		{
			writeToSerialBuffer(i, c);
			i++;
		}
	}
}

enum SpecialKey getSpecialKey()
{
	static char c;
	long counter = 0;
	char error = 0;

	// Try to get escape codes to navigate screen
	while (UARTCharsAvail(UART0_BASE) && counter < 3) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x0D)
		{
			return Enter;
		}

		if (!error){
			switch (counter) {
				case 0:
				{
					if (c != 0x1B)
						return None;
					break;
				}
				case 1:
				{
					if (c != '[')
						return None;
					break;
				}
				case 2:
				{
					switch (c) {
						case 'A':
							return Up;
						case 'B':
							return Down;
						case 'C':
							return Right;
						case 'D':
							return Left;
						default:
							return None;
					}
				}
			}
			counter++;
		}
	}

	if (c == 0x1B)
		return Esc;

	return None;
}

void moveMouse()
{
	const enum SpecialKey d = getSpecialKey();

	switch (d)
	{
		case Up:
			mouse.y -= 1;
			break;
		case Down:
			mouse.y += 1;
			break;
		case Right:
			mouse.x += 1;
			break;
		case Left:
			mouse.x -= 1;
			break;
		case Esc:
			state = 0;
			mouse.x = 0;
			mouse.y = 0;
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				cleanSerial();
				xSemaphoreGive(displayS);
			}
			break;
		default:
			break;
	}
}

void interactWorkers()
{
	static char c;
	static long i = 0;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x0D)
		{
			if (i > 0)
			{
				int x = atoi(serialBuffer);
				xQueueSendToBack(workersQueue, &x, ( TickType_t ) 0 );
			}
			continue;
		}

		if (c == 0x1B)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				while (i > 0)
				{
					serialBuffer[i] = 0;
					i--;
				}
				serialBuffer[i] = 0;
				stopWorkers();
				cleanSerial();
				state = 0;
				xSemaphoreGive(displayS);
			}
			break;
		}

		if ((i < 3 && (isdigit(c) || c == 0x08)) || (i == 3 && c == 0x08))
			UARTCharPut(UART0_BASE, c);

		if (c == 0x08)
		{
			if (i == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			i--;
			writeToSerialBuffer(i, 0);
		}
		else
		{
			if (i < 3 && isdigit(c))
			{
				writeToSerialBuffer(i, c);
				i++;
			}
		}
	}
}

void interactMenu()
{
	const enum SpecialKey d = getSpecialKey();

	char changeState = false;

	switch (d)
	{
		case Up:
			menu -= 1;
			break;
		case Down:
			menu += 1;
			break;
		case Enter:
			changeState = true;
			break;
		case Esc:
			state = 0;
			break;
		default:
			break;
	}

	menu %= numberOfMenuStates;

	if (changeState)
	{
		state = menu + 1;

		if (state == WORKERS)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				initWorkers();
				xSemaphoreGive(displayS);
			}
		}

		if (state == SNAKE)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				initSnake();
				xSemaphoreGive(displayS);
			}
		}
	}
}

void interactSnakeGame()
{
	const enum SpecialKey d = getSpecialKey();

	switch (d)
	{
		case Up:
			if (snake.d != S_DOWN)
				snake.d = S_UP;
			break;
		case Down:
			if (snake.d != S_UP)
				snake.d = S_DOWN;
			break;
		case Right:
			if (snake.d != S_LEFT)
				snake.d = S_RIGHT;
			break;
		case Left:
			if (snake.d != S_RIGHT)
				snake.d = S_LEFT;
			break;
		case Esc:
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				stopSnake();
				state = 0;
				xSemaphoreGive(displayS);
			}
			break;
		default:
			break;
	}
}
