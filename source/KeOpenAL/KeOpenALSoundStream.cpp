//
//  KeOpenALSoundStream.cpp
//
//  Created by Shogun3D on 4/19/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeAudioDevice.h"
#include "KeOpenALAudioDevice.h"


/*
 * Name: IKeOpenALSoundStream::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenALSoundStream::Destroy()
{
    /* Delete this instance */
    delete this;
}