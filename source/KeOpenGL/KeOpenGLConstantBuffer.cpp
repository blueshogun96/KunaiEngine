//
//  KeOpenGLConstantBuffer.cpp
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
 * Laziness for compatibility with core OpenGL...
 */
#ifdef __MOBILE_OS__
#define glMapBuffer glMapBufferOES
#define glUnmapBuffer glUnmapBufferOES
#endif



/* OpenGL buffer access flags */
uint32_t access_flags[3] =
{
#ifndef __MOBILE_OS__
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE
#else
    0, //GL_READ_ONLY_OES,
    GL_WRITE_ONLY_OES,
    0, //GL_READ_WRITE_OES
#endif
};



/*
 * Name: IKeOpenGLConstantBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLConstantBuffer::Destroy()
{
    /* Delete the UBO */
    glDeleteBuffers( 1, &ubo );
    
    /* Delete this instance */
    delete this;
}


/*
 * Name: IKeOpenGLConstantBuffer::MapData
 * Desc: Returns a pointer to the data contained within this constant buffer.
 */
void* IKeOpenGLConstantBuffer::MapData( uint32_t flags )
{
    GLenum error = glGetError();
    
    /* Bind the UBO */
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    OGL_DISPDBG( KE_ERROR, "Error binding UBO!" );
    
    /* Get a pointer to the UBO's data */
    void* ptr = glMapBuffer( GL_UNIFORM_BUFFER, access_flags[flags] );
    OGL_DISPDBG( KE_ERROR, "Error mapping UBO data!" );
    
    return ptr;
}

/*
 * Name: IKeOpenGLConstantBufffer::UnmapData
 * Desc: Call this after you are finished with the pointer from MapData above.
 */
void IKeOpenGLConstantBuffer::UnmapData( void* ptr )
{
    GLenum error = glGetError();
    
    glUnmapBuffer( GL_UNIFORM_BUFFER );
    OGL_DISPDBG( KE_ERROR, "Error unmapping UBO data!" );
    
    /* TODO: Unbind buffer? */
}

/*
 * Name: IKeOpenGLConstantBuffer::SetConstantData
 * Desc: Updates the constant buffer's data contents.
 */
bool IKeOpenGLConstantBuffer::SetConstantData( uint32_t offset, uint32_t size, void* ptr )
{
    GLenum error = glGetError();
    
    /* Bind the UBO and update it with a call to glBufferSubData */
    glBindBuffer( GL_UNIFORM_BUFFER, ubo );
    OGL_DISPDBG_RB( KE_ERROR, "Error binding UBO!" );
    glBufferSubData( GL_UNIFORM_BUFFER, offset, size, ptr );
    OGL_DISPDBG_RB( KE_ERROR, "Error updating UBO data!" );
    
    return true;
}

/*
 * Name: IKeOpenGLConstantBuffer::GetDesc
 * Desc: 
 */
void IKeOpenGLConstantBuffer::GetDesc( KeConstantBufferDesc* desc )
{
    if( !desc )
        return;
    
    desc->data_size = data_size;
    desc->flags = flags;
    memmove( desc->block_name, block_name, sizeof(char) * 64 );
}
