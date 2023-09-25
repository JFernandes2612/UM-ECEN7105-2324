/*
 * utils.c
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#include "globals.h"

char serialBuffer[1024] = {0};

struct Mouse mouse = {0, 0};

enum MenuSelection menu = SCREEN_TYPE_SELECTION;
