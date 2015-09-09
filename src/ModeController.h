//****************************************************************************
//
// SysteemController.h
//
//****************************************************************************

#ifndef _ModeController
#define _ModeController


#include "pRTOS.h"
#include "ButtonListener.h"
#include "Button.h"
#include "string.h"
#include "DisplayController.h"
#include "ButtonListener.h"
#include "Button.h"
#include "Data.h"
#include "TimeController.h"
#include "HallController.h"

class HallController;

//! Een controller klasse verantwoordelijk voor het besturen, bijhouden en updaten van de modes, ook verantwoordelijk voor het afhandelen van de modeknop.
/*!
 * deze implementeerd RTOS::task, en ButtonListener zodat deze naar de knoppen kan luisteren.
 */
class ModeController : public RTOS::task, public ButtonListener {
public:
    
    RTOS::timer mHoldTimer;     //!< een timer voor het controlleren of een knop is ingedrukt gehouden
    RTOS::flag btnFlag;         //!< een flag om doortegeven dat er een knop is ingedrukt
    RTOS::flag mtDataFlag;      //!< een flag om doortegeven dat er nieuwe data beschikbaar is in de Data klass
    RTOS::flag mtInactiveFlag;  //!< een flag om doortegeven dat de HallSensor al enige tijd inactief is
    
    //! Constructor
    /*!
     \param prio is een integer om de prioriteit van de RTOS task te zetten
     \param displayController is een referentie naar een instantie van de DisplayController
     \param data is een referentie naar de Data
     \param button is een referentie naar een instantie van de Button, deze moet de modeButton voorstellen
     \param hallController is een referentie naar een instantie van de HallController
     */
    ModeController( int prio,  DisplayController &displayController, 
                   Data &data, Button &button, 
                   HallController &hallController );
    //! implementatie van de functie buttonDown
    /*!
     * deze functie moet hij implementeren omdat het een ButtonListener is.
     * hierin zet hij de mHoldTimer op 2 seconden, zodat het systeem deze kan afvangen als de tijd voorbij is.
     \param b een Button pointer die aangeeft welke Button buttonDown heeft gestuurd.
     */
    void buttonDown( Button * b ){
        mHoldTimer.set(2 S);
    };
    //! implementatie van de functie buttonUp
    /*!
     * deze functie moet hij implementeren omdat het een ButtonListener is.
     * hierin zet hij de btnFlag. waarna hij deze later kan afhandelen als hij de beurt krijgt.
     \param b een Button pointer die aangeeft welke Button buttonUp heeft gestuurd.
     */
    void buttonUp( Button * b ){
        btnFlag.set();
    };
    //! overide van RTOS::resume
    /*!
     * deze wordt overschreven omdat, hij het LCD scherm wil clearen via de DisplayController
     * waarna hij op de DisplayController de functie suspend wil aanroepen, zodat deze niet meer naar het scherm schrijft
     * ook zet hij de huidige mode op IDLE, zodat de ModeController niets meer naar de DisplayController gaat toeschrijven,
     * terwijl deze op suspend staat.
     */
    virtual void resume( void );
    virtual void suspend( void );
private:
    //! Main functie vanuit RTOS
    /*!
     * De ModeController gaat eerst wachten totdat een van zijn Waitables wordt gezet, zodra dit wordt gedaan en hij krijgt de beurt gaat hij bepaalde acties liggend aan de gezete waitable uitvoeren.
     * Als het de ModeController#btnFlag was geweest, gaat hij eerst controlleren of de boolean holded op true staat, is dit zo dan zet hij deze op false, omdat de knop niet is ingehouden. Als holded op true stond, het andere geval dus, dan telt het als een enkele niet ingehouden druk op de knop. Waarna er op de ModeController#mHoldTimer cancel wordt uitgevoerd, zodat de timer van het inhouden gestopt wordt.
     * Daarna gaat hij aan de hand van de ModeController#modus waarin de ModeController zich bevindt een bepaalde actie uitvoeren.
     - acties bij enkele klik aan de hand van de ModeController#modus waarin hij zich bevindt.
     -# ModeController::IDLE: in dit geval wordt op de HallController & de DisplayController resume aangeroepen, en de modus op ModeController::SPD gezet.
     -# ModeController::CAL: de modus wordt op ModeController::SPD gezet, omdat hij ModeController::IDLE moet overslaan bij het switchten tussen modi
     -# ModeController::ERR: de ModeController#modus wordt op ModeController::SPD gezet om de ModeController te herstarten, en wordt er op Data de funtie Data::clearCurrent() uitgevoerd om de waardes op 0 te zetten.
     -# default: in alle andere gevallen wordt de modus geswitched naar de daar opvolgende 
     *
     * Als de waitable ModeController#mHoldTimer is geweest, dan wordt de boolean holded op true gezet en wordt er aan de hand van de ModeController#modus waarin het systeem zich bevindt een actie uitgevoerd
     - acties bij het inhouden van de knop aan de hand van de Modus.
     -# ModeController::SPD, ModeController::AVG, ModeController::MAX, ModeController::DST: hier wordt er data gecleared ( Data::clearCurrent() ), en de modus wordt op ModeController::IDLE gezet zodat de ModeController stopt met zijn taken.
     -# ModeController::ODO: Hier wordt alle Data leeggehaald en gaat het systeem weer verder.
     -# ModeController::TM, ModeController::CALL: deze zijn niet geimplementeerd.
     * 
     * Als de ModeController#mtInactivityFlag de gene was die gezet was. Wordt de ModeController#modus op ModeController::IDLE gezet zodat de ModeController stopt.
     * Hiernaa gaat hij kijken of Data niet te hoge waardes heeft doormiddel van de functie Data::isError() aan teroepen op de Data, is dit wel het geval, dan wordt de modus op ModeController::ERR gezet.
     * hierna gaat hij altijd aan de hand van de modus waarin hij zich bevindt een actie uitvoeren, wat hij hier precies doet is terug te zien in de definitie van de enum ModeController#Modi
     */
    void main( void );
    //! verschillende modi waarin de ModeController zich kan bevinden
    /*!
     *
     \enum Modi verschillende modi waarin de ModeController zich kan bevinden
     */
    enum Modi {
        IDLE,   /**< in deze mode blijft het systeem wachten op een event van de theModeKnop zodat deze naar de eerste mode kan gaan*/
        SPD,    /**< in deze mode laat het systeem de huidige snelheid zien */
        AVG,    /**< in deze mode laat het systeem de gemiddelde snelheid zien */
        MAX,    /**< in deze mode laat het systeem de maximale gereden snelheid zien */
        DST,    /**< in deze mode laat het systeem de gereden afstand zien*/
        ODO,    /**< in deze mode laat het systeem de totale gerende afstand zien*/
        TM,     /**< in deze mode laat het systeem de totale rij tijd zien */
        CAL,    /**< in deze mode laat het systeem het calorie verbruik zien */
        ERR     /**< in deze mode bevind het systeem zich in een mode waarin er een maximale waarde is overschreven */
    };   
    
    //! formateren van de snelheid
    /*!
     \param speed
     \param string
     \return geformateerde string met de snelheid
     */
    char* formatSpeed( int speed, char * string );
    //! formateren van de gemiddelde snelheid
    /*!
     \param avarage
     \param string
     \return geformateerde string met de gemiddelde snelheid
     */
    char* formatAverage( int average, char * string );
    //! formateren van de maximale snelheid
    /*!
     \param max
     \param string
     \return geformateerde string met de
     */
    char* formatMax( int max, char * string );
    //! formateren van de afstand
    /*!
     \param distance
     \param string
     \return geformateerde string met de
     */
    char* formatDistance( int distance, char * string );
    //! formateren van de totale afstand
    /*!
     \param totalDistance
     \param string
     \return geformateerde string met de
     */
    char* formatTotalDistance( int totalDistance, char * string );
    //! formateren van de rijTijd
    /*!
     \param drivingTime
     \param string
     \return geformateerde string met de
     */
    char* formatDrivingTime( int drivingTime, char * string );
    //! het formateren van de verbruikte calorieen
    /*! 
     \param calories
     \param string
     \return geformateerde string met de
     */
    char* formatCalories( int calories, char * string );
    //! het formateren van een error
    /*!
     \param string
     \return geformateerde string voor het displayen van een error
     */
    char* formatError( char * string );
    
    HallController &theHallController;          //!< referentie naar de HallController
    DisplayController &theDisplayController;    //!< referentie naar de DisplayController
    Data &theData;                              //!< referentie naar de Data
    Button &theModeButton;                      //!< referentie naar de Button, die als modeButton wordt gebruikt
    
    char output [16];   //!< de output string die naar de DisplayController wordt gestuurd.
    int modus;          //!< de variabel waarin wordt opgeslagen in welke modus de ModeController zich bevindt.
    
};

#endif
