#ifndef __SensorListener
#define __SensorListener

class HallSensor;

class HallSensorListener{
public:
	virtual void HallSensorPulse (HallSensor *s) = 0;
	virtual int HallSensorTime (HallSensor *s) = 0;
};