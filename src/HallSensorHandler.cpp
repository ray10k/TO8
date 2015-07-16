#include "HallSensorHandler.h"

HallSensorHandler :: HallSensorHandler(HallSensor * hallSensor):
	RTOS::task(HallPriority, "Hall Sensor Handler"),
	sensor(hallSensor),
	ticker(this,1 MS,"Hall clock");
{}

void HallSensorHandler :: main ()
{
	while (1==1)
	{
		wait(); //resume once the clock ticks.
		this -> sensor -> updateState();
	}
}
