#include "TimeController.h"

TimeController :: TimeController( int prio, DisplayController &dc ) :
task( prio, "TimeController" ),
interval( this, 1 S, "tClock" ),
theDisplayController(dc) {
    
    currentTime = 0;
    stringTime[0] = 'C';
    stringTime[1] = 'L';
    stringTime[2] = 'K';
    stringTime[3] = ' ';
    stringTime[4] = ':';
    stringTime[5] = ' ';
    
    suspend();
}

void TimeController :: main( void ) {
	for(;;) {
		RTOS::event e = wait();
		if(e == interval) {              
			stringTime[6] = (currentTime/36000) % 10 + '0'; // 10h
			stringTime[7] = (currentTime/3600) % 10 + '0'; // h
			stringTime[8] = ':';
			stringTime[9] = (currentTime/600) % 6 + '0'; //10m           
			stringTime[10] = (currentTime/60) % 10 + '0'; //m
			stringTime[11] = ':';
			stringTime[12] = (currentTime/10) % 6 + '0'; //10s
			stringTime[13] = (currentTime/1) % 10 + '0'; //s
			stringTime[14] = '\0';
            
			theDisplayController.displayData( stringTime, 1 );
			currentTime++;
		}
	}
}

void TimeController :: resetTime( void ) {
    currentTime = 0;
}

