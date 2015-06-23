#ifndef __DISPLAY_CONTROLLER_H
#define __DISPLAY_CONTROLLER_H

#include "display_states.h"
#include "display_info.h"
#include "pRTOS.h"

class DisplayController
{
public:
	DisplayController(void);

	void write(DisplayInfo & info);

private:
	RTOS::flag update(this,"display update flag");
	/* data */
};

#endif
