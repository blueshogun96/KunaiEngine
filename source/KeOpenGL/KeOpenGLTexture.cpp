//
//  KeOpenGLTexture.cpp
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
 * Name: IKeOpenGLTexture::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLTexture::Destroy()
{
    /* Delete this texture handle */
    glDeleteTextures( 1, &handle );
    
    /* Delete this instance */
    delete this;
}

void* IKeOpenGLTexture::MapData( uint32_t flags )
{
    /* TODO */
    
    return NULL;
}

void IKeOpenGLTexture::UnmapData( void* data_ptr )
{
    /* TODO */
}

bool IKeOpenGLTexture::SetTextureData( KeTextureDesc* texture_data, void* pixels )
{
    /* TODO: Set the texture data based on the texture's actual type */
    
    return false;
}