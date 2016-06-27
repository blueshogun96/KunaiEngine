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
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OGL_DISPDBG( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }
#define OGL_DISPDBG_R( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return; }
#define OGL_DISPDBG_RB( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return false; }



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
    GLenum error = glGetError();
    
    /* Set texture before modifying it */
    glBindTexture( this->target, handle );
    
    /* GL_TEXTURE_1D */
#ifndef __MOBILE_OS__
    if( this->target == GL_TEXTURE_1D )
    {
        glTexSubImage1D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->width, this->internal_format, GL_UNSIGNED_BYTE, pixels );
        OGL_DISPDBG_RB( KE_ERROR, "Error setting 1D texture data!" );
        return true;
    }
#endif
    
    /* GL_TEXTURE_2D & GL_TEXTURE_RECTANGLE */
    if( this->target == GL_TEXTURE_2D
#ifndef __MOBILE_OS__
       || this->target == GL_TEXTURE_RECTANGLE
#endif
       )
    {
        glTexSubImage2D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->offsety, texture_data->width, texture_data->height, this->internal_format, GL_UNSIGNED_BYTE, pixels );
        OGL_DISPDBG_RB( KE_ERROR, "Error setting 2D texture data!" );
        return true;
    }
    
    /* GL_TEXTURE_3D */
    if( this->target == GL_TEXTURE_3D )
    {
        glTexSubImage3D( this->target, texture_data->mipmap, texture_data->offsetx, texture_data->offsety, texture_data->offsetz, texture_data->width, texture_data->height, texture_data->depth, this->internal_format, GL_UNSIGNED_BYTE, pixels );
        OGL_DISPDBG_RB( KE_ERROR, "Error setting 3D texture data!" );
        return true;
    }
    
    /* TODO: Cube texture sides */
    
    return false;
}


bool IKeOpenGLTexture::GetTextureDesc( KeTextureDesc* texture_desc )
{
	if( !texture_desc )
		return false;

	/* TODO: Fix this */

	texture_desc->width = width;
	texture_desc->height = height;
	texture_desc->mipmap = mipmap;

	return true;
}