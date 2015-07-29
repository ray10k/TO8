#ifndef __HalController
#define __HalController

#include "pRTOS.h"
#include "HallSensor.h"

class HallSensorHandler : public RTOS::task{
public:
	HallSensorHandler (HallSensor &hallSensor);
	
private:
	void main (void);

	HallSensor * sensor;
	
	RTOS::Clock ticker;
};
#endif
