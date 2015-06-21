//
//  KeOpenGLESGeometryBuffer.cpp
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


/* OpenGL buffer access flags */
uint32_t access_flags[3] =
{
    0, //GL_READ_ONLY_OES,
    GL_WRITE_ONLY_OES,
    0, //GL_READ_WRITE_OES
};

/* OpenGL buffer target */
uint32_t buffer_target[3] =
{
    0,
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
};

/*
 * Name: IKeOpenGLGeometryBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLESGeometryBuffer::Destroy()
{
    /* Delete the VBO and VAO */
    glDeleteBuffers( 1, &vbo[0] );
    if( vbo[1] ) glDeleteBuffers( 1, &vbo[1] );
    glDeleteVertexArraysOES( 1, &vao );
    
    /* Delete this instance */
    delete this;
}

void* IKeOpenGLESGeometryBuffer::MapData( uint32_t flags )
{
    /* In order to map/lock vertex or index buffer data, we have to set it as the current buffer.
       this does not seem ideal (at least to me, it isn't), so for now, be careful of any synchronization 
       issues due to Khronos's API design choices. */
    
    uint8_t b1 = (flags&0xF0)/16;
    uint8_t b2 = flags&0x0F;
    
    /* Which buffer are we asking for? */
    uint32_t bo_type = buffer_target[b1];
    
    /* Sanity check */
    if( !bo_type )
    {
        DISPDBG( KE_ERROR, "No specific buffer type was specified!" );
        return NULL;
    }
    
    /* Bind the desired buffer */
    glBindBuffer( bo_type, this->vbo[b1-1] );
    
    /* Lock the desired buffer */
    void* ptr = glMapBufferOES( bo_type, access_flags[b2] );
    if( !ptr )
    {
        OGL_DISPDBG( KE_ERROR, "Error mapping buffer data!", glGetError() );
        return NULL;
    }
    
    /* Save the flags used */
    lock_flags = flags;

    return ptr;
}

void IKeOpenGLESGeometryBuffer::UnmapData( void* data_ptr )
{
    uint8_t b1 = (lock_flags&0xF0)/16;
    
    /* Which buffer are we asking for? */
    uint32_t bo_type = buffer_target[b1];
    
    /* Unmap this buffer */
    glUnmapBufferOES(bo_type);
    OGL_DISPDBG( KE_ERROR, "Error unmapping buffer data!", glGetError() );
    
    /* TODO: Unbind the buffer? */
}

bool IKeOpenGLESGeometryBuffer::SetVertexData( uint32_t offset, uint32_t size, void* ptr )
{
    /* Set our vertex buffer data */
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferSubData( GL_ARRAY_BUFFER, offset, size, ptr );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting vertex buffer data!", glGetError() );
    
    return true;
}

bool IKeOpenGLESGeometryBuffer::SetIndexData( uint32_t offset, uint32_t size, void* ptr )
{
    /* Set our vertex buffer data */
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset, size, ptr );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting indexex buffer data!", glGetError() );
    
    return true;
}
