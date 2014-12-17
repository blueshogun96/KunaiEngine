#pragma once

/*
 * NVIDIA fencing functions (GL_NV_fence)
 */
bool ke_ogl_insert_fence_nv( ke_ogl_fence_t* fence )
{
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesNV( 1, &fence->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceNV( fence->fence, GL_ALL_COMPLETED_NV );
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
bool ke_ogl_insert_fence_apple( ke_ogl_fence_t* fence )
{
	GLenum error = glGetError();

	/* Generate a new fence */
	glGenFencesAPPLE( 1, &fence->fence );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating new fence!", glGetError() );

	/* Set the fence */
	glSetFenceAPPLE( fence->fence );
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