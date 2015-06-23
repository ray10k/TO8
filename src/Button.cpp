#include "Button.h"
#include "Armboard.h"

Button :: Button (mkt_key aKey) : 
	key (aKey)
{
		
}

void Button :: setListener (ButtonListener *lst){
	theLst = lst;
	//pressed = armboard::keypad::is_pressed (key);
	pressed = false;
	cheatTimer = 0;
}

void Button :: updateState (void){
	bool b = armboard::keypad::is_pressed (key);
	
	if(pressed){
		if(!b){
			pressed = false;
			cheatTimer = 0;
			theLst->buttonShortPress (this);
		}
		else if(cheatTimer >= 1500){
			pressed = false;
			cheatTimer = 0;
			theLst->buttonLongPress (this);
		}
		else{
			cheatTimer += 100;
		}
	}
	else if(b){
		pressed = true;
	}
}

//longPressTimer.set(1500 MS);