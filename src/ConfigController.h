/* ========================================================================== */
/*                                                                            */
/*   ConfigController.h                                                       */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _ConfigController
#define _ConfigController

/*! \define de snelheid in MS waarmee de display test moet worden uitgevoerd  */ 
#define DISPLAY_TEST_SPEED 200 MS

#include "pRTOS.h"
#include "DisplayController.h"
#include "TimeController.h"
#include "ModeController.h"
#include "ButtonListener.h"
#include "Button.h"
#include "Data.h"

//! De configuratie controller klasse, deze is verantwoordelijk voor het afvangen van de reset knop en het uitvoeren van de display check
/*!
 * de ConfigController wordt als eerste gestart in het systeem.
 * deze implementeerd RTOS::task en ButtonListener zodat deze kan luisteren naar de knoppen.
 */
class ConfigController : public RTOS::task, public ButtonListener {
public:
    RTOS::timer disChkTimer;    //!< timer voor het uitvoeren van de display check
    RTOS::timer rHoldTimer;     //!< timer voor het kijken of de reset knop werd ingehouden.
    RTOS::flag btnFlag;         //!< flag voor het aangeven of er een knop is ingedrukt.
    
    //! Constructor
    /*!
     \param prio is een integer om de prioriteit van de RTOS task te zetten
     \param dc referentie naar de DisplayController
     \param tc referentie naar de TimeController
     \param mc referentie naar de ModeController
     \param rb referentie naar een instantie van Button die de theResetButton moet gaan zijn.
     \param data referentie naar de Data
     */
    ConfigController( int prio, DisplayController &dc, TimeController &tc,ModeController &mc, Button &rb, Data &data );
    //! implementatie van buttonDown
    /*!
     * de holdTimer wordt gezet op 2 seconden zodat als deze afvuurt, 
     * de ConfigController weet dat er een button ingehouden was
     */
    void buttonDown( Button * b );
    //! implementatie van buttonUp
    /*!
     * Hier wordt de btnFlag gezet zodat de ConfigController weet dat er een knop ingedrukt wordt.
     */
    void buttonUp( Button * b );
    
private:
    //! Main functie vanuit RTOS
    /*!
     * hierin gaat hij eerst in een wachtende staat, waar hij wacht op een van zijn waitables
     * Als dit de ConfigController#disChkTimer is dan gaat hij aan de hand van zijn ConfigController#state een actie uitvoeren
     *
     * Als zijn ConfigController#state ConfigController::CHECKING is dan voert hij de displayCheck uit. als hij hiermee klaar is wordt de ConfigController#state op ConfigController::RUNNING gezet
     * Als hij op ConfigController::RUNNING is dan start hij de ModeController & TimeController doormiddel van een resume
     *
     * Als het de ConfigController#btnFlag was, dan zet kijkt hij eerst of holded op true stond, en zet deze op false, als dit wel zo was dan wordt er op ConfigController#rHoldTimer cancel uitgevoerd. (normaal gesproken zou hij hier in callibiratie toestand komen.
     *
     * Als het ConfigController#rHoldTimer was geweest betekend het dat het systeem moet herstarten. Dan wordt de boolean holded op true gezet. en wordt de TimeController::resetTime() uitgevoerd, Data::clearAll() en worden de ModeController & TimeController gestopt doormiddel van suspend. Waarna hij de DisplayController resumed en DisplayController::clearDisplay uitoverd. Als laatst zet hij zijn eigen state op ConfigController::CHECKING zodat de displayTest weer start
     *
     */
    void main( void );
    
    //! een enum waarin alle statusen waarin de ConfigController zich kan bevinden bijhoud.
    enum States {
        CHECKING, //!< in deze status voert hij de displayCheck uit
        RUNNING   //!< in deze status start hij de ModeController en TimeController zodat het totale systeem in running mode komt     
    };
    
    DisplayController &theDisplayController;//!< referentie naar een instantie van de DisplayController
    TimeController &theTimeController;      //!< referentie naar een instantie van de TimeController
    ModeController &theModeController;      //!< referentie naar een instantie van de ModeController
    Button &theResetButton;                 //!< referentie naar een instantie van een Button, die de reset button moet representeren
    Data &theData;                          //!< referentie naar een instantie van de Data
    
    States state;   //!< een variabele om de status waarin het systeem zich bevindt bij te houden
    int segment;    //!< een variabele om bij te houden in welk segement de display check zich bevind
    int row;        //!< een variabele om bij te houden in welke rij de displayCheck zich bevindt
};

#endif
