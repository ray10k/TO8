#ifndef __DATACONTROLLER_H
#define __DATACONTROLLER_H 

#include "pRTOS.h"
#include "ButtonListener.h"
#include "HallSensorListener.h"
#include "DisplayState.h"
#include "display_controller.h"
#include "TripData.h"

const int dataControllerPriority = 231; //TODO: change to a more meaningful number.

//*************************
//
// class DataController
//
//! Task to manage the data to be displayed during
//! the system's normal state.
//
//*************************


class DataController : public RTOS::task, public ButtonListener, public HallSensorListener
{

public:
	//! Construct a DataController object, and prepare the task for execution.
	DataController (void);
	//! implementation of the ButtonListener interface.
	void buttonShortPress(Button *b);
	//! implementation of the ButtonListener interface.
	void buttonLongPress(Button *b);
	//! implementation of the HallSensorListener interface.
	void HallSensorPulse(unsigned long long int time);
	//! Write the initial time for the 24-hour clock and the circumference of the 
	//! wheel, to initialize the object.
	void init(int time, int size);
	//! Schedule a change in the information displayed.
	//! WARNING: if the object hasn't been initialized with a call to init(int,int),
	//! calling this function will result in undefined behaviour.
	void statusChange(DisplayState newState);
	//! Write a new initial time to the 24-hour clock.
	void setTime(int time);

private:
	void main(void);
	void clockInit(void);
	void clockUpdate(unsigned long long int now);
	void updateDisplay(void);

	RTOS::flag longPressFlag;
	RTOS::channel<unsigned long long int, 25) pulseTimeChannel;
	RTOS::channel<DisplayState, 1> stateChannel;
	RTOS::pool<int> sizePool;
	RTOS::pool<int> timePool;
	RTOS::clock updateClock;

	DisplayState currentState;
	TripData * currentTrip;
	
	bool errored;
	unsigned int speed;
	unsigned int maxSpeed;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned long long int fraction;
	//for standby/speed timeouts.
	unsigned long long int lastPulse;
	//for updating the 24-hour clock
	unsigned long long int lastTick;

	const unsigned long long int STANDBY_TIMEOUT = 5 * 60 S;
	const unsigned long long int STANDSTILL_TIMEOUT = 5 S;
}DCon;

#endif
