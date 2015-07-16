#ifndef __HallSensor
#define __HallSensor

#include "HallSensorListener.h"
#include "pRTOS.h"

#define HALL_PIN 7

class HallSensor{
public:
	HallSensor (void);
	void updateState (void);
	void setListener(HallSensorListener *lst);
	HallSensorListener *getListener (void);
	
private:
	HallSensorListener *theLst
	unsigned long long int oldPulseTime, currentPulseTime;
};