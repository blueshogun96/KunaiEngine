//
//  ke_al_audiodevice.cpp
//
//  Created by Shogun3D on 6/28/15.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeXAudio2AudioDevice.h"
#include "KeDebug.h"



/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define XA2_DISPDBG( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); }
#define XA2_DISPDBG_R( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); return; }
#define XA2_DISPDBG_RB( a, b ) if(FAILED(hr)) { DISPDBG( a, b << "\nError code: (" << hr << ")" ); return false; }


/*
 * Name: ke_al_renderdevice::
 * Desc:
 */
IKeXAudio2AudioDevice::IKeXAudio2AudioDevice() 
{
    this->initialized = No;
}


IKeXAudio2AudioDevice::IKeXAudio2AudioDevice( KeAudioDeviceDesc* audiodevice_desc ) : initialized(No)
{
    uint32_t flags = 0;

#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
	flags = XAUDIO2_DEBUG_ENGINE;
#endif

	/* Initialize COM */
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	/* Initialize XAudio2 */
	HRESULT hr = XAudio2Create( &xa2, flags );
	if( FAILED( hr ) )
	{
		XA2_DISPDBG( KE_ERROR, "Error initializing XAudio2!" );
		CoUninitialize();

		return;
	}

	/* Create a mastering voice */
	hr = xa2->CreateMasteringVoice( &mv );
	if( FAILED( hr ) )
	{
		XA2_DISPDBG( KE_ERROR, "Error creating mastering voice!" );
		xa2 = 0;
		CoUninitialize();

		return;
	}
}

IKeXAudio2AudioDevice::~IKeXAudio2AudioDevice()
{
    /* Uninitialize XAudio2 */
	if( mv )
		mv->DestroyVoice();
	xa2 = 0;	/* Calls ::Release() */

	/* Uninitialize COM */
	CoUninitialize();
}

bool IKeXAudio2AudioDevice::ConfirmDevice()
{
    return initialized;
}

bool IKeXAudio2AudioDevice::CreateSoundBuffer( WAVEFORMATEX* wfx, IKeSoundBuffer** sound_buffer )
{
    IKeXAudio2SoundBuffer* sb;

    /* Allocate a new sound buffer instance */
    (*sound_buffer) = new IKeXAudio2SoundBuffer;
    sb = static_cast<IKeXAudio2SoundBuffer*>( *sound_buffer );
    
	/* Create mastering voice */
	memmove( &sb->wfx, wfx, sizeof( WAVEFORMATEX ) );
	HRESULT hr = xa2->CreateSourceVoice( &sb->subvoice, &sb->wfx );
	XA2_DISPDBG_RB( KE_ERROR, "Error creating source voice!" );

    /* Default 3D sound position and velocity */
    memset( sb->position, 0, sizeof( float ) * 3 );
	memset( sb->velocity, 0, sizeof( float ) * 3 );

	sb->buffer_data = NULL;
	sb->buffer_size = 0;
    
    return true;
}


void IKeXAudio2AudioDevice::SetListenerPosition( float* position )
{
    /* Set the listener position */
}

void IKeXAudio2AudioDevice::SetListenerVelocity( float* velocity )
{
    /* Set the listener velocity */
}

void IKeXAudio2AudioDevice::SetListenerOrientation( float* at, float* up )
{
    float orientation[6];
    
    /* Set the listener orientation */
    memmove( orientation, at, sizeof( float ) * 3 );
    memmove( &orientation[3], up, sizeof( float ) * 3 );
}
