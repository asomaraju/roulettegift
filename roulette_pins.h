/*
 * roulette_pins.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Abhinav
 */

#ifndef ROULETTE_PINS_H_
#define ROULETTE_PINS_H_

#include "mbed.h"
//Set to the right board select pins
#define BOARD_SELECT_1 D0
#define BOARD_SELECT_2 D1
#define BOARD_SELECT_3 D2
#define BOARD_SELECT_4 D3
#define BOARD_SELECT_5 D4
#define BOARD_SELECT_6 D5
#define BOARD_SELECT_7 D6
#define BOARD_SELECT_8 D7
#define BOARD_SELECT_9 D8

#define LED_SELECT_1 D9
#define LED_SELECT_2 D10
#define LED_SELECT_3 D11
#define LED_SELECT_4 D12
#define LED_SELECT_5 D13

#define BOARD_SELECT(i) BOARD_SELECT_##i
#define LED_SELECT(i) LED_SELECT_##i

#endif /* ROULETTE_PINS_H_ */
