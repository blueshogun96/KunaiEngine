//
//  ke_renderdevice.cpp
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeRenderDevice.h"

#ifndef _UWP	/* No OpenGL support for Windows 10 apps and Xbox */
#include "KeOpenGL/KeOpenGLRenderDevice.h"
#endif

#ifdef _WIN32	/* Direct3D support only for Windows and other Microsoft platforms */
#include "KeDirect3D11/KeDirect3D11RenderDevice.h"
#endif



/*
 * Render device creation function
 * TODO: This should be moved to a dynamic library for OSes that support/allow it.
 */
IKeRenderDevice* KeCreateRenderDevice( KeRenderDeviceDesc* renderdevice_desc )
{
#ifndef _UWP
    if( renderdevice_desc->device_type == KE_RENDERDEVICE_OGL3 || renderdevice_desc->device_type == KE_RENDERDEVICE_OGL4 || renderdevice_desc->device_type == KE_RENDERDEVICE_OGLES2 || renderdevice_desc->device_type == KE_RENDERDEVICE_OGLES3 )
        return new IKeOpenGLRenderDevice( renderdevice_desc );
#endif
    
#ifdef _WIN32
	if( renderdevice_desc->device_type == KE_RENDERDEVICE_D3D11 )
		return new IKeDirect3D11RenderDevice( renderdevice_desc );
#endif

    return NULL;
}