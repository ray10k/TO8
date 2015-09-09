/***************************************************************************\

file      RTOS.cpp
version   3.0
author    Marten Wensink / Wouter van Ooijen
date      2011-03-23

\***************************************************************************/

#include "pRTOS.h"
#include "string.h"

using namespace std;


/***************************************************************************\

task

\***************************************************************************/

#define task_logging             (global_logging && 0)
#define debug_task_logging       (global_logging && 0)
#define hartbeat_logging         (global_logging && 0)

#define TASK_STATE( task ) \
   ( (task)->is_blocked() ? "B" : "-" ) << \
   ( (task)->is_suspended() ? "S" : "-" ) << \
   ( (task)->is_ready() ? "R" : "-" )

#define task_trace \
   if (debug_task_logging) \
      trace \
         <<  name() \
         << " " << TASK_STATE( this ) \
         << " "

void task_trampoline( void ){
   RTOS::current_task()->main();
   cout
      << "\nfatal error: task "
      << RTOS::current_task()->name()
      << " main() returned\n";
   // mkt will call mkt_fatal when a coroutine terminates
}


#define TASK_NAME( t ) (((t) == NULL)?"-":(t)->name())

unsigned int xstrlen( const char *s ){
   unsigned int n = 0;
   while( *s++ != '\0' ){
      n++;
   }
   return n;
}

void xstrcpy( char *d, const char *s ){
   for(;;){
      *d = *s;
      if( *d == '\0' ){
         return;
   }
   s++;
   d++;
   }
}

const char * string_allocate( const char *name ){
   char * result = (char *) mkt_memory_must_allocate( 1 + xstrlen( name ));
   xstrcpy( result, name );
   return result;
}

int nr_from_mask( unsigned int mask ){
   for( int i = 0; i < 32; i++ ){
      if( mask & ( 1 << i )){
         return i;
     }
   }
   return -1;
}

RTOS::task::task(
   unsigned int priority,
   const char * tname,
   unsigned int stacksize
) :
   task_is_blocked( false ),
   task_is_suspended( false ),
   task_priority( priority ),
   waitables( this ),
   logging( task_logging )
{
   RTOS_STATISTICS( task_name = string_allocate( tname ); )

   sleep_timer = NULL;
   coroutine = new mkt_coroutine_class( task_trampoline, stacksize );
   if( debug_task_logging ){
      trace
         << name()
         << " SP="
         << hex << mkt_stack_root( coroutine->stack() )
         << ".."
         << hex << mkt_stack_top( coroutine->stack() )
         << dec << "\n";
   }
   ignore_this_activation = false;
   RTOS::add( this );
   task_trace << "CREATED";
}

void RTOS::task::suspend (void) {
   task_trace << "suspend";
   task_is_suspended = true;
   release();
}

void RTOS::task::resume (void) {
   task_trace << "resume";
   task_is_suspended = false;
   release();
}

void RTOS::task::unblock (void) {
   task_trace << "unblock";
   task_is_blocked = false;
   release();
}

