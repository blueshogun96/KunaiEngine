//
//  ke_al_audiodevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_al_audiodevice.h"
#include "nvdebug.h"


/*
 * OpenAL extension functions
 */
LPALGENEFFECTS      alGenEffects = NULL;
LPALDELETEEFFECTS   alDeleteEffects = NULL;
LPALISEFFECT        alIsEffect = NULL;



/*
 * Name: ke_al_renderdevice::
 * Desc:
 */
ke_al_audiodevice_t::ke_al_audiodevice_t(): device(NULL), context(NULL)
{
    this->initialized = No;
}


ke_al_audiodevice_t::ke_al_audiodevice_t( ke_audiodevice_desc_t* audiodevice_desc ): device(NULL), context(NULL)
{
    ALint attributes[4] = {0};
    initialized = No;
    
    /* Create the default OpenAL device */
    device = alcOpenDevice( NULL );
	if( !device )
		return;
    
    DISPDBG( 1, "alcOpenDevice() = OK\n" );
    
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
    
    initialized = Yes;
}

ke_al_audiodevice_t::~ke_al_audiodevice_t()
{
    /* Uninitialize OpenAL */
    
    if( context )
        alcDestroyContext( context );
    if( device )
        alcCloseDevice( device );
}

bool ke_al_audiodevice_t::confirm_device()
{
    return initialized;
}

bool ke_al_audiodevice_t::create_sound_buffer( ke_audioformat_t* audio_format, ke_soundbuffer_t** sound_buffer )
{
    ke_al_soundbuffer_t* sb;
    ALenum error = alGetError();
    
    /* Allocate a new sound buffer instance */
    (*sound_buffer) = new ke_al_soundbuffer_t;
    sb = static_cast<ke_al_soundbuffer_t*>( *sound_buffer );
    
    /* Save audio format */
    memmove( &sb->audio_format, audio_format, sizeof( ke_audioformat_t ) );
    
    /* Generate a sound buffer and source */
    alGenBuffers( 1, &sb->buffer );
    error = alGetError();
    alGenSources( 1, &sb->source );
    error = alGetError();
    
    /* Set buffer data to NULL */
//    alBufferData( sb->source, audio_format->format, NULL, audio_format->data_size, audio_format->frequency );
    
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

void ke_al_audiodevice_t::release_sound_buffer( ke_soundbuffer_t* sound_buffer )
{
    ke_al_soundbuffer_t* sb = static_cast<ke_al_soundbuffer_t*>( sound_buffer );
    
    /* Delete the OpenAL buffer and sources */
    if( sound_buffer )
    {
        alDeleteBuffers( 1, &sb->buffer );
        alDeleteSources( 1, &sb->source );
    }
}

bool ke_al_audiodevice_t::set_buffer_data( ke_soundbuffer_t* sound_buffer, void* buffer_data, uint32_t buffer_size )
{
    ke_al_soundbuffer_t* sb = static_cast<ke_al_soundbuffer_t*>( sound_buffer );
    
    /* Set buffer data */
    alBufferData( sb->source, sb->audio_format.format, buffer_data, buffer_size, sb->audio_format.frequency );
    
    return true;
}

bool ke_al_audiodevice_t::play_sound_buffer( ke_soundbuffer_t* sound_buffer, bool looping )
{
    ke_al_soundbuffer_t* sb = static_cast<ke_al_soundbuffer_t*>( sound_buffer );
    
    /* Play the sound effect */
    alSourcei( sb->source, AL_LOOPING, looping );
	alSourcePlay( sb->source );
    
    return true;
}

void ke_al_audiodevice_t::stop_sound_buffer( ke_soundbuffer_t* sound_buffer )
{
    ke_al_soundbuffer_t* sb = static_cast<ke_al_soundbuffer_t*>( sound_buffer );
    
    alSourceStop( sb->source );
}

void ke_al_audiodevice_t::pause_sound_buffer( ke_soundbuffer_t* sound_buffer )
{
    ke_al_soundbuffer_t* sb = static_cast<ke_al_soundbuffer_t*>( sound_buffer );
    
    alSourcePause( sb->source );
}

void ke_al_audiodevice_t::set_listener_position( float* position )
{
    /* Set the listener position */
    alListenerfv( AL_POSITION, position );
}

void ke_al_audiodevice_t::set_listener_velocity( float* velocity )
{
    /* Set the listener velocity */
    alListenerfv( AL_VELOCITY, velocity );
}

void ke_al_audiodevice_t::set_listener_orientation( float* at, float* up )
{
    float orientation[6];
    
    /* Set the listener orientation */
    memmove( orientation, at, sizeof( float ) * 3 );
    memmove( &orientation[3], up, sizeof( float ) * 3 );
    
    alListenerfv( AL_ORIENTATION, orientation );
}
