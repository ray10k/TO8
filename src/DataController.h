#ifndef __DATACONTROLLER_H
#define __DATACONTROLLER_H 

#include "pRTOS.h"
#include "ButtonListener.h"
#include "HallSensorListener.h"
#include "DisplayState.h"
#include "display_controller.h"

const int dataControllerPriority = 231;


class DataController : public RTOS::task, public ButtonListener, public HallSensorListener
{

public:
	DataController (void);
	void buttonShortPress(Button *b);
	void buttonLongPress(Button *b);
	void HallSensorPulse(unsigned long long int time);
	void init(int time, int size);
	void statusChange(DisplayState newState);
	void setTime(int time);

private:
	void main(void);
	void clockInit(void);
	void clockUpdate(void);
	void wipeTripData(void);
	void calcStats(void);
	void updateDisplay(void);

	RTOS::flag longPressFlag;
	RTOS::channel<unsigned long long int, 25) pulseTimeChannel;
	RTOS::channel<DisplayState, 1> stateChannel;
	RTOS::pool<int> sizePool;
	RTOS::pool<int> timePool;
	RTOS::clock updateClock;

	DisplayState currentState;
	TripData currentTrip;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned long long int sincePulse;
	unsigned long long int lastPulse;



	const unsigned long long int STANDBY_TIMEOUT = 5 * 60 S;
};

#endif
