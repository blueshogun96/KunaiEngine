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
 * Function pointer arrays (for internal use only)
 */
#define KE_FENCE_ARB	0	/* GL_ARB_sync (core OpenGL) */
#define KE_FENCE_NV		1	/* GL_NV_fence (NVIDIA and Android) */
#define KE_FENCE_APPLE	2	/* GL_APPLE_fence (MacOSX) or GL_APPLE_sync (iOS) */
#define KE_FENCE_EGL	4	/* EGL (Android, BB10, etc.) */


extern bool ( *KeOpenGLCreateFence[4] )( IKeOpenGLFence** );
extern bool ( *KeOpenGLInsertFence[4] )( IKeOpenGLFence** );
extern bool ( *KeOpenGLTestFence[4] )( IKeOpenGLFence* );
extern void ( *KeOpenGLBlockOnFence[4] )( IKeOpenGLFence* );
extern void ( *KeOpenGLDeleteFence[4] )( IKeOpenGLFence* );
extern bool ( *KeOpenGLIsFence[4] )( IKeOpenGLFence* );

