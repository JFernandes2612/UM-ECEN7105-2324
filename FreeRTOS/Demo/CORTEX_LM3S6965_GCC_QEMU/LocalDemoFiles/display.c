/*
 * display.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include <globals.h>
#include "display.h"
#include "serial.h"
#include "osram128x64x4.h"

void printScreanType()
{
	long int i = 0;
	// Write line to line
	while (serialBuffer[i])
	{
		OSRAM128x64x4StringDraw(serialBuffer + i, 0, i/21 * 8, 0xF, 1);
		i += 21;
	}
}

void printMouse()
{
	OSRAM128x64x4PixelDraw(mouse.x % OSRAM128x64x4WIDTH, mouse.y % OSRAM128x64x4HEIGHT, 0xF, 1);
}

void printMenu()
{
	switch (menu)
	{
		case SCREEN_TYPE_SELECTION:
			OSRAM128x64x4StringDraw("Screen Type Mode", 0, 0, 0xF, 1);
			break;
		case MOUSE_SELECTION:
			OSRAM128x64x4StringDraw("Mouse Mode", 0, 0, 0xF, 1);
			break;
	}
}