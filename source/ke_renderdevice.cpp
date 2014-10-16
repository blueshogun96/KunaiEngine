//
//  ke_renderdevice.cpp
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_renderdevice.h"

#ifdef __MOBILE_OS__
#include "ke_ogles2_renderdevice.h"
#else
#include "ke_ogl_renderdevice.h"
#endif

#ifdef _WIN32
#include "ke_d3d11_renderdevice.h"
#endif



/*
 * Render device creation function
 * TODO: This should be moved to a dynamic library for OSes that support/allow it.
 */
ke_renderdevice_t* ke_create_renderdevice( ke_renderdevice_desc_t* renderdevice_desc )
{
#ifndef __MOBILE_OS__
    if( renderdevice_desc->device_type == KE_RENDERDEVICE_OGL3 || renderdevice_desc->device_type == KE_RENDERDEVICE_OGL4 )
        return new ke_ogl_renderdevice_t( renderdevice_desc );
#else
    if( renderdevice_desc->device_type == KE_RENDERDEVICE_OGLES2 )
       return new ke_ogles2_renderdevice_t( renderdevice_desc );
#endif
    
#ifdef _WIN32
	if( renderdevice_desc->device_type == KE_RENDERDEVICE_D3D11 )
		return new ke_d3d11_renderdevice_t( renderdevice_desc );
#endif

    return NULL;
}