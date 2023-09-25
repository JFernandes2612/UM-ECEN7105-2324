/*
 * utils.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_GLOBALS_H_
#define LOCALDEMOFILES_GLOBALS_H_

// MOUSE
struct Mouse {
	unsigned char x, y;
};

extern struct Mouse mouse;

// SERIAL INPUT BUFFER
extern char serialBuffer[1024];

// MENU

enum MenuSelection {
	SCREEN_TYPE_SELECTION,
	MOUSE_SELECTION,
};

extern enum MenuSelection menu;

#endif /* LOCALDEMOFILES_GLOBALS_H_ */
