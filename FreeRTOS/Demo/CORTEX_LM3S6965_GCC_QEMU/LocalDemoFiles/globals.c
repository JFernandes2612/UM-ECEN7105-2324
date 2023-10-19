/*
 * utils.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include "globals.h"

SemaphoreHandle_t displayS = NULL;

enum State state = MENU;

char serialBuffer[169] = { 32 };
unsigned int serialBufferIndex = 0;

struct Pos mouse = {0, 0};

enum MenuSelection menu = SCREEN_TYPE_SELECTION;

signed char numberOfMenuStates = 4;

QueueHandle_t workersQueue = NULL;

unsigned int current_timer_1 = 0;
unsigned int current_timer_2 = 0;
unsigned int current_timer_3 = 0;

struct Snake snake = {{{0, 0}}, {{0, 0}}, 0, S_RIGHT, {0, 0}};


