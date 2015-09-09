/* ========================================================================== */
/*                                                                            */
/*   ConfigController.cpp                                                     */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#include "ConfigController.h"

ConfigController :: ConfigController( int prio, DisplayController &dc, TimeController &tc, ModeController &mc, Button &rb, Data &data ) : 
task( prio, "SystemController" ),
disChkTimer( this, "disChkTimer" ),
rHoldTimer( this, "rHoldTimer" ),
btnFlag( this, "btnFlag" ),
theDisplayController( dc ),
theTimeController( tc ),
theModeController( mc ),
theResetButton ( rb ),
theData( data )
{
    theResetButton.setListener( this );
    disChkTimer.set( DISPLAY_TEST_SPEED );
    state = CHECKING;
    segment = 0;
    row = 0;
}

void ConfigController :: buttonDown( Button * b ) {
    rHoldTimer.set( 2 S );
}

void ConfigController :: buttonUp( Button * b ) {
    set(btnFlag);
}

void ConfigController :: main( void ) {
    bool holded = false;
    for(;;) {
        RTOS::event e = wait();   
        if(e == disChkTimer){
            switch(state) {
                case CHECKING:
                    char checkString[17];
                    int i;
                    for( i = 0; i < 16; i++ ) {
                        if(i == segment) {
                            checkString[i] = '\xFF';
                        }
                        else {
                            checkString[i] = ' ';
                        }
                    }
                    
                    disChkTimer.set( DISPLAY_TEST_SPEED );
                    theDisplayController.displayData(checkString, row);
                    
                    if(segment >= 16 && row == 0) {
                        segment = 0;
                        row = 1;
                    }
                    else if(segment >= 16 && row == 1) {
                        state = RUNNING;
                    }
                    else {
                        segment++;
                    } 
                    break;
                case RUNNING:
                    theModeController.resume();
                    theTimeController.resume();  
                    break;
            }
        }else if(e == btnFlag){
            if(holded)
                holded = false;
            else{
                rHoldTimer.cancel();
                // Calibiratie toestand 
            }
        }else if(e == rHoldTimer){
            holded = true;
            
            theTimeController.resetTime();
            theData.clearAll();
            
            theModeController.suspend(); 
            theTimeController.suspend();
            
            theDisplayController.resume();
            theDisplayController.clearDisplay();        
            
            state = CHECKING;
            segment = 0;
            row = 0;
            disChkTimer.set( DISPLAY_TEST_SPEED );
        }
    }
}
