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
#define OAL_DISPDBG( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define OAL_DISPDBG_R( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define OAL_DISPDBG_RB( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }


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
    /* Set buffer data */
    alBufferData( source, audio_format.format, buffer_data, buffer_size, audio_format.frequency );
    OAL_DISPDBG_RB( KE_ERROR, "Error setting sound buffer data!", alGetError() );
    
    return true;
}

bool IKeOpenALSoundBuffer::Play( bool looping )
{
    /* Play the sound effect */
    alSourcei( source, AL_LOOPING, looping );
    alSourcePlay( source );
    OAL_DISPDBG_RB( KE_ERROR, "Error playing sound buffer!", alGetError() );
    
    return true;
}

void IKeOpenALSoundBuffer::Stop()
{
    alSourceStop( source );
    OAL_DISPDBG( KE_ERROR, "Error stopping sound buffer!", alGetError() );
}

void IKeOpenALSoundBuffer::Pause()
{
    alSourcePause( source );
    OAL_DISPDBG( KE_ERROR, "Error pausing sound buffer!", alGetError() );
}

void IKeOpenALSoundBuffer::SetPosition( float* position )
{
    alSourcefv( source, AL_POSITION, position );
    memmove( this->position, position, sizeof( float ) * 3 );
}

void IKeOpenALSoundBuffer::SetVelocity( float* velocity )
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
