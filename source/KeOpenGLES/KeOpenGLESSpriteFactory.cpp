//
//  KeOpenGLESSpriteFactory.cpp
//
//  Created by Shogun3D on 5/8/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
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
 * Name: IKeOpenGLSpriteFactory::IKeOpenGLSpriteFactory
 * Desc: Default constructor; initializes the sprite factory with a dynamic geometry buffer...
 */
IKeOpenGLESSpriteFactory::IKeOpenGLESSpriteFactory( IKeRenderDevice* renderdevice, KeVertexAttribute* vertex_attributes, uint32_t batch_size )
{
    /* Sanity checks */
    if( !renderdevice )
        DISPDBG_R( KE_ERROR, "Invalid rendering device!" );
    if( !vertex_attributes )
        DISPDBG_R( KE_ERROR, "Invalid vertex attributes!" );
    
    /* Create a dynamic geometry buffer */
    if( !renderdevice->CreateGeometryBuffer( NULL, batch_size, NULL, 0, KE_UNSIGNED_SHORT, KE_USAGE_STREAM_READ_WRITE, vertex_attributes, &this->dynamic_geometrybuffer ) )
        DISPDBG_R( KE_ERROR, "Error creating dynamic geometry buffer!" );
    
    this->renderdevice = reinterpret_cast<IKeRenderDevice*>(renderdevice);
    this->batch_size = batch_size;
}

/*
 * Name: IKeOpenGLSpriteFactory::~IKeOpenGLSpriteFactory
 * Desc:
 */
IKeOpenGLESSpriteFactory::~IKeOpenGLESSpriteFactory()
{
    if( dynamic_geometrybuffer )
        renderdevice->DeleteGeometryBuffer( dynamic_geometrybuffer );
}


/*
 * Name:
 * Desc:
 */
bool IKeOpenGLESSpriteFactory::Initialized()
{
    return initialized;
}

/*
 * Name:
 * Desc:
 */
void IKeOpenGLESSpriteFactory::SetStates( IKeStateBuffer* state )
{
    
}

void IKeOpenGLESSpriteFactory::SetProgram( IKeGpuProgram* program )
{
    
}

void IKeOpenGLESSpriteFactory::SetTexture( IKeTexture* texture )
{
    
}

void IKeOpenGLESSpriteFactory::SetVertexData( void* vertex_data, uint32_t offset, uint32_t vertex_data_size )
{
    
}

bool IKeOpenGLESSpriteFactory::Lock( void** data )
{
    return false;
}

void IKeOpenGLESSpriteFactory::Unlock( void* data )
{
    
}

void IKeOpenGLESSpriteFactory::Draw()
{
    
}
