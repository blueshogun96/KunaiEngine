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
#include <vector>


/*
 * Display information structure
 */
struct KeDisplayInfo
{
    int display_number;
    char display_name[256];
    std::vector<KeDisplayMode> display_modes;
};

/*
 * Globals
 */
SDL_Event   event;
KeButton    keys[256];
KeMouse     mouse;
int         display_count = 1;
std::vector<KeDisplayInfo> displays;
int			quitting = No;

/*uint8_t rdtsc[] = { 0x0F, 0x31, 0xC3 };
uint32_t (*ke_rdtsc)(void) = (uint32_t(*)(void)) rdtsc;*/

void (*pfnKeInitialize)( void* ) = NULL;
void (*pfnKeUninitialize)( void* ) = NULL;
void (*pfnKeMouse)( void*, void* ) = NULL;
void (*pfnKeKeyboard)( void*, void* ) = NULL;
void (*pfnKeGamepad)( void*, void* ) = NULL;
void (*pfnKeAppTerminating)( void* ) = NULL;
void (*pfnKeAppLowMemory)( void* ) = NULL;
void (*pfnKeAppWillEnterBackground)( void* ) = NULL;
void (*pfnKeAppDidEnterBackground)( void* ) = NULL;
void (*pfnKeAppWillEnterForeground)( void* ) = NULL;
void (*pfnKeAppDidEnterForeground)( void* ) = NULL;
void* pKeContext = NULL;


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
void KeUpdateKeys();
void KeProcessKeyEvent( SDL_Event* event );
void KeProcessMouseEvent( SDL_Event* event );

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
				quitting = Yes;
                break;
                
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                KeProcessKeyEvent( &event );
                KeOnKeyboard( KeGetContextPointer(), &event );
                break;
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                KeProcessMouseEvent( &event );
                KeOnMouse( KeGetContextPointer(), &event );
                break;
                
            case SDL_MOUSEMOTION:
                KeProcessMouseEvent( &event );
                break;
                
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                KeOnGamepad( KeGetContextPointer(), &event );
                break;
        }
    }
    
    /* Update keys */
    KeUpdateKeys();
}


/*
 * Name: KeProcessAppEvents
 * Desc: Processes special events for mobile apps.
 */
int KeProcessAppEvents( void *userdata, SDL_Event *event )
{
#ifdef __MOBILE_OS__
    switch( event->type )
    {
        case SDL_APP_TERMINATING:
            /* Terminate the app.
             Shut everything down before returning from this function.
             */
            if( pfnKeAppTerminating )
                pfnKeAppTerminating( userdata );
            return 0;
        case SDL_APP_LOWMEMORY:
            /* You will get this when your app is paused and iOS wants more memory.
             Release as much memory as possible.
             */
            if( pfnKeAppLowMemory )
                pfnKeAppLowMemory( userdata );
            return 0;
        case SDL_APP_WILLENTERBACKGROUND:
            /* Prepare your app to go into the background.  Stop loops, etc.
             This gets called when the user hits the home button, or gets a call.
             */
            if( pfnKeAppWillEnterBackground )
                pfnKeAppWillEnterBackground( userdata );
            return 0;
        case SDL_APP_DIDENTERBACKGROUND:
            /* This will get called if the user accepted whatever sent your app to the background.
             If the user got a phone call and canceled it, you'll instead get an    SDL_APP_DIDENTERFOREGROUND event and restart your loops.
             When you get this, you have 5 seconds to save all your state or the app will be terminated.
             Your app is NOT active at this point.
             */
            if( pfnKeAppDidEnterBackground )
                pfnKeAppDidEnterBackground( userdata );
            return 0;
        case SDL_APP_WILLENTERFOREGROUND:
            /* This call happens when your app is coming back to the foreground.
             Restore all your state here.
             */
            if( pfnKeAppWillEnterForeground )
                pfnKeAppWillEnterForeground( userdata );
            return 0;
        case SDL_APP_DIDENTERFOREGROUND:
            /* Restart your loops here.
             Your app is interactive and getting CPU again.
             */
            if( pfnKeAppDidEnterForeground )
                pfnKeAppDidEnterForeground( userdata );
            return 0;
        default:
            /* No special processing, add it to the event queue */
            return 1;
    }
#else
    return 0;
#endif
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
    pKeContext = context_pointer;
}

/*
 * Name: KeGetContextPointer
 * Desc:
 */
void* KeGetContextPointer()
{
    return pKeContext;
}

/*
 * Name: KeSetInitializeCallback
 * Desc:
 */
void KeSetInitializeCallback(void (*callback)(void*))
{
    pfnKeInitialize = callback;
}

/*
 * Name: KeSetUnitializeCallback
 * Desc:
 */
void KeSetUnitializeCallback(void (*callback)(void*))
{
    pfnKeUninitialize = callback;
}

/*
 * Name: KeSetKeyboardCallback
 * Desc: 
 */
void KeSetKeyboardCallback(void (*callback)(void*, void*))
{
    pfnKeKeyboard = callback;
}

/*
 * Name: KeSetMouseCallBack
 * Desc:
 */
void KeSetMouseCallBack(void (*callback)(void*, void*))
{
    pfnKeMouse = callback;
}

/*
 * Name: KeSetGamepadCallback
 * Desc: 
 */
void KeSetGamepadCallback(void (*callback)(void*, void*))
{
    pfnKeGamepad = callback;
}

/*
 * Name: KeSetAppTerminateCallback
 * Desc:
 */
void KeSetAppTerminateCallback( void (*callback)(void*) )
{
    pfnKeAppTerminating = callback;
}

/*
 * Name: KeSetAppLowMemoryCallback
 * Desc:
 */
void KeSetAppLowMemoryCallback( void (*callback)(void*) )
{
    pfnKeAppLowMemory = callback;
}

/*
 * Name: KeSetAppWillEnterBackgroundCallback
 * Desc:
 */
void KeSetAppWillEnterBackgroundCallback( void (*callback)(void*) )
{
    pfnKeAppWillEnterBackground = callback;
}

/*
 * Name: KeSetAppDidEnterBackgroundCallback
 * Desc:
 */
void KeSetAppDidEnterBackgroundCallback( void (*callback)(void*) )
{
    pfnKeAppDidEnterBackground = callback;
}

/*
 * Name: KeSetAppWillEnterForegroundCallback
 * Desc:
 */
void KeSetAppWillEnterForegroundCallback( void (*callback)(void*) )
{
    pfnKeAppWillEnterForeground = callback;
}

/*
 * Name: KeSetAppDidEnterForegroundCallback
 * Desc:
 */
void KeSetAppDidEnterForegroundCallback( void (*callback)(void*) )
{
    pfnKeAppDidEnterForeground = callback;
}

/*
 * Name: KeOnInitialize
 * Desc: Called when the app is being initialized.
 */
void KeOnInitialize( void* context )
{
    if( pfnKeInitialize )
        pfnKeInitialize( context );
}

/*
 * Name: KeOnUnintialize
 * Desc: Called when this app is uninitialized.
 */
void KeOnUnintialize( void* context )
{
    if( pfnKeUninitialize )
        pfnKeUninitialize( context );
}

/*
 * Name: KeOnKeyboard
 * Desc:
 */
void KeOnKeyboard( void* context, void* input_context )
{
    if( pfnKeKeyboard )
        pfnKeKeyboard( context, input_context );
}

/*
 * Name: KeOnMouse
 * Desc:
 */
void KeOnMouse( void* context, void* input_context )
{
    if( pfnKeMouse )
        pfnKeMouse( context, input_context );
}

/*
 * Name: KeOnGamepad
 * Desc:
 */
void KeOnGamepad( void* context, void* input_context )
{
    if( pfnKeGamepad )
        pfnKeGamepad( context, input_context );
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
 * Name: KeResetMouse
 * Desc: Resets the mouse state structure
 */
void KeResetMouse()
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
 * Name: KeUpdateKeys
 * Desc: Updates the timestamp values for keyboard keys
 */
void KeUpdateKeys()
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
 * Name: KeProcessKeyEvent
 * Desc: Processes key events.
 */
void KeProcessKeyEvent( SDL_Event* event )
{
    /* Sanity check */
    if( !event )
        return;
    
    if( event->key.keysym.sym > 255 )
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
 * Name: KeProcessMouseEvent
 * Desc: 
 */
void KeProcessMouseEvent( SDL_Event* event )
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

/*
 * Name: KeGatherAllDisplayInformation
 * Desc: Gathers information about this user's available displays, such as
 *       the number of monitors, and the resolutions supported for each.
 *
 * NOTE: This is an internal function that should only be called ONCE in
 *       KeInitialize.  The user should never have to access this function
 *       directly!
 */
int KeGatherAllDisplayInformation()
{
    /* Initialize the video subsystem for a minute */
    int res = SDL_InitSubSystem( SDL_INIT_VIDEO );
    if( res == -1 )
        return No;
    
    /* Get the number of displays available for this platform */
    display_count = SDL_GetNumVideoDisplays();
    
    /* Gather display information for each display supported by SDL */
    for( int i = 0; i < display_count; i++ )
    {
        KeDisplayInfo disp_info;
        strcpy( disp_info.display_name, SDL_GetDisplayName(i) );
        disp_info.display_number = i;
        
        /* Gather the supported resolutions for this display mode */
        for( int j = 0; j < SDL_GetNumDisplayModes(i); j++ )
        {
            KeDisplayMode disp_mode;
            SDL_DisplayMode disp;
            
            SDL_GetDisplayMode( i, j, &disp );
            disp_mode.width = disp.w;
            disp_mode.height = disp.h;
            disp_mode.pixel_format = disp.format;
            disp_mode.landscape = ( disp.w > disp.h );
            
            /* Save this display mode */
            disp_info.display_modes.push_back( disp_mode );
        }
        
        /* Save the display info */
        displays.push_back( disp_info );
    }
    
    /* Close the video subsystem */
    SDL_QuitSubSystem( SDL_INIT_VIDEO );
    
    return Yes;
}

/*
 * Name: KeDisplayCount
 * Desc: Returns the number of display adapters (i.e. monitors) this machine supports.
 */
int KeGetDisplayCount()
{
    return display_count;
}

/*
 * Name: KeGetDisplayModeCount
 * Desc: Returns the number of display modes (resolutions) available on this display adapter.
 */
int KeGetDisplayModeCount( int display )
{
    return displays[display].display_modes.size();
}

/*
 * Name: KeGetDisplayModes
 * Desc: Returns a list of display modes available on the specified display adapter number.
 */
void KeGetDisplayModes( int display, KeDisplayMode* modes )
{
    memcpy( modes, displays[display].display_modes.data(), sizeof( KeDisplayMode ) * displays[display].display_modes.size() );
    
    /*for( int i = 0; i < SDL_GetNumDisplayModes( display ); i++ )
    {
        SDL_DisplayMode disp;
        
        SDL_GetDisplayMode( display, i, &disp );
        modes[i].width = disp.w;
        modes[i].height = disp.h;
        modes[i].pixel_format = disp.format;
        modes[i].landscape = disp.w > disp.h ? Yes : No;
    }*/
}

/*
 * Name: KeGetDisplayName
 * Desc: Returns the name of this display adapter, assuming it even has one...
 */
const char* KeGetDisplayName( int display )
{
    return (const char*) displays[display].display_name;
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
	SDL_ShowSimpleMessageBox( sdl_flags, title, message, /*SDL_GL_GetCurrentWindow()*/ NULL );
}

void KeDisplayLastMessage( const char* title, uint32_t flags )
{
	std::string str = GETLASTMSG();

	KeMessageBox( str.c_str(), title, flags );
}
