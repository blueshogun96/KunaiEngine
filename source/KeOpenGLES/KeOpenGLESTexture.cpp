//
//  KeOpenGLESTexture.cpp
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLESRenderDevice.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OGL_DISPDBG( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define OGL_DISPDBG_R( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define OGL_DISPDBG_RB( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }


/*
 * Name: IKeOpenGLTexture::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLESTexture::Destroy()
{
    /* Delete this texture handle */
    glDeleteTextures( 1, &handle );
    
    /* Delete this instance */
    delete this;
}

void* IKeOpenGLESTexture::MapData( uint32_t flags )
{
    /* TODO */
    
    return NULL;
}

void IKeOpenGLESTexture::UnmapData( void* data_ptr )
{
    /* TODO */
}

bool IKeOpenGLESTexture::SetTextureData( KeTextureDesc* texture_data, void* pixels )
{
    /* Set texture before modifying it */
    glBindTexture( this->target, handle );

    /* GL_TEXTURE_2D & GL_TEXTURE_RECTANGLE */
    if( this->target == GL_TEXTURE_2D )
    {
        glTexSubImage2D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->offsety, texture_data->width, texture_data->height, this->internal_format, GL_UNSIGNED_BYTE, pixels );
    }
    else
    {
        DISPDBG_RB( KE_ERROR, "Texture target not supported: " << this->target );
    }
    
    
    
    /* TODO: Cube texture sides */
    
    return true;
}
