#include "display_info.h"

const int DISPLAY_WIDTH = 16;
const int DISPLAY_HEIGHT = 2;
const int READ_X = 0;
const int READ_Y = 1;

DisplayInfo::DisplayInfo (void)
{
	this -> content = new char [DISPLAY_HEIGHT][DISPLAY_WIDTH];
	this -> reading = new int [2];
}

void DisplayInfo::setChar(int x, int y, char c)
{
	this -> content [y][x] = c;
}

void DisplayInfo::setChar(int x, int y, const char * c, int length)
{
	int i = 0;
	for (;i < length ; i++)
	{
		this -> setChar(x+i,y,*(c+i));
	}
}

void DisplayInfo::reset(void)
{
	this -> reading[READ_X] =0;
	this -> reading[READ_Y] =0;
}

char DisplayInfo::nextChar(void)
{
	if (this -> reading[READ_X] < DISPLAY_WIDTH)
	{
		char retval = this -> content[this -> reading[READ_Y]] [this -> reading[READ_X]];
		this -> reading[READ_X] = this -> reading[READ_X] + 1;
		return retval;
	}
	else
	{
		return 0xff;
	}
}

void DisplayInfo::nextLine(void)
{
	if (this -> reading[READ_Y] < DISPLAY_HEIGHT)
	{
		this -> reading[READ_Y] = this -> reading[READ_Y] + 1;
	}
	else
	{
		this -> reading[READ_Y] = 0;
	}
	this -> reading[READ_X] = 0;
}
