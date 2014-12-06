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
ke_button_t keys[256];
ke_mouse_t  mouse;

/*uint8_t rdtsc[] = { 0x0F, 0x31, 0xC3 };
uint32_t (*ke_rdtsc)(void) = (uint32_t(*)(void)) rdtsc;*/

void (*ke_pfn_initialize)( void* ) = NULL;
void (*ke_pfn_uninitialize)( void* ) = NULL;
void (*ke_pfn_mouse)( void*, void* ) = NULL;
void (*ke_pfn_keyboard)( void*, void*  ) = NULL;
void (*ke_pfn_gamepad)( void*, void* ) = NULL;
void* ke_context_ptr = NULL;


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


/* Input functions */
void ke_update_keys();
void ke_process_key_event( SDL_Event* event );
void ke_process_mouse_event( SDL_Event* event );

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
            case SDL_KEYUP:
                ke_process_key_event( &event );
                ke_on_keyboard( ke_get_context_pointer(), &event );
                break;
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                ke_process_mouse_event( &event );
                ke_on_mouse( ke_get_context_pointer(), &event );
                break;
                
            case SDL_MOUSEMOTION:
                ke_process_mouse_event( &event );
                break;
                
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                ke_on_gamepad( ke_get_context_pointer(), &event );
                break;
        }
    }
    
    /* Update keys */
    ke_update_keys();
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
 * Name: ke_set_context_pointer
 * Desc: 
 */
void ke_set_context_pointer( void* context_pointer )
{
    ke_context_ptr = context_pointer;
}

/*
 * Name: ke_get_context_pointer
 * Desc:
 */
void* ke_get_context_pointer()
{
    return ke_context_ptr;
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
void ke_set_keyboard_callback(void (*callback)(void*, void*))
{
    ke_pfn_keyboard = callback;
}

/*
 * Name: ke_set_mouse_callback
 * Desc:
 */
void ke_set_mouse_callback(void (*callback)(void*, void*))
{
    ke_pfn_mouse = callback;
}

/*
 * Name: ke_set_gamepad_callback
 * Desc: 
 */
void ke_set_gamepad_callback(void (*callback)(void*, void*))
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
void ke_on_keyboard( void* context, void* input_context )
{
    if( ke_pfn_keyboard )
        ke_pfn_keyboard( context, input_context );
}

/*
 * Name: ke_on_mouse
 * Desc:
 */
void ke_on_mouse( void* context, void* input_context )
{
    if( ke_pfn_mouse )
        ke_pfn_mouse( context, input_context );
}

/*
 * Name: ke_on_gamepad
 * Desc:
 */
void ke_on_gamepad( void* context, void* input_context )
{
    if( ke_pfn_gamepad )
        ke_pfn_gamepad( context, input_context );
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

/*
 * Name: ke_reset_keys
 * Desc: Resets the keyboard state structure.
 */
void ke_reset_keys()
{
    ZeroMemory( keys, sizeof( ke_button_t ) * 256 );
}

/* 
 * Name: ke_reset_mouse
 * Desc: Resets the mouse state structure
 */
void ke_reset_mouse()
{
    ZeroMemory( &mouse, sizeof( ke_mouse_t ) );
}

/*
 * Name: ke_get_key_state
 * Desc: Get the state of the keyboard.
 */
void ke_get_key_state( ke_button_t* _keys )
{
    memmove( _keys, keys, sizeof( ke_button_t ) * 256 );
}

/*
 * Name: ke_got_any_key
 * Desc: Return true if any keyboard keys were pressed.
 */
bool ke_got_any_key()
{
    int i = 0;
    while( i < 256 )
    {
        if( keys[i].pressed )
            return true;
    }
    
    return false;
}

/*
 * Name: ke_update_keys
 * Desc: Updates the timestamp values for keyboard keys
 */
void ke_update_keys()
{
    for( int i = 0; i < 256; i++ )
    {
        if( keys[i].pressed )
        {
            keys[i].timestamp.frames++;
            keys[i].timestamp.elapsed_time = float(ke_get_tick_count()) - keys[i].timestamp.start_time;
        }
    }
}

/* 
 * Name: ke_get_mouse_state
 * Desc: Gets the state of the mouse.
 */
void ke_get_mouse_state( ke_mouse_t* _mouse )
{
    memmove( _mouse, &mouse, sizeof( ke_mouse_t ) );
}

/*
 * Name: ke_process_key_event
 * Desc: Processes key events.
 */
void ke_process_key_event( SDL_Event* event )
{
    /* Sanity check */
    if( !event )
        return;
    
    /* Determine what key was pressed or released */
    //keys[event->key.keysym.sym].pressed = event->key.state = SDL_PRESSED ? true : false;
    
    /* Reset timestamps if this is the first key press */
    if( !keys[event->key.keysym.sym].pressed && event->key.state == SDL_PRESSED )
    {
        keys[event->key.keysym.sym].timestamp.frames = 0;
        keys[event->key.keysym.sym].timestamp.start_time = float(ke_get_tick_count());
        keys[event->key.keysym.sym].timestamp.elapsed_time = 0;
        keys[event->key.keysym.sym].pressed = true;
    }
    
    /* Mark key as released when it's up */
    if( event->key.state == SDL_RELEASED )
    {
        keys[event->key.keysym.sym].pressed = false;
    }
}

/*
 * Name: ke_process_mouse_event
 * Desc: 
 */
void ke_process_mouse_event( SDL_Event* event )
{
    /* TODO: Figure out why the heck this mofo code isn't working when SDL's documentation tells me this is right */
    
    int button = 0;
    
    if( event->type == SDL_MOUSEMOTION )
    {
        mouse.x = event->motion.x;
        mouse.y = event->motion.y;
    }
    
    if( event->type == SDL_MOUSEBUTTONDOWN )
    {
        if( event->button.button == SDL_BUTTON_LEFT ) button = 0;
        if( event->button.button == SDL_BUTTON_RIGHT ) button = 1;
        if( event->button.button == SDL_BUTTON_MIDDLE ) button = 2;
        
        mouse.button[button] = Yes;
    }
    if( event->type == SDL_MOUSEBUTTONUP )
    {
        if( event->button.button == SDL_BUTTON_LEFT ) button = 0;
        if( event->button.button == SDL_BUTTON_RIGHT ) button = 1;
        if( event->button.button == SDL_BUTTON_MIDDLE ) button = 2;
        
        mouse.button[button] = No;
    }
}

void ke_message_box( const char* message, const char* title, uint32_t flags )
{
	uint32_t sdl_flags;

	/* Interpret message type */
	switch( flags )
	{
	case KE_ERROR:		sdl_flags = SDL_MESSAGEBOX_ERROR; break;
	case KE_WARNING:	sdl_flags = SDL_MESSAGEBOX_WARNING; break;
	default:			sdl_flags = SDL_MESSAGEBOX_INFORMATION; break;
	}

	/* Show message box with title and text */
	SDL_ShowSimpleMessageBox( sdl_flags, title, message, SDL_GL_GetCurrentWindow() );
}

void ke_display_last_message( const char* title, uint32_t flags )
{
	std::string str = GETLASTMSG();

	ke_message_box( str.c_str(), title, flags );
}
