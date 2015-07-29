#ifndef __Button
#define __Button

#include "ButtonListener.h"
#include "pRTOS.h"

class ButtonListener;

class Button{
public:
	Button(mkt_key aKey);
	void updateState(void);
	void setListener(ButtonListener *lst);
	
private:
	const mkt_key key;
	bool pressed;
	ButtonListener *theLst;
	unsigned long long int startTime;
};
#endif
