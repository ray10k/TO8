#ifndef __DISPLAY_CONTROLLER_H
#define __DISPLAY_CONTROLLER_H

#include "display_states.h"
#include "pRTOS.h"

class DisplayController
{
public:
	DisplayController(arguments);
	~DisplayController();

	void displayClock(int state);
	void setClockTime(int hours, int minutes, int seconds);
	void setDisplayState(DisplayState state);

	/* data */
};

#endif
