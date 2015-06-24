#configure port			com4
#configure board        hu_arm_v4
#configure clcd_lines   2
#configure memory       rom
#configure baudrate     38400

#include "display_info.h"
#include "display_controller.h"
#include "pRTOS.h"
#include "common.h"

int main( void ){
	std::cout << "starting..." << std::endl;
   RTOS::run();
   return 0;
}

DisplayController cont;

class Test : public RTOS::task
{
public:
	Test():
		stepper(this,3 S,"step clock"),
		RTOS::task(10,"test")
	{}

private:
	RTOS::clock stepper;
	void main()
	{
		std::cout << "test task starting." <<std::endl;
		int state = 0;
		DisplayInfo render;
		forever
		{
			wait();
			switch (state)
			{
			case 0:
				cont.test();
				break;
			case 1:
				render.setChar(0,0,"Hello world!",12);
				cont.display(render);
				break;
			case 2:
				render.setChar(1,1,"Hey!",4);
				cont.display(render);
				break;
			default:
				state = -1;
				render.clear();
				render.setChar(0,1,'a');
				render.setChar(1,1,'b');
				render.setChar(1,0,'c');
				cont.display(render);
				render.clear();
			}

			state++;
		}
	}
};

Test t;
