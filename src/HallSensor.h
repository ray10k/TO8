/* ========================================================================== */
/*                                                                            */
/*   HallSensor.h                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef __HallSensor
#define __HallSensor
#define HALL_PIN 7

#include "SensorListener.h"
//! boundary klasse voor het aansturen van een HallSensor
class HallSensor {
public:
    //! Constructor
	HallSensor (void);
    //! Functie voor het updaten van de state van de HallSensor
    /*!
     * door middel van deze functie kan je de HallSensor Pollen
     */
	void updateState(void);
    //! Functie voor het zetten van een SensorListener
    /*!
     * deze SensorListener krijgt dan events die door de HallSensor worden gegenereerd.
     \param lst een pointer van de SensorListener waar de HallSensor berichten naar moet sturen
     */
	void setListener(SensorListener * lst){
		theLst = lst;
	}
	
    //! Functie voor het verkrijgen van een pointer naar de SensorListener die naar deze HallSensor luistert
    /*!
     \return theLst de betreffende SensorListener
     */
	SensorListener * getListener (void){
		return theLst;
	}
	
private:
    //! de SensorListener die events kan ontvangen van de HallSensor
	SensorListener * theLst;
    //! de status waar in de HallSensor zich bevind
    /*!
     * deze bool staat op 0 als er niks wordt gedetecteerd en 1 als er wel iets gedetecteerd wordt.
     */
	bool state;
    
};


#endif
