//
//  ke_system.cpp
//
//  Created by Shogun3D on 6/2/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke.h"
#include "ke_system.h"
#include "nvdebug.h"


/*
 * Globals
 */
SDL_Event   event;
uint8_t rdtsc[] = { 0x0F, 0x31, 0xC3 };
uint32_t (*ke_rdtsc)(void) = (uint32_t(*)(void)) rdtsc;

void (*ke_pfn_initialize)( void* );
void (*ke_pfn_uninitialize)( void* );
void (*ke_pfn_mouse)( void*, int );
void (*ke_pfn_keyboard)( void*, int  );
void (*ke_pfn_gamepad)( void*, int );


/* Thread structure */
struct ke_thread_t
{
    SDL_Thread*     thread;
    SDL_threadID    thread_id;
};

/* Mutex structure */
struct ke_mutex_t
{
    SDL_mutex*      mutex;
};

/* Semaphore structure */
struct ke_semaphore_t
{
    SDL_semaphore*  semaphore;
};

/* Critical section structure*/
struct ke_critical_section_t
{
    pthread_mutex_t mutex;
};



/*
 * Name: ke_process_events
 * Desc: Handles system events during the application's lifetime.
 * TODO: Setup callbacks for each event type.
 */
void ke_process_events()
{
    //ke_rdtsc();
    
    /* Check for any SDL supported events */
    while( SDL_PollEvent( &event ) )
    {
        /* Respond to events accordingly */
        switch( event.type )
        {
            case SDL_QUIT:
                break;
                
            case SDL_KEYDOWN:
                ke_on_keyboard( &event, Yes );
                break;
                
            case SDL_KEYUP:
                ke_on_keyboard( &event, No );
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                ke_on_mouse( &event, Yes );
                break;
                
            case SDL_MOUSEBUTTONUP:
                ke_on_mouse( &event, No );
                break;
                
            case SDL_CONTROLLERBUTTONDOWN:
                ke_on_gamepad( &event, Yes );
                break;
                
            case SDL_CONTROLLERBUTTONUP:
                ke_on_gamepad( &event, No );
                break;
        }
    }
}

/*
 * Name: ke_quit_requested
 * Desc: Returns true if there is a quit request.
 */
bool ke_quit_requested()
{
    /* Was a quit event processed? */
    return ( event.type == SDL_QUIT );
}

/*
 * Name: ke_set_initialize_callback
 * Desc:
 */
void ke_set_initialize_callback(void (*callback)(void*))
{
    ke_pfn_initialize = callback;
}

/*
 * Name: ke_set_uninitialize_callback
 * Desc:
 */
void ke_set_uninitialize_callback(void (*callback)(void*))
{
    ke_pfn_uninitialize = callback;
}

/*
 * Name: ke_set_keyboard_callback
 * Desc: 
 */
void ke_set_keyboard_callback(void (*callback)(void*, int))
{
    ke_pfn_keyboard = callback;
}

/*
 * Name: ke_set_mouse_callback
 * Desc:
 */
void ke_set_mouse_callback(void (*callback)(void*, int))
{
    ke_pfn_mouse = callback;
}

/*
 * Name: ke_set_gamepad_callback
 * Desc: 
 */
void ke_set_gamepad_callback(void (*callback)(void*, int))
{
    ke_pfn_gamepad = callback;
}

/*
 * Name: ke_on_initialize
 * Desc: Called when the app is being initialized.
 */
void ke_on_initialize( void* context )
{
    if( ke_pfn_initialize )
        ke_pfn_initialize( context );
}

/*
 * Name: ke_on_uninitialize
 * Desc: Called when this app is uninitialized.
 */
void ke_on_uninitialize( void* context )
{
    if( ke_pfn_uninitialize )
        ke_pfn_uninitialize( context );
}

/*
 * Name: ke_on_keyboard
 * Desc:
 */
void ke_on_keyboard( void* context, int down )
{
    if( ke_pfn_keyboard )
        ke_pfn_keyboard( context, down );
}

/*
 * Name: ke_on_mouse
 * Desc:
 */
void ke_on_mouse( void* context, int down )
{
    if( ke_pfn_mouse )
        ke_pfn_mouse( context, down );
}

/*
 * Name: ke_on_gamepad
 * Desc:
 */
void ke_on_gamepad( void* context, int down )
{
    if( ke_pfn_gamepad )
        ke_pfn_gamepad( context, down );
}

/*
 * Name: ke_delay
 * Desc: Invokes a delay in the number of milliseconds
 */
void ke_delay( uint32_t milliseconds )
{
    SDL_Delay( milliseconds );
}

/*
 * Name: ke_get_performance_counter
 * Desc: 
 */
uint64_t ke_get_performance_counter()
{
    return SDL_GetPerformanceCounter();
}

/*
 * Name: ke_get_performance_frequency
 * Desc:
 */
uint64_t ke_get_performance_frequency()
{
    return SDL_GetPerformanceFrequency();
}

/*
 * Name: ke_get_tick_count
 * Desc: Returns the amount of time that has passed (in milliseconds) since this engine
 *       has started.
 */
uint32_t ke_get_tick_count()
{
    return SDL_GetTicks();
}

/*
 * Name: ke_initialize_critical_section
 * Desc: Initializes a critical section object using POSIX. This is the most portable method
 *       I could find.
 */
bool ke_initialize_critical_section( struct ke_critical_section_t** critical_section )
{
    /* Initialize pthread critical section */
    (*critical_section) = new ke_critical_section_t;
    (*critical_section)->mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
    
    return true;
}

/*
 * Name: ke_uninitialize_critical_section
 * Desc: Doesn't really do much, just deletes the object.
 */
void ke_uninitialize_critical_section( struct ke_critical_section_t* critical_section )
{
    /* Delete this critical section object */
    delete critical_section;
}

/*
 * Name: ke_enter_critical_section
 * Desc: Simply locks the mutex representing the critical section.
 */
void ke_enter_critical_section( struct ke_critical_section_t* critical_section )
{
    /* Lock this critical section, and lock other threads out. */
    pthread_mutex_lock( &critical_section->mutex );
}

/*
 * Name: ke_leave_critical_section
 * Desc: Unlocks the mutex representing the critical section.
 */
void ke_leave_critical_section( struct ke_critical_section_t* critical_section )
{
    /* Unlock this critical section */
    pthread_mutex_unlock( &critical_section->mutex );
}

/*
 * Name: ke_try_enter_critical_section
 * Desc: Attempts to lock the mutex representing the critical section, and returns the result.
 */
bool ke_try_enter_critical_section( struct ke_critical_section_t* critical_section )
{
    /* Attempt to lock this critical section. Return the result. */
    return (bool) pthread_mutex_trylock( &critical_section->mutex );
}