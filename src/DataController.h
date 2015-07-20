#ifndef __DATACONTROLLER_H
#define __DATACONTROLLER_H 

#include "pRTOS.h"
#include "ButtonListener.h"
#include "HallSensorListener.h"
#include "DisplayState.h"


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

	DisplayState currentState;
	TripData currentTrip;
};

#endif
