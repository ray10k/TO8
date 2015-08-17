#ifndef __HalController
#define __HalController

#include "pRTOS.h"
#include "HallSensor.h"

//****************************
//
// class HallSensorHandler
//
//! Handler for the HallSensor boundry object,
//! prompts said object to perform a status update
//! periodically.
//
//****************************

const int HallPriority = 545; //chosen pretty much at random for testing purposes. 

class HallSensorHandler : public RTOS::task{
public:
	//! Create the handler task, and make it pulse the given HallSensor.
	HallSensorHandler (HallSensor &hallSensor);
	
private:
	void main (void);

	HallSensor * sensor;
	
	RTOS::Clock ticker;
} HSHandler(HALL);
#endif
