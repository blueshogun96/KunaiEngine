//
//  KeXAudio2SoundBuffer.cpp
//
//  Created by Shogun3D on 6/28/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeAudioDevice.h"
#include "KeXAudio2AudioDevice.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define XA2_DISPDBG( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); }
#define XA2_DISPDBG_R( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); return; }
#define XA2_DISPDBG_RB( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); return false; }


/*
 * Name: IKeXAudio2SoundBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeXAudio2SoundBuffer::Destroy()
{
    /* Delete the sound buffer */
	if( this->subvoice )
		this->subvoice->DestroyVoice();

	if( buffer_data )
		free( buffer_data );
    
    /* Delete this instance */
    delete this;
}

bool IKeXAudio2SoundBuffer::SetBufferData( void* buffer_data, uint32_t buffer_size )
{
	HRESULT hr;

	/* Allocate buffer if it doesn't exist yet */
    if( this->buffer_data == NULL )
	{
		this->buffer_data = (uint8_t*) malloc( buffer_size );
		this->buffer_size = buffer_size;
	}

	/* Reallocate the buffer if the size has changed */
	if( this->buffer_size != buffer_size )
	{
		this->buffer_data = (uint8_t*) realloc( this->buffer_data, buffer_size );
		this->buffer_size = buffer_size;
	}

	XAUDIO2_BUFFER buffer = {0};
	buffer.pAudioData = &this->buffer_data[0];
	buffer.AudioBytes = this->buffer_size;

	hr = subvoice->SubmitSourceBuffer( &buffer );
	XA2_DISPDBG_RB( KE_ERROR, "An error occured when submitting buffer data!" );

	return true;
}

bool IKeXAudio2SoundBuffer::Play( bool looping )
{
    /* Play the sound effect */
    HRESULT hr = subvoice->Start();
	XA2_DISPDBG_RB( KE_ERROR, "Error playing sound effect!" );

    return true;
}

void IKeXAudio2SoundBuffer::Stop()
{
	HRESULT hr = subvoice->Stop();
	XA2_DISPDBG_R( KE_ERROR, "Error stopping sound effect!" );
}

void IKeXAudio2SoundBuffer::Pause()
{
	HRESULT hr = subvoice->Stop();
	XA2_DISPDBG_R( KE_ERROR, "Error pausing sound effect!" );
}

void IKeXAudio2SoundBuffer::SetPosition( float* position )
{
}

void IKeXAudio2SoundBuffer::SetVelocity( float* velocity )
{
}

void IKeXAudio2SoundBuffer::SetVolume( float volume )
{
}

void IKeXAudio2SoundBuffer::SetPitch( float pitch )
{
}
