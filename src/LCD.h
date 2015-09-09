//****************************************************************************
//
// 
//
//****************************************************************************

#include "pRTOS.h"

#ifndef _LCD
#define _LCD

//! boundary klasse voor het aansturen van het LCD scherm
class LCD {
public:
    //! functie voor het aanzetten van het scherm
    /*!
     */
    void enable( void );
    //! functie voor het uitzetten van het scherm
    /*!
     */
    void disable( void );
    //! functie voor het leegmaken van het scherm
    /*!
     alle lcd segementen worden leeggemaakt
     */
    void clear( void );
    //! functie om text op het scherm te zetten
    /*!
     \param data een const char pointer met een rij van karakters die op het scherm geschreven moeten worden
     \param row de onderste of bovenste rij van het scherm waar geschreven op kan worden, kan alleen 0 of 1 zijn
     */
    void setText( const char * data, int row );
    //! functie om een karakter te schrijven
    /*!
     * de rij en lokatie kunnen gezet worden doormiddel van gotoRowCol
     \param character het karakter dat geschreven moet worden naar het LCD scherm
     */
    void setChar( const char character );
    //! functie om de lokatie voor het schrijven van bv een karakter te zetten
    /*!
     * een karakter kan bv geschreven worden met setChar
     \param x het nummer op de x lijn
     \param y de bovenste of onderste rij , kan alleen 0 of 1 zijn
     */
    void gotoRowCol( const int x, const int y );
};

#endif