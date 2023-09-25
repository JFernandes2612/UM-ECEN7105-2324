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

char serialBuffer[1024] = {0};

struct Mouse mouse = {0, 0};

void printSerial()
{
	static char c;
	static long i = 0;

	while (UARTCharsAvail(UART0_BASE)) {
		c = UARTCharGet(UART0_BASE);

		UARTCharPut(UART0_BASE, c);

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

void moveMouse()
{
	static char c;
	long counter = 0;
	char error = 0;

	// Try to get escape codes to navigate screen
	while (UARTCharsAvail(UART0_BASE) && counter < 3) {
		c = UARTCharGet(UART0_BASE);


		if (!error){
			switch (counter) {
				case 0:
				{
					if (c != 0x1b)
						error = 1;
					break;
				}
				case 1:
				{
					if (c != '[')
						error = 1;
					break;
				}
				case 2:
				{
					switch (c) {
						case 'A':
							mouse.y -= 1;
							break;
						case 'B':
							mouse.y += 1;
							break;
						case 'C':
							mouse.x += 1;
							break;
						case 'D':
							mouse.x -= 1;
							break;
						default:
							error = 1;
							break;
					}
				}
			}
			counter++;
		}
	}
}
