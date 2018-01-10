//
//  KeDirect3D11State.cpp
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
 * Name: IKeDirect3D11RenderState::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11RenderStateBuffer::Destroy()
{
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeDirect3D11RenderState::PVT_InitializeWithDefaults
 * Desc: Initialize state buffer settings with the defaults as defined
 *		 by the C++ constructors (i.e. CD3D11_DEPTH_STENCIL_DESC, etc.)
 */
bool IKeDirect3D11RenderStateBuffer::PVT_CreateWithDefaults( CD3D11Device device )
{
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dssdesc(def);
	CD3D11_BLEND_DESC bsdesc(def);
	CD3D11_RASTERIZER_DESC rsdesc(def);
	HRESULT hr = S_OK;

	/* Uninitialize any previous states that may exist */
	this->bs = 0; this->dss = 0; this->rs = 0;

	/* Initialize each state with default values */
	hr = device->CreateBlendState( &bsdesc, &bs );
	D3D_DISPDBG( KE_ERROR, "Error creating blend state!", hr );

	hr = device->CreateRasterizerState( &rsdesc, &rs );
	D3D_DISPDBG( KE_ERROR, "Error creating raster state!", hr );

	hr = device->CreateDepthStencilState( &dssdesc, &dss );
	D3D_DISPDBG( KE_ERROR, "Error creating depth stencil state!", hr );

	return true;
}

/*
 * Name: IKeDirect3D11RenderState::PVT_InitializeWithDefaults
 * Desc: Initialize state buffer settings with the defaults as defined
 *		 by the C++ constructors (i.e. CD3D11_DEPTH_STENCIL_DESC, etc.)
 */
bool IKeDirect3D11RenderStateBuffer::PVT_CreateWithDefaults( CD3D11Device1 device )
{
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dssdesc(def);
	CD3D11_BLEND_DESC bsdesc(def);
	CD3D11_RASTERIZER_DESC rsdesc(def);
	HRESULT hr = S_OK;

	/* Uninitialize any previous states that may exist */
	this->bs = 0; this->dss = 0; this->rs = 0;

	/* Initialize each state with default values */
	hr = device->CreateBlendState( &bsdesc, &bs );
	D3D_DISPDBG( KE_ERROR, "Error creating blend state!", hr );

	hr = device->CreateRasterizerState( &rsdesc, &rs );
	D3D_DISPDBG( KE_ERROR, "Error creating raster state!", hr );

	hr = device->CreateDepthStencilState( &dssdesc, &dss );
	D3D_DISPDBG( KE_ERROR, "Error creating depth stencil state!", hr );

	return true;
}

/*
 * Name: IKeDirect3D11RenderState::PVT_InitializeWithDefaults
 * Desc: Initialize state buffer settings with the defaults as defined
 *		 by the C++ constructors (i.e. CD3D11_DEPTH_STENCIL_DESC, etc.)
 */
bool IKeDirect3D11RenderStateBuffer::PVT_CreateWithDefaults( CD3D11Device2 device )
{
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dssdesc(def);
	CD3D11_BLEND_DESC bsdesc(def);
	CD3D11_RASTERIZER_DESC rsdesc(def);
	HRESULT hr = S_OK;

	/* Uninitialize any previous states that may exist */
	this->bs = 0; this->dss = 0; this->rs = 0;

	/* Initialize each state with default values */
	hr = device->CreateBlendState( &bsdesc, &bs );
	D3D_DISPDBG( KE_ERROR, "Error creating blend state!", hr );

	hr = device->CreateRasterizerState( &rsdesc, &rs );
	D3D_DISPDBG( KE_ERROR, "Error creating raster state!", hr );

	hr = device->CreateDepthStencilState( &dssdesc, &dss );
	D3D_DISPDBG( KE_ERROR, "Error creating depth stencil state!", hr );

	return true;
}

/*
 * Name: IKeDirect3D11TextureSamler::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11TextureSamplerBuffer::Destroy()
{
	/* Delete this instance */
	delete this;
}