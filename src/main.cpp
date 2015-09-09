//**************************************************************************
//
// This file contains its own documentation in Doxygen format.
// After running doxygen: open the rtf file; select all; r-click->update.
//
//! \mainpage Fietscomputer
//!
//! Een fiets computer van team 2 voor project #8
//!
//! \authors        
//!    - Rick van Rijn (rick.vanrijn@student.hu.nl)
//!    - Hendrik Cornelisse (hendrik.cornelisse@student.hu.nl)
//!    - Wouter van den Heuvel (wouter.vandenheuvel@student.hu.nl)
//!
//! \version
//!    V1.0 (2015-08-30)
//!
//! \par Introduction
//!
//! Met de opleiding Technische Informatica maak je kennis met het ontwerpen en modelleren van realtime- en embedded systemen. Voor de themaopdracht in blok 8 van moeten in Software Ideas Modeler modellen gemaakt worden voor een fietscomputer en deze worden gerealiseerd in C++.
//
//***************************************************************************

#configure board        hu_arm_v4
#configure clcd_lines   2
#configure memory       rom
#configure baudrate     38400

#include "pRTOS.h"
#include "TimeController.h"
#include "DisplayController.h" 
#include "SystemController.h"
#include "ModeController.h"
#include "ButtonHandler.h"
#include "Button.h"
#include "LCD.h"     
#include "HallSensor.h"  
#include "Data.h"      
#include "HallController.h"                                                      

//! Fietscomputer
/*!
 * Main klasse, verantwoordelijk voor het aanmaken van alle taken in het RTOS.
 */

int main( void ){
	
    LCD theLCD;
    Data theData;
    
    HallSensor theHallSensor;
    Button theResetButton( mkt_key_d );
    Button theModeButton( mkt_key_sharp ); 
    
    ButtonHandler theButtonHandler( 1 );
    
    theButtonHandler.addButton( theModeButton );
    theButtonHandler.addButton( theResetButton );
    
    DisplayController theDisplayController( 5, theLCD );
    HallController theHallController( 0, theHallSensor, theData );
    
    TimeController theTimeController( 2, theDisplayController );   
    
    ModeController theModeController( 3, theDisplayController, theData, theModeButton, theHallController );
    ConfigController theConfigController( 4, theDisplayController, theTimeController, theModeController, theResetButton, theData );            
    
    RTOS::run();
    return 0;
}
