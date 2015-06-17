/******************************************************************************\
 FILE:   test.cpp

 This file contains several programs to test the RTOS package.

\******************************************************************************/

#configure port			com3
#configure board        hu_arm_v4
#configure clcd_lines   2
#configure memory       rom
#configure baudrate     38400
//
#include "pRTOS.h"
#include "Armboard.h"
//
// Output routine for debug and statistics info
//
static void uart0_write (char c) {
   mkt_uart_char_write (UART0, c);
   RTOS::task *t = RTOS::current_task();
   if( t != 0 ) {
      t->ignore_activation_time();
      t->release();
   }
}

static void recWrite (int n) {
   if (n > 0) {
      recWrite (n / 10);
      armboard::LCD::char_write_raw ('0' + n % 10);
   }
}

static void decimalWrite (int n) {
   if (n > 0)
      recWrite (n);
   else if (n == 0)
      armboard::LCD::char_write_raw ('0');
   else {
      armboard::LCD::char_write_raw ('-');
      decimalWrite (-n);
   }
}

/******************************************************************************\
 IdleTask class
\******************************************************************************/
class IdleTask : public RTOS::task {
public:
   IdleTask (RTOS::mutex & _m ) : task (3, "IdleTask"), n (0), m (_m) {
   }

private:
   void main (void) {
		for (;;) {
	      if (n % 10000 == 0) {
            m.wait();
		      armboard::LCD::goto_xy (8, 0);
	         decimalWrite (n / 10000);
            m.signal();
         }
	      n += 1;
			release();
		}
   }

   int n;
   RTOS::mutex & m;
};

/******************************************************************************\
 PoolTask class
\******************************************************************************/
class PoolTask : public RTOS::task {
public:
   PoolTask (short prio, const char * name, RTOS::pool<int> * p, int i, RTOS::mutex & _m) :
      RTOS::task (prio, name),
		pp (p),
		id (i),
		tm (this, "pool timer"),
		n (0),
      m (_m)
	{
      pp->write (0);
   }

private:

   void main (void) {
		for (;;) {
         if (id == 1) {
            m.wait();
		      armboard::LCD::goto_xy (0, id - 1);
	         decimalWrite (n);
            m.signal();
	         pp->write (n++);
	      }
	      else {
	         n = pp->read();
            m.wait();
	         armboard::LCD::goto_xy (0, id - 1);
            decimalWrite (n);
            m.signal();
	      }
	      tm.set (id == 1 ? 500 MS : 1000 MS);
	      wait(tm);
		}
   }

private:
   RTOS::pool<int> * pp;
	int id;
	RTOS::timer tm;
   int n;
   RTOS::mutex & m;
};

/******************************************************************************\
 Pool test
\******************************************************************************/
void poolTest (void) {

   RTOS::pool<int> pool ("int pool");
   RTOS::mutex m ("mutex");

   PoolTask p0 (1, "PoolTask 1", &pool, 1, m);
   PoolTask p1 (2, "PoolTask 2", &pool, 2, m);
   IdleTask it (m);

   RTOS::run();
}


/******************************************************************************\
 MutexTask class
\******************************************************************************/
class MutexTask : public RTOS::task {
public:
   MutexTask (short prio, const char * name, RTOS::mutex * _mut, int i, RTOS::mutex & _m) :
      RTOS::task (prio, name),
      id (i),
		pm (_mut),
      m (_m),
		tm (this, "mutex timer")
	{
   }

private:

   void main (void) {
		for (;;) {
	   	pm->wait();
	      if (id == 1) {
            m.wait();
	         armboard::LCD::goto_xy (0, 0);
	         decimalWrite (++n);
            m.signal();
	         tm.set (500 MS);
			}
			else {
            m.wait();
				armboard::LCD::goto_xy (0, 1);
	         decimalWrite (n);
            m.signal();
				tm.set (750 MS);
	      }
	      pm->signal();
			wait (tm);
		}
   }

private:
   static int n;

	int id;
   RTOS::mutex * pm;
   RTOS::mutex & m;
	RTOS::timer   tm;
};

int MutexTask::n = 0;

/******************************************************************************\
 Mutex test
\******************************************************************************/
void mutexTest (void) {

   RTOS::mutex mut ("mutex 1");
   RTOS::mutex m ("mutex 2");

   MutexTask mt0 (1, "MutexTask 1", &mut, 1, m);
   MutexTask mt1 (2, "MutexTask 2", &mut, 2, m);
   IdleTask it (m);

   RTOS::run();
}

/******************************************************************************\
 MailboxTask class
\******************************************************************************/
class MailboxTask : public RTOS::task  {
public:
   MailboxTask (short prio, const char * name, RTOS::mailbox<int> * mb, int i, RTOS::mutex & _m) :
      RTOS::task (prio, name),
		pmb (mb),
		id (i),
      m (_m),
		tm (this, "mailbox timer"),
		n (0)
	{
   }

private:

