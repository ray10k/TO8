//**************************************************************************
//
//! \par Files
//! - armboard.h
//! - armboard.cpp
//! - mkt.py
//!
//***************************************************************************

#ifndef __armboard
#define __armboard

#define armboard_VERSION "V1.00 (2012-04-04)"

// We need the RTOS
#include "pRTOS.h"


//***************************************************************************
//
// class armboard
//
//! interface to some peripherals of the HU ARM board
//
//! This static class provides interfaces to some of the peripherals of the
//! HU LPC2148 ARM board. 
//!
//! The class and all its constituents are static, so no instatiation is 
//! needed.
//
//***************************************************************************

class armboard {
public:

   //! delay : wait some time
   //! 
   //! This delay routine will use the RTOS when it is running, avoid
   //! busy waiting, which would impact the real-time performance of 
   //! the application.
   //! When the RTOS is not running it will use busy waiting.
   static void wait( unsigned long long int t );

   //! an output pin
   class output_pin {
   public:
   	
   	  //! set the pin high or low
      virtual void set( bool x )=0;
      
      //! configure the pin as output pin
      virtual void direction_set_output(){}
   };
  
   //! an input pin
   class input_pin {
   public:
   	
   	  //! read the current value (high or low) of the pin
      virtual bool get()=0;
      
      //! configure the pin as input pin
      virtual void direction_set_input(){}
   };
  
   //! specfies the direction of a pin
   enum direction { input, output };
  
   //! a pin that can be configure as input or output
   class input_output_pin : public output_pin, public input_pin {
   public:
   	
   	  //! configure the direction of the pin
      virtual void direction_set( direction d ){
         if( d == input ){
            direction_set_input();
         } else {
            direction_set_output();
         }
      }
      
      //! configure the pin as input
      void direction_set_input(){
         direction_set( input );
      }
      
      //! configure the pin as output
      void direction_set_output(){
         direction_set( output );  
      }                      
   };

   //! an input-output pin implemented using the mkt_ routines
   class input_output_pin_mkt: public input_output_pin {
   private:
      unsigned int nr;
      
   public: 
   	
   	  //! create a pin: specify the port nr and the pin nr
      input_output_pin_mkt( unsigned int port, unsigned int pin ):
         nr( port * 32 + pin ){}
     
      //! configure the pin as input
      void direction_set_input(){
         mkt_pin_configure( nr, mkt_input );
      }
     
      //! configure the pin as output
      void direction_set_output(){
         mkt_pin_configure( nr, mkt_output );  
      }
     
      //! set the pin high or low
      void set( bool x ){
         mkt_pin_write( nr, x );        
      }
     
   	  //! read the current value (high or low) of the pin     
      bool get(){ 
         return mkt_pin_read( nr );
      }
   };
   
   //! the pins of the LPC2148 chip on the board
   class chip {
   public:
      static input_output_pin_mkt pin_0_0;
      static input_output_pin_mkt pin_0_1;
      static input_output_pin_mkt pin_0_2;
      static input_output_pin_mkt pin_0_3;
      static input_output_pin_mkt pin_0_4;
      static input_output_pin_mkt pin_0_5;
      static input_output_pin_mkt pin_0_6;
      static input_output_pin_mkt pin_0_7;
      static input_output_pin_mkt pin_0_8;
      static input_output_pin_mkt pin_0_9;
      static input_output_pin_mkt pin_0_10;
      static input_output_pin_mkt pin_0_11;
      static input_output_pin_mkt pin_0_12;
      static input_output_pin_mkt pin_0_13;
      static input_output_pin_mkt pin_0_14;
      static input_output_pin_mkt pin_0_15;
      static input_output_pin_mkt pin_0_16;
      static input_output_pin_mkt pin_0_17;
      static input_output_pin_mkt pin_0_18;
      static input_output_pin_mkt pin_0_19;
      static input_output_pin_mkt pin_0_20;
      static input_output_pin_mkt pin_0_21;
      static input_output_pin_mkt pin_0_22;
      static input_output_pin_mkt pin_0_23;
      
      static input_output_pin_mkt pin_0_25;
      
      static input_output_pin_mkt pin_0_28;
      static input_output_pin_mkt pin_0_29;
      static input_output_pin_mkt pin_0_30;
      static input_output_pin_mkt pin_0_31;
      
      static input_output_pin_mkt pin_1_16;
      static input_output_pin_mkt pin_1_17;
      static input_output_pin_mkt pin_1_18;
      static input_output_pin_mkt pin_1_19;
      static input_output_pin_mkt pin_1_20;
      static input_output_pin_mkt pin_1_21;
      static input_output_pin_mkt pin_1_22;
      static input_output_pin_mkt pin_1_23;
      static input_output_pin_mkt pin_1_24;
      static input_output_pin_mkt pin_1_25;
      static input_output_pin_mkt pin_1_26;
      static input_output_pin_mkt pin_1_27;      
      static input_output_pin_mkt pin_1_28;
      static input_output_pin_mkt pin_1_29;
      static input_output_pin_mkt pin_1_30;
      static input_output_pin_mkt pin_1_31;
   };


   //! the 8 LEDs on the board
   class LEDs {
   private:
      static input_output_pin & hold_clock;
      static input_output_pin & shift_clock;
      static input_output_pin & data;
      static void init( void );  
      
   public:
   	  //! write V to the LEDs
   	  //!
   	  //! The lower 8 bits of V will each determine the state of one LED.
   	  //! A zero bit will cause the corresponding LED to be off, a one
   	  //! bit will cause it to be on.
      static void write( unsigned int V ); 
   };

   //! the 2x16 characters LCD on the board
   class LCD {
   private:
      static input_output_pin & d4;
      static input_output_pin & d5;
      static input_output_pin & d6;
      static input_output_pin & d7;
      static input_output_pin & rs;
      static input_output_pin & e;
      static void write4( unsigned char n );
      static void write8( unsigned char isData, unsigned char b );      
      static void init( void );
      
   public:
   	  //! send the command byte cmd to the LCD
      static void command( unsigned char cmd );
      
      //! send the data byte chr to the LCD
      static void data( unsigned char chr );
      
      //! clear the LCD
      //!
      //! This function clears the LCD and puts the cursor at the
      //! top-left position (= 0,0).
      static void clear( void );
      
      //! write character chr and advance the cursor
      //!
      //! The following characters are handled specially:
      //!
      //! \\n : advance the cursor to the first position of the next line
      //!
      //! \\r : advance the cursor to the first position of the current line  
      //!
      //! \\v : clear the screen, cursor goes to upper left (first) position      
      static void char_write( char chr );
      
      //! write character chr and advance the cursor
      //!
      //! Unlike \ref char_write() no special handling of
      //! \\n etc. is done.
      static void char_write_raw( char chr );
      
      //! put the cursor at (x,y)
      static void goto_xy( unsigned char x, unsigned char y ); 
      
      //! put the cursor at position x of the current line
      static void goto_x( unsigned char x );    
   };

   //! the keypad on the board
   //!
   //! The keys are represented by the enumerate
   //! \code
   //!    enum mkt_key_enum {
   //!         mkt_key_0 = 5,     mkt_key_1 = 11,    mkt_key_2 = 10, 
   //!         mkt_key_3 = 9,     mkt_key_4 = 8,     mkt_key_5 = 15, 
   //!         mkt_key_6 = 14,    mkt_key_7 = 13,    mkt_key_8 = 12, 
   //!         mkt_key_9 = 7,     mkt_key_a = 3,     mkt_key_b = 2,  
   //!         mkt_key_c = 1,     mkt_key_d = 0,     mkt_key_star = 6,  
   //!         mkt_key_sharp = 4, mkt_key_none = 0xFF
   //!      };   
   //! \endcode
   class keypad {
   private:
      static input_output_pin & dat;
      static input_output_pin & clk;
      static void init( void );
   public:
   	
   	  //! read all keys, return a bitmask
      static int read_all( void );
      
      //! check whether the specified key is pressed
      static unsigned char is_pressed( mkt_key key );   
   };

};


#endif
