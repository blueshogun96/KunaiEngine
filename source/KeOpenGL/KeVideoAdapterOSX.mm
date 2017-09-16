//
//  KeVideoAdapterOSX.mm
//
//  Created by Shogun3D on 2/26/16.
//  Copyright (c) 2016 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeDebug.h"
#include "KeVideoAdapterOSX.h"

#include <IOKit/graphics/IOGraphicsLib.h>
#include <IOKit/iokitmig.h>


/*
 * NOTES: This code is based off of Apple's official documentation, in particular can be found at the link below.
 *        Being OSX specific, ideally this code should be kept internal and generally inaccessable to the developer
 *        that isn't modifying the source code directly.
 *
 * Reference: https://developer.apple.com/library/mac/qa/qa1168/_index.html
 */


#define CGL_DISPDBG( a, b ) if(error != kCGLNoError) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }

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
    
    CGLError error = CGLQueryRendererInfo( (GLuint) renderer_id, &renderer_info, &renderer_count );
    CGL_DISPDBG( KE_ERROR, "Error querying renderer info!" );
    
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
    
    for( int i = 0; i < renderer_count; i++ )
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

bool KeEnumerateVideoAdaptersOSX( std::vector<KeVideoAdapterOSX>& video_adapters )
{
    bool renderer_found = false;
    
    /* Get renderer information for all available renderers */
    int renderer_count = 0;
    CGLRendererInfoObj renderer_info;
    
    CGLQueryRendererInfo( (unsigned) -1, &renderer_info, &renderer_count );
    
    for( int i = 0; i < renderer_count; i++ )
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

void KeEnableMultithreadedEngineOSX( bool enable )
{
    CGLContextObj context = [[NSOpenGLContext currentContext] CGLContextObj];
    CGLError error;
    
    if( enable )
        error = CGLEnable( context, kCGLCEMPEngine );
    else
        error = CGLDisable( context, kCGLCEMPEngine );
    
    CGL_DISPDBG( KE_WARNING, "Error " << (enable ? "enabling" : "disabling") << " the OpenGL multithreading engine!" );
}

bool KeMultithreadedEngineEnabledOSX( int* enabled )
{
    CGLError error = CGLIsEnabled( [[NSOpenGLContext currentContext] CGLContextObj], kCGLCEMPEngine, enabled );
    
    CGL_DISPDBG( KE_WARNING, "Error querying OpenGL multithreading engine state!" );
    
    return error != kCGLNoError ? true : false;
}

void KeInitializeVerticalBlankInterrupt( int display_number )
{
    /* TODO: This stuff is so poorly documented, it's ridiculous.  Find out how this
       IOFrameBuffer stuff works and how the actual fudge you initialize it! */
#if 0
    /* Supporting only the main display for now */
    if( display_number != 0 )
        return;
    
    kern_return_t err;
    
    /* Start off by getting our display ID */
    io_service_t display_service_port = CGDisplayIOServicePort( kCGDirectMainDisplay );
    //io_service_t root = IODisplayForFramebuffer( display_service_port, kNilOptions );
    
    /* This only works when ARC is disabled, and I'm too lazy to deal with that now... */
    //IORegistryEntryCreateCFProperties( root, (CFMutableDictionaryRef*) &io_registry_dict, kCFAllocatorDefault, kNilOptions );
    
    /* So instead of the above, let's use the most basic C API */
    CGDirectDisplayID display_id = CGMainDisplayID();
    task_port_t task_port = mach_task_self();
    io_connect_t display_connection = 0;
    
    /* Open the framebuffer */
    err = IOFramebufferOpen( display_service_port, task_port, kIOFBSharedConnectType, &display_connection );
    if( err != KERN_SUCCESS )
        return;
    
    semaphore_t semaphore;
    err = io_connect_get_notification_semaphore( mach_host_self(), kIOFBVBLInterruptType, &semaphore );
    //IOConnectMapMemory
#endif
    
}
