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
 * Name: IKeOpenGLConstantBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeOpenGLConstantBuffer::Destroy()
{
    /* Delete this instance */
    delete this;
}