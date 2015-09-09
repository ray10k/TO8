//****************************************************************************
//
// flags example
//
//****************************************************************************


#include "pRTOS.h"
#include "LCD.h"

#ifndef _DisplayController
#define _DisplayController

//! een struct voor het opslaan van een DisplayInfo dat naar het LCD scherm moet worden gestuurd
/*!
 \struct DisplayInfo 
 */
struct DisplayInfo {
    //! string
    char buf[16];  /**< char buffer voor het opslaan van de string die naar het scherm moet worden geschreven deze kan maximaal 16 karakters lang zijn */
    short row; //!< de rij waar de string op moet worden geschreven
    short len; //!< lengte van de string
};  

//! controller klasse voor het verantwoordelijk voor het aansturen van het LCD scherm
/*!
 * deze implementeerd RTOS::task
 */
class DisplayController : public RTOS::task {
public:   
    //! Constructor
    /*!
     \param prio is een integer om de prioriteit van de RTOS task te zetten
     \param lcd referentie naar het LCD scherm, zodat Display Controller deze kan aansturen.
     */
    DisplayController( int prio, LCD &lcd );
    //! functie om data naar het channel toe te schrijven, wat weer naar het LCD scherm moet worden gestuurd.
    /*!
     \param data een constante char pointer naar een string die op het scherm moet worden gezet
     \param row een int met de rij waar op het LCD scherm moet worden geschreven, kan alleen 0 of 1 zijn
     */
    void displayData( const char * data, int row );
    //! functie om het display leeg te maken.
    void clearDisplay( void );
    //! overide van RTOS::suspend
    /*!
     * deze functie wordt overschreven omdat, wanneer suspend wordt aangeroepen, ook het LCD scherm meteen uit wordt gezet.
     */
    virtual void suspend( void );
    //! overide van RTOS::resume
    /*!
     * deze functie wordt overschreven omdat, waneer resume wordt aangeropen, ook het LCD scherm meteen aan wordt gezet
     */
    virtual void resume( void );
private:    
    //! main functie vanuit RTOS
    /*!
     * in deze functie wordt er gekeken in welke state de DisplayController zich bevindt
     * Als hij zich in de status DisplayController::WAITING bevindt dan gaat hij net zo lang wachten totdat er iets in de DisplayController#buffer (channel) wordt
     * geschreven, zodra er iets in staat en hij krijgt de beurt, wordt de status op DisplayController::FLUSHING gezet.
     *
     * In de status DisplayController::FLUSHING, worden alle letters vanuit het nieuwe DisplayInfo om de beurt naar het LCD scherm geschreven.
     * na elke keer wordt er een release aangeroepen zodat andere taken de beurt kunnen krijgen en de DisplayController niet te 
     * veel tijd in neemt, zodat de rest van de taken de beurt niet optijd krijgen.
     * als een hele DisplayInfo klaar is met op het scherm zetten wordt de Status weer op DisplayController::WAITING gezet.
     */
    void main( void );
    
    //! An enum type met de status waarin de DisplayController zich kan bevinden 
    /*!
     \enum States verschillende statusen
     */
    enum States {
        WAITING, /**< hierin wacht hij op een event, zodat hij daarna het correct het bijhorende event kan afhandelen */
        FLUSHING /**< in deze staat stuurt hij de karakters naar het scherm */
    };
    
    //! een RTOS::channel voor het opslaan van alle DisplayInfo 's die naar het LCD scherm moeten worden geschreven
    /*!
     * hierin staan alle structs DisplayInfo waarin de informatie staat die naar het LCD scherm geschreven worden,
     * de channel is 10 groot
     */
    RTOS::channel<DisplayInfo, 10> buffer;
    //! een DisplayInfo dat geschreven moet worden naar het LCD scherm
    DisplayInfo di;
    //! een enum van de States zodat de displayController weet in welke hij zich bevindt.
    States state;
    //! het karakter op het scherm waar hij nu is, dit wordt gebruikt bij het testen van het scherm
    int index;
    //! een referentie naar het LCD scherm
    /*!
     * de referentie wordt bewaard, zodat de DisplayController berichten kan sturen naar het LCD scherm 
     */
    LCD & theLcd;
};

#endif
