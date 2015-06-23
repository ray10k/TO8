#include "HallSensorHandler.h"

HallSensorHandler :: HallSensorhandler (int prioriteit, HallSensor &hallSensor):
	RTOS::task (prioriteit, "HallSensorHandler"),
	theHallSensor(&hallSensor),
	pulseClock (this, 1 MS, "pulseClock"),
	pulseTimer (this, "pulseTimer"),
	setPulse(false)
{
	theHallsensor -> setListener(this);
}

void HallSensorHandler :: sensorEvent (HallSensor *s){
	if(!setPulse){
		pulseTimer
	}
}