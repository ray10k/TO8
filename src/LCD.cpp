//****************************************************************************
//
// 
//
//****************************************************************************

#include "LCD.h"

void LCD::enable( void ) {
    mkt_clcd_command (0x0c); 
}

void LCD::disable( void ) {
    mkt_clcd_command (0x08); 
}

void LCD::clear( void ) {
    mkt_clcd_clear();
}

void LCD::setText( const char * data, int row ) {
    gotoRowCol( 0, row );
    for( ; *data != '\0'; data++ ) {
        mkt_clcd_char_write( *data );
    } 
}

void LCD::gotoRowCol( const int x, const int y ) {
    mkt_clcd_goto_xy(x, y);
}

void LCD::setChar( const char character ) {
    mkt_clcd_char_write( character );
}