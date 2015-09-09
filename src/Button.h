/* ========================================================================== */
/*                                                                            */
/*   Button.h                                                               */
/*   (c) 2001 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#ifndef __Button
#define __Button


#include "ButtonListener.h"

class ButtonListener;

//! boundary klasse verantwoordelijk voor het aansturen van de knoppen
class Button {
public:
	//! Constructor.
    /*!
     \param aKey is van het type mkt_key
     */
	Button(mkt_key aKey);
    
    //! UpdateState functie om de button te pollen
    /*!
     * Deze wordt aangeroepen door de buttonHandler
     */
	void updateState(void);
    //! functie om een listener toetevoegen aan een instantie van een button
    /*!
     \param lst een klasse die ButtonListener implementeerd
     */
	void setListener (ButtonListener * lst);
private:
	
    //! mkt_key voor het opslaan welke key bij een instantnie van een button hoort
    /*!
     *
     */
	const mkt_key key;
	
    //! downSeen
    /*!
     *
     */
	short downSeen;
    //! upSeen
    /*!
     *
     */
	short upSeen;
    //! last bool voor het opslaan van de vorige status
    /*!
     * of de vorige keer de knop ingedrukt was of niet
     */
	bool last;
	
    //! theLst referentie van ButtonListener
    /*!
     * zodat de button weet naar welke ButtonListener hij een event moet sturen.
     */
	ButtonListener * theLst;
    
};




#endif
