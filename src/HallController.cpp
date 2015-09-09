/* ========================================================================== */
/*                                                                            */
/*   HallController.cpp                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#include "HallController.h"


HallController::HallController( int prio, HallSensor &hall, Data &data ) : 
RTOS::task (prio, "HallController"),
theHallSensor(&hall), theData(&data), 
hcClock (this , 2 MS, "hcClock"),
hcTimer (this, "hcTimer") 
{	
	theHallSensor->setListener(this);
	inactiveTime = 10;
	suspend();
}


void HallController::resume() {
    hcTimer.set( 1 S );
    timerEvents = 0;
    RTOS::task::resume();
}

void HallController::sensorEvent (HallSensor * s){
 	theData->addPulse(RTOS::run_time());
    timerEvents = 0;   
}

void HallController::main(){
	for (;;) {
		RTOS::event e = wait();
		if (e == hcClock){
			theHallSensor->updateState();
			release();
		} else if (e == hcTimer){
            // x Seconde inactiviteit		
			if(timerEvents >= inactiveTime) {
                theModeController->mtInactiveFlag.set();
                suspend();
            }
            // Geen pulse voor X aantal seconden?
            else {
                theModeController->mtDataFlag.set(); 	
                
                timerEvents++;
                hcTimer.set( 1 S );
            }
		}
		
	}
}

void HallController::setModeController(ModeController *mc){
	theModeController = mc;
}