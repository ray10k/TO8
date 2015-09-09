/* ========================================================================== */
/*                                                                            */
/*   Button.cpp                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "pRTOS.h"
#include "Button.h"


Button::Button (mkt_key aKey) : key (aKey){}

void Button :: setListener (ButtonListener * lst){
	theLst = lst;
	last = mkt_kbd_is_pressed(key);
	downSeen = 0;
	upSeen = 0;
    
}

void Button :: updateState (void){
	bool b = mkt_kbd_is_pressed (key);
	
	if (b) {
        upSeen = 0;
		if (++downSeen == 2){
			last = true;
			theLst->buttonDown (this);
		}
	}
	else if (last){	
		downSeen = 0;
		if (++upSeen == 2 ) {
       		theLst->buttonUp (this);
       		last = false;
       	}
        
    } 
}


