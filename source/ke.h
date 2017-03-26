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


/*
 * Namespaced API for those that want it
 */
namespace Ke
{
    bool (*Initialize)() = KeInitialize;
    bool (*InitializeEx)( std::string, IKeRenderDevice**, IKeAudioDevice** ) = KeInitializeEx;
    void (*Uninitialize)() = KeUninitialize;
    bool (*CreateWindowAndDevice)( KeRenderDeviceDesc*, IKeRenderDevice** ) = KeCreateWindowAndDevice;
    void (*DestroyWindowAndDevice)( IKeRenderDevice* ) = KeDestroyWindowAndDevice;
    IKeRenderDevice* (*GetRenderDevice)() = KeGetRenderDevice;
    bool (*CreateAudioDevice)( KeAudioDeviceDesc*, IKeAudioDevice** ) = KeCreateAudioDevice;
    void (*DestroyAudioDevice)( IKeAudioDevice* device ) = KeDestroyAudioDevice;
    IKeAudioDevice* (*GetAudioDevice)() = KeGetAudioDevice;
}

#endif /* defined(__ke__) */
