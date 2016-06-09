//
//  ke_al_audiodevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeOpenALAudioDevice.h"
#include "KeDebug.h"


/*
 * OpenAL extension functions
 */
#if defined(__APPLE__) && defined(__MOBILE_OS__)
/* TODO: Extensions for iOS */
#else
LPALGENEFFECTS      alGenEffects = NULL;
LPALDELETEEFFECTS   alDeleteEffects = NULL;
LPALISEFFECT        alIsEffect = NULL;
#endif

/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OAL_DISPDBG( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }
#define OAL_DISPDBG_R( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return; }
#define OAL_DISPDBG_RB( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return false; }
#define OALC_DISPDBG( a, b ) error = alcGetError(device); if(error) { DISPDBG( a, b << "\nALC error code: (" << error << ")" ); }
#define OALC_DISPDBG_R( a, b ) error = alcGetError(device); if(error) { DISPDBG( a, b << "\nALC error code: (" << error << ")" ); return; }
#define OALC_DISPDBG_RB( a, b ) error = alcGetError(device); if(error) { DISPDBG( a, b << "\nALC error code: (" << error << ")" ); return false; }

/*
 * Name: ke_al_renderdevice::
 * Desc:
 */
IKeOpenALAudioDevice::IKeOpenALAudioDevice(): device(NULL), context(NULL)
{
    this->initialized = No;
}


IKeOpenALAudioDevice::IKeOpenALAudioDevice( KeAudioDeviceDesc* audiodevice_desc ): device(NULL), context(NULL)
{
    ALint attributes[4] = {0};
    ALenum error = 0;
    
    initialized = No;
    
    /* Create the default OpenAL device */
    device = alcOpenDevice( NULL );
	if( !device )
    {
        DISPDBG_R( KE_ERROR, "Error initializing OpenAL audio device!" );
    }
    
#if defined(__APPLE__) && defined(__MOBILE_OS__)
    /* Not available for iOS afaik */
#else
    /* Does the user want to initialize EFX? */
    if( audiodevice_desc->aux_sends != 0 )
    {
        /* Verify the EFX extension is supported */
        /* If not, uninitialize and exit */
        if( !alcIsExtensionPresent( device, "ALC_EXT_EFX" ) )
        {
            DISPDBG( 1, "ALC_EXT_EFX extension is not present!\n" );
            alcCloseDevice( device );
            return;
        }
        
        /* Set EFX attributes */
        attributes[0] = ALC_MAX_AUXILIARY_SENDS;
        attributes[1] = audiodevice_desc->aux_sends;
        
        /* TODO: More? */
    }
#endif
    
    /* Create an OpenAL context */
	context = alcCreateContext( device, audiodevice_desc->aux_sends == 0 ? NULL : attributes );
    OALC_DISPDBG( KE_ERROR, "Error initializing OpenAL context!" );
	if( !context )
	{
		alcCloseDevice( device );
		return;
	}
    
    /* Set the newly created OpenAL context */
    ALboolean res = alcMakeContextCurrent( context );
	if( !res )
    {
        OALC_DISPDBG( KE_ERROR, "An error occured setting OpenAL context!" );
        alcDestroyContext( context );
        alcCloseDevice( device );
		return;
    }
    
	/* So far, so good, right? */
	error = alGetError();
    
#if defined(__APPLE__) && defined(__MOBILE_OS__)
    /* Not available for iOS afaik */
#else
    /* Verify that we get the desired number of auxiliry sends */
    if( audiodevice_desc->aux_sends != 0 )
    {
        int sends = 0;
        alcGetIntegerv( device, ALC_MAX_AUXILIARY_SENDS, 1, &sends );
        
        if( sends != audiodevice_desc->aux_sends )
            DISPDBG( 1, "Requested " << audiodevice_desc->aux_sends << " max auxiliary sends, got " << sends << "instead...\n" );
        else
            DISPDBG( 1, "Max auxiliary sends: " << sends << "\n" );
    }
#endif
    
    const ALCchar* extensions = alcGetString( device, ALC_EXTENSIONS );
    int extension_count = 0;
    std::vector<ALCchar> ext;
    
    ext.push_back('\t');
    ext.push_back('\t');
    
    /* Count extensions */
    for( int i = 0; i < strlen( extensions )+1; i++ )
    {
        if( extensions[i] == ' ' || extensions[i] == '\0' )
        {
            extension_count++;
            
            ext.push_back('\n');
            ext.push_back('\t');
            ext.push_back('\t');
        }
        else
            ext.push_back(extensions[i]);
    }
    
    ext.push_back('\n');
    
	/* Print OpenAL driver/implementation details */
    DISPDBG( 1, "\n\tOpenAL Vendor: " << alGetString( AL_VENDOR ) << 
		"\n\tOpenAL Version: " << alGetString( AL_VERSION ) << 
		"\n\tOpenAL Renderer: " << alGetString( AL_RENDERER ) << "\n" );

    /* Print extensions */
    std::stringstream sstr;
    sstr << extension_count;
    std::string ext_str = "\n\tOpenAL Extensions (" + sstr.str() + "):\n";
    ext_str += ext.data();
    
    DISPDBG( KE_DBGLVL(0), ext_str );
    
    initialized = Yes;
}

