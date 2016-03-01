//
//  KeVideoAdapterOSX.h
//
//  Created by Shogun3D on 2/26/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#pragma once

#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/gl3.h>

#ifdef __OBJC__
#include <AppKit/NSOpenGL.h>
#endif

#include <vector>


/*
 * Video adapter information for OSX
 */
struct KeVideoAdapterOSX
{
    char vendor_string[64];
    char renderer_string[64];
    int  device_id;
    int  vendor_id;
    int  adapter_id;
    int  renderer_id;
    int  video_memory;
    int  texture_memory;
};


/*
 * Name: KeGetTotalVideoMemoryOSX
 * Desc: Returns the amount of video memory for the GPU that currently active virtual screen
 *       is representing.
 */

int KeGetTotalVideoMemoryOSX();

bool KeGetCurrentVideoAdapterInformationOSX( KeVideoAdapterOSX* video_adapter );

bool KeEnumerateVideoAdaptersOSX( std::vector<KeVideoAdapterOSX>& video_adapters );