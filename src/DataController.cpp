#include "DataController.h"
#include <stdio.h>

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
	fraction(0),
	lastPulse(0),
	lastTick(0)
{
	//I'm sorry about the sheer load of state in this piece of junk.
}

void DataController::buttonShortPress(Button * b)
{
	//ignore short button presses.
	return;
}

void DataController::buttonLongPress(Button * b)
{
	longPressFlag.set();
}

void DataController::HallSensorPulse(unsigned long long int time)
{
	pulseTimeChannel.write(time);
}

void DataController::init(int time,int size)
{
	sizePool.write(size);
	timePool.write(time);
}

void DataController::statusChange (DisplayState newState)
{
	stateChannel.write(newState);
}

void DataController::setTime(int time)
{
	timePool.write(time);
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

			updateDisplay();
		}

		if (what == pulseTimeChannel)
		{
			lastPulse = RTOS::run_time();
			//since we cannot check if the channel has multiple items waiting (why!?)
			//we have to assume there was only one.
			unsigned long long int pulse = pulseTimeChannel.read();

			speed = (currentTrip -> tick (pulse));
		}

		if (what == longPressFlag)
		{
			switch (currentState)
			{
				case SPD:
					currentTrip -> wipe();
					break;
				case ODO:
					currentTrip -> reset();
					break;
				case TM:
					currentState = NONE;
					wait(stateChannel);

					currentState = stateChannel.read();
					clockInit();
					break;
			}
		}
	}
}

void DataController::clockInit(void)
{
	second = 0;
	int temp = timePool.read();
	hour = temp / 60;
	minute = temp % 60;
}

void DataController::clockUpdate(unsigned long long int now)
{
	//sledgehammer approach. Assume the device can be in stand-by for weeks on end.
	//clock updates do not happen while in stand-by, but the system clock keeps
	//running.
	fraction += now - lastTick;
	while (fraction >= 1 S)
	{
		second ++;
		fraction -= 1 S;
	}

	while (second >= 60)
	{
		minute ++;
		second -= 60;
	}

	while (minute >= 60)
	{
		hour ++;
		minute -= 60;
	}

	while (hour >= 24)
	{
		hour -= 24;
	}

}

void DataController::updateDisplay(void)
{
	if (currentState == NONE)
	{
		return; //Screen is busy, do not disturb.
	}

	DisplayInfo inf();
	char scratch[6]; //scratch space for changing ints to chars.

	//display the clock.
	sprintf (scratch,"%2u",hour);
	inf.setChar(0,0,scratch,2);
	inf.setChar(2,0,':');

	sprintf (scratch,"%02u",minute);
	inf.setChar(3,0,scratch,2);
	inf.setChar(5,0,':');

	sprintf (scratch,"%02u",second);
	inf.setChar(6,0,scratch,2);

	switch (currentState)
	{
		case SPD:

			break;
		case AVG:

			break;
		case MAX:

			break;
		case DST:

			break;
		case ODO:

			break;
		case TM:

			break;
	}


}
