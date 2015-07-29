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
	speed(0),
	hour(0),
	minute(0),
	second(0),
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
	currentTrip = new TripData(sizePool.read());

	while (1==1)
	{
		//in running state.
		RTOS::event what = wait();

		if (what == stateChannel)
		{
			currentState = stateChannel.read();
		}

		if (what == updateClock)
		{
			unsigned long long int now = RTOS::run_time();
			clockUpdate(now);
			if (now > lastPulse + STANDBY_TIMEOUT)
			{
				//enter stand-by state.
				
				bool resume = false;
				while (!resume)
				{
					RTOS::event e = wait(PulseTimeChannel + stateChannel);

					if (e == pulseTimeChannel)
					{
						pulseTimeChannel.read();
					}

					if (e == stateChannel)
					{
						resume = true;
						currentState = stateChannel.read();
					}
				}
			}
			else if (now > lastPulse + STANDSTILL_TIMEOUT)
			{
				speed = 0;
			}


		}

		if (what == pulseTimeChannel)
		{

		}

		if (what == longPressFlag)
		{

		}
	}
}
