#include "HallSensor.h"

HallSensor :: HallSensor (void):
	previousPulseTime(0),
	lastReading(false),
	firstListener(NULL)
{}

void HallSensor :: updateState(void)
{
	bool b = mkt_pin_read(HallPin);
	
	//check for rising edge: previous reading was false, current reading is true.
	if(b && !lastReading)
	{
		unsigned long long int now = RTOS::run_time();
		unsigned long long int diff = now - previousPulseTime;
		previousPulseTime = now;

		notifyListeners(diff);
	}

	lastReading = b;
}

void HallSensor :: addListener(HallSensorListener * listener)
{
	if (this -> firstListener != NULL)
	{
		this -> firstListener.attach(listener);
	}
	else
	{
		HallSensor::ListenerListItem * first = new ListenerListItem(listener);
		this -> firstListener = first;
	}
}

void HallSensor :: ListenerListItem :: ListenerListItem(HallSensorListener * listener):
	l(listener),
	n(NULL)
{}

void HallSensor :: ListenerListItem :: notify(unsigned long long int time)
{
	this -> l -> HallSensorPulse(time);//TODO: pass the event along.
	
	if (this -> n != NULL)
	{
		this -> n -> notify(time);
	}
}
