/*
 * utils.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include "globals.h"

SemaphoreHandle_t displayS = NULL;

enum State state = MENU;

char serialBuffer[1024] = {0};

struct Mouse mouse = {0, 0};

enum MenuSelection menu = SCREEN_TYPE_SELECTION;

signed char numberOfMenuStates = 3;

QueueHandle_t workersQueue = NULL;

unsigned int current_timer_1 = 0;
unsigned int current_timer_2 = 0;
unsigned int current_timer_3 = 0;


