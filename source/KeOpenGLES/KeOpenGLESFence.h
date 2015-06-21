#pragma once

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



/*
 * NVIDIA fencing functions (GL_NV_fence)
 */
bool KeOpenGLInsertFenceNV( IKeOpenGLESFence** fence )
{
#if GL_NV_fence
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesNV( 1, &(*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceNV( (*fence)->fence, GL_ALL_COMPLETED_NV );
	OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!", glGetError() );

	return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

bool KeOpenGLTestFenceNV( IKeOpenGLESFence* fence )
{
#if GL_NV_fence
	if( glTestFenceNV( fence->fence ) )
		return true;

	return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

void KeOpenGLBlockOnFenceNV( IKeOpenGLESFence* fence )
{
#if GL_NV_fence
	glFinishFenceNV( fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

void KeOpenGLDeleteFenceNV( IKeOpenGLESFence* fence )
{
#if GL_NV_fence
	glDeleteFencesNV( 1, &fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_NV_fence not supported!" );
#endif
}

bool KeOpenGLIsFenceNV( IKeOpenGLESFence* fence )
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
bool KeOpenGLInsertFenceAPPLE( IKeOpenGLESFence** fence )
{
#if GL_APPLE_fence
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesAPPLE( 1, &(*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceAPPLE( (*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!", glGetError() );

	return true;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif
}

bool KeOpenGLTestFenceAPPLE( IKeOpenGLESFence* fence )
{
#if GL_APPLE_fence
	if( glTestFenceAPPLE( fence->fence ) )
		return true;

	return false;
#else
    DISPDBG_RB( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif

}

void KeOpenGLBlockOnFenceAPPLE( IKeOpenGLESFence* fence )
{
#if GL_APPLE_fence
	glFinishFenceAPPLE( fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif

}

void KeOpenGLDeleteFenceAPPLE( IKeOpenGLESFence* fence )
{
#if GL_APPLE_fence
	glDeleteFencesAPPLE( 1, &fence->fence );
#else
    DISPDBG( KE_ERROR, "GL_APPLE_fence not supported!" );
#endif

}

bool KeOpenGLIsFenceAPPLE( IKeOpenGLESFence* fence )
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
bool KeOpenGLInsertFenceARB( IKeOpenGLESFence** fence )
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

bool KeOpenGLTestFenceARB( IKeOpenGLESFence* fence )
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

void KeOpenGLBlockOnFenceARB( IKeOpenGLESFence* fence )
{
#if GL_ARB_sync
	/* Stall the current thread until this sync object is signaled */
	//glWaitSync( fence->sync, 0, GL_TIMEOUT_IGNORED );
	GLenum ret = glClientWaitSync( fence->sync, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED );
#else
    DISPDBG( KE_ERROR, "GL_ARB_sync not supported!" );
#endif

}

void KeOpenGLDeleteFenceARB( IKeOpenGLESFence* fence )
{
#if GL_ARB_sync
	glDeleteSync( fence->sync );
#else
    DISPDBG( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}

bool KeOpenGLIsFenceARB( IKeOpenGLESFence* fence )
{
#if GL_ARB_sync
	return glIsSync( fence->sync ) ? true : false;
#else
    DISPDBG_RB( KE_ERROR, "GL_ARB_sync not supported!" );
#endif
}


/*
 * Function pointer arrays
 */
#define KE_FENCE_ARB	0
#define KE_FENCE_NV		1
#define KE_FENCE_APPLE	2

bool ( *KeOpenGLInsertFence[3] )( IKeOpenGLESFence** ) = { KeOpenGLInsertFenceARB, KeOpenGLInsertFenceNV, KeOpenGLInsertFenceAPPLE };
bool ( *KeOpenGLTestFence[3] )( IKeOpenGLESFence* ) = { KeOpenGLTestFenceARB, KeOpenGLTestFenceNV, KeOpenGLTestFenceAPPLE };
void ( *KeOpenGLBlockOnFence[3] )( IKeOpenGLESFence* ) = { KeOpenGLBlockOnFenceARB, KeOpenGLBlockOnFenceNV, KeOpenGLBlockOnFenceAPPLE };
void ( *KeOpenGLDeleteFence[3] )( IKeOpenGLESFence* ) = { KeOpenGLDeleteFenceARB, KeOpenGLDeleteFenceNV, KeOpenGLDeleteFenceAPPLE };
bool ( *KeOpenGLIsFence[3] )( IKeOpenGLESFence* ) = { KeOpenGLIsFenceARB, KeOpenGLIsFenceNV, KeOpenGLIsFenceAPPLE };
