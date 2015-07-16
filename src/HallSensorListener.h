#ifndef __SensorListener
#define __SensorListener

#include "HallSensor.h"

const int HallPriority = 545; //chosen at random. Seriously, why do we have to do this ourselves?
//was "RTOS::AVAILABLE_PRIORITY++" too much trouble?!

class HallSensorListener{
public:
	virtual void HallSensorPulse (unsigned long long int time) = 0;
};

#endif