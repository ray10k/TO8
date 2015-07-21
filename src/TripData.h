#ifndef __TRIPDATA_H
#define __TRIPDATA_H

class TripData
{
public:
	TripData(int circumference);
	int getKilometres(void);
	int getSeconds(void);
	int getAverageSpeed(void);
	void tick(unsigned long long int time);
	void wipe(void);
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