   void main (void) {
		for (;;) {
	      if (id == 1) {
	         n = pmb->read();
            m.wait();
				armboard::LCD::goto_xy (0, 1);
	         decimalWrite (n);
            m.signal();
	         tm.set ((n & 1) == 0 ? 1000 MS : 500 MS);
	      }
	      else {
            m.wait();
				armboard::LCD::goto_xy (0, 0);
	         decimalWrite (n);
            m.signal();
	         pmb->write (n++);
	         tm.set ((n & 1) == 0 ? 1500 MS : 1000 MS);
			}
			wait (tm);
		}
   }

  RTOS::mailbox<int> * pmb;
  int id;
  RTOS::mutex & m;
  RTOS::timer tm;
  int n;
};

/******************************************************************************\
 Mailbox test
\******************************************************************************/
void mailboxTest (void) {

   RTOS::mailbox<int> mb("int box");
   RTOS::mutex m ("mutex");

   MailboxTask mbt0 (1, "MailboxTask 1", &mb, 1, m);
   MailboxTask mbt1 (2, "MailboxTask 2", &mb, 2, m);
   IdleTask it (m);

   RTOS::run();
}

/******************************************************************************\
 ChannelTask class
\******************************************************************************/
class ChannelTask : public RTOS::task {
public:
   ChannelTask (short prio, const char * name, ChannelTask * ct, int i, RTOS::mutex & _m) :
      RTOS::task (prio, name),
		chTask (ct),
		id (i),
      m (_m),
      ch (this, "int channel"),
		tm (this, "channel timer"),
		n (0)
	{
   }

private:

	void writeChannel (int n) {
		ch.write (n);
	}

   void main (void) {
		for (;;) {
	      if (id == 0) {
				n = ch.read();
            m.wait();
				armboard::LCD::goto_xy (n >= 0 ? 0 : 8, 1);
	         decimalWrite (n);
            m.signal();
	      }
	      else {
	         if (id == 1) {
				   chTask->writeChannel (n);
               m.wait();
					armboard::LCD::goto_xy (0, 0);
	            decimalWrite (n++);
               m.signal();
	            tm.set ((n & 1) == 0 ? 500 MS : 250 MS);
	         }
	         else {
				   chTask->writeChannel (n);
               m.wait();
					armboard::LCD::goto_xy (8, 0);
	            decimalWrite (n--);
               m.signal();
	            tm.set ((n & 1) == 1 ? 750 MS : 1500 MS);
	         }
	         wait(tm);
	      }
		}
   }

   ChannelTask * chTask;
   int id;
   RTOS::mutex & m;
	RTOS::channel<int, 4> ch;
	RTOS::timer tm;
	int n;
};

/******************************************************************************\
 Channel test
\******************************************************************************/
void channelTest (void) {

   RTOS::mutex m ("mutex");

   ChannelTask cht0 (1, "ChannelTask 1", 0, 0, m);
   ChannelTask cht1 (2, "ChannelTask 2", &cht0, 1, m);
   ChannelTask cht2 (3, "ChannelTask 3", &cht0, 2, m);

   RTOS::run();
}


/******************************************************************************\
Timing test
\******************************************************************************/
void lcd_int_write( int n ){
   armboard::LCD::char_write_raw( n / 10 + '0' );
   armboard::LCD::char_write_raw( n % 10 + '0' );
}

class TimingTask : public RTOS::task {
public:
   TimingTask (short prio) :
      RTOS::task (prio, "TimingTask"),
      clk ( this, 1 S, "my clock" )
   { }

   void main ( void ) {
      int h = 0, m = 0, s = 0;
      armboard::LCD::char_write( '\v' );
      lcd_int_write( h );
      armboard::LCD::char_write_raw(':');
      lcd_int_write( m );
      armboard::LCD::char_write_raw(':');
      lcd_int_write( s );

      for( ; m <= 1 ; ) {
         wait( clk );
         s++;
         if( s >= 60 ){ s = 0; m++; }
         if( m >= 60 ){ m = 0; h++; }

         armboard::LCD::char_write( '\v' );
         lcd_int_write( h );
         armboard::LCD::char_write_raw(':');
         lcd_int_write( m );
         armboard::LCD::char_write_raw(':');
         lcd_int_write( s );
      }
      RTOS::display_statistics();
      for(;;);
   }

private:
   RTOS::clock clk;
};

void timingTest (void) {
   TimingTask t(0);
   RTOS::run();
}


/******************************************************************************\
 main routine
\******************************************************************************/
int main( void )
{
   mkt_user_error_vector = mkt_default_error_handler;
   mkt_uart_init (UART0, mkt_baudrate);
   mkt_print_vector = uart0_write;
   armboard::LEDs::write (0);
   armboard::LCD::clear();
   if (global_logging) {
      trace << "main started\n";
   }

   timingTest();
   //poolTest();
   //mutexTest();
   //mailboxTest();
   //channelTest();
}
