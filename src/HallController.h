/* ========================================================================== */
/*                                                                            */
/*   HalController.h                                                            */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#ifndef __HalController
#define __HalController

#include "pRTOS.h"
#include "HallSensor.h" 
#include "SensorListener.h"
#include "Data.h"
#include "ModeController.h"

class ModeController;

//! Een controller klasse voor het aansturen van de HallSensor
/*!
 * Deze klasse beheert de hallSensor, en stuurt berichten naar de Mode Controller om te zeggen of
 * er nieuwe data aanwezig is en of de hall sensor inactief is
 * de HallController implementeerd zowel RTOS::task en SensorListener
 */
class HallController : public RTOS::task, public SensorListener {
public:   
    //!Constructor
    /*!
     \param prio een int voor het zetten van de pRTOS prioriteit
     \param hall een HallSensor referentie, zodat deze de HallSensor kan aansturen
     \param data een Data referentie zodat de HallController de Data klasse kan updaten met een nieuwe pulse
     */
	HallController( int prio, HallSensor &hall, Data &data );
    //! vanuit SensorListener
    /*!
     \param s de HallSensor die het event genereerd
     */
	void sensorEvent (HallSensor * s);
    //! functie om de ModeController te setten.
    /*!
     * om de ModeController bekend te maken binnen deze klasse, zodat deze flags binnen de ModeController kan setten.
     \param mc Een ModeController pointer
     */
	void setModeController(ModeController *mc);
	
	//!Overrides RTOS::resume
    /*!
     * dit wordt overschreven om zo de hcTimer op 1 seconden te zetten en timerEvents op 0 te zetten
     * zodra de modeController.resume() wordt aangeroepen. 
     */
	virtual void resume();
private:    
    //! main funtie vanuit het RTOS
    /*!
     * de HallController gaat wachten op zijn waitableset.
     * Als de HallController#hcClock wordt bekeken voort hij een HallSensor::updateState() uit op HallController#theHallSensor. 
     * zodat deze gaat kijken of er nieuwe pulse gedetecteerd is.
     * 
     * als dit wel zo is voert theHallSensor HallController::sensorEvent() uit op zijn SensorListener, wat de HallController ook is, 
     * waarna hij een nieuwe pulse toevoegd op de Data klase doormiddel van Data::addPulse().
     *
     * Als de HallController::hcTimer wordt gedetecteerd gaat hij eerst kijken of er niet al enige tijd inactiviteit is geweest. 
     * en zet de ModeController zijn mtInactive flag zodat deze dit ook weet, daarna suspend hij zich zelf door middel van HallController::suspend.
     * Als dit niet waar is, zet hij de ModeController zijn ModeController::mtDataFlag zodat deze weet dat hij moet gaan kijken 
     * naar nieuwe data in de Data klass, ook telt hij 1 bij HallController#timerEvents op, en zet hij de HallController#hcTimer op 1 Seconden.
     */
	void main (void);
    
	ModeController *theModeController;  //!< referentie naar de ModeController
    HallSensor *theHallSensor;          //!< referntie naar de HallSensor
    Data *theData;                      //!< referentie naar de Data
    
    RTOS::clock hcClock; //!< een Klok voor het periodiek aanroepen van updateState op de HallSensor
    RTOS::timer hcTimer; //!< een timer voor het updaten van de gewachte tijd
	
    int timerEvents;    //!< een int variabele voor het opslaan van de hoeveel tijd events
    int inactiveTime;   //!< een int variable voor het opslaan van de lengte van de tijd dat er geen HallSensor events gedetecteerd zijn.
};


#endif
