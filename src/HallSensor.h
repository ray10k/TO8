#ifndef __HallSensor
#define __HallSensor

#include "HallSensorListener.h"
#include "pRTOS.h"

const int HallPin 7;

//*****************************
//
// class HallSensor
//
//! Boundary class to read out the Hall sensor.
//
//*****************************

class HallSensor {
public:
	//! Construct a HallSensor.
	HallSensor (void);
	//! Check the state of the physical Hall sensor, and notify
	//! listeners if necessary.
	void updateState (void);
	//! Add a listener to notify of HallSensor events.
	void addListener(HallSensorListener *listener);
	
private:

	void notifyListeners(unsigned long long int time);

	// I'm assuming here that no HSL will ever be removed,
	// and that as such, assigning via new will not lead
	// to memory leaks.
	class ListenerListItem
	{
	public:
		ListenerListItem(HallSensorListener * listener);
		void notify(unsigned long long int time);
		void attach(ListenerListItem * next);
	private:
		HallSensorListener * l;
		ListenerListItem * n;
	};

	ListenerListItem * firstListener;

	unsigned long long int previousPulseTime;
	bool lastReading;
} HALL;

#endif