IKeOpenALAudioDevice::~IKeOpenALAudioDevice()
{
    /* Uninitialize OpenAL */
    
    if( context )
        alcDestroyContext( context );
    if( device )
        alcCloseDevice( device );
}

bool IKeOpenALAudioDevice::ConfirmDevice()
{
    return initialized;
}

bool IKeOpenALAudioDevice::CreateSoundBuffer( WAVEFORMATEX* wfx, IKeSoundBuffer** sound_buffer )
{
    IKeOpenALSoundBuffer* sb;
    ALenum error = alGetError();
    
    /* Allocate a new sound buffer instance */
    (*sound_buffer) = new IKeOpenALSoundBuffer;
    sb = static_cast<IKeOpenALSoundBuffer*>( *sound_buffer );
    
    /* Save audio format */
    memmove( &sb->wfx, wfx, sizeof( WAVEFORMATEX ) );
    
    /* Generate a sound buffer and source */
    alGenBuffers( 1, &sb->buffer );
    error = alGetError();
	if( error != AL_NO_ERROR )
	{
		sb->Destroy();
		DISPDBG( KE_ERROR, "Error creating buffer (0x" << error << ")!\n" );
		return false;
	}

    alGenSources( 1, &sb->source );
    error = alGetError();
	if( error != AL_NO_ERROR )
	{
		sb->Destroy();
		DISPDBG( KE_ERROR, "Error creating source (0x" << error << ")!\n" );
		return false;
	}

    /* Default 3D sound position and velocity */
    //memset( sb->position, 0, sizeof( float ) * 3 );
	//memset( sb->velocity, 0, sizeof( float ) * 3 );
    sb->position = nv::vec3f( 0.0f, 0.0f, 0.0f );
    sb->velocity = nv::vec3f( 0.0f, 0.0f, 0.0f );
    
	/* Set the sound source's default attributes */
    sb->volume = 1.0f;
    sb->pitch = 1.0f;
	alSourcei( sb->source, AL_BUFFER, sb->buffer );
	alSourcef( sb->source, AL_PITCH,  sb->volume );
	alSourcef( sb->source, AL_GAIN,	 sb->pitch );
	alSourcefv( sb->source, AL_POSITION, sb->position._array );
	alSourcefv( sb->source, AL_VELOCITY, sb->velocity._array );
	alSourcei( sb->source, AL_LOOPING, 0 );
    
    return true;
}



void IKeOpenALAudioDevice::SetListenerPosition( nv::vec3f& position )
{
    /* Set the listener position */
    alListenerfv( AL_POSITION, position._array );
}

void IKeOpenALAudioDevice::SetListenerVelocity( nv::vec3f& velocity )
{
    /* Set the listener velocity */
    alListenerfv( AL_VELOCITY, velocity._array );
}

void IKeOpenALAudioDevice::SetListenerOrientation( nv::vec3f& at, nv::vec3f& up )
{
    float orientation[6];
    
    /* Set the listener orientation */
    memmove( orientation, at._array, sizeof( float ) * 3 );
    memmove( &orientation[3], up._array, sizeof( float ) * 3 );
    
    alListenerfv( AL_ORIENTATION, orientation );
}


