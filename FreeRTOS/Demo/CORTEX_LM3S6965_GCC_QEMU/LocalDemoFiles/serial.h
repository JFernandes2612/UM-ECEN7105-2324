/*
 * serial.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_SERIAL_H_
#define LOCALDEMOFILES_SERIAL_H_

struct Mouse {
	unsigned char x, y;
};

extern struct Mouse mouse;

extern char serialBuffer[1024];

void printSerial();

void moveMouse();

#endif /* LOCALDEMOFILES_SERIAL_H_ */
