/*
 * serial.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_SERIAL_H_
#define LOCALDEMOFILES_SERIAL_H_

enum SpecialKey {
	Up,
	Down,
	Right,
	Left,
	Enter,
	Esc,
	None,
};

enum SpecialKey getSpecialKey();

void defaultSerial();

void moveMouse();

void interactMenu();

void interactWorkers();

#endif /* LOCALDEMOFILES_SERIAL_H_ */
