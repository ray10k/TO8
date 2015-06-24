#ifndef __DISPLAY_INFO_H
#define __DISPLAY_INFO_H 

#include "common.h"

//**********
//
// class DisplayInfo
//
//! A container to pass data to the display.
//
//! Acts as a means of transporting the data to be displayed
//! to the display.
//
//**********

class DisplayInfo
{
public:
	//! Default constructor.
	DisplayInfo(void);
	//! Writes the given character to the given position.
	//
	//! Characters that would be placed out of bounds are ignored
	//! without further warning.
	void setChar(unsigned int x, unsigned int y, char c);
	//! Writes the string to the given position.
	void setChar(unsigned int x, unsigned int y, const char * c, int length);
	//! Returns the reading position to the top-left of the display.
	void reset(void);
	//! Reads out the next character.
	//
	//! Returns the character at the current position, and automatically
	//! changes lines at the end of each line.
	char nextChar(void);
	//! Empties the buffer
	void clear(void);
	//! Copies the contents of the line to the given array.
	//
	//! Does not confirm that the buffer is large enough to contain the
	//! line, and may cause overrun.
	void copyBuffer(unsigned int line, char * buffer);
private:
	char content[display::characters];
	unsigned int reading;
};

#endif
