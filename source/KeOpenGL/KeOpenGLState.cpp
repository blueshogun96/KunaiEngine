//
//  KeOpenGLState.cpp
//
//  Created by Shogun3D on 4/17/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
 * Name: IKeOpenGLRenderStateBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLRenderStateBuffer::Destroy()
{
    /* Delete the list of states */
    delete this->states;
    
    /* Delete this instance */
    delete this;
}


/*
 * Name: IKeOpenGLTextureSamplerBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLTextureSamplerBuffer::Destroy()
{
	/* Delete the list of states */
	delete this->states;

	/* Delete this instance */
	delete this;
}