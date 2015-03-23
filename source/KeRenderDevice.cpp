//
//  ke_renderdevice.cpp
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeRenderDevice.h"

#ifdef __MOBILE_OS__
#include "ke_ogles2_renderdevice.h"
#else
#include "KeOpenGLRenderDevice.h"
#endif

#ifdef _WIN32
#include "KeDirect3D11RenderDevice.h"
#endif



/*
 * Render device creation function
 * TODO: This should be moved to a dynamic library for OSes that support/allow it.
 */
IKeRenderDevice* KeCreateRenderDevice( KeRenderDeviceDesc* renderdevice_desc )
{
#ifndef __MOBILE_OS__
    if( renderdevice_desc->device_type == KE_RENDERDEVICE_OGL3 || renderdevice_desc->device_type == KE_RENDERDEVICE_OGL4 )
        return new IKeOpenGLRenderDevice( renderdevice_desc );
#else
    if( renderdevice_desc->device_type == KE_RENDERDEVICE_OGLES2 )
       return new ke_ogles2_renderdevice_t( renderdevice_desc );
#endif
    
#ifdef _WIN32
	if( renderdevice_desc->device_type == KE_RENDERDEVICE_D3D11 )
		return new IKeDirect3D11RenderDevice( renderdevice_desc );
#endif

    return NULL;
}