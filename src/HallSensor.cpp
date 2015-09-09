/* ========================================================================== */
/*                                                                            */
/*   HallSensor.cpp                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "HallSensor.h"

HallSensor :: HallSensor (void) {
    state = false;
}

void HallSensor::updateState(void){
    bool b = mkt_pin_read(HALL_PIN);
	
    if(b && !state) { 
        theLst->sensorEvent(this);
        state = true;
    } 
    else if(!b && state) {
        
        state = false;
    }
}
