#ifndef __ButtonListener
#define __ButtonListener

//#include "Button.h"

class Button;

class ButtonListener{
public:
	virtual void buttonShortPress(Button *b) = 0;
	virtual void buttonLongPress(Button *b) = 0;
};
#endif