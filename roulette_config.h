/*
 * config.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Abhinav
 */

#ifndef ROULETTE_CONFIG_H_
#define ROULETTE_CONFIG_H_

//Parameters to configure the roulette game

//Number of LEDs in the game
#define NUM_LEDS 38

//Number of LEDs per board
#define LEDS_PER_BOARD 5
//Number of Boards
#define NUM_BOARDS 9

//Time taken to complete a full circle in ms at top speed
#define MAX_SPEED_PERIOD 1000

//Time taken to complete a full circle in ms at slowest speed
#define MIN_SPEED_PERIOD 11000

//Time taken in ms to hit top speed from bottom speed
#define ACCLERATION_TIME 2000

#define ACCLERATION_SLOPE (MIN_SPEED_PERIOD-MAX_SPEED_PERIOD)/ACCLERATION_TIME

//Time taken in ms to hit bottom speed from top speed
#define DECELERATION_TIME 10000

#define DECELERATION_SLOPE (MAX_SPEED_PERIOD-MIN_SPEED_PERIOD)/DECELERATION_TIME


//Update interval in ms when the position of the roulette ball is updated
#define UPDATE_INTERVAL 50

//Biggest jump to make when button hits obstacle
//Evenly distributed between [-BALL_JUMP,BALL_JUMP]
#define BALL_JUMP 2

//If LEDs behind current LED should be on at a lower DIM level using PWM?
//Dim level of LED next to roulette led
//#define FADE_RATE 0.1

//If LEDs behind current LED should be on at a lower DIM level using PWM?
//No of Leds to keep on
//#define NUM_LED_ON 2

typedef enum roulette_state{
	IDLE,			 //Show ready to play pattern
	BUTTON_PRESSED,  //Start timer to calculate top speed, increase rotation speed
	SLOWING_DOWN,    //
	HIT_OBSTACLE,
	IN_SLOT 		//Same as IDLE?
} roulette_state_t;

#endif /* ROULETTE_CONFIG_H_ */
