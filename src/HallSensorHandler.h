#ifndef __HalController
#define __HalController

#include "pRTOS.h"
#indluce "HallSensor.h"
#include "HallSensorListener.h"

class HallSensorHandler : public RTOS::task, public SensorListener{
public:
	HallController (int prioriteit, HallSensor &hallSensor);
	void HallSensorPulse (HallSensor *s);
	
private:
	void main (void);
	
	HallSensor *theHallSensor;
	RTOS::clock pulseClock;
	RTOS::timer pulseTimer;
	
	short timeBetweenPulse;
	bool setPulse;
};