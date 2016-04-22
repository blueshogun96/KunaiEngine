/*
 * OpenGL fencing/syncronization functionality
 * Notes: The renderer needs to determine the appropriate functionality to use based on the
 *		  OS and it's available extensions.  Choose wisely.
 *
 * Legacy and Core OpenGL:
 * https://www.opengl.org/registry/specs/NV/fence.txt
 * https://www.opengl.org/registry/specs/APPLE/fence.txt
 * https://www.opengl.org/registry/specs/ARB/sync.txt
 *
 * OpenGL ES:
 * https://www.khronos.org/registry/vg/extensions/KHR/EGL_KHR_fence_sync.txt
 * https://www.khronos.org/registry/gles/extensions/APPLE/APPLE_sync.txt
 *
 */


#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"
#include "KeOpenGLFence.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OGL_DISPDBG( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }
#define OGL_DISPDBG_R( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return; }
#define OGL_DISPDBG_RB( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return false; }


/*
 * NVIDIA fencing functions (GL_NV_fence)
 */
bool KeOpenGLCreateFenceNV( IKeOpenGLFence** fence )
{
#if GL_NV_fence
    GLenum error = glGetError();
    
    /* Generate a new fence */
    glGenFencesNV( 1, &(*fence)->fence );
    OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!" );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

bool KeOpenGLInsertFenceNV( IKeOpenGLFence** fence )
{
#if GL_NV_fence
    GLenum error = glGetError();
    
    /* Set the fence */
    glSetFenceNV( (*fence)->fence, GL_ALL_COMPLETED_NV );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!" );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

bool KeOpenGLTestFenceNV( IKeOpenGLFence* fence )
{
#if GL_NV_fence
    if( glTestFenceNV( fence->fence ) )
    return true;
    
    return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

void KeOpenGLBlockOnFenceNV( IKeOpenGLFence* fence )
{
#if GL_NV_fence
    glFinishFenceNV( fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

void KeOpenGLDeleteFenceNV( IKeOpenGLFence* fence )
{
#if GL_NV_fence
    glDeleteFencesNV( 1, &fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

bool KeOpenGLIsFenceNV( IKeOpenGLFence* fence )
{
#if GL_NV_fence
    if( glTestFenceNV( fence->fence ) )
    return true;
    
    return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}


/*
 * APPLE fencing functions (GL_APPLE_fence)
 */
bool KeOpenGLCreateFenceAPPLE( IKeOpenGLFence** fence )
{
#if GL_APPLE_fence
    GLenum error = glGetError();
    
    /* Generate a new fence */
    glGenFencesAPPLE( 1, &(*fence)->fence );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
}

bool KeOpenGLInsertFenceAPPLE( IKeOpenGLFence** fence )
{
#if GL_APPLE_fence
    GLenum error = glGetError();
    
    /* Generate a new fence */
    glGenFencesAPPLE( 1, &(*fence)->fence );
    OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!" );
    
    /* Set the fence */
    glSetFenceAPPLE( (*fence)->fence );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!" );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
}

bool KeOpenGLTestFenceAPPLE( IKeOpenGLFence* fence )
{
#if GL_APPLE_fence
    if( glTestFenceAPPLE( fence->fence ) )
    return true;
    
    return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
    
}

void KeOpenGLBlockOnFenceAPPLE( IKeOpenGLFence* fence )
{
#if GL_APPLE_fence
    glFinishFenceAPPLE( fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
    
}

void KeOpenGLDeleteFenceAPPLE( IKeOpenGLFence* fence )
{
#if GL_APPLE_fence
    glDeleteFencesAPPLE( 1, &fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
    
}

bool KeOpenGLIsFenceAPPLE( IKeOpenGLFence* fence )
{
#if GL_APPLE_fence
    if( glTestFenceAPPLE( fence->fence ) )
    return true;
    
    return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
    
}


/*
 * ARB synchronization functions (GL_ARB_sync)
 */
bool KeOpenGLCreateFenceARB( IKeOpenGLFence** fence )
{
#if GL_ARB_fence
    /* ARB_sync creates and inserts the fence with the same API call */
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}

bool KeOpenGLInsertFenceARB( IKeOpenGLFence** fence )
{
#if GL_ARB_fence
    GLenum error = glGetError();
    
    /* Create sync object.  It will automatically be set in the unsignaled state
     if successful. */
    (*fence)->sync = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
    OGL_DISPDBG_RB( KE_ERROR, "Error creating and setting new sync object!", glGetError() );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}

bool KeOpenGLTestFenceARB( IKeOpenGLFence* fence )
{
#if GL_ARB_sync
    int signaled = GL_UNSIGNALED;
    
    /* Test this sync object for it's status and return the result */
    glGetSynciv( fence->sync, GL_SYNC_STATUS, sizeof( int ), NULL, &signaled );
    
    return signaled ? true : false;
#else
    DISPDBG_RB( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}

void KeOpenGLBlockOnFenceARB( IKeOpenGLFence* fence )
{
#if GL_ARB_sync
    /* Stall the current thread until this sync object is signaled */
    //glWaitSync( fence->sync, 0, GL_TIMEOUT_IGNORED );
    GLenum ret = glClientWaitSync( fence->sync, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED );
#else
    DISPDBG( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
    
}

void KeOpenGLDeleteFenceARB( IKeOpenGLFence* fence )
{
#if GL_ARB_sync
    glDeleteSync( fence->sync );
#else
    DISPDBG( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}

bool KeOpenGLIsFenceARB( IKeOpenGLFence* fence )
{
#if GL_ARB_sync
    return glIsSync( fence->sync ) ? true : false;
#else
    DISPDBG_RB( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}


bool ( *KeOpenGLCreateFence[3] )( IKeOpenGLFence** ) = { KeOpenGLCreateFenceARB, KeOpenGLCreateFenceNV, KeOpenGLCreateFenceAPPLE };
bool ( *KeOpenGLInsertFence[3] )( IKeOpenGLFence** ) = { KeOpenGLInsertFenceARB, KeOpenGLInsertFenceNV, KeOpenGLInsertFenceAPPLE };
bool ( *KeOpenGLTestFence[3] )( IKeOpenGLFence* ) = { KeOpenGLTestFenceARB, KeOpenGLTestFenceNV, KeOpenGLTestFenceAPPLE };
void ( *KeOpenGLBlockOnFence[3] )( IKeOpenGLFence* ) = { KeOpenGLBlockOnFenceARB, KeOpenGLBlockOnFenceNV, KeOpenGLBlockOnFenceAPPLE };
void ( *KeOpenGLDeleteFence[3] )( IKeOpenGLFence* ) = { KeOpenGLDeleteFenceARB, KeOpenGLDeleteFenceNV, KeOpenGLDeleteFenceAPPLE };
bool ( *KeOpenGLIsFence[3] )( IKeOpenGLFence* ) = { KeOpenGLIsFenceARB, KeOpenGLIsFenceNV, KeOpenGLIsFenceAPPLE };


/*
 * Name: IKeOpenGLFence::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLFence::Destroy()
{
    /* Destroy the fence */
    KeOpenGLDeleteFence[vendor]( this );
    
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeOpenGLFence::Insert
 * Desc: Inserts GPU fence object into the execution pipeline.
 */
bool IKeOpenGLFence::Insert()
{
    /* Sanity check */
    if( !fence )
        return false;
    
    IKeOpenGLFence* f = this;
    
    return KeOpenGLInsertFence[vendor]( (IKeOpenGLFence**) &f );
}


/*
 * Name: IKeOpenGLFence::Test
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeOpenGLFence::Test()
{
    if( !fence )
        return false;
    
    return KeOpenGLTestFence[vendor]( this );
}


/*
 * Name: IKeOpenGLFence::Block
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeOpenGLFence::Block()
{
    KeOpenGLBlockOnFence[vendor]( this );
}


/*
 * Name: IKeOpenGLFence::Valid
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeOpenGLFence::Valid()
{
    if( !fence )
        return false;
    
    return KeOpenGLIsFence[vendor]( this );
}