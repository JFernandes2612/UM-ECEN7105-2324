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
#include "display.h"
#include "func.h"

void cleanSerial()
{
	while (UARTCharsAvail(UART0_BASE)) {
		UARTCharGet(UART0_BASE); // Clean serial inner buffer
	}

	UARTCharPut(UART0_BASE, 0x1B);
	UARTCharPut(UART0_BASE, 0x5B);
	UARTCharPut(UART0_BASE, 0x32);
	UARTCharPut(UART0_BASE, 0x4A);

	UARTCharPut(UART0_BASE, 0x1B);
	UARTCharPut(UART0_BASE, 0x5B);
	UARTCharPut(UART0_BASE, 0x48);
}

void writeToSerialBuffer(char v)
{
	if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
		serialBuffer[serialBufferIndex] = v;
		xSemaphoreGive(displayS);
	}
}

void defaultSerial()
{
	static char c;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x1B)
		{
			enum SpecialKey sk = getSpecialKey(1);

			if (sk == None) // is esc
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					while (serialBufferIndex > 0)
					{
						serialBuffer[serialBufferIndex] = 32;
						serialBufferIndex--;
					}
					serialBuffer[serialBufferIndex] = 32;
					cleanSerial();
					stopFlashCursor();
					state = 0;
					xSemaphoreGive(displayS);
				}
			} else {
				switch (sk)
				{
					case Left:
					{
						if (serialBufferIndex > 0)
							serialBufferIndex--;
						break;
					}
					case Right:
					{
						if (serialBufferIndex < 167)
							serialBufferIndex++;
						break;
					}
					case Up:
					{
						if (serialBufferIndex >= 21)
							serialBufferIndex-=21;
						break;
					}
					case Down:
					{
						if (serialBufferIndex <= 147)
							serialBufferIndex+=21;
						break;
					}
					default:
						break;
				}
			}
			break;
		}

		if (serialBufferIndex != 168)
			UARTCharPut(UART0_BASE, c);

		if (c == 0x08)
		{
			if (serialBufferIndex == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			serialBufferIndex--;
			writeToSerialBuffer(0);
		}
		else
		{
			if (serialBufferIndex != 168)
			{
				flash_cursor = 12;
				writeToSerialBuffer(c);
				serialBufferIndex++;
			}
		}
	}
}

enum SpecialKey getSpecialKey(const unsigned char skipESC)
{
	static char c;
	long counter = skipESC;
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
	const enum SpecialKey d = getSpecialKey(0);

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

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x0D)
		{
			if (serialBufferIndex > 0)
			{
				int x = atoi(serialBuffer);
				xQueueSendToBack(workersQueue, &x, ( TickType_t ) 0 );
			}
			continue;
		}

		if (c == 0x1B)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				while (serialBufferIndex > 0)
				{
					serialBuffer[serialBufferIndex] = 32;
					serialBufferIndex--;
				}
				serialBuffer[serialBufferIndex] = 32;
				stopWorkers();
				cleanSerial();
				stopFlashCursor();
				state = 0;
				xSemaphoreGive(displayS);
			}
			break;
		}

		if ((serialBufferIndex < 3 && (isdigit(c) || c == 0x08)) || (serialBufferIndex == 3 && c == 0x08))
			UARTCharPut(UART0_BASE, c);

		if (c == 0x08)
		{
			if (serialBufferIndex == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			serialBufferIndex--;
			writeToSerialBuffer(0);
		}
		else
		{
			if (serialBufferIndex < 3 && isdigit(c))
			{
				flash_cursor = 12;
				writeToSerialBuffer(c);
				serialBufferIndex++;
			}
		}
	}
}

void interactMenu()
{
	const enum SpecialKey d = getSpecialKey(0);

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

		switch (state)
		{
			case WORKERS:
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					initWorkers();
					startFlashCursor();
					xSemaphoreGive(displayS);
				}
				break;
			}
			case SNAKE:
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					initSnake();
					xSemaphoreGive(displayS);
				}
				break;
			}
			case SCREEN_TYPE:
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					startFlashCursor();
					xSemaphoreGive(displayS);
				}
				break;
			}
			case VIDEO:
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					frame = 0;
					videoMutex = xSemaphoreCreateMutex();
					xSemaphoreGive(displayS);
				}
				break;
			}
			case SIN:
			{
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					startFlashCursor();
					startFunc();
					xSemaphoreGive(displayS);
				}
				break;
			}
			default:
				break;
		}
	}
}

void interactSnakeGame()
{
	const enum SpecialKey d = getSpecialKey(0);

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

void interactVideo()
{
	const enum SpecialKey d = getSpecialKey(0);

	switch (d)
	{
		case Right:
			if (xSemaphoreTake(videoMutex, portMAX_DELAY ) == pdTRUE){
				if (frame < totalVideoFrames - 30)
				{
					frame += 30;
				}
				xSemaphoreGive(videoMutex);
			}
			break;
		case Left:
			if (xSemaphoreTake(videoMutex, portMAX_DELAY ) == pdTRUE){
				if (frame >= 30)
				{
					frame -= 30;
				}
				xSemaphoreGive(videoMutex);
			}
			break;
		case Esc:
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				state = 0;
				frame = 0;
				vSemaphoreDelete(videoMutex);
				xSemaphoreGive(displayS);
			}
			break;
		default:
			break;
	}
}

void interactFunc()
{
	static char c;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		if (c == 0x0D)
		{
			if (serialBufferIndex > 0 && funcState != DONE)
			{
				int x = atoi(serialBuffer);
				xQueueSendToBack(funcQueue, &x, ( TickType_t ) 0 );
				if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
					while (serialBufferIndex > 0)
					{
						serialBuffer[serialBufferIndex] = 32;
						serialBufferIndex--;
					}
					serialBuffer[serialBufferIndex] = 32;
					cleanSerial();
					xSemaphoreGive(displayS);
				}
			}
			continue;
		}

		if (c == 0x1B)
		{
			if (xSemaphoreTake(displayS, portMAX_DELAY ) == pdTRUE){
				while (serialBufferIndex > 0)
				{
					serialBuffer[serialBufferIndex] = 32;
					serialBufferIndex--;
				}
				serialBuffer[serialBufferIndex] = 32;
				stopFunc();
				cleanSerial();
				stopFlashCursor();
				state = 0;
				xSemaphoreGive(displayS);
			}
			break;
		}

		if (((serialBufferIndex < 3 && (isdigit(c) || c == 0x08 || (c == '-' && serialBufferIndex == 0))) || (serialBufferIndex == 3 && c == 0x08)) && funcState != DONE)
			UARTCharPut(UART0_BASE, c);

		if (c == 0x08 && funcState != DONE)
		{
			if (serialBufferIndex == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			serialBufferIndex--;
			writeToSerialBuffer(0);
		}
		else if (serialBufferIndex < 3 && (isdigit(c) || (c == '-' && serialBufferIndex == 0)) && funcState != DONE)
		{
			flash_cursor = 12;
			writeToSerialBuffer(c);
			serialBufferIndex++;
		}
	}
}
