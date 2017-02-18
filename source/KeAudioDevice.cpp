//
//  ke_audiodevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeAudioDevice.h"

#ifndef _UWP	/* Disable OpenAL support for Universal Windows Projects */
#include "KeOpenAL/KeOpenALAudioDevice.h"
#endif

#ifdef _WIN32
/* TODO: XAudio2 */
#include "KeXAudio2/KeXAudio2AudioDevice.h"
#endif




/*
 * Creates an audio device based on the user's API preferences 
 */
IKeAudioDevice* KeCreateAudioDevice( KeAudioDeviceDesc* audiodevice_desc )
{
#ifndef _UWP
    if( audiodevice_desc->device_type == KE_AUDIODEVICE_OPENAL )
        return new IKeOpenALAudioDevice( audiodevice_desc );
#endif
    
#ifdef _WIN32
	/* TODO: XAudio2 */
	if( audiodevice_desc->device_type == KE_AUDIODEVICE_XAUDIO2 )
		return new IKeXAudio2AudioDevice( audiodevice_desc );
#endif
    
    return NULL;
}