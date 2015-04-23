//
//  KeDirect3D11Texture.cpp
//
//  Created by Shogun3D on 4/22/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Name: IKeDirect3D11Texture::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11Texture::Destroy()
{
    /* Delete this instance */
    delete this;
}

void* IKeDirect3D11Texture::MapData( uint32_t flags )
{
    /* TODO */
    
    return NULL;
}

void IKeDirect3D11Texture::UnmapData( void* data_ptr )
{
    /* TODO */
}