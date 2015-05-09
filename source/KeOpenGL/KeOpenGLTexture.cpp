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
    /* Set texture before modifying it */
    glBindTexture( this->target, handle );
    
    /* GL_TEXTURE_1D */
    if( this->target == GL_TEXTURE_1D )
    {
        glTexSubImage1D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->width, this->internal_format, GL_UNSIGNED_BYTE, pixels );
    }
    
    /* GL_TEXTURE_2D & GL_TEXTURE_RECTANGLE */
    if( this->target == GL_TEXTURE_2D || this->target == GL_TEXTURE_RECTANGLE )
    {
        glTexSubImage2D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->offsety, texture_data->width, texture_data->height, this->internal_format, GL_UNSIGNED_BYTE, pixels );
    }
    
    /* GL_TEXTURE_3D */
    if( this->target == GL_TEXTURE_3D )
    {
        glTexSubImage3D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->offsety, texture_data->offsetz, texture_data->width, texture_data->height, texture_data->depth, this->internal_format, GL_UNSIGNED_BYTE, pixels );
    }
    
    /* TODO: Cube texture sides */
    
    return false;
}
