//
//  KeDirect3D11GeometryBuffer.cpp
//
//  Created by Shogun3D on 4/22/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define D3D_DISPDBG( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define D3D_DISPDBG_R( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define D3D_DISPDBG_RB( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }

#if 0
/* Direct3D11 buffer access flags */
uint32_t access_flags[3] =
{
    0,
    0,
    0
};

/* Direct3D11 buffer target */
uint32_t buffer_target[3] =
{
    0,
    0,
    0,
};
#endif

/*
 * Name: IKeDirect3D11GeometryBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11GeometryBuffer::Destroy()
{
    /* Delete this instance */
    delete this;
}

void* IKeDirect3D11GeometryBuffer::MapData( uint32_t flags )
{
    return NULL;
}

void IKeDirect3D11GeometryBuffer::UnmapData( void* data_ptr )
{
    
}

bool IKeDirect3D11GeometryBuffer::SetVertexData( uint32_t offset, uint32_t size, void* ptr )
{
    return true;
}

bool IKeDirect3D11GeometryBuffer::SetIndexData( uint32_t offset, uint32_t size, void* ptr )
{
    return true;
}

void IKeDirect3D11GeometryBuffer::GetDesc( KeGeometryBufferDesc* desc )
{
	if( desc )
	{
		/* TODO */
		return;
	}

	DISPDBG( KE_WARNING, "Invalid pointer..." );
}