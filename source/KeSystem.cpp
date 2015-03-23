//
//  ke_system.cpp
//
//  Created by Shogun3D on 6/2/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeSystem.h"
#include "nvdebug.h"
#include <thread>

std::thread th;

/*
 * Globals
 */
SDL_Event   event;
KeButton keys[256];
KeMouse  mouse;

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
 * Name: KeProcessEvents
 * Desc: Handles system events during the application's lifetime.
 * TODO: Setup callbacks for each event type.
 */
void KeProcessEvents()
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
                KeOnKeyboard( KeGetContextPointer(), &event );
                break;
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                ke_process_mouse_event( &event );
                KeOnMouse( KeGetContextPointer(), &event );
                break;
                
            case SDL_MOUSEMOTION:
                ke_process_mouse_event( &event );
                break;
                
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                KeOnGamepad( KeGetContextPointer(), &event );
                break;
        }
    }
    
    /* Update keys */
    ke_update_keys();
}

/*
 * Name: KeQuitRequested
 * Desc: Returns true if there is a quit request.
 */
bool KeQuitRequested()
{
    /* Was a quit event processed? */
    return ( event.type == SDL_QUIT );
}

/*
 * Name: KeSetContextPointer
 * Desc: 
 */
void KeSetContextPointer( void* context_pointer )
{
    ke_context_ptr = context_pointer;
}

/*
 * Name: KeGetContextPointer
 * Desc:
 */
void* KeGetContextPointer()
{
    return ke_context_ptr;
}

/*
 * Name: KeSetInitializeCallback
 * Desc:
 */
void KeSetInitializeCallback(void (*callback)(void*))
{
    ke_pfn_initialize = callback;
}

/*
 * Name: KeSetUnitializeCallback
 * Desc:
 */
void KeSetUnitializeCallback(void (*callback)(void*))
{
    ke_pfn_uninitialize = callback;
}

/*
 * Name: KeSetKeyboardCallback
 * Desc: 
 */
void KeSetKeyboardCallback(void (*callback)(void*, void*))
{
    ke_pfn_keyboard = callback;
}

/*
 * Name: KeSetMouseCallBack
 * Desc:
 */
void KeSetMouseCallBack(void (*callback)(void*, void*))
{
    ke_pfn_mouse = callback;
}

/*
 * Name: KeSetGamepadCallback
 * Desc: 
 */
void KeSetGamepadCallback(void (*callback)(void*, void*))
{
    ke_pfn_gamepad = callback;
}

/*
 * Name: KeOnInitialize
 * Desc: Called when the app is being initialized.
 */
void KeOnInitialize( void* context )
{
    if( ke_pfn_initialize )
        ke_pfn_initialize( context );
}

/*
 * Name: KeOnUnintialize
 * Desc: Called when this app is uninitialized.
 */
void KeOnUnintialize( void* context )
{
    if( ke_pfn_uninitialize )
        ke_pfn_uninitialize( context );
}

/*
 * Name: KeOnKeyboard
 * Desc:
 */
void KeOnKeyboard( void* context, void* input_context )
{
    if( ke_pfn_keyboard )
        ke_pfn_keyboard( context, input_context );
}

/*
 * Name: KeOnMouse
 * Desc:
 */
void KeOnMouse( void* context, void* input_context )
{
    if( ke_pfn_mouse )
        ke_pfn_mouse( context, input_context );
}

/*
 * Name: KeOnGamepad
 * Desc:
 */
void KeOnGamepad( void* context, void* input_context )
{
    if( ke_pfn_gamepad )
        ke_pfn_gamepad( context, input_context );
}

/*
 * Name: KeDelay
 * Desc: Invokes a delay in the number of milliseconds
 */
void KeDelay( uint32_t milliseconds )
{
    SDL_Delay( milliseconds );
}

/*
 * Name: KeGetPerformanceCounter
 * Desc: 
 */
uint64_t KeGetPerformanceCounter()
{
    return SDL_GetPerformanceCounter();
}

/*
 * Name: KeGetPerformanceFrequency
 * Desc:
 */
uint64_t KeGetPerformanceFrequency()
{
    return SDL_GetPerformanceFrequency();
}

/*
 * Name: KeGetTickCount
 * Desc: Returns the amount of time that has passed (in milliseconds) since this engine
 *       has started.
 */
uint32_t KeGetTickCount()
{
    return SDL_GetTicks();
}



/*
 * Name: KeResetKeys
 * Desc: Resets the keyboard state structure.
 */
void KeResetKeys()
{
    ZeroMemory( keys, sizeof( KeButton ) * 256 );
}

/* 
 * Name: ke_reset_mouse
 * Desc: Resets the mouse state structure
 */
void ke_reset_mouse()
{
    ZeroMemory( &mouse, sizeof( KeMouse ) );
}

/*
 * Name: KeGetKeyState
 * Desc: Get the state of the keyboard.
 */
void KeGetKeyState( KeButton* _keys )
{
    memmove( _keys, keys, sizeof( KeButton ) * 256 );
}

/*
 * Name: KeyGotAnyKey
 * Desc: Return true if any keyboard keys were pressed.
 */
bool KeyGotAnyKey()
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
            keys[i].timestamp.elapsed_time = float(KeGetTickCount()) - keys[i].timestamp.start_time;
        }
    }
}

/* 
 * Name: KeGetMouseState
 * Desc: Gets the state of the mouse.
 */
void KeGetMouseState( KeMouse* _mouse )
{
    memmove( _mouse, &mouse, sizeof( KeMouse ) );
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
        keys[event->key.keysym.sym].timestamp.start_time = float(KeGetTickCount());
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

void KeMessageBox( const char* message, const char* title, uint32_t flags )
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

void KeDisplayLastMessage( const char* title, uint32_t flags )
{
	std::string str = GETLASTMSG();

	KeMessageBox( str.c_str(), title, flags );
}
