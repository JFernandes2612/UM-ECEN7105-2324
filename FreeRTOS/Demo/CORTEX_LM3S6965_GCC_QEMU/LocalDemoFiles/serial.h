/*
 * serial.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_SERIAL_H_
#define LOCALDEMOFILES_SERIAL_H_

enum Direction {
	Up,
	Down,
	Right,
	Left,
	Enter,
	Esc,
	None,
};

enum Direction getSpecialKey();

void printSerial();

void moveMouse();

void interactMenu();

#endif /* LOCALDEMOFILES_SERIAL_H_ */
