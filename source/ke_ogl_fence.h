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
bool ke_ogl_insert_fence_nv( ke_ogl_fence_t** fence )
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

bool ke_ogl_test_fence_nv( ke_ogl_fence_t* fence )
{
	if( glTestFenceNV( fence->fence ) )
		return true;

	return false;
}

void ke_ogl_block_on_fence_nv( ke_ogl_fence_t* fence )
{
	glFinishFenceNV( fence->fence );
}

void ke_ogl_delete_fence_nv( ke_ogl_fence_t* fence )
{
	glDeleteFencesNV( 1, &fence->fence );
}

bool ke_ogl_is_fence_nv( ke_ogl_fence_t* fence )
{
	if( glTestFenceNV( fence->fence ) )
		return true;

	return false;
}


/*
 * APPLE fencing functions (GL_APPLE_fence)
 */
bool ke_ogl_insert_fence_apple( ke_ogl_fence_t** fence )
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

bool ke_ogl_test_fence_apple( ke_ogl_fence_t* fence )
{
	if( glTestFenceAPPLE( fence->fence ) )
		return true;

	return false;
}

void ke_ogl_block_on_fence_apple( ke_ogl_fence_t* fence )
{
	glFinishFenceAPPLE( fence->fence );
}

void ke_ogl_delete_fence_apple( ke_ogl_fence_t* fence )
{
	glDeleteFencesAPPLE( 1, &fence->fence );
}

bool ke_ogl_is_fence_apple( ke_ogl_fence_t* fence )
{
	if( glTestFenceAPPLE( fence->fence ) )
		return true;

	return false;
}


/*
 * ARB synchronization functions (GL_ARB_sync)
 */
bool ke_ogl_insert_fence_arb( ke_ogl_fence_t** fence )
{
	GLenum error = glGetError();

	/* Create sync object.  It will automatically be set in the unsignaled state
	   if successful. */
	(*fence)->sync = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
	OGL_DISPDBG_RB( KE_ERROR, "Error creating and setting new sync object!", glGetError() );

	return true;
}

bool ke_ogl_test_fence_arb( ke_ogl_fence_t* fence )
{
	int signaled = GL_UNSIGNALED;

	/* Test this sync object for it's status and return the result */
	glGetSynciv( fence->sync, GL_SYNC_STATUS, sizeof( int ), NULL, &signaled );

	return signaled ? true : false;
}

void ke_ogl_block_on_fence_arb( ke_ogl_fence_t* fence )
{
	/* Stall the current thread until this sync object is signaled */
	//glWaitSync( fence->sync, 0, GL_TIMEOUT_IGNORED );
	GLenum ret = glClientWaitSync( fence->sync, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED );
}

void ke_ogl_delete_fence_arb( ke_ogl_fence_t* fence )
{
	glDeleteSync( fence->sync );
}

bool ke_ogl_is_fence_arb( ke_ogl_fence_t* fence )
{
	return glIsSync( fence->sync ) ? true : false;
}


/*
 * Function pointer arrays
 */
#define KE_FENCE_ARB	0
#define KE_FENCE_NV		1
#define KE_FENCE_APPLE	2

bool ( *ke_ogl_insert_fence[3] )( ke_ogl_fence_t** ) = { ke_ogl_insert_fence_arb, ke_ogl_insert_fence_nv, ke_ogl_insert_fence_apple };
bool ( *ke_ogl_test_fence[3] )( ke_ogl_fence_t* ) = { ke_ogl_test_fence_arb, ke_ogl_test_fence_nv, ke_ogl_test_fence_apple };
void ( *ke_ogl_block_on_fence[3] )( ke_ogl_fence_t* ) = { ke_ogl_block_on_fence_arb, ke_ogl_block_on_fence_nv, ke_ogl_block_on_fence_apple };
void ( *ke_ogl_delete_fence[3] )( ke_ogl_fence_t* ) = { ke_ogl_delete_fence_arb, ke_ogl_delete_fence_nv, ke_ogl_delete_fence_apple };
bool ( *ke_ogl_is_fence[3] )( ke_ogl_fence_t* ) = { ke_ogl_is_fence_arb, ke_ogl_is_fence_nv, ke_ogl_is_fence_apple };
