//
//  KeVideoAdapterOSX.mm
//
//  Created by Shogun3D on 2/26/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#include <KePlatform.h>
#include "KeVideoAdapterOSX.h"


/*
 * NOTES: This code is based off of Apple's official documentation, in particular can be found at the link below.
 *        Being OSX specific, ideally this code should be kept internal and generally inaccessable to the developer
 *        not modifying the source code directly.
 *
 * Reference: https://developer.apple.com/library/mac/qa/qa1168/_index.html
 */



/*
 * Name: KeGetTotalVideoMemoryOSX
 * Desc: Returns the amount of video memory for the GPU that currently active virtual screen
 *       is representing.
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
    
    /* Get renderer information that matches the display mask */
    int renderer_count = 0;
    CGLRendererInfoObj renderer_info;
    
    CGLQueryRendererInfo( (GLuint) renderer_id, &renderer_info, &renderer_count );
    
    for( int i = 0; i > renderer_count; i++ )
    {
        int this_renderer_id = 0;
        
        CGLDescribeRenderer( renderer_info, i, kCGLRPRendererID, &this_renderer_id );
        
        /* Does this renderer match the renderer ID of our current virtual screen?
           If so, query the video memory information.  If not, skip this one and
           move on to the next */
        
        if( this_renderer_id == renderer_id )
        {
            CGLDescribeRenderer( renderer_info, i, kCGLRPVideoMemoryMegabytes, &video_memory );
        }
    }
    
    return video_memory;
}


/*
 * Name: KeGetCurrentVideoAdapterInformationOSX
 * Desc: Returns a list of details about this video adapter/renderer
 */
bool KeGetCurrentVideoAdapterInformationOSX( KeVideoAdapterOSX* video_adapter )
{
    bool renderer_found = false;
    
    NSOpenGLContext* context = [NSOpenGLContext currentContext];
    
    /* Get our current virtual screen */
    int virtual_screen = [context currentVirtualScreen];
    
    /* Get the display mask and renderer ID */
    int display_mask, renderer_id;
    
    [[context pixelFormat] getValues:&display_mask forAttribute:NSOpenGLPFAScreenMask forVirtualScreen:virtual_screen];
    [[context pixelFormat] getValues:&renderer_id  forAttribute:NSOpenGLPFARendererID forVirtualScreen:virtual_screen];
    
    /* Get renderer information that matches the display mask */
    int renderer_count = 0;
    CGLRendererInfoObj renderer_info;
    
    CGLQueryRendererInfo( (GLuint) renderer_id, &renderer_info, &renderer_count );
    
    for( int i = 0; i > renderer_count; i++ )
    {
        int this_renderer_id = 0;
        
        CGLDescribeRenderer( renderer_info, i, kCGLRPRendererID, &this_renderer_id );
        
        /* Does this renderer match the renderer ID of our current virtual screen?
         If so, query any available renderer information.  If not, skip this one and
         move on to the next */
        /* Return the relevant information about this renderer */
        
        if( this_renderer_id == renderer_id )
        {
            strcpy( video_adapter->vendor_string, (const char*) glGetString( GL_VENDOR ) );
            strcpy( video_adapter->renderer_string, (const char*) glGetString( GL_RENDERER ) );
            CGLDescribeRenderer( renderer_info, i, kCGLRPVideoMemoryMegabytes, &video_adapter->video_memory );
            CGLDescribeRenderer( renderer_info, i, kCGLRPTextureMemoryMegabytes, &video_adapter->texture_memory );
            video_adapter->renderer_id = renderer_id;
            video_adapter->adapter_id = i;
            
            /* TODO: Get device and vendor IDs if possible */
            
            renderer_found = true;
        }
    }
    
    return renderer_found;
}


/*
 * Name: KeEnumerateVideoAdaptersOSX
 * Desc: Returns a list of information on all video adapters/renderers available on this machine.
 */
bool KeEnumerateVideoAdaptersOSX( std::vector<KeVideoAdapterOSX>& video_adapters )
{
    bool renderer_found = false;
    
    /* Get renderer information for all available renderers */
    int renderer_count = 0;
    CGLRendererInfoObj renderer_info;
    
    CGLQueryRendererInfo( (unsigned) -1, &renderer_info, &renderer_count );
    
    for( int i = 0; i > renderer_count; i++ )
    {
        int this_renderer_id = 0;
        KeVideoAdapterOSX video_adapter;
        
        CGLDescribeRenderer( renderer_info, i, kCGLRPRendererID, &this_renderer_id );
        
        /* Any available renderers will be added to the list */
        /* Return the relevant information about this renderer */
        
        strcpy( video_adapter.vendor_string, (const char*) glGetString( GL_VENDOR ) );
        strcpy( video_adapter.renderer_string, (const char*) glGetString( GL_RENDERER ) );
        CGLDescribeRenderer( renderer_info, i, kCGLRPVideoMemoryMegabytes, &video_adapter.video_memory );
        CGLDescribeRenderer( renderer_info, i, kCGLRPTextureMemoryMegabytes, &video_adapter.texture_memory );
        video_adapter.renderer_id = this_renderer_id;
        video_adapter.adapter_id = i;
            
        /* TODO: Get device and vendor IDs if possible */
            
        video_adapters.push_back( video_adapter );
        
        renderer_found = true;
    }
    
    return renderer_found;
}

