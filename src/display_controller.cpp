#include "display_controller.h"
#include "Armboard.h"

const int DISPLAY_HEIGHT = 2;
const int DISPLAY_WIDTH = 16;

void DisplayController::write(DisplayInfo & info)
{
	info.reset();
	Armboard::LCD::clear();
	int x, y;
	x = y = 0;
	for (;y<DISPLAY_HEIGHT;y++)
	{
		for(x = 0; x < DISPLAY_WIDTH;x++)
		{
			
		}
	}
}