#ifndef __TRIPDATA_H
#define __TRIPDATA_H

class TripData
{
public:
	TripData(int circumference);
	int getHectometres(void);
	int getTotalHectometres(void);
	int getSeconds(void);
	int getAverageSpeed(void);
	//takes pulse time, returns associated speed in hectometres/hour.
	unsigned int tick(unsigned long long int time);
	//resets trip data, but keeps total distance.
	void wipe(void);
	//resets all data other than wheel circumference.
	void reset(void);

private:
	//distance in cm (derived from wheel circumference)
	unsigned long long int distance;
	//distance for ODO display state.
	unsigned long long int totalDistance;
	//trip time in uS (derived from pulse periods)
	unsigned long long int time;
	//wheel circumference.
	int circumference;
};

#endif
