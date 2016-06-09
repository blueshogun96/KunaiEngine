//
//  KeOpenALSoundBuffer.cpp
//
//  Created by Shogun3D on 4/19/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeAudioDevice.h"
#include "KeOpenALAudioDevice.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OAL_DISPDBG( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }
#define OAL_DISPDBG_R( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return; }
#define OAL_DISPDBG_RB( a, b ) error = alGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return false; }



ALenum KeGetFormat_AL( WAVEFORMATEX* wfx )
{
	ALenum format = 0;

	if( wfx->nChannels == 1 )
	{
		if( wfx->wBitsPerSample == 8 ) format = AL_FORMAT_MONO8;
		if( wfx->wBitsPerSample == 16 ) format = AL_FORMAT_MONO16;
	}

	if( wfx->nChannels == 2 )
	{
		if( wfx->wBitsPerSample == 8 ) format = AL_FORMAT_STEREO8;
		if( wfx->wBitsPerSample == 16 ) format = AL_FORMAT_STEREO16;
	}

	return format;
}

/*
 * Name: IKeOpenALSoundBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenALSoundBuffer::Destroy()
{
    /* Delete the sound buffer and source */
    alDeleteBuffers( 1, &buffer );
    alDeleteSources( 1, &source );
    
    /* Delete this instance */
    delete this;
}

bool IKeOpenALSoundBuffer::SetBufferData( void* buffer_data, uint32_t buffer_size )
{
	ALenum error = alGetError();
    ALenum fmt = KeGetFormat_AL( &wfx );
    
    /* Unbind the buffer from our source before continuing.  Failing to do so will 
       generate an AL_INVALID_OPERATION error when calling alBufferData(). */
    alSourcei( source, AL_BUFFER, 0 );
    OAL_DISPDBG( KE_ERROR, "Error unbinding buffer from source" );
    
    /* Set buffer data */
    alBufferData( buffer, fmt, buffer_data, buffer_size, wfx.nSamplesPerSec );
    OAL_DISPDBG_RB( KE_ERROR, "Error setting sound buffer data!" );
    
    /* Rebind the buffer to the source */
    alSourcei( source, AL_BUFFER, buffer );
    OAL_DISPDBG( KE_ERROR, "Error re-binding buffer to source" );
    
    return true;
}

bool IKeOpenALSoundBuffer::Play( bool looping )
{
	ALenum error;

    /* Play the sound effect */
    alSourcei( source, AL_LOOPING, looping );
    alSourcePlay( source );
    OAL_DISPDBG_RB( KE_ERROR, "Error playing sound buffer!" );
    
    return true;
}

void IKeOpenALSoundBuffer::Stop()
{
	ALenum error;

    alSourceStop( source );
    OAL_DISPDBG( KE_ERROR, "Error stopping sound buffer!" );
}

void IKeOpenALSoundBuffer::Pause()
{
	ALenum error;

    alSourcePause( source );
    OAL_DISPDBG( KE_ERROR, "Error pausing sound buffer!" );
}

void IKeOpenALSoundBuffer::SetPosition( nv::vec3f& position )
{
    alSourcefv( source, AL_POSITION, position );
    memmove( this->position, position, sizeof( float ) * 3 );
}

void IKeOpenALSoundBuffer::SetVelocity( nv::vec3f& velocity )
{
    alSourcefv( source, AL_VELOCITY, velocity );
    memmove( this->velocity, velocity, sizeof( float ) * 3 );
}

void IKeOpenALSoundBuffer::SetVolume( float volume )
{
    alSourcef( source, AL_GAIN, volume );
    this->volume = volume;
}

void IKeOpenALSoundBuffer::SetPitch( float pitch )
{
    alSourcef( source, AL_PITCH, pitch );
    pitch = pitch;
}