void RTOS::task::block (void) {
   task_trace << "block";

   // Only a running task can block itself
   if (RTOS::current_task() != this){
      mkt_fatal (#UNIQUE_ERROR); // task not blocked by itself
   }

   if( ! is_ready() ){
      mkt_fatal (#UNIQUE_ERROR); // panic: running task is not READY??
   }

   task_is_blocked = true;
   release();
}

void RTOS::task::release( void ){
   if( ! RTOS::scheduler_running ){
      return;
   }

   if( mkt_coroutine_running == mkt_coroutine_main ){

      // the callbacks run in the context of the RTOS!
      return;

      // the RTOS scheduler should not call release, only tasks should
      mkt_fatal (#UNIQUE_ERROR); // panic: scheduler calls release??
   }

   mkt_coroutine_resume( mkt_coroutine_main );
}

void RTOS::task::sleep( unsigned int time ){
   if( sleep_timer == NULL ){
      sleep_timer = new timer( this, "sleep timer" );
   }
   sleep_timer->start( time );
   wait( *sleep_timer );
}

void RTOS::task :: print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream
         << endl << endl << dec
         << setw( 18 ) << left  << "task name"
         << setw(  6 ) << right << "prio"
         << setw(  6 ) << right << "state"
         << setw(  8 ) << right << "rt_max"
         << setw(  8 ) << right << "act."
         << setw( 12 ) << right << "st u/m"
         << endl;
   }
   stream
      << dec
      << setw( 18 ) << left  << task_name
      << setw(  6 ) << right << dec << task_priority
      << setw(  4 ) << right << TASK_STATE( this )
      << setw(  8 ) << right << runtime_max
      << setw(  8 ) << right << activations
      << setw(  6 ) << right << mkt_stack_used( coroutine->stack() )
      << "/"
      << setw(  5 ) << right << mkt_stack_size( coroutine->stack() )
      << hex
      << endl;
#endif
}


//***************************************************************************
//
// event
//
//***************************************************************************

bool RTOS::event :: operator==( const event & rhs ) const {
   if( t != rhs.t ) {
      mkt_fatal (#UNIQUE_ERROR); // comparing incompatible waitables
   }
   return mask == rhs.mask;
}

bool RTOS::event :: operator!=( const event & rhs ) const {
   if( t != rhs.t ){
      mkt_fatal (#UNIQUE_ERROR); // comparing incompatible waitables
   }
   return mask != rhs.mask;
}

bool RTOS::event :: operator==( const waitable & rhs ) const {
   return *this == (event)rhs;
}

bool RTOS::event :: operator!=( const waitable & rhs ) const {
   return *this != (event)rhs;
}

RTOS::event RTOS::event :: operator+( const event & rhs ) const {
   if( t != rhs.t ){
      mkt_fatal (#UNIQUE_ERROR); // adding incompatible waitables
   }
   return event( t, mask | rhs.mask );
}

void RTOS::event::print( std::ostream & s ) const {
   s << "event t=" << t->name() << " m=0b" << std::bin << mask;
}


//***************************************************************************
//
// waitable
//
//***************************************************************************

RTOS::waitable :: waitable( task *t, const char *arg_name ) :
   event( t, 0 )
{
   RTOS_STATISTICS( waitable_name = string_allocate( arg_name ); )
   mask = t->waitables.waitable_allocate();
}

//***************************************************************************
//
// flag
//
//***************************************************************************

RTOS::flag::flag( task *t, const char *name ):
   waitable( t, name )
#if RTOS_STATISTICS_ENABLED
   ,n_sets( 0 ),
   n_gets( 0 )
#endif
{
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::flag::set( void ){
   RTOS_STATISTICS( n_sets++; )
   waitable::set();
}

void RTOS::flag::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "flag name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  3 ) << right << "fn";
      stream << setw(  8 ) << right << "sets";
      stream << setw(  8 ) << right << "gets";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << waitable_name;
   stream << setw ( 18 ) << left  << TASK_NAME( t );
   stream << setw(   3 ) << right << dec << nr_from_mask( mask );
   stream << setw(   8 ) << right << dec << n_sets;
   stream << setw(   8 ) << right << dec << n_gets;
   stream << "\n";
#endif
}


//***************************************************************************
//
// timer
//
//***************************************************************************

RTOS::timer::timer( task * t, const char *name ):
   waitable( t, name ),
   callback( name )
#if RTOS_STATISTICS_ENABLED
   ,n_sets( 0 )
   ,n_cancels( 0 )
#endif
{
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::timer::set( unsigned long long int time ){
   RTOS_STATISTICS( n_sets++; )
   RTOS::callback::start( time );
}

void RTOS::timer::cancel( void ){
   RTOS_STATISTICS( n_cancels++; )
   RTOS::callback::cancel();
   RTOS::waitable::clear();
}

void RTOS::timer::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "timer name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  3 ) << right << "fn";
      stream << setw(  8 ) << right << "sets";
      stream << setw(  8 ) << right << "cancels";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << waitable_name;
   stream << setw ( 18 ) << left  << TASK_NAME( t );
   stream << setw(   3 ) << right << dec << nr_from_mask( mask );
   stream << setw(   8 ) << right << dec << n_sets;
   stream << setw(   8 ) << right << dec << n_cancels;
   stream << "\n";
#endif
}


//***************************************************************************
//
// clock
//
//***************************************************************************

RTOS::clock::clock(
   task * t,
   unsigned long long int interval,
   const char *name
):
   waitable( t, name ),
   callback( name ),
   _interval( interval )
#if RTOS_STATISTICS_ENABLED
   ,ticks( 0 )
#endif
{
   callback::start( _interval );
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::clock::time_up( void ){
   RTOS_STATISTICS( ticks++; )
   callback::restart( _interval );
   waitable::set();
}

void RTOS::clock::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "clock name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  3 ) << right << "fn";
      stream << setw(  9 ) << right << "interval";
      stream << setw( 10 ) << right << "ticks";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << waitable_name;
   stream << setw ( 18 ) << left  << TASK_NAME( t );
   stream << setw(   3 ) << right << dec << nr_from_mask( mask );
   stream << setw(   9 ) << right << dec << _interval;
   stream << setw(  10 ) << right << dec << ticks;
   stream << "\n";
#endif
}


//***************************************************************************
//
// waitable_set
//
//***************************************************************************

unsigned int RTOS::waitable_set :: waitable_allocate( void ){
   if( used > 8 * sizeof( current_waitables )){
      mkt_fatal (#UNIQUE_ERROR); // max 32 waitables per group
   }
   return 1 << used++;
}

void RTOS::waitable_set::set ( const waitable &w ) {

   // set the waitable bit
   current_waitables |= w.mask;

   // the client will figure out whether he is runnable again
   if( requested_waitables != 0 ){
      w.t->unblock();
   }
}

void RTOS::waitable_set::clear( const waitable &w ){
   current_waitables &= ~ w.mask;
}

RTOS::event RTOS::waitable_set::wait ( unsigned int mask ) {

   for(;;){

      // try to find a waitable for which we are waiting
      for (unsigned int i = 0 ; i < used; i++) {
         if( current_waitables & mask & ( 1U << i )){

            // clear the waitable
            current_waitables &= ~(1U << i);

#if RTOS_STATISTICS_ENABLED
				// update statistics
   			for( flag * f = flags; f != NULL; f = f->next_flag ){
      			if (f->t == client && f->mask == (1U << i)) {
						f->n_gets++;
      				break;
					}
   			}
#endif
            // return an event for the waitable
            return event( client, 1U << i );
         }
      }

      // no waitable found? wait for better times..
      requested_waitables = mask;
      client->block();
      requested_waitables = NULL;
   }
}


/***************************************************************************\

mutex

\***************************************************************************/

RTOS::mutex::mutex( const char *name ):
   owner( NULL ),
   waiters( NULL )
#if RTOS_STATISTICS_ENABLED
   ,wait_count( NULL )
#endif
{
   RTOS_STATISTICS( mutex_name = string_allocate( name ); )
}

void RTOS::mutex::wait (void) {
   RTOS_STATISTICS( wait_count++; )
   if( owner == NULL ){
      owner = RTOS::current_task();
   }
	else {
      task * t = RTOS::current_task();
      task **p = &waiters;

      // get p to point to the last pointer
      while( *p != NULL ){
         p = & (*p)-> next_mutex_waiter;
      }

      // insert t after the last pointer
      *p = t;
      t->next_mutex_waiter = NULL;

      // we wait for better times...
      t->block();
   }
}

void RTOS::mutex::signal (void) {
   if( owner != RTOS::current_task()) {
      mkt_fatal (#UNIQUE_ERROR); // mutex signaled by another task
   }
	else {
      task *t = waiters;
      if ( t != NULL ) {
         // remove task t from the queue
         waiters = waiters->next_mutex_waiter;

         // t is now the owner of the mutex and can run again
         owner = t;
         t->unblock();
      }
		else
			owner = NULL;
   }
}

void RTOS::mutex::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "mutex name";
      stream << setw( 18 ) << left  << "owner";
      stream << setw(  8 ) << right << "waits";
      stream << "   waiters";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << mutex_name;
   if( owner == NULL ){
      stream << "-";
   } else {
      stream << owner->task_name;
   }
   stream << setw(   8 ) << right << dec << "waits";
   stream << "[ ";
   for( task *t = waiters; t!= NULL; t = t->next_mutex_waiter ){
      stream << t->task_name << " ";
   }
   stream << "]";
#endif
}

RTOS::mutex::~mutex( void ) {
   mkt_fatal (#UNIQUE_ERROR); // mutex destructor called
}


/***************************************************************************\

callback

\***************************************************************************/

RTOS::callback::callback( const char *name ) :
    time_to_wait (0)
{
   RTOS_STATISTICS( object_name = string_allocate( name ); )
   RTOS::add( this );
}


//***************************************************************************
//
// channel
//
//***************************************************************************

RTOS::channel_base::channel_base( task *t, const char *name ):
   waitable( t, name ),
#if RTOS_STATISTICS_ENABLED
   writes( 0 ),
   ignores( 0 ),
#endif
   qSize( 0 ),
   head( NULL ),
   tail ( NULL )
{
#if RTOS_STATISTICS_ENABLED
   channel_name = string_allocate( name );
   RTOS::add( this );
#endif
}

void RTOS::channel_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "channel name";
      stream << setw( 18 ) << left  << "owner";
      stream << setw(  3 ) << right << "fn";
      stream << setw(  8 ) << right << "writes";
      stream << setw(  8 ) << right << "ignores";
      stream << setw( 10 ) << right << "cur.size";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << channel_name;
   stream << setw ( 18 ) << left  << t->task_name;
   stream << setw(   3 ) << right << nr_from_mask( mask );
   stream << setw(   8 ) << right << dec << writes;
   stream << setw(   8 ) << right << dec << ignores;
   stream << setw(  10 ) << right << dec << qSize;
   stream << "\n";
#endif
}


//***************************************************************************
//
// pool
//
//***************************************************************************

RTOS::pool_base::pool_base( const char *name )
#if RTOS_STATISTICS_ENABLED
   : reads( 0 ),
   writes( 0 )
#endif
{
   RTOS_STATISTICS( pool_name = string_allocate( name ); )
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::pool_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "pool name";
      stream << setw(  8 ) << right << "writes";
      stream << setw(  8 ) << right << "reads";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << pool_name;
   stream << setw(   8 ) << right << dec << writes;
   stream << setw(   8 ) << right << dec << reads;
   stream << "\n";
#endif
}


//***************************************************************************
//
// mailbox
//
//***************************************************************************

RTOS::mailbox_base::mailbox_base( const char *name )
#if RTOS_STATISTICS_ENABLED
   : client( NULL ),
   writes( 0 )
#endif
{
   RTOS_STATISTICS( mailbox_name = string_allocate( name ); )
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::mailbox_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ){
      stream << setw( 18 ) << left  << "mailbox name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  8 ) << right << "writes";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << mailbox_name;
   stream << setw ( 18 ) << left  << TASK_NAME( client );
   stream << setw(   8 ) << right << dec << writes;
   stream << "\n";
#endif
}


//***************************************************************************
//
// << operators
//
//***************************************************************************

std::ostream & operator<< ( std::ostream & s, const RTOS::task & t ){
   t.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::flag & f ){
   f.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::event & e ){
   e.print( s );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::timer & t ){
   t.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::clock & c ){
   c.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::channel_base & c ){
   c.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::mutex & m ){
   m.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::mailbox_base & m ){
   m.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::pool_base & p ){
   p.print( s, false );
   return s;
}


/***************************************************************************\

RTOS

\***************************************************************************/

// Reference to the task currently executed:
RTOS::task * RTOS::rtos_current_task = NULL;

// timer list head
RTOS::callback * RTOS::timerList = NULL;

// list of tasks, highest prority first
RTOS::task * RTOS::taskList = NULL;

// amount of us elapsed since last run of beat
unsigned long long int run_time_high = 0;
unsigned int timer_last = 0;	
	
unsigned long long int RTOS::run_time( void ){
   unsigned int timer_now = T0TC;
   if( timer_now < timer_last ){
      run_time_high += 1ULL << 32;
   }
   timer_last = timer_now;
   return run_time_high + timer_now;   
}

// flag set to clear statistics
bool RTOS::must_clear = false;

   // not running yet
bool RTOS::scheduler_running = false;

const char * RTOS::task::name( void ) const {
#if RTOS_STATISTICS_ENABLED
   return task_name;
#else
   return "";
#endif
}

// adding various objects to the RTOS lists
#if RTOS_STATISTICS_ENABLED

RTOS::flag * RTOS::flags = NULL;
RTOS::timer * RTOS::timers = NULL;
RTOS::clock * RTOS::clocks = NULL;
RTOS::mutex * RTOS::mutexes = NULL;
RTOS::channel_base * RTOS::channels = NULL;
RTOS::mailbox_base * RTOS::mailboxes = NULL;
RTOS::pool_base * RTOS::pools = NULL;

void RTOS::add( flag * f ){
   f->next_flag = flags;
   flags = f;
}

void RTOS::add( timer * t ){
   t->next_timer = timers;
   timers = t;
}

void RTOS::add( clock * c ){
   c->next_clock = clocks;
   clocks = c;
}

void RTOS::add( mutex * m ){
   m->next_mutex = mutexes;
   mutexes = m;
}

void RTOS::add( channel_base * c ){
   c->next_channel = channels;
   channels = c;
}

void RTOS::add( mailbox_base * m ){
   m->next_mailbox = mailboxes;
   mailboxes = m;
}

void RTOS::add( pool_base *p ){
   p->next_pool = pools;
   pools = p;
}

#endif

void RTOS::print( ostream & stream ){

   // global info
   stream << "\n\n";
   stream << "mkt version  : " << mkt_version << "\n";
   stream << "RTOS version : " << RTOS_VERSION << "\n";
   stream << "RAM free     : " << dec << mkt_memory_free() << "\n";

#if RTOS_STATISTICS_ENABLED
   bool header;

   if( rtos_current_task != NULL ){
      rtos_current_task->ignore_this_activation = true;
   }

   // tasks
   header = true;
   for( task * t = taskList; t != NULL; t = t->nextTask ){
      t->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no tasks\n";
   }
   stream << "\n";

   // flags
   header = true;
   for( flag * f = flags; f !=NULL; f = f->next_flag ){
      f->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no flags\n";
   }
   stream << "\n";

   // timers
   header = true;
   for( timer * t = timers; t != NULL; t = t->next_timer ){
      t->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no timers\n";
   }
   stream << "\n";

   // clocks
   header = true;
   for( clock * c = clocks; c != NULL; c = c->next_clock ){
      c->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no clocks\n";
   }
   stream << "\n";

   // channels
   header = true;
   for( channel_base * c = channels; c != NULL; c = c->next_channel ){
      c->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no channels\n";
   }
   stream << "\n";

   // mutexes
   header = true;
   for( mutex * m = mutexes; m != NULL; m = m->next_mutex ){
      m->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no mutexes\n";
   }
   stream << "\n";

   // mailboxes
   header = true;
   for( mailbox_base * m = mailboxes; m != NULL; m = m->next_mailbox ){
      m->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no mailboxes\n";
   }
   stream << "\n";

   // pools
   header = true;
   for( pool_base * p = pools; p != NULL; p = p->next_pool ){
      p->print( stream, header );
      header = false;
   }
   if( header ){
      stream << "no pools\n";
   }

#endif
}

unsigned long long int last_run_time = 0;

void RTOS::beat (void) {

   // get the elapse time since last beat, and reset it to 0
   unsigned long long int new_run_time = RTOS::run_time();
   unsigned int elapsed = new_run_time - last_run_time;
   last_run_time = new_run_time;

   if (elapsed > 0) {
      // service the callback timer queue
      for (
         callback * t = timerList;
         t != NULL;
         t = t->nextTimer
      ){
         if(0){
            trace
               RTOS_STATISTICS( << t->object_name )
               << "@" << std::hex << (int)t
               << " ttw=" << (int) t->time_to_wait;
         }
         if( t->time_to_wait > 0 ) {
            t->time_to_wait -= elapsed;
            if( t->time_to_wait <= 0 ){
               t->time_up();
            }
         }
      }
   }

   // find the highest-priority ready task and run it, then return
   // rtos_current_task is a class attribute, used by current_task
   for (
      rtos_current_task = taskList;
      rtos_current_task != NULL;
      rtos_current_task = rtos_current_task->nextTask
   ){
      if (rtos_current_task->is_ready()) {
         unsigned long long int start = RTOS::run_time();
         if(0){
            trace
               << "resume " << rtos_current_task->name()
               << " prio="  << (int) rtos_current_task->priority()
               << "\n";
         }
         rtos_current_task->coroutine->resume();
         if(0){
            trace << "back from " << rtos_current_task->name() << "\n";
         }
         unsigned long long int end = RTOS::run_time();
         int runtime = end - start;
         if( ! rtos_current_task->ignore_this_activation ){
            if (runtime > rtos_current_task->runtime_max) {
               rtos_current_task->runtime_max = runtime;
            }
         }

         if(0) if( runtime > 10 MS ){
            trace
               << rtos_current_task->name()
               << " runtime=" << dec << runtime ;
         }

         rtos_current_task->ignore_this_activation = false;
         rtos_current_task->activations++;
         if (must_clear) {
            if(0) do_statistics_clear();
            must_clear = false;
         }

         return;
      }
   }

#if RTOS_STATISTICS_ENABLED

   // no runnable task has been found, nothing to do right now
   // we might as well do deadlock detection
   for( clock *c = clocks; c != NULL; c = c->next_clock ){
      if( c->t->waitables.requested_waitables & c->mask ){
         return;
      }
   }
   for( timer *t = timers; t != NULL; t = t->next_timer ){
      if(
         ( t->time_to_wait > 0 )
         && ( t->t->waitables.requested_waitables & t->mask )
      ){
         return;
      }
   }

   // no task is waiting for a running timer or clock: DEADLOCK
   cout << "\n\n********** DEADLOCK DETECTED **********\n\n";
   print( cout );
   cout << "\n\n********** DEADLOCK DETECTED **********\n\n";

   mkt_fatal (#UNIQUE_ERROR); // deadlock detected

#endif
}

void RTOS::run (void) {

   if (global_logging) {
      trace << "start the timer";
   }
   mkt_timer_free_running( T0, 1 );

   scheduler_running = true;
   rtos_current_task = NULL;
   do_statistics_clear();
   print( std::cout );

   if (global_logging) {
      trace << "scheduler starts";
   }

   int n = 0;
   for(;;){
      if( hartbeat_logging ){
         if ( ++n > 10000 ){
            std::cout << '.';
            n = 0;
         }
      }
      beat();
   }
}

// register a timer
void RTOS::add( callback * t ){

   // add the timer to the timer list
   t->nextTimer = timerList;
   timerList = t;
}

// register a task
void RTOS::add( task * new_task ){
   if(0) if (global_logging) {
      trace << "register task " << new_task->name();
   }

   if( new_task->task_priority > RTOS_MAX_PRIORITY ){
      mkt_fatal (#UNIQUE_ERROR); // illegal task priority
   }

   // walk the task queue untill the next task either
   // - does not exist, or
   // - has a lower priority (higher priority number) than the new task
   task ** t = &taskList;

   if(0){
      trace
         << "task " << new_task->name()
         << " priority=" << new_task->task_priority;
   }
   while( ( *t != NULL ) && ( (*t)->task_priority <= new_task->task_priority ) ) {
      if( (*t)->task_priority == new_task->task_priority ){
         if( new_task->task_priority >= RTOS_MAX_PRIORITY ){
            new_task->task_priority++;
         } else {
            mkt_fatal (#UNIQUE_ERROR); // duplicate task priority
         }
      }
      t = &( (*t)->nextTask );
   }
   // now insert the new task after the current task
   new_task->nextTask = *t;
   *t = new_task;

   if(0) if (global_logging) {
      trace << "registering done ";
   }
}

void RTOS::do_statistics_clear (void) {
   for (
      task * task = taskList;
      task != NULL;
      task = task->nextTask
   )
   {  task->statistics_clear();
   }
}

void RTOS::display_statistics( void ) {
   std::cout << std::endl;
   print (std::cout);
   statistics_clear();
}
