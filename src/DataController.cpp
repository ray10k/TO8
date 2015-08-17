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
	errored(false),
	speed(0),
	maxSpeed(0),
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
			if (speed > 999)
			{
				errored = true;
			}

			if (maxSpeed < speed)
			{
				maxSpeed = speed;
			}
		}

		if (what == longPressFlag)
		{
			switch (currentState)
			{
				case SPD:
					currentTrip -> wipe();
					errored = currentTrip -> isErrored();
					maxSpeed = 0;
					break;
				case ODO:
					currentTrip -> reset();
					errored = false;
					maxSpeed = 0;
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

void writeSpeed(unsigned int speed, DisplayInfo & dest)
{
	char scratch[6];

	sprintf(scratch,"%5u",speed);
	dest.setChar(3,0,scratch,4);
	dest.setChar(7,0,'.');
	dest.setChar(8,0,scratch[4]);
	dest.setChar(10,0,"Km/h",4);
}

void writeDistance(unsigned int distance, DisplayInfo & dest)
{
	char scratch[8];

	sprintf(scratch, "%6u",distance);
	dest.setChar(3,0,scratch,4);
	dest.setChar(8,0,'.');
	dest.setChar(9,0,scratch[5],2);
	dest.setChar(12,0,"Km",2);
}

void DataController::updateDisplay(void)
{
	if (currentState == NONE)
	{
		return; //Screen is busy, do not disturb.
	}

	errored = currentTrip -> isErrored();

	DisplayInfo inf();
	char scratch[3]; //scratch space for changing ints to chars.

	//display the clock.
	sprintf (scratch,"%2u",hour);
	inf.setChar(0,1,scratch,2);
	inf.setChar(2,1,':');

	sprintf (scratch,"%02u",minute);
	inf.setChar(3,1,scratch,2);
	inf.setChar(5,1,':');

	sprintf (scratch,"%02u",second);
	inf.setChar(6,1,scratch,2);

	if (errored)
	{
		inf.setChar(4,1,"ERROR",5);
	}
	else
	{
		switch (currentState)
		{
			case SPD:
				inf.setChar(0,0,"SPD",3);

				writeSpeed(speed,inf);
				
				break;
			case AVG:
				inf.setChar(0,0,"AVG",3);
				
				unsigned int avg = this -> currentTrip -> getAverageSpeed();
				writeSpeed(avg,inf);

				break;
			case MAX:
				inf.setChar(0,0,"MAX",3);

				writeSpeed(maxSpeed,inf);

				break;
			case DST:
				inf.setChar(0,0,"DST",3);

				unsigned int dst = this -> currentTrip -> getHectometres();
				writeDistance(dst,inf);

				break;
			case ODO:
				inf.setChar(0,0,"ODO",3);

				unsigned int odo = this -> currentTrip -> getTotalHectometres();
				writeDistance(odo,inf);

				break;
			case TM:
				inf.setChar(0,0,"TM",2);

				int h,m,s,temp;
				temp = this -> currentTrip -> getSeconds();

				h = temp / (60 * 60 S);
				temp -= h * (60 * 60 S);

				m = temp / (60 S);
				temp -= m * (60 S);

				s = temp;

				char clockScratch[9];
				sprintf(clockScratch,"%2u:% 2u:% 2u",h,m,s);
				inf.setChar(2,0,clockScratch,8);
				break;
		}
	}
	Display.display(inf);
}
