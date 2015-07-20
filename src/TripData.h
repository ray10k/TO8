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

private:
	//distance in cm (derived from wheel circumference)
	unsigned long long int distance;
	unsigned long long int time;
	int circumference;
};

#endif
