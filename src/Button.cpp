#include "Button.h"
#include "Armboard.h"

Button :: Button (mkt_key aKey) : 
	key (aKey),
	pressed (false),
	longPress(false),
	startTime (0)
{}

void Button :: setListener (ButtonListener *lst){
	theLst = lst;
}

void Button :: updateState(void){
	bool b = armboard::keypad::is_pressed(key);
	
	if(pressed){
		if(!b){
			pressed = false;
			startTime = 0;
			theLst->buttonShortPress(this);
		}
		else if(startTime <= RTOS::run_time()){
			pressed = false;
			longPress = true;
			startTime = 0;
			theLst->buttonLongPress(this);
		}
	}
	else if(longPress && !b){
		longPress = false;
	}
	else if(!longPress && b){
		pressed = true;
		startTime = RTOS::run_time() + 1500000;
	}
}
