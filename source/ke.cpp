//
//  Ke.cpp
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeSystem.h"
#include "KeDebug.h"


NVDebug*			dbg = nullptr;				/* Debug log */
IKeRenderDevice*	renderdevice = nullptr;		/* Copy of the rendering device */
IKeAudioDevice*		audiodevice = nullptr;		/* Copy of the audio device */



/*
 * Name: KeInitialize
 * Desc: Initializes any necessary components of the engine prior to initializing other
 *       sub-system components.
 */
bool KeInitialize()
{
    /* Set the current directory to our resource directory */
    KeSetCurrentPathToResourceDirectory();
    
    /* Initial debug logging */
    dbg = new NVDebug( KE_DBG_LEVEL, "debug.txt" );
    DISPDBG( KE_DBGLVL(0), "Initialization started\n" );
    
    /* Initialize SDL and the necessary sub-systems. For now, we only want to initialize 
       timing and events. */
    if( SDL_Init( SDL_INIT_EVENTS | SDL_INIT_TIMER ) != 0 )
	{
		DISPDBG( KE_ERROR, "Error initializing SDL timer and events!" );
	}
	else
	{
		DISPDBG( KE_DBGLVL(3), "SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER) = OK\n" );
	}
    
    /* Gather display modes */
    extern int KeGatherAllDisplayInformation();
    KeGatherAllDisplayInformation();
    
    /* Insert event handler for mobile/embedded platforms */
#ifdef __MOBILE_OS__
    extern int KeProcessAppEvents( void *userdata, SDL_Event *event );
    SDL_SetEventFilter( KeProcessAppEvents, KeGetContextPointer() );
#endif
    
    /* Call user specified initialization routine */
    KeOnInitialize( KeGetContextPointer() );
    
    /* Reset keys */
    KeResetKeys();
    
    return true;
}

/*
 * Name: KeUninitialize
 * Desc: Uninitializes everything that was done above.
 */
void KeUninitialize()
{
    /* Call user specified uninitialization routine */
    KeOnUnintialize( KeGetContextPointer() );
    
    DISPDBG( KE_DBGLVL(0), "Uninitializing SDL..." );
    
    /* Delete the debug log */
    delete dbg;
    
    /* Uninitialize SDL and it's sub-systems */
    SDL_Quit();
}

/*
 * Name: KeCreateWindowAndDevice
 * Desc: Initializes our window and 3D rendering API of choice (i.e. Direct3D 11, OpenGL 4.x).
 */
bool KeCreateWindowAndDevice( KeRenderDeviceDesc* device_desc, IKeRenderDevice** device )
{
    /* Sanity check */
    if( !device )
        return false;
    
    /* Create the device and rendering window */
    (*device) = KeCreateRenderDevice( device_desc );
    if( !(*device) )
        return false;
    
    /* Confirm that this device was created successfully */
    if( !(*device)->ConfirmDevice() )
        return false;
    
	/* Save a copy of the rendering device */
	renderdevice = (*device);

    return true;
}

/*
 * Name: KeDestroyWindowAndDevice
 * Desc: Deletes this render device, uninitializes our window and shuts down the
 *		 chosen rendering API.
 */
void KeDestroyWindowAndDevice( IKeRenderDevice* device )
{
    /* Destroy the device and unitialize SDL's video component or Direct3D */
    delete device;

	renderdevice = nullptr;
}

/*
 * Name: KeGetRenderDevice
 * Desc: Returns a copy of the previously created render device.
 */
IKeRenderDevice* KeGetRenderDevice( void )
{
	return renderdevice;
}

/*
 * Name: KeCreateAudioDevice
 * Desc: Initializes our audio API of choice (i.e. XAudio2, OpenAL, etc.).
 */
bool KeCreateAudioDevice( KeAudioDeviceDesc* device_desc, IKeAudioDevice** device )
{
	 /* Sanity check */
    if( !device )
        return false;
    
    /* Create the device and rendering window */
    (*device) = KeCreateAudioDevice( device_desc );
    if( !(*device) )
        return false;
    
    /* Confirm that this device was created successfully */
    if( !(*device)->ConfirmDevice() )
        return false;
    
	/* Save a copy of the audio device */
	audiodevice = (*device);

    return true;
}

/*
 * Name: KeDestroyAudioDevice
 * Desc: Deletes this audio device and shuts down the chosen audio API.
 */
void KeDestroyAudioDevice( IKeAudioDevice* device )
{
	/* Destroy the audio device */
	delete device;

	audiodevice = nullptr;
}

/*
 * Name: KeGetAudioDevice
 * Desc: Returns a copy of the previously created audio device.
 */
IKeAudioDevice* KeGetAudioDevice( void )
{
	return audiodevice;
}
