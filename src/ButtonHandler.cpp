/* ========================================================================== */
/*                                                                            */
/*   ButtonHandler.cpp                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "ButtonHandler.h"

ButtonHandler::ButtonHandler (int prio) : 
RTOS::task (prio, "ButtonHandler"),
bhPeriod (this , 50 MS, "bhClock"),
nr(0)
{}

void ButtonHandler::addButton (Button &button){
	if (nr < MAX_BUTTONS){
		trace << "add button";
		theButtons[nr] = &button;
		nr+=1;
	}
}

void ButtonHandler::main(void){
	for (;;) {
		(void) wait (bhPeriod);
		for (int i = 0; i < nr; i++){
			theButtons[i]->updateState();
			release();
		}
	}
}


