#ifndef __DISPLAY_CONTROLLER_H
#define __DISPLAY_CONTROLLER_H

#include "display_info.h"
#include "pRTOS.h"
#include "common.h"

//*************************************
//
// class DisplayController
//
//! Acts as a separation between the LCD display and other tasks,
//! so displaying information can be done asynchronously.
//! Note that this class assumes that it can find the number
//! of characters the display can take can be found in the
//! common.h header file.
//
//*************************************

class DisplayController : public RTOS::task
{
public:
	//! default constructor.
	DisplayController(void) : 
		RTOS::task(999, "display controller"),
		standbyFlag(this,"display standby flag"), 
		testFlag(this,"display test flag"),
		LCDChannel(this,"LCD data channel"), 
		running(true),
		tested(false)
	{
	 	buffLen = display::characters+display::height;
	}	//total number of characters on display,
		//plus one extra character per line for newline character.
	//! Queues up the offered content for displaying.
	void display(DisplayInfo & content);
	//! Schedule display test.
	void test();

private:
	void main(void);
	void write(void);
	void LCDTest(void);
	void writeString(const char * string, int length);
	
	RTOS::flag standbyFlag, testFlag;
	RTOS::channel<DisplayInfo,3> LCDChannel;
	bool running,tested;
	char buffer[display::height][display::width];
	int buffLen;
};

#endif
