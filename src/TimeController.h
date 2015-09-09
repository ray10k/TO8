#include "pRTOS.h"
#include "DisplayController.h"

#ifndef _TimeController
#define _TimeController

//!  Controller Klasse die verantwoordelijk is voor het bijhouden van de 24 uurs klok.
/*!
 * Naast het bijhouden van de 24 uurs klok moet het deze tijd ook op het
 * scherm schrijven.
 * deze erft van RTOS::task
 */
class TimeController : public RTOS::task {
public:
    
    RTOS::clock interval; //!< clock voor het aangeven of er een nieuwe tijd moet geschreven worden naar de DisplayController
    //! constructor
    /*!
     \param prio is een integer om de prioriteit van de RTOS task te zetten
     \param dc een referentie naar de DisplayController
     */
    TimeController( int prio, DisplayController &dc );
    //! het reseten van de tijd
    /*!
     * hierin wordt de currentTime op 0 gezet. zodat de tijd herstart.
     */
    void resetTime( void );
    
private:  
    //! Main functie vanuit RTOS
    /*!
     * Hier wacht hij op het TimeController#interval, wat gelijk staat aan de state WAITING deze wordt elke seconde periodiek aangeroepen. Hiernaa format hij zijn TimeController#stringTime naar het correcte formaat met de huidige tijd, en schrijft hij dit naar de DisplayController::displayData();. Hiernaa verhoogd hij de TimeController#currentTime
     */
    void main( void );
    
    DisplayController &theDisplayController;//!< referentie naar een instantie van de DisplayController
    char stringTime[17];                    //!< een string om de tijd in opteslaan, waarna deze gestuurd kan worden naar de DisplayController
    int currentTime;                        //!< variabele om de tijd in bij te houden
    int sendingData;                        //!< een integern om bij te houden of er data wordt gestuurd
};

#endif
