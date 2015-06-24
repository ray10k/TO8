#include "display_info.h"
#include "common.h"


DisplayInfo::DisplayInfo (void) : reading(0)
{
	clear();
}

void DisplayInfo::setChar(unsigned int x, unsigned int y, char c)
{
	unsigned int offset = x+(display::width*y);
	if ( offset < display::characters)
	{
		this -> content [offset] = c;
	}
}

void DisplayInfo::setChar(unsigned int x, unsigned int y, const char * c, int length)
{
	int i;
	for (i = 0;i < length ; i++)
	{
		this -> setChar(x+i,y,*(c+i));
	}
}

char DisplayInfo::nextChar(void)
{
	if (this -> reading > (display::width*display::height))
	{
		this -> reset();
	}
	char retval = this -> content[this -> reading];
	this -> reading++;
	return retval;
}

void DisplayInfo::clear(void)
{

	int i;
	for (i = 0; i < display::characters; i++)
	{
		content[i] = 0x10;
	}
}

void DisplayInfo::copyBuffer(unsigned int line, char * buffer)
{
	int i;
	for (i = 0; i < display::width; i++)
	{
		*(buffer+i) = content[i+(display::width*line)];
	}
}


