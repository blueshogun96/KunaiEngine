//
//  KeDirect3D11RenderTarget.cpp
//
//  Created by Shogun3D on 4/22/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Name: IKeDirect3D11RenderTarget::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11RenderTarget::Destroy()
{
    /* Delete this instance */
    delete this;
}

void* IKeDirect3D11RenderTarget::MapData( uint32_t flags )
{
	/* TODO */

	return NULL;
}

void IKeDirect3D11RenderTarget::UnmapData( void* data_ptr )
{
	/* TODO */
}


bool IKeDirect3D11RenderTarget::GetTexture( IKeTexture** texture )
{
	if( !texture )
		return false;

	*texture = this->texture;

	return true;
}

IKeTexture* IKeDirect3D11RenderTarget::GetTexture2() { return texture; }