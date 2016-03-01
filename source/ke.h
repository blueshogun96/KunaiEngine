//
//  ke.h
//
//  Created by Shogun3D on 7/11/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke__
#define __ke__

#include "KePlatform.h"
#include "KeSystem.h"
#include "KeRenderDevice.h"
#include "KeAudioDevice.h"

/* Debug output level */
#ifndef KE_DBG_LEVEL
#define KE_DBG_LEVEL 1
#endif

/*
 * Kunai engine basics
 */
bool KeInitialize();
bool KeInitializeEx( std::string settings_file, IKeRenderDevice** rd, IKeAudioDevice** ad );
void KeUninitialize();
bool KeCreateWindowAndDevice( KeRenderDeviceDesc* device_desc, IKeRenderDevice** device );
void KeDestroyWindowAndDevice( IKeRenderDevice* device );
IKeRenderDevice* KeGetRenderDevice( void );
bool KeCreateAudioDevice( KeAudioDeviceDesc* device_desc, IKeAudioDevice** device );
void KeDestroyAudioDevice( IKeAudioDevice* device );
IKeAudioDevice* KeGetAudioDevice( void );


#endif /* defined(__ke__) */
