#ifndef __SensorListener
#define __SensorListener

//****************************
//
// class HallSensorListener
//
//! interface class to allow listening for events from the Hall Sensor.
//
//****************************

class HallSensorListener{
public:
	//! Called whenever the Hall Sensor is tripped. The argument is the number
	//! of microseconds since the last pulse, measured between rising edges.
	virtual void HallSensorPulse (unsigned long long int time) = 0;
};

#endif
