

#include "ModeController.h"

ModeController ::ModeController( int prio,  DisplayController &displayController, 
                                Data &data, Button &button, HallController &hallController ) : 
task( prio, "ModeController" ),
mHoldTimer (this, "mHoldTimer"),
btnFlag(this, "mBtnFlag"),
mtDataFlag(this, "mtDataFlag"),
mtInactiveFlag(this, "mtInactiveFlag"),
theHallController(hallController), 
theDisplayController(displayController),
theData(data), theModeButton(button)
{
   	theModeButton.setListener(this);
   	theHallController.setModeController(this); 
   	output[15] = '\0';
   	suspend();
}

void ModeController :: resume( void ) {
    theDisplayController.clearDisplay();
    theDisplayController.suspend();
    modus = IDLE;
    RTOS::task::resume();
}

void ModeController :: suspend( void ) {
    theHallController.suspend();
    RTOS::task::suspend();
}

void ModeController :: main( void ) {
  	bool holded = false;
    for(;;) {
        RTOS::event e = wait();   
        if (e == btnFlag){
            if(holded) {
          		holded = false;
            }
            else { //enkele klik
          		mHoldTimer.cancel();
                switch (modus){
                    case IDLE:
                        theData.setPrevTime(RTOS::run_time());
                        theHallController.resume();
                        theDisplayController.resume();
                        modus = SPD;
                        break;
                    case ERR:
                        theData.clearCurrent();
                        modus = SPD;
                        break;
                    case CAL: // IDLE overslaan
                        modus = SPD;
                        break;
                    default:
                        modus++;
                        break;
                }
            }
        }
        else if (e == mHoldTimer) { //ingedrukt gehouden
            holded = true;
            switch (modus) {
                case SPD:
                case AVG:
                case MAX:
                case DST:
                    theData.clearCurrent();
                    modus = IDLE;
                    break;
                case ODO:
                    theData.clearAll();
                    break;
                case TM:
                    // Configure TM
                    break;
                case CAL:
                    // Configure CAL
                    break;
            }
        }
        else if(e == mtInactiveFlag) {
            modus = IDLE;
        }
        
        if(theData.isError()) {
            modus = ERR;
        }
		
        
			switch( modus ){
				case SPD:
					theDisplayController.displayData( formatSpeed( theData.getSpeed(), output), 0 );
					break;
				case AVG:
					theDisplayController.displayData( formatAverage( theData.getAverage(), output), 0 );
					break;
				case MAX:
					theDisplayController.displayData( formatMax( theData.getMax(), output), 0 );
					break;
				case DST:
					theDisplayController.displayData( formatDistance( theData.getDistance(), output), 0 );
					break;
				case ODO:
					theDisplayController.displayData( formatTotalDistance( theData.getTotalDistance(), output), 0 );
					break;
				case TM:
					theDisplayController.displayData( formatDrivingTime( theData.getDrivingTime(), output), 0 );
					break;
				case CAL:
					theDisplayController.displayData( formatCalories( theData.getCalories(), output), 0 );
					break; 
				case ERR:
					theDisplayController.displayData( formatError( output ), 0 );
					break;
				case IDLE:
        default:
					theHallController.suspend();
					theDisplayController.clearDisplay();
					theDisplayController.suspend();
					break;
					
        }	
    }
}


