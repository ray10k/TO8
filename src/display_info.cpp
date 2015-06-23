#include "display_info.h"
#include "common.h"

DisplayInfo::DisplayInfo (void) : reading(0)
{
	this -> content = new unsigned char [display::height*display::width];
}

void DisplayInfo::setChar(int x, int y, unsigned char c)
{
	this -> content [x+(display::width*y)] = c;
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
	this -> reading = 0;
}

unsigned char DisplayInfo::nextChar(void)
{
	if (this -> reading > (display::width*display::height))
	{
		this -> reset();
	}
	char retval = this -> content[this -> reading];
	this -> reading++;
	return retval;
}
