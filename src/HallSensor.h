#ifndef __HallSensor
#define __HallSensor

#include "HallSensorListener.h"
#include "pRTOS.h"

const int HallPin 7;

//*****************************
//
// class HallSensor
//
//! 
//*****************************

class HallSensor {
public:

	HallSensor (void);
	void updateState (void);
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
};

#endif