char* ModeController :: formatSpeed( int speed, char * output ) { 
	output[0] = 'S';
	output[1] = 'P';
	output[2] = 'D';
	output[3] = ' ';
	output[4] = ':'; 
	output[5] = ' ';
	output[6] = (char) 48 + ((speed / 10000) % 10);
	output[7] = (char) 48 + ((speed / 1000) % 10);
	output[8] = '.';
	output[9] = (char) 48 + ((speed / 100) % 10);
	output[10] = 'K';
	output[11] = 'm';
	output[12] = '/';
	output[13] = 'h';   
	output[14] = ' ';    
    return output;
}
char* ModeController :: formatAverage( int average, char * output ) {
    output[0] = 'A';
    output[1] = 'V';
    output[2] = 'G';
    output[3] = ' ';
    output[4] = ':';
    output[5] = ' ';
	output[6] = (char) 48 + ((average / 10000) % 10);
	output[7] = (char) 48 + ((average / 1000) % 10);
	output[8] = '.';
	output[9] = (char) 48 + ((average / 100) % 10);
	output[10] = 'K';
	output[11] = 'm';
	output[12] = '/';
	output[13] = 'h';   
	output[14] = ' ';  
    return output;
}
char* ModeController :: formatMax( int max, char * output ) {
    output[0] = 'M';
    output[1] = 'A';
    output[2] = 'X';
    output[3] = ' ';
    output[4] = ':';
    output[5] = ' ';
	output[6] = (char) 48 + ((max / 10000) % 10);
	output[7] = (char) 48 + ((max / 1000) % 10);
	output[8] = '.';
	output[9] = (char) 48 + ((max / 100) % 10);
	output[10] = 'K';
	output[11] = 'm';
	output[12] = '/';
	output[13] = 'h';   
	output[14] = ' ';
    return output;
}
char* ModeController :: formatDistance( int distance, char * output ) {
    output[0] = 'D';
    output[1] = 'S';
    output[2] = 'T';
    output[3] = ' ';
    output[4] = ':';
    output[5] = ' ';
	output[6] = (char) 48 + ((distance / 10000) % 10);
	output[7] = (char) 48 + ((distance / 1000) % 10);
	output[8] = (char) 48 + (distance / 100 % 10);
	output[9] = '.';
	output[10] = (char) 48 + ((distance / 10) % 10);
	output[11] = (char) 48 + (distance % 10);
	output[12] = 'k';
	output[13] = 'm';  
    output[14] = ' ';
    return output;
}
char* ModeController :: formatTotalDistance( int totalDistance, char * output ) {
	output[0] = 'O';
	output[1] = 'D';
	output[2] = 'O';
	output[3] = ' ';
	output[4] = ':';
	output[5] = ' ';
	output[6] = (char) 48 + ((totalDistance / 100000) % 10);
	output[7] = (char) 48 + ((totalDistance / 10000) % 10);
	output[8] = (char) 48 + ((totalDistance / 1000) % 10);
	output[9] = (char) 48 + (totalDistance / 100 % 10);
	output[10] = '.';
	output[11] = (char) 48 + ((totalDistance / 10) % 10);
	output[12] = (char) 48 + (totalDistance % 10);
	output[13] = 'k';
	output[14] = 'm'; 
	return output;
}
char* ModeController :: formatDrivingTime( int drivingTime, char * output ) {
	output[0] = 'T';
	output[1] = 'M';
	output[2] = ' ';
	output[3] = ' ';
	output[4] = ':'; 
	output[5] = ' ';
	output[6] = (drivingTime/36000) % 10 + '0'; // 10h
	output[7] = (drivingTime/3600) % 10 + '0'; // h
	output[8] = ':';
	output[9] = (drivingTime/600) % 6 + '0'; //10m
	output[10] = (drivingTime/60) % 10 + '0'; //m
	output[11] = ':';
	output[12] = (drivingTime/10) % 6 + '0'; //10s  
	output[13] = (drivingTime/1) % 10 + '0'; //s 
    output[14] = ' ';
	return output;
}
char* ModeController :: formatCalories( int calories, char * output ) {
	output[0] = 'C';
	output[1] = 'A';
	output[2] = 'L';
	output[3] = ' ';
	output[4] = ':';
	output[5] = ' ';
	output[6] = (char) 48 + ((calories / 1000) % 10);
	output[7] = (char) 48 + ((calories / 100) % 10);
	output[8] = (char) 48 + ((calories / 10) % 10);
	output[9] = (char) 48 + (calories % 10);
	output[10] = ' ';
	output[11] = 'K';
	output[12] = 'c';
	output[13] = 'a';
	output[14] = 'l';    
	return output;
}

char* ModeController::formatError ( char * output ) {
	output[0] = 'E';
	output[1] = 'r';
	output[2] = 'r';
	output[3] = 'o';
	output[4] = 'r';
	output[5] = '!';
	output[6] = ' ';
	output[7] = ' ';
	output[8] = ' ';
	output[9] = ' ';
	output[10] = ' ';
	output[11] = ' ';
	output[12] = ' ';   
	output[13] = ' ';
	output[14] = ' ';
	return output;
}
