#include "Button.h"
#include "Armboard.h"

Button :: Button (mkt_key aKey) : 
	key (aKey),
	pressed (false),
	startTime (0)
{}

void Button :: setListener (ButtonListener *lst){
	theLst = lst,
}

void Button :: updateState (void){
	bool b = armboard::keypad::is_pressed (key);
	
	if (pressed){
		if (!b){
			pressed = false;
			startTime = 0;
			theLst->buttonShortPress (this);
		}
		else if (startTime >= run_time()){
			pressed = false;
			startTime = 0;
			theLst->buttonLongPress (this);
		}
	}
	else if (b){
		pressed = true;
		startTime = run_time() + 1500;
	}
}