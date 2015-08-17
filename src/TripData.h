#ifndef __TRIPDATA_H
#define __TRIPDATA_H

//****************************
//
// class TripData
//
//! a container for information regarding the current
//! trip, responsible for keeping track of distance 
//! traveled and total trip time.
//
//****************************

class TripData
{
public:
	//! Construct the tripdata, with the wheel's circumference.
	TripData(int circumference);
	//! returns the number of hectometres traveled during this trip.
	unsigned int getHectometres(void);
	//! returns the number of hectometres traveled total, as kept by the 
	//! odometer.
	unsigned int getTotalHectometres(void);
	//! Returns the number of seconds since the start of the trip.
	unsigned int getSeconds(void);
	//! Returns the average speed for the current trip, in hectometres per hour.
	unsigned int getAverageSpeed(void);
	//! Returns true if the trip has been going on for too long or too far.
	bool isErrorred(void);
	//! Takes pulse time, returns associated speed in hectometres/hour.
	unsigned int tick(unsigned long long int turnTime);
	//! Resets trip data, but keeps total distance.
	void wipe(void);
	//! Resets all data other than wheel circumference.
	void reset(void);

private:
	//distance in cm (derived from wheel circumference)
	unsigned long long int distance;
	//distance for ODO display state.
	unsigned long long int totalDistance;
	//trip time in uS (derived from pulse periods)
	unsigned long long int time;
	//wheel circumference.
	unsigned int circumference;
};

#endif
