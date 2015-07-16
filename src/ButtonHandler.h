#ifndef __ButtonHandler
#define __ButtonHandler

#include "pRTOS.h"
#include "Button.h"

#define MAX_BUTTONS 2

class ButtonHandler : public RTOS::task{
public:
	ButtonHandler (int prio);
	void addButton (Button &button);
	
private:
	void main(void);
	RTOS::clock CheckBtnClock;
	short nr;
	Button *theButtons[2];
};
#endif