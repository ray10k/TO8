//**************************************************************************
//
//! Armboard implementation
//
//***************************************************************************

#include "pRTOS.h"
#include "armboard.h"

// we need NULL
#include <cstring>


//***************************************************************************
//
// wait
//
//***************************************************************************

void armboard::wait( unsigned long long int t ){
   RTOS::task * current_task = RTOS::current_task();
   if( current_task == NULL ){
      mkt_wait_us( t );
   } else {
      current_task->sleep( t );
   }
}


//***************************************************************************
//
// PINs
//
//***************************************************************************

armboard::input_output_pin_mkt armboard::chip::pin_0_0 (  0,  0 );
armboard::input_output_pin_mkt armboard::chip::pin_0_1 (  0,  1 );
armboard::input_output_pin_mkt armboard::chip::pin_0_2 (  0,  2 );
armboard::input_output_pin_mkt armboard::chip::pin_0_3 (  0,  3 );
armboard::input_output_pin_mkt armboard::chip::pin_0_4 (  0,  4 );
armboard::input_output_pin_mkt armboard::chip::pin_0_5 (  0,  5 );
armboard::input_output_pin_mkt armboard::chip::pin_0_6 (  0,  6 );
armboard::input_output_pin_mkt armboard::chip::pin_0_7 (  0,  7 );
armboard::input_output_pin_mkt armboard::chip::pin_0_8 (  0,  8 );
armboard::input_output_pin_mkt armboard::chip::pin_0_9 (  0,  9 );
armboard::input_output_pin_mkt armboard::chip::pin_0_10(  0, 10 );
armboard::input_output_pin_mkt armboard::chip::pin_0_11(  0, 11 );
armboard::input_output_pin_mkt armboard::chip::pin_0_12(  0, 12 );
armboard::input_output_pin_mkt armboard::chip::pin_0_13(  0, 13 );
armboard::input_output_pin_mkt armboard::chip::pin_0_14(  0, 14 );
armboard::input_output_pin_mkt armboard::chip::pin_0_15(  0, 15 );
armboard::input_output_pin_mkt armboard::chip::pin_0_16(  0, 16 );
armboard::input_output_pin_mkt armboard::chip::pin_0_17(  0, 17 );
armboard::input_output_pin_mkt armboard::chip::pin_0_18(  0, 18 );
armboard::input_output_pin_mkt armboard::chip::pin_0_19(  0, 19 );
armboard::input_output_pin_mkt armboard::chip::pin_0_20(  0, 20 );
armboard::input_output_pin_mkt armboard::chip::pin_0_21(  0, 21 );
armboard::input_output_pin_mkt armboard::chip::pin_0_22(  0, 22 );
armboard::input_output_pin_mkt armboard::chip::pin_0_23(  0, 23 );

armboard::input_output_pin_mkt armboard::chip::pin_0_25(  0, 25 );

armboard::input_output_pin_mkt armboard::chip::pin_0_28(  0, 28 );
armboard::input_output_pin_mkt armboard::chip::pin_0_29(  0, 29 );
armboard::input_output_pin_mkt armboard::chip::pin_0_30(  0, 30 );
armboard::input_output_pin_mkt armboard::chip::pin_0_31(  0, 31 );

armboard::input_output_pin_mkt armboard::chip::pin_1_16(  1, 16 );
armboard::input_output_pin_mkt armboard::chip::pin_1_17(  1, 17 );
armboard::input_output_pin_mkt armboard::chip::pin_1_18(  1, 18 );
armboard::input_output_pin_mkt armboard::chip::pin_1_19(  1, 19 );
armboard::input_output_pin_mkt armboard::chip::pin_1_20(  1, 20 );
armboard::input_output_pin_mkt armboard::chip::pin_1_21(  1, 21 );
armboard::input_output_pin_mkt armboard::chip::pin_1_22(  1, 22 );
armboard::input_output_pin_mkt armboard::chip::pin_1_23(  1, 23 );
armboard::input_output_pin_mkt armboard::chip::pin_1_24(  1, 24 );
armboard::input_output_pin_mkt armboard::chip::pin_1_25(  1, 25 );
armboard::input_output_pin_mkt armboard::chip::pin_1_26(  1, 26 );
armboard::input_output_pin_mkt armboard::chip::pin_1_27(  1, 27 );
armboard::input_output_pin_mkt armboard::chip::pin_1_28(  1, 28 );
armboard::input_output_pin_mkt armboard::chip::pin_1_29(  1, 29 );
armboard::input_output_pin_mkt armboard::chip::pin_1_30(  1, 30 );
armboard::input_output_pin_mkt armboard::chip::pin_1_31(  1, 31 );

//***************************************************************************
//
// LEDs
//
//***************************************************************************

armboard::input_output_pin & armboard::LEDs::hold_clock  = armboard::chip::pin_0_16;
armboard::input_output_pin & armboard::LEDs::shift_clock = armboard::chip::pin_0_4;
armboard::input_output_pin & armboard::LEDs::data        = armboard::chip::pin_0_6;

void armboard::LEDs::init( void ){
   // the pins we need must be outputs
   hold_clock.direction_set_output();
   shift_clock.direction_set_output();
   data.direction_set_output();  
}  

