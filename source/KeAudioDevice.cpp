//
//  ke_audiodevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeAudioDevice.h"
#include "KeOpenAL/KeOpenALAudioDevice.h"
#ifdef _WIN32
/* TODO: XAudio2 */
#endif




/*
 * Creates an audio device based on the user's API preferences 
 */
IKeAudioDevice* KeCreateAudioDevice( KeAudioDeviceDesc* audiodevice_desc )
{
    if( audiodevice_desc->device_type == KE_AUDIODEVICE_OPENAL )
        return new IKeOpenALAudioDevice( audiodevice_desc );
    
#ifdef _WIN32
	/* TODO: XAudio2 */
#endif
    
    return NULL;
}