/* ========================================================================== */
/*                                                                            */
/*   ButtonListener.h                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Een interface                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef _ButtonListener
#define _ButtonListener

class Button;

//! Een interface voor het afhandelen van Button Events
/*!
 * Een klasse die wil luisteren naar Button events moet deze implementeren
 */
class ButtonListener {
public:
    //! Public virtual functie voor het afhandelen van een buttonDown
    /*!
     \param b van het type Button, de button wordt mee gestuurd zodat de Listener weet welke actie hij moet uitvoeren aan de hand van de button
     */
	virtual void buttonDown (Button * b) = 0;
    //! Public virtual functie voor het afhandelen van een buttonDown
    /*!
     \param b van het type Button, de button wordt mee gestuurd zodat de Listener weet welke actie hij moet uitvoeren aan de hand van de button
     */
	virtual void buttonUp (Button * b) = 0;
    
};

#endif
