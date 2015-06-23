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

class DisplayController : public task
{
public:
	//! default constructor.
	DisplayController(void) : standbyFlag(this,"display standby flag"), LCDChannel(this,"LCD data channel"), running(true)
	{
	 	buffLen = display::characters+display::height;
		buffer = new unsigned char [buffLen]
	}	//total number of characters on display,
		//plus one extra character per line for newline character.
	//! Queues up the offered content for displaying.
	void display(DisplayInfo & content);
	//! Performs display test.
	//
	//! Note that this can be resource intensive.
	void test();

private:
	void main(void);
	void write(DisplayInfo * info);
	
	RTOS::flag standbyFlag;
	RTOS::channel<DisplayInfo,3> LCDChannel;
	bool running;
	unsigned char buffer[];
	int buffLen;
};

#endif