void armboard::LEDs::write( unsigned int V ){
       
   // for lazy users: always initialise. doesn't do any harm.
   init();
         
   // shift the 8 data bits out
   for( unsigned int i = 0; i < 8; i++ ){
      data.set( V & 1 );
      wait( 1 US );
      shift_clock.set( 0 );
      wait( 1 US );
      shift_clock.set( 1 );
      wait( 1 US );
      V = V >> 1;
   }
   hold_clock.set( 0 );
   wait( 1 US );
   hold_clock.set( 1 );
   wait( 1 US );
};


//***************************************************************************
//
// LCD
//
//***************************************************************************

#define _clcd_lines       2
#define _clcd_columns     16

armboard::input_output_pin & armboard::LCD::d4 = armboard::chip::pin_0_9;
armboard::input_output_pin & armboard::LCD::d5 = armboard::chip::pin_0_10;
armboard::input_output_pin & armboard::LCD::d6 = armboard::chip::pin_0_11;
armboard::input_output_pin & armboard::LCD::d7 = armboard::chip::pin_0_12;
armboard::input_output_pin & armboard::LCD::rs = armboard::chip::pin_0_14;
armboard::input_output_pin & armboard::LCD::e  = armboard::chip::pin_0_13;

void armboard::LCD::write4( unsigned char n ){
   wait( 1 US );
   d4.set( n & 0x01 );
   d5.set( n & 0x02 );
   d6.set( n & 0x04 );
   d7.set( n & 0x08 );
   wait( 10 US );
   e.set( 1 );
   wait( 10 US );
   e.set( 0 );
   wait( 45 US );  // enough for most instructions
}
         
unsigned char _clcd_must_init = 1;

void armboard::LCD::write8( unsigned char isData, unsigned char b ){
   if( _clcd_must_init ){ init(); }
   rs.set( isData );
   write4( b >> 4 );
   write4( b );
}
   
void armboard::LCD::init( void ){
         
   // avoid recursive initialisations, so clear the flag now!
   _clcd_must_init = 0;
         
   // the pins we need must be outputs
   d4.direction_set_output();
   d5.direction_set_output();
   d6.direction_set_output();
   d7.direction_set_output();
   e.direction_set_output();
   rs.direction_set_output();
         
   // give LCD time to wake up
   e.set( 0 );
   wait( 50 MS ); // was 20
         
   // interface initialisation: make sure the LCD is in 4 bit mode
   // (magical sequence, taken from the HD44780 datasheet)
   write4( 0x03 );
   wait( 5 MS );
   write4( 0x03 );
   wait( 100 US );
   write4( 0x03 );
   write4( 0x02 );     // 4 bit mode
         
   // functional initialisation
   command( 0x28 );    // 4 bit mode, 2 lines, 5x8 font
   command( 0x0C );    // display on, no cursor, no blink
   clear();            // clear display, 'cursor' home
   goto_xy( 0, 0 );    // 'cursor' home
}            
                    
void armboard::LCD::command( unsigned char cmd ){
   write8( 0, cmd );
}
         
void armboard::LCD::data( unsigned char chr ){
   write8( 1, chr );
}

int _clcd_x, _clcd_y;
            
void armboard::LCD::clear( void ){
   command( 0x01 );
   wait( 2 MS );
   _clcd_x = _clcd_y = 0;
}      
               
void armboard::LCD::char_write( char chr ){
   if( chr == '\n' ){
      goto_xy( 0, _clcd_x + 1 );
      return;
   }
   if( chr == '\r' ){
      goto_xy( 0, _clcd_y );
      return;
   }
   if( chr == '\v' ){
      clear();
      return;
   } 
   char_write_raw( chr );
}
             
void armboard::LCD::char_write_raw( char chr ){
   // don't write outside the boundaries
   if( 
      ( _clcd_x < _clcd_columns )
      && ( _clcd_y < _clcd_lines )
   ){
      data( chr );
      _clcd_x++;
   }
}      
             
void armboard::LCD::goto_xy( unsigned char x, unsigned char y ){
   _clcd_x = x;
   _clcd_y = y;
   command( 0x80 + 
      (( y > 0 ) ? 0x40 : 0x00 ) 
      + ( x ));
}      
             
void armboard::LCD::goto_x( unsigned char x ){
   goto_xy( x, _clcd_y );
}


//***************************************************************************
//
// Keypad
//
//***************************************************************************
               
armboard::input_output_pin & armboard::keypad::dat = armboard::chip::pin_0_21;
armboard::input_output_pin & armboard::keypad::clk = armboard::chip::pin_0_22;
   
void armboard::keypad::init( void ){
  
   clk.direction_set_output();
   dat.direction_set_input();

   // start with clock low
   clk.set( 0 );
}

unsigned char _kbd_init_done = 0;

int armboard::keypad::read_all( void ){
   int data = 0;

   if( ! _kbd_init_done ){
      _kbd_init_done = 1;
      init();
   }

   // start a conversion
   clk.set( 1 );
   
   // the documentation says 30
   // add some margin
   wait( 50 US );

   // Read data
   for( unsigned int i = 0; i < 16; i++ ){
            
      // Generate a falling edge
      clk.set( 0 );

		// Shift bit
		data = data << 1;

		// Data's clocked out on rising edge
		// so read in after the falling edge
		data |= dat.get();

		// Data is clocked out on the rising edge
		clk.set( 1 );

		// 25 seems to be enough.
		// add some margin
		wait( 35 US );
   }
   return  data;
}
                        
unsigned char armboard::keypad::is_pressed( mkt_key key ){
   int data = read_all();                
               
   if( key == mkt_key_none ){ 
      return ( data == 0 );
   } else {
      return ( data & ( 1 << (int) key )) != 0;
   }
}
           
                         

