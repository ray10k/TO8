//****************************************************************************
//
// 
//
//****************************************************************************


#include "DisplayController.h"

DisplayController :: DisplayController( int prio, LCD &lcd ) : 
task( prio, "DisplayController" ),
buffer( this, "LCDChannel" ),
theLcd ( lcd ) {   
    
    state = WAITING;   
}

void DisplayController::suspend( void ) {
    theLcd.disable();
    RTOS::task::suspend();
}

void DisplayController::resume( void ) {
    theLcd.enable();
    RTOS::task::resume();
}

void DisplayController::main( void ) {
    for(;;) {     
        switch(state) {
            case WAITING:
                di = buffer.read();
                index = 0;
                state = FLUSHING;
                break;
            case FLUSHING:
                if(index < di.len) {
                    theLcd.gotoRowCol( index, di.row );
                    theLcd.setChar( di.buf[index++] );
                }
                else {
                    state = WAITING;
                }
                break;
        }
        release();
    }              
}

void DisplayController::displayData( const char * data, int row ) {
    int i = 0;
    DisplayInfo di;
    di.row = row;
    
    for( i = 0; data[i] != '\0'; i++ ) {
        di.buf[i] = data[i];
    }
    
    di.len = i;
    buffer.write(di);
}

void DisplayController::clearDisplay(void) {
    buffer.clear();
    theLcd.clear();
}
