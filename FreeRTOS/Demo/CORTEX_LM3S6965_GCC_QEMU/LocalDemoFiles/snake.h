/*
 * snake.h
 *
 *  Created on: 15 Oct 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_SNAKE_H_
#define LOCALDEMOFILES_SNAKE_H_

#define SNAKEGAME_START_X 19
#define SNAKEGAME_START_Y 12

#define SNAKEGAME_WIDTH 90
#define SNAKEGAME_HEIGHT 40

void SnakeTask( void *pvParameters );

void initSnake();

void stopSnake();

#endif /* LOCALDEMOFILES_SNAKE_H_ */
