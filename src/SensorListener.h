/* ========================================================================== */
/*                                                                            */
/*   SensorListener.h                                                              */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef __SensorListener
#define __SensorListener

class HallSensor;

//! Interface voor het luisteren naar een sensor.
/*!
 een klasse kan deze interface implementeren zodat deze naar een Sensor kan luisteren, en een sensorEvent kan ontvangen.
 */
class SensorListener {
public:
    //! public virutal functie voor het ontvangen van een sensorEvent
    /*!
     deze wordt gegenereerd vanuit een HallSensor
     \param s een pointer naar een HallSensor, om zo te kunnen verifieren welke HallSensor het bericht heeft gestuurd
     */
	virtual void sensorEvent (HallSensor * s) = 0;
	
    
};

#endif
