//
//  KeVideoAdapterOSX.mm
//
//  Created by Shogun3D on 2/26/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#include <KePlatform.h>
#include "KeVideoAdapterOSX.h"

/*
 * Name: KeGetTotalVideoMemoryOSX
 * Desc: Returns the amount of video memory for the GPU that currently active virtual screen
 *       is representing.
 * Reference: https://developer.apple.com/library/mac/qa/qa1168/_index.html
 */

int KeGetTotalVideoMemoryOSX()
{
    int video_memory = 0;
    NSOpenGLContext* context = [NSOpenGLContext currentContext];
    
    /* Get our current virtual screen */
    int virtual_screen = [context currentVirtualScreen];
    
    /* Get the display mask and renderer ID */
    int display_mask, renderer_id;
    
    [[context pixelFormat] getValues:&display_mask forAttribute:NSOpenGLPFAScreenMask forVirtualScreen:virtual_screen];
    [[context pixelFormat] getValues:&renderer_id  forAttribute:NSOpenGLPFARendererID forVirtualScreen:virtual_screen];
    
    return video_memory;
}

int KeGetCurrentVideoAdapterInformationOSX( KeVideoAdapterOSX* video_adapter )
{
    return 0;
}

bool KeEnumerateVideoAdaptersOSX( int* adapter_count, KeVideoAdapterOSX* video_adapters )
{
    return true;
}