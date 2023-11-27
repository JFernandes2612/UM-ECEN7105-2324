/*
 * display.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_DISPLAY_H_
#define LOCALDEMOFILES_DISPLAY_H_

void printScreanType();

void printMouse();

void printMenu();

void printWorkers();

void drawSnakeGame();

void drawVideo();

void drawFunc();

// Flash Cursor

void FlashCursorTask( void *pvParameters );

void startFlashCursor();

void stopFlashCursor();

// Clock

void ClockTask( void *pvParameters );

#endif /* LOCALDEMOFILES_DISPLAY_H_ */
