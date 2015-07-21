#include "DataController.h"

DataController::DataController (void):
	RTOS::task(dataControllerPriority, "DataController"),
	longPressFlag(this,"DataLongPressFlag"),
	pulseTimeChannel(this,"DataPulseChannel"),
	stateChannel(this,"DataStateChannel"),
	sizePool("DataSizePool"),
	timePool("DataTimePool"),
	updateClock(this, 500 MS, "DataUpdateClock"),
	currentState(NONE),
	hour(0),
	minute(0),
	second(0),
	sincePulse(0),
	lastPulse(0)
{
	//I'm sorry about the sheer load of state in this piece of junk.
}

void DataController::main(void)
{
	
	//enter unitialized state, until a state is passed.
	wait(stateChannel);
	currentState = stateChannel.read();
	clockInit();
	currentTrip(sizePool.read());

	while (1==1)
	{
		
	}
}
