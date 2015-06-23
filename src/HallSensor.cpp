#include "HallSensor.h"

HallSensor :: HallSensor (void):
	oldPulseTime(0)
{}

void HallSensor :: updateState(void){
	bool b = mkt_pin_read(HALL_PIN);
	
	if(b){
		currentPulseTime = run_time() - oldPulseTime;
		oldPulseTime = run_time();
	}
}

void setListener(HallSensorListener *lst){
	theLst = lst;
}

SensorListener *getListener(void){
	return theLst;
}