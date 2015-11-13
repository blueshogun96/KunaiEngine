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

bool IKeDirect3D11Texture::SetTextureData( KeTextureDesc* texture_data, void* pixels )
{
	return false;
}

bool IKeDirect3D11Texture::GetTextureDesc( KeTextureDesc* texture_desc )
{
	if( !texture_desc )
		return false;

	/* TODO: Fix this */

	texture_desc->width = width;
	texture_desc->height = height;
	//texture_desc->mipmap = mipmap;

	return true;
}