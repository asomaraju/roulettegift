#include "mbed.h"

#include "roulette_pins.h"
#include "roulette_config.h"

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)


roulette_state_t current_state;
int last_button_release_period = 0; //period in ms when the button was realsed. This is used to determine how long the ball goes around

Serial pc(USBTX,USBRX);
InterruptIn button(USER_BUTTON);

Timer state_timer; //Used to calculate the state of the ball
Timer update_timer; //Used to update the position of the ball every UPDATE_INTERVAL ms

DigitalOut boards[] = {(BOARD_SELECT_1), (BOARD_SELECT_2), (BOARD_SELECT_3), (BOARD_SELECT_4), (BOARD_SELECT_5), (BOARD_SELECT_6), (BOARD_SELECT_7), (BOARD_SELECT_8), (BOARD_SELECT_9)};
DigitalOut leds[] = {(LED_SELECT_1),(LED_SELECT_2),(LED_SELECT_3),(LED_SELECT_4),(LED_SELECT_5)};

//return time period in ms if the speed != 0
//	period = -1 on error
//	period = 0 if speed = 0
int calculate_speed_period(){
	switch (current_state) {
	case IN_SLOT:
		return 0;
	case IDLE:
		return 0;
	case BUTTON_PRESSED: //i.e. ball is speeding up at speed 0 when button pressed
	{
		int elapsed_time = state_timer.read_ms();
		return MIN_SPEED_PERIOD + MIN(elapsed_time, ACCLERATION_TIME)*ACCLERATION_SLOPE;
	}
	case SLOWING_DOWN:
	{
		int elapsed_time = state_timer.read_ms();
		return (last_button_release_period + elapsed_time*DECELERATION_SLOPE);
	}
	default: //Error speed unknown when obstacle hit
		return -1;
	}
}


//Function called when button is pressed
//Start speeding up the roulette ball
//Longer the button is pressed the faster the ball moves
//Should only react when the state is IDLE or IN_SLOT
void button_pressed(){
	if ((current_state == IN_SLOT)||(current_state == IDLE)){
		current_state = BUTTON_PRESSED;
		state_timer.reset();
		state_timer.start();
	}
	else {
		pc.printf("Button can only be pressed if idle or IN_SLOT\n");
	}
}

//Function called when button is released
//Start slowing down the roulette ball
//Only do something if alread in Button pressed state
//ToDo: If the button released function is not called then will be stuck indefinitely in the wrong state: Add reset functionality if pressed and not released for a long time e.g. 20-30 seconds.
void button_released(){
	if(current_state == BUTTON_PRESSED){
		last_button_release_period = calculate_speed_period();
		//ToDo: Check if state_timer needs to be restarted
		srand(state_timer.read_ms());
		state_timer.reset();
		current_state = SLOWING_DOWN;
	}
	else
		pc.printf("Button can only be relesed in BUTTON_PRESSED state\n");
}


//Calulates board_select pin and led_select pin based on ball position
//Turns on the baord/led pins correctly
//ToDo: Modify this function based on how board_select/led_select is connected
void draw_ball_position(int i){

	pc.printf("Drawing R %d\n", i);
	if (current_state == IDLE)
		pc.printf("Idle state - may want to make a blinky pattern\n");

	int board_select = i/LEDS_PER_BOARD;
	int led_select = i%LEDS_PER_BOARD;

	for (int j = 0; j ++ ; j < NUM_BOARDS){
		if (j == board_select)
			boards[j] = 1;
		else
			boards[j] = 0;
	}
	for (int j = 0; j ++ ; j < LEDS_PER_BOARD){
		if (j == led_select)
			leds[j] = 1;
		else
			leds[j] = 0;
	}
}

//error returns -1 otherwise ball position is returned
int update_ball_position(){
	static int last_ball_position = 0;
	int time_since_last_update = update_timer.read_ms();
	int period = calculate_speed_period();
	if (period != 0)
		last_ball_position = (last_ball_position + NUM_LEDS*time_since_last_update/period) % NUM_LEDS;
	else if (period = -1)
		return -1;

	if (current_state == SLOWING_DOWN){
		//Check if simulating of ball hitting obstacle should be done
		if (period > MIN_SPEED_PERIOD){
			state_timer.stop();
			int ball_jump = rand()*BALL_JUMP*2/RAND_MAX - BALL_JUMP;
			last_ball_position = last_ball_position+ball_jump;
			current_state = IN_SLOT;
		}
	}
	//else if period == 0 then no change in ball position i.e. speed = 0

	draw_ball_position(last_ball_position);
	update_timer.reset();
	return last_ball_position;
}

// main() runs in its own thread in the OS
// (note the calls to Thread::wait below for delays)
int main() {
    pc.baud(115200);
    pc.printf("Roulette game\n");

    //ToDo:Check if pressed and release correspond to the falling and rising edges
    button.fall(button_pressed);
    button.rise(button_released);
    update_timer.start();
    current_state = IDLE;
    while (true) {
        pc.printf("Updating roulette display\n");
        if(update_ball_position() == -1)
        	pc.printf("Error in updating ball position");
        Thread::wait(UPDATE_INTERVAL);
    }
}
