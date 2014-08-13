//
//  ke.cpp
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke.h"
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
    DISPDBG( 1, "ke_initialize(): Initialization started\n" );
    
    /* Initialize SDL and the necessary sub-systems. For now, we only want to initialize 
       timing and events. */
    if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_TIMER ) != 0 )
        return false;
    DISPDBG( 1, "SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER) = OK" );
    
    return true;
}

/*
 * Name: ke_uninitialize
 * Desc: Uninitializes everything that was done above.
 */
void ke_uninitialize()
{
    DISPDBG( 1, "ke_uninitialize(): Uninitializing SDL..." );
    
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
    /* Unitialize SDL */
    //SDL_Quit(); /* TODO: Do this elsewhere */
}

/*
 * Name: ke_create_audio_device
 * Desc: Initializes our audio API of choice (i.e. XAudio2, OpenAL, etc.).
 */
bool ke_create_audio_device( ke_audiodevice_desc_t* device_desc, ke_audiodevice_t** device )
{
    return true;
}

/*
 * Name:
 * Desc:
 */
void ke_destroy_audio_device( ke_audiodevice_t* device )
{
    
}
