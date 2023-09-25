/*
 * state.h
 *
 *  Created on: 25 Sept 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_STATE_H_
#define LOCALDEMOFILES_STATE_H_

enum State {
	MENU,
	SCREEN_TYPE,
	MOUSE,
};

static enum State s = MOUSE;

#endif /* LOCALDEMOFILES_STATE_H_ */
