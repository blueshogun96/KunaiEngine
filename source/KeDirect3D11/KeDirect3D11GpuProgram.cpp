//
//  KeDirect3D11GpuProgram.cpp
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



/*
 * Name: IKeDirect3D11GpuProgram::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11GpuProgram::Destroy()
{
	/* Delete vertex attributes */
	delete this->va;

    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeDirect3D11GpuProgram::GetVertexAttributes
 * Desc: 
 */
void IKeDirect3D11GpuProgram::GetVertexAttributes( KeVertexAttribute* vertex_attributes )
{
	vertex_attributes = va;
}