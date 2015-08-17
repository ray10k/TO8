#include "TripData.h"

#ifndef S
#define S * (1000 * 1000)
#endif
#define M * (60 S)
#define H * (60 * 60 S)

TripData::TripData(int circumference):
	circumference(circumference)
	{}

unsigned int TripData::getHectometres(void)
{
	return (unsigned int) (distance / (100*100)); //need 100s of metres, distance is in cm.
}

unsigned int TripData::getTotalHectometres(void)
{
	return (unsigned int) (totalDistance / (100*100));
}

unsigned int TripData::getSeconds(void)
{
	return (unsigned int) (time / (1 S)); //time is in US.
	//deliberately down-casting because an ull int does not fit in 32 bits.
	//additionally, the smallest error value (359 999) fits well in an 32-
	//bit int.
}

unsigned int TripData::getAverageSpeed(void)
{
	return getDecametres() / (getSeconds() / (1 H));
}

bool TripData::isErrorred(void)
{
	return (time >= ((59 S) + (59 M) + (99 H))
		|| distance >= 99999
		|| totalDistance >= 999999)
}

unsigned int TripData::tick (unsigned long long int turnTime)
{
	distance += circumference;
	totalDistance += circumference;
	time += turnTime;

	unsigned int rotations = (1 H) / (unsigned int) turnTime; //number of times the wheel would
	//turn, with the given time per turn.
	unsigned int spd = rotations * circumference; //cm / hour
	spd /= (100 * 10); //divide by cm in 1 dam to get dam / hour.
	return spd;
}

void TripData::wipe(void)
{
	distance = time = 0;
}

void TripData::reset(void)
{
	wipe();
	totalDistance = 0;
}
