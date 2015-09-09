/* ========================================================================== */
/*                                                                            */
/*   ButtonHandler.h                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef __ButtonHandler
#define __ButtonHandler

#define MAX_BUTTONS 2

#include "pRTOS.h"
#include "Button.h"

//! handler klasse verantwoordelijk voor het pollen van de knoppen, zodat deze events kunnen door sturen naar de ButtonListener 's
/*!
 * deze implementeerd RTOS::task
 */
class ButtonHandler : public RTOS::task{
public:   
    //! Constructor
    /*!
     \param prio is een integer om de prioriteit van de RTOS task te zetten
     */
	ButtonHandler (int prio);
    //! Public functie voor het toevoegen van een instantie van een Button
    /*!
     \param button is een Button waar de ButtonHandler berichten naar gaat sturen.
     */
	void addButton (Button &button);
	
private:
    //! Main functie, geimplementeerd vanuit RTOS
    /*!
     * In deze functie worden alle buttons die toegeveogd zijn aan deze ButtonHandler,
     * aangeroepen met de functie Button::updateState(), zodat de de werkelijke buttons gepolled kunnen worden.
     */
	void main (void);
	
    //! Private Variable, voor de clock
    /*!
     * de main clock waar de ButtonHandler naar kijkt om te zien of hij een beurt krijgt.
     */
	RTOS::clock bhPeriod;
	
    //! Private variable, om het aantal toegevoegde buttons in op te slaan.
	short nr;
    
    //! Een Array met referenties naar instanties van Buttons.
    /*!
     * deze worden doorgelopen om te pollen.
     */
	Button * theButtons[2];
    
};

#endif
