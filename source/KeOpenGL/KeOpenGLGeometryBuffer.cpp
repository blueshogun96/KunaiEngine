//
//  KeOpenGLGeometryBuffer.cpp
//  Aquatic
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
 * Name: IKeOpenGLGeometryBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLGeometryBuffer::Destroy()
{
    /* Delete the VBO and VAO */
    glDeleteBuffers( 1, &vbo[0] );
    if( vbo[1] ) glDeleteBuffers( 1, &vbo[1] );
    glDeleteVertexArrays( 1, &vao );
    
    /* Delete this instance */
    delete this;
}

void* IKeOpenGLGeometryBuffer::MapData( uint32_t flags )
{
    /* TODO */

    return NULL;
}

void IKeOpenGLGeometryBuffer::UnmapData( void* data_ptr )
{
    /* TODO */
}