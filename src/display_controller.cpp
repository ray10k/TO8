#include "display_controller.h"
#include "Armboard.h"
#include "pRTOS.h"
#include "common.h"

void DisplayController::display(DisplayInfo & content)
{
	LCDChannel.write(content);
}

void DisplayController::write()
{
	armboard::LCD::clear();
	int y;

	for (y = 0; y < display::height; y++)
	{
		writeString(buffer[y],display::width);
		armboard::LCD::goto_xy(0,y+1);// char_write('/n') werkt niet.
	}
}

void DisplayController::main(void)
{
	forever
	{
		RTOS::event e = wait();
		if (e == standbyFlag)
		{
			armboard::LCD::command(0x08); //turn off LCD entirely.
			running = false;
		}
		else if (e == testFlag)
		{
			if (!running)
			{
				armboard::LCD::command(0x0C);
				running = true;
			}
			LCDTest();
		}
		else if (e == LCDChannel)
		{
			if (!running)
			{
				armboard::LCD::command(0x0C); //LCD on, no blinking cursor.
				running = true;
			}
			DisplayInfo toPrint = LCDChannel.read();
			int x,y;
			int index = 0;

			// Since writing to the display involves several waits,
			// the data to be written is copied to a buffer first
			// so the source of the data can't accidentally overwrite
			// it during writing to the display.
			/*
			for (y = 0; y < display::height; y++)
			{
				for (x = 0; x < display::width; x++)
				{
					buffer[index] = toPrint.nextChar();
					index++;
				}
				buffer[index] = '\n';
				index++;
			}
			*/
			for (y = 0; y < display::height; y++)
			{
				toPrint.copyBuffer(y,buffer[y]);
			}

			write();
		}
	}
}

void DisplayController::test(void)
{
	this -> testFlag.set();
}

void DisplayController::writeString(const char * string, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		armboard::LCD::char_write_raw(*(string+i));
	}
}

void DisplayController::LCDTest()
{
	if (!tested)
	{
		tested = true;
		armboard::LCD::command(0x40);
		writeString("5*5*5*5*",8);
		writeString("*5*5*5*5",8);
		armboard::LCD::command(0x80);
	}
	armboard::LCD::clear();
	int x,y;
	for (y = 0; y < display::height; y++)
	{
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(0);
		}
		armboard::LCD::char_write('\n');
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
		armboard::LCD::char_write('\n');
		for (x = 0; x < display::width; x++)
		{
			armboard::LCD::char_write_raw(1);
		}

	}
}