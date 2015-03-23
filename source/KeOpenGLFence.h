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
bool KeOpenGLInsertFenceNV( IKeOpenGLFence** fence )
{
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesNV( 1, &(*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceNV( (*fence)->fence, GL_ALL_COMPLETED_NV );
	OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!", glGetError() );

	return true;
}

bool KeOpenGLTestFenceNV( IKeOpenGLFence* fence )
{
	if( glTestFenceNV( fence->fence ) )
		return true;

	return false;
}

void KeOpenGLBlockOnFenceNV( IKeOpenGLFence* fence )
{
	glFinishFenceNV( fence->fence );
}

void KeOpenGLDeleteFenceNV( IKeOpenGLFence* fence )
{
	glDeleteFencesNV( 1, &fence->fence );
}

bool KeOpenGLIsFenceNV( IKeOpenGLFence* fence )
{
	if( glTestFenceNV( fence->fence ) )
		return true;

	return false;
}


/*
 * APPLE fencing functions (GL_APPLE_fence)
 */
bool KeOpenGLInsertFenceAPPLE( IKeOpenGLFence** fence )
{
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesAPPLE( 1, &(*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceAPPLE( (*fence)->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error setting new fence!", glGetError() );

	return true;
}

bool KeOpenGLTestFenceAPPLE( IKeOpenGLFence* fence )
{
	if( glTestFenceAPPLE( fence->fence ) )
		return true;

	return false;
}

void KeOpenGLBlockOnFenceAPPLE( IKeOpenGLFence* fence )
{
	glFinishFenceAPPLE( fence->fence );
}

void KeOpenGLDeleteFenceAPPLE( IKeOpenGLFence* fence )
{
	glDeleteFencesAPPLE( 1, &fence->fence );
}

bool KeOpenGLIsFenceAPPLE( IKeOpenGLFence* fence )
{
	if( glTestFenceAPPLE( fence->fence ) )
		return true;

	return false;
}


/*
 * ARB synchronization functions (GL_ARB_sync)
 */
bool KeOpenGLInsertFenceARB( IKeOpenGLFence** fence )
{
	GLenum error = glGetError();

	/* Create sync object.  It will automatically be set in the unsignaled state
	   if successful. */
	(*fence)->sync = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
	OGL_DISPDBG_RB( KE_ERROR, "Error creating and setting new sync object!", glGetError() );

	return true;
}

bool KeOpenGLTestFenceARB( IKeOpenGLFence* fence )
{
	int signaled = GL_UNSIGNALED;

	/* Test this sync object for it's status and return the result */
	glGetSynciv( fence->sync, GL_SYNC_STATUS, sizeof( int ), NULL, &signaled );

	return signaled ? true : false;
}

void KeOpenGLBlockOnFenceARB( IKeOpenGLFence* fence )
{
	/* Stall the current thread until this sync object is signaled */
	//glWaitSync( fence->sync, 0, GL_TIMEOUT_IGNORED );
	GLenum ret = glClientWaitSync( fence->sync, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED );
}

void KeOpenGLDeleteFenceARB( IKeOpenGLFence* fence )
{
	glDeleteSync( fence->sync );
}

bool KeOpenGLIsFenceARB( IKeOpenGLFence* fence )
{
	return glIsSync( fence->sync ) ? true : false;
}


/*
 * Function pointer arrays
 */
#define KE_FENCE_ARB	0
#define KE_FENCE_NV		1
#define KE_FENCE_APPLE	2

bool ( *KeOpenGLInsertFence[3] )( IKeOpenGLFence** ) = { KeOpenGLInsertFenceARB, KeOpenGLInsertFenceNV, KeOpenGLInsertFenceAPPLE };
bool ( *KeOpenGLTestFence[3] )( IKeOpenGLFence* ) = { KeOpenGLTestFenceARB, KeOpenGLTestFenceNV, KeOpenGLTestFenceAPPLE };
void ( *KeOpenGLBlockOnFence[3] )( IKeOpenGLFence* ) = { KeOpenGLBlockOnFenceARB, KeOpenGLBlockOnFenceNV, KeOpenGLBlockOnFenceAPPLE };
void ( *KeOpenGLDeleteFence[3] )( IKeOpenGLFence* ) = { KeOpenGLDeleteFenceARB, KeOpenGLDeleteFenceNV, KeOpenGLDeleteFenceAPPLE };
bool ( *KeOpenGLIsFence[3] )( IKeOpenGLFence* ) = { KeOpenGLIsFenceARB, KeOpenGLIsFenceNV, KeOpenGLIsFenceAPPLE };
