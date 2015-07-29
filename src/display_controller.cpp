#include "display_controller.h"
#include "Armboard.h"
#include "pRTOS.h"
#include "common.h"

void DisplayController::display(DisplayInfo & content)
{
	LCDChannel.write(content);
}

void DisplayController::test(void)
{
	this -> testFlag.set();
}

void DisplayController::standby()
{
	this -> standbyFlag.set();
}


void DisplayController::main(void)
{
	forever
	{
		RTOS::event e = wait();
		if (e == standbyFlag)
		{
			setDisplay(false);
		}
		else if (e == testFlag)
		{
			setDisplay(true);
			LCDTest();
		}
		else if (e == LCDChannel)
		{
			setDisplay(true);

			DisplayInfo toPrint = LCDChannel.read();
			int y;

			// Since writing to the display involves several waits,
			// the data to be written is copied to a buffer first
			// so the source of the data can't accidentally overwrite
			// it during writing to the display.

			for (y = 0; y < display::height; y++)
			{
				toPrint.copyBuffer(y,buffer[y]);
				release();
			}

			write();
		}
	}
}

void DisplayController::write()
{
	armboard::LCD::clear();
	int y;

	for (y = 0; y < display::height; y++)
	{
		armboard::LCD::goto_xy(0,y);
		writeString(buffer[y],display::width);
		release();
	}
}

void DisplayController::LCDTest()
{
	if (!tested) //First time only: write
	{//custom character data for checkerboard pattern.
		tested = true;
		armboard::LCD::command(0x40); //write to custom char data
		writeString(TEST0,8);
		writeString(TEST1,8);
		armboard::LCD::command(0x80); //write to display data
	}

	armboard::LCD::clear();
	int x,y;
	for (y = 0; y < display::height; y++)
	{
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(0);
		}
		armboard::LCD::goto_xy(0,1);
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(0);
		}
	}
	sleep (1 S);
	armboard::LCD::clear();
	for (y = 0; y < display::height; y++)
	{
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(1);
		}
		armboard::LCD::goto_xy(0,1);
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(1);
		}

	}
}

void DisplayController::setDisplay(const bool state)
{
	if (!running && state)
	{
		running = true;
		armboard::LCD::command(ON);
	}
	else if (running && !state)
	{
		running = false;
		armboard::LCD::command(OFF);
	}
}

void DisplayController::writeString(const char * string, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		armboard::LCD::char_write_raw(*(string+i));
	}
}
