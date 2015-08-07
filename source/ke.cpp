//
//  Ke.cpp
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeSystem.h"
#include "KeDebug.h"
#include "yaml.h"


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
 * Name: KeInitializeEx
 * Desc: Same as above, but initializes more engine components as specified in the settings
 *		 script.  The first param is the path to the YAML based file where the engine settings
 *       are stored.  Any components listed will be initialized in this function.
 */
bool KeInitializeEx( std::string settings_file, IKeRenderDevice** rd, IKeAudioDevice** ad )
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
   
    /* Reset keys */
    KeResetKeys();

	/* Open settings file */
	FILE* fp = fopen( settings_file.c_str(), "r" );
	if( !fp )
	{
		DISPDBG( KE_ERROR, "Error opening settings file!\nFile: " << settings_file << std::endl );
		return false;
	}

	/* Initialize YAML parser */
	yaml_parser_t yaml_parser;
	if( !yaml_parser_initialize( &yaml_parser ) )
	{
		fclose(fp);
		DISPDBG( KE_ERROR, "Error intiializing YAML parser!" );
		return false;
	}

	/* Read settings file */
	yaml_parser_set_input_file( &yaml_parser, fp );

	bool init_renderer = No, init_audio = No, init_leap_motion = No, init_kinect = No;
	KeRenderDeviceDesc rddesc;
	KeAudioDeviceDesc  addesc;

	ZeroMemory( &rddesc, sizeof( KeRenderDeviceDesc ) );
	ZeroMemory( &addesc, sizeof( KeAudioDeviceDesc ) );

	/* Parse the data */
	yaml_token_t token;

	do
	{
		static std::string keyval = " ", valueval = " ";
		static int key = No, value = No;

		yaml_parser_scan( &yaml_parser, &token );

		switch (token.type)
		{
		/* Stream start/end */
		case YAML_STREAM_START_TOKEN:	break;
		case YAML_STREAM_END_TOKEN:		break;

		/* Token types (read before actual token) */
        case YAML_KEY_TOKEN:   key = Yes; break;
        case YAML_VALUE_TOKEN: value = Yes; break;

		/* Block delimeters */
		case YAML_BLOCK_SEQUENCE_START_TOKEN: break;
		case YAML_BLOCK_ENTRY_TOKEN:          break;
		case YAML_BLOCK_END_TOKEN:            break;

		/* Data */
		case YAML_BLOCK_MAPPING_START_TOKEN:  
			if( value )
			{
				if( keyval == "RenderDevice" )
					init_renderer = Yes;
				if( keyval == "AudioDevice" )
					init_audio = Yes;
				if( keyval == "LeapMotionDevice" )
					init_leap_motion = Yes;
				if( keyval == "KinectDevice" )
					init_kinect = Yes;

				value = No;
			}
			break;
		case YAML_SCALAR_TOKEN:
                fprintf(stderr, "scalar %s \n", token.data.scalar.value);
                
                /* Key token value */
                if( key )
                {
                    keyval = (const char*) token.data.scalar.value;
                }
                
                /* Value token value */
                if( value )
                {
                    valueval = (const char*) token.data.scalar.value;
                    
					/* RenderDevice settings */
					if( keyval == "WindowWidth" )
						rddesc.width = atoi( valueval.c_str() );
					else if( keyval == "WindowHeight" )
						rddesc.height = atoi( valueval.c_str() );
					else if( keyval == "ColourBpp" )
						rddesc.colour_bpp = atoi( valueval.c_str() );
					else if( keyval == "DepthBpp" )
						rddesc.depth_bpp = atoi( valueval.c_str() );
					else if( keyval == "StencilBpp" )
						rddesc.stencil_bpp = atoi( valueval.c_str() );
					else if( keyval == "BufferCount" )
						rddesc.buffer_count = atoi( valueval.c_str() );
					else if( keyval == "RefreshRate" )
						rddesc.device_type = atoi( valueval.c_str() );
					else if( keyval == "DeviceType" )
					{
						if( valueval == "OpenGL3" )		rddesc.device_type = KE_RENDERDEVICE_OGL3;
						if( valueval == "OpenGL4" )		rddesc.device_type = KE_RENDERDEVICE_OGL4;
						if( valueval == "Direct3D11" )	rddesc.device_type = KE_RENDERDEVICE_D3D11;
						if( valueval == "OpenGLES2" )	rddesc.device_type = KE_RENDERDEVICE_OGLES2;
						if( valueval == "OpenGLES3" )	rddesc.device_type = KE_RENDERDEVICE_OGLES3;
						if( valueval == "Metal" )		rddesc.device_type = KE_RENDERDEVICE_METAL;
					}
					else if( keyval == "Fullscreen" )
						rddesc.fullscreen = valueval == "Yes" ? Yes : No;

					/* AudioDevice settings */

						
                    /* Reset key and values */
                    keyval = " ", valueval = " ";
                }
                
                key = No, value = No;
                break;

		/* Others */
		default:
			fprintf( stderr, "Got token of type %d\n", token.type );
		}
		if( token.type != YAML_STREAM_END_TOKEN )
			yaml_token_delete( &token );
	} while( token.type != YAML_STREAM_END_TOKEN );
	yaml_token_delete( &token );

	/* Close YAML parser */
	yaml_parser_delete( &yaml_parser );
	fclose(fp);
 
	/* Create window and rendering device */
	if( init_renderer && rd != NULL )
	{
		if( !KeCreateWindowAndDevice( &rddesc, rd ) )
			return false;
	}

    /* Call user specified initialization routine */
    KeOnInitialize( KeGetContextPointer() );
    
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
