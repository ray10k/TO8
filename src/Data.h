/* ========================================================================== */
/*                                                                            */
/*   Data                                                            */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef __Data
#define __Data

#include "pRTOS.h"
//! Een entity klasse verantwoordelijk voor het opslaan van alle Data, en deze te berekenen.
/*!
 * Hierin worden ook alle berekeningen gedaan om pulsen naar het correcte formaat te zetten.
 */
class Data {
public:
    
    //! Constructor
    Data();
    
    //! functie om de snelheid te verkrijgen
    /*!
     \return current speed
     */
	int getSpeed(void);
    //! functie om de gemiddelde snelheid te verkrijgen
    /*!
     \return current speed
     */
	int getAverage(void);
    //! functie om de maximale snelheid te verkrijgen
    /*!
     \return max speed
     */
	int getMax(void);
    //! functie om de totaal gereden snelheid te verkijgen
    /*!
     \return total distance
     */
	int getTotalDistance(void);
    //! functie om de totaal gereden rijtijd te verkrijgen
    /*!
     \return driving time
     */
	int getDrivingTime(void);
    //! functie om de verbruikte calorieen te verkrijgen
    /*!
     \return calories
     */
	int getCalories(void);
    //! functie om de gereden afsand te verkijgen
    /*!
     \return distance
     */
	int getDistance(void);
    //! functie om de tijd die bij de laatste pulse hoord te verkrijgen
    /*!
     \return Last Pulse time
     */
	int getLastPulseTime(void);
    //! functie om alle data the clearen
    /*!
     * alle data variabelen worden op 0 gezet. deze roept ook clearCurrent aan
     */
	void clearAll (void);
    //! functie om minder the clearen
    /*!
     * deze zet alleen prevTime, firstTime, pulses, speed, maxSpeed, 
     * avarageSpeed en distance op 0
     */
	void clearCurrent (void);
    //! functie om een nieuwe pulse toe tevoegen
    /*!
     \param time een int met de systeem tijd om berekeningen mee uit te voeren.
     */
	void addPulse(unsigned long long int pulseTime);
	//! functie om te controlleren of maximale waarde niet overschreven zijn.
	bool isError(void);
    //! functie om de vorige tijd te updaten
    /*!
     \param t in het formaat van RTOS::run_time()
     */
    void setPrevTime(unsigned long long int t);
private:
    unsigned long long int prevTimeTmMode;      //! voor het opslaan van de vorige tijd
    unsigned long long int prevTime;      //! voor het opslaan van de vorige tijd 
    unsigned long long int firstTime;     //! voor het opslaan van de eerste time
    unsigned int pulses;        //! voor het opslaan van het aantal pulses
    unsigned long long int time;          //! voor het opslaan van de tijd
    
    unsigned long long int speed;         //! voor het opslaan van de snelheid
    unsigned int maxSpeed;      //! voor het opslaan van de maximale snelheid
    unsigned int averageSpeed;  //! voor het opslaan van de gemiddelde snelheid
    unsigned int distance;      //! voor het opslaan van de afstand
    unsigned int totalDistance; //! voor het opslaan van de totale afgelegde afstand
    
    
    unsigned int wheelRadius;   //! voor het opslaan van het wiel radius
    unsigned int weight;        //! voor het opslaan van het gewicht van de gebruiker
    
};



#endif
