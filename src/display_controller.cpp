#include "display_controller.h"
#include "Armboard.h"
#include "pRTOS.h"
#include "common.h"

void LCDTest(void);

void DisplayController::display(DisplayInfo & content)
{
	LCDChannel.write(content);
}

void DisplayController::write()
{
	Armboard::LCD::command(0x02); //cursor to home position.
	wait(2 MS);//the above command can take a while to complete...
	int index;

	for (index = 0; index < this -> buffLen; index ++)
	{
		Armboard::LCD::char_write(this -> buffer[index]);
	}

}

void DisplayController::main(void)
{
	forever
	{
		RTOS::event e = wait();
		if (e == standbyFlag)
		{
			Armboard::LCD::command(0x08); //turn off LCD entirely.
			running = false;
		}
		else if (e == LCDChannel)
		{
			if (!running)
			{
				Armboard::LCD::command(0x0C); //LCD on, no blinking cursor.
				running = true;
			}
			DisplayInfo toPrint = LCDChannel.read();
			toPrint.reset();

			int x,y;
			int index = 0;

			// Since writing to the display involves several waits,
			// the data to be written is copied to a buffer first
			// so the source of the data can't accidentally overwrite
			// it during writing to the display.
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

			write();
		}
	}
}

void DisplayController::test(void)
{
	LCDTest();
}

void writeString(const unsigned char * string, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		Armboard::LCD::char_write(*(string+i));
	}
}

void LCDTest()
{
	Armboard::LCD::command(0x40);
	writeString("5*UJuj5*",8);
	writeString("*UJuj5*5",8);
	
	Armboard::LCD::command(0x80);
	Armboard::LCD::clear();
	int x,y;
	for (y = 0; y < display::height; y++)
	{
		for (x = 0; x < display::width; x++)
		{
			Armboard::LCD::char_write_raw(0);
		}
		Armboard::LCD::char_write('\n');
	}
	wait(2 S);
	for (y = 0; y < display::height; y++)
	{
		for (x = 0; x < display::width; x++)
		{
			Armboard::LCD::char_write_raw(1);
		}
		Armboard::LCD::char_write('\n');
	}
	
}