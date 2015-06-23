#ifndef __DISPLAY_INFO_H
#define __DISPLAY_INFO_H 

class DisplayInfo
{
public:
	DisplayInfo(void);
	void setChar(int x, int y, char c);
	void setChar(int x, int y, const char * c, int length);
	void reset(void);
	char nextChar(void);
	void nextLine(void);
private:
	char[][] content;
	int[] reading;
};

#endif
