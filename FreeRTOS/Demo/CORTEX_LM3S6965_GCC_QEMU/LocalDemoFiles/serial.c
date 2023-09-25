/*
 * serial.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include "serial.h"

#include "hw_memmap.h"
#include "hw_types.h"

#include "uart.h"

#include <globals.h>
#include "state.h"

void printSerial()
{
	static char c;
	static long i = 0;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		UARTCharPut(UART0_BASE, c);

		if (c == 0x1B)
		{
			state = 0;
			while (i > 0)
			{
				serialBuffer[i] = 0;
				i--;
			}
			serialBuffer[i] = 0;
			break;
		}

		if (c == 0x08)
		{
			if (i == 0)
				return;

			c = ' ';
			UARTCharPut(UART0_BASE, c);
			UARTCharPut(UART0_BASE, 0x08);

			i--;
			serialBuffer[i] = 0;
		}
		else
		{
			serialBuffer[i] = c;
			i++;
		}
	}
}

enum Direction getSpecialKey()
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
	const enum Direction d = getSpecialKey();

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
			break;
		default:
			break;
	}
}

void interactMenu()
{
	const enum Direction d = getSpecialKey();

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

	menu %= 2;

	if (changeState)
		state = menu + 1;
}