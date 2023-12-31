/*
 * serial.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_SERIAL_H_
#define LOCALDEMOFILES_SERIAL_H_

void cleanSerial();

enum SpecialKey {
	Up,
	Down,
	Right,
	Left,
	Enter,
	Esc,
	None,
};

enum SpecialKey getSpecialKey(const unsigned char skipESC);

void defaultSerial();

void moveMouse();

void interactMenu();

void interactWorkers();

void interactSnakeGame();

void interactVideo();

void interactFunc();

#endif /* LOCALDEMOFILES_SERIAL_H_ */
