//
//  KeOpenGLGpuProgram.cpp
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
 * Name: IKeOpenGLGpuProgram::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLGpuProgram::Destroy()
{
    /* Delete this GLSL program */
    glDeleteProgram( program );

	/* Delete vertex attributes */
	delete this->va;
    
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeOpenGLGpuProgram::GetVertexAttributes
 * Desc: 
 */
void IKeOpenGLGpuProgram::GetVertexAttributes( KeVertexAttribute* vertex_attributes )
{
	//vertex_attributes = va;
    
    /* Copy vertex attributes */
    int va_size = 0;
    while( va[va_size].index != -1 )
        va_size++;
    
    memmove( vertex_attributes, va, sizeof( KeVertexAttribute ) * (va_size+1) );
}