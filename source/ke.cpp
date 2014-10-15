//
//  ke.cpp
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke.h"
#include "ke_system.h"
#include "nvdebug.h"


NVDebug* dbg = NULL;


/*
 * Name: ke_initialize
 * Desc: Initializes any necessary components of the engine prior to initializing other
 *       sub-system components.
 */
bool ke_initialize()
{
    /* Set the current directory to our resource directory */
    ke_set_current_path_to_resource_directory();
    
    /* Initial debug logging */
    dbg = new NVDebug( KE_DBG_LEVEL, "debug.txt" );
    DISPDBG( 1, "Initialization started\n" );
    
    /* Initialize SDL and the necessary sub-systems. For now, we only want to initialize 
       timing and events. */
    if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_TIMER ) != 0 )
        return false;
    DISPDBG( 1, "SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER) = OK\n" );
    
    /* Call user specified initialization routine */
    ke_on_initialize( ke_get_context_pointer() );
    
    /* Reset keys */
    ke_reset_keys();
    
    return true;
}

/*
 * Name: ke_uninitialize
 * Desc: Uninitializes everything that was done above.
 */
void ke_uninitialize()
{
    /* Call user specified uninitialization routine */
    ke_on_uninitialize( ke_get_context_pointer() );
    
    DISPDBG( 1, "Uninitializing SDL..." );
    
    /* Delete the debug log */
    delete dbg;
    
    /* Uninitialize SDL and it's sub-systems */
    SDL_Quit();
}

/*
 * Name: ke_create_window_and_device
 * Desc: Initializes our window and 3D rendering API of choice (i.e. Direct3D 11, OpenGL 4.x).
 */
bool ke_create_window_and_device( ke_renderdevice_desc_t* device_desc, ke_renderdevice_t** device )
{
    /* Sanity check */
    if( !device )
        return false;
    
    /* Create the device and rendering window */
    (*device) = ke_create_renderdevice( device_desc );
    if( !(*device) )
        return false;
    
    /* Confirm that this device was created successfully */
    if( !(*device)->confirm_device() )
        return false;
    
    return true;
}

/*
 * Name: ke_destroy_window_and_device
 * Desc:
 */
void ke_destroy_window_and_device( ke_renderdevice_t* device )
{
    /* Destroy the device and unitialize SDL's video component or Direct3D */
    delete device;
}

/*
 * Name: ke_create_audio_device
 * Desc: Initializes our audio API of choice (i.e. XAudio2, OpenAL, etc.).
 */
bool ke_create_audio_device( ke_audiodevice_desc_t* device_desc, ke_audiodevice_t** device )
{
	 /* Sanity check */
    if( !device )
        return false;
    
    /* Create the device and rendering window */
    (*device) = ke_create_audiodevice( device_desc );
    if( !(*device) )
        return false;
    
    /* Confirm that this device was created successfully */
    if( !(*device)->confirm_device() )
        return false;
    
    return true;
}

/*
 * Name:
 * Desc:
 */
void ke_destroy_audio_device( ke_audiodevice_t* device )
{
	/* Destroy the audio device */
	delete device;
}

/*
 * Name: ke_alloc_inhereted_resource
 * Desc: Allocates the base pointer as the inhereted pointer type, and returns a pointer of the inhereted type
 *		 with the same location as the base pointer.  Returns NULL upon failure.
 */
template <class ke_base_resource_t, class ke_inhereted_resource_t>
ke_inhereted_resource_t* ke_alloc_inhereted_resource( ke_base_resource_t** resource )
{
	/* Sanity check */
	if( resource == NULL )
		return NULL;

	/* Attempt to allocate a new resource pointer */
	*resource = new ke_inhereted_resource_t;
	if( *resource == NULL )
	{
		DISPDBG( 1, "ke_alloc_inhereted_resource() failed: Out of memory!\n" );
		return NULL;
	}

	/* Return a copy of the inhereted type */
	ke_inhereted_resource_t ir = static_cast<ke_inhereted_resource_t*>( *resource );

	return ir;
}