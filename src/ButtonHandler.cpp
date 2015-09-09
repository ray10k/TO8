#include "ButtonHandler.h"

ButtonHandler :: ButtonHandler (int prio):
	RTOS::task (prio, "ButtonHandler"),
	CheckBtnClock (this, 100 MS, "CheckBtnClock"),
	nr(0)
{}

void ButtonHandler :: addButton (Button &button){
	if (nr < MAX_BUTTONS){
		theButtons[nr] = &button;
		nr += 1;
	}
}

void ButtonHandler :: main(void){
	for (;;){
		(void) wait (CheckBtnClock);
		for (int i = 0; i < nr; i++){
			theButtons[i] -> updateState();
			release();
		}
	}
}
