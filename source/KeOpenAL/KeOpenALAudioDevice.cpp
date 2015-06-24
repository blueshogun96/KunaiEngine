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
    initialized = No;
    
    /* Create the default OpenAL device */
    device = alcOpenDevice( NULL );
	if( !device )
		return;
    
    DISPDBG( 1, "alcOpenDevice() = OK\n" );
    
#if defined(__APPLE__) && defined(__MOBILE_OS__)
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
	if( !context )
	{
		alcCloseDevice( device );
		return;
	}
    
    DISPDBG( 1, "alcCreateContext() = OK\n" );
    
    /* Set the newly created OpenAL context */
	if( !alcMakeContextCurrent( context ) )
    {
        alcDestroyContext( context );
        alcCloseDevice( device );
		return;
    }
    
	/* Clear the error bit (why doesn't OpenAL do this for us?) */
	alGetError();
    
#if defined(__APPLE__) && defined(__MOBILE_OS__)
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
    
	/* Print OpenAL driver/implementation details */
    DISPDBG( 1, "\n\tOpenAL Vendor: " << alGetString( AL_VENDOR ) << 
		"\n\tOpenAL Version: " << alGetString( AL_VERSION ) << 
		"\n\tOpenAL Renderer: " << alGetString( AL_RENDERER ) << "\n" );

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

bool IKeOpenALAudioDevice::CreateSoundBuffer( KeAudioFormat* audio_format, IKeSoundBuffer** sound_buffer )
{
    IKeOpenALSoundBuffer* sb;
    ALenum error = alGetError();
    
    /* Allocate a new sound buffer instance */
    (*sound_buffer) = new IKeOpenALSoundBuffer;
    sb = static_cast<IKeOpenALSoundBuffer*>( *sound_buffer );
    
    /* Save audio format */
    memmove( &sb->audio_format, audio_format, sizeof( KeAudioFormat ) );
    
    /* Generate a sound buffer and source */
    alGenBuffers( 1, &sb->buffer );
    error = alGetError();
	if( error != AL_NO_ERROR )
	{
		ReleaseSoundBuffer( sb );
		DISPDBG( KE_ERROR, "Error creating buffer (0x" << error << ")!\n" );
		return false;
	}

    alGenSources( 1, &sb->source );
    error = alGetError();
	if( error != AL_NO_ERROR )
	{
		ReleaseSoundBuffer( sb );
		DISPDBG( KE_ERROR, "Error creating source (0x" << error << ")!\n" );
		return false;
	}

    /* Default 3D sound position and velocity */
    memset( sb->position, 0, sizeof( float ) * 3 );
	memset( sb->velocity, 0, sizeof( float ) * 3 );
    
	/* Set the sound source's default attributes */
	alSourcei( sb->source, AL_BUFFER, sb->buffer );
	alSourcef( sb->source, AL_PITCH,  1.0f );
	alSourcef( sb->source, AL_GAIN,	 1.0f );
	alSourcefv( sb->source, AL_POSITION, (float*) &sb->position );
	alSourcefv( sb->source, AL_VELOCITY, (float*) &sb->velocity );
	alSourcei( sb->source, AL_LOOPING, 0 );
    
    return true;
}

void IKeOpenALAudioDevice::ReleaseSoundBuffer( IKeSoundBuffer* sound_buffer )
{
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    /* Delete the OpenAL buffer and sources */
    if( sound_buffer )
    {
        alDeleteBuffers( 1, &sb->buffer );
        alDeleteSources( 1, &sb->source );
    }
}

bool IKeOpenALAudioDevice::SetBufferData( IKeSoundBuffer* sound_buffer, void* buffer_data, uint32_t buffer_size )
{
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    /* Set buffer data */
    alBufferData( sb->source, sb->audio_format.format, buffer_data, buffer_size, sb->audio_format.frequency );
    
    return true;
}

bool IKeOpenALAudioDevice::PlaySoundBuffer( IKeSoundBuffer* sound_buffer, bool looping )
{
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    /* Play the sound effect */
    alSourcei( sb->source, AL_LOOPING, looping );
	alSourcePlay( sb->source );
    
    return true;
}

void IKeOpenALAudioDevice::StopSoundBuffer( IKeSoundBuffer* sound_buffer )
{
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourceStop( sb->source );
}

void IKeOpenALAudioDevice::PauseSoundBuffer( IKeSoundBuffer* sound_buffer )
{
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourcePause( sb->source );
}

void IKeOpenALAudioDevice::SetListenerPosition( float* position )
{
    /* Set the listener position */
    alListenerfv( AL_POSITION, position );
}

void IKeOpenALAudioDevice::SetListenerVelocity( float* velocity )
{
    /* Set the listener velocity */
    alListenerfv( AL_VELOCITY, velocity );
}

void IKeOpenALAudioDevice::SetListenerOrientation( float* at, float* up )
{
    float orientation[6];
    
    /* Set the listener orientation */
    memmove( orientation, at, sizeof( float ) * 3 );
    memmove( &orientation[3], up, sizeof( float ) * 3 );
    
    alListenerfv( AL_ORIENTATION, orientation );
}

void IKeOpenALAudioDevice::SetBufferPosition( IKeSoundBuffer* sound_buffer, float* position )
{
    if( !sound_buffer )
        return;
    
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourcefv( sb->source, AL_POSITION, position );
    memmove( sb->position, position, sizeof( float ) * 3 );
}

void IKeOpenALAudioDevice::SetBufferVelocity( IKeSoundBuffer* sound_buffer, float* velocity )
{
    if( !sound_buffer )
        return;
    
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourcefv( sb->source, AL_VELOCITY, velocity );
    memmove( sb->velocity, velocity, sizeof( float ) * 3 );
}

void IKeOpenALAudioDevice::SetBufferVolume( IKeSoundBuffer* sound_buffer, float volume )
{
    if( !sound_buffer )
        return;
    
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourcef( sb->source, AL_GAIN, volume );
    sb->volume = volume;
}

void IKeOpenALAudioDevice::SetBufferPitch( IKeSoundBuffer* sound_buffer, float pitch )
{
    if( !sound_buffer )
        return;
    
    IKeOpenALSoundBuffer* sb = static_cast<IKeOpenALSoundBuffer*>( sound_buffer );
    
    alSourcef( sb->source, AL_PITCH, pitch );
    sb->pitch = pitch;
}