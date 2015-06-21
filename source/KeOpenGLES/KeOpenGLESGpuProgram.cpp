//
//  KeOpenGLESGpuProgram.cpp
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLESRenderDevice.h"


/*
 * Name: IKeOpenGLGpuProgram::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLESGpuProgram::Destroy()
{
    /* Delete this GLSL program */
    glDeleteProgram( program );
    
    /* Delete this instance */
    delete this;
}