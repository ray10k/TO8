#ifndef __DISPLAY_INFO_H
#define __DISPLAY_INFO_H 

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
	//! Note that this function does not check if it's going out
	//! of bounds, and should be used with caution.
	void setChar(int x, int y, char c);
	//! Writes the string to the given position.
	//
	//! Note that this function does not check if it's going out
	//! of bounds, and should be used with caution.
	void setChar(int x, int y, const char * c, int length);
	//! Returns the reading position to the top-left of the display.
	void reset(void);
	//! Reads out the next character.
	//
	//! Returns the character at the current position, and automatically
	//! changes lines at the end of each line.
	unsigned char nextChar(void);
private:
	unsigned char[] content;
	unsigned int reading;
};

#endif
