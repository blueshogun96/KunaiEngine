//
//  KeOpenGLCommandList.cpp
//
//  Created by Shogun3D on 7/4/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeOpenGLRenderDevice.h"


/*
* Name: IKeOpenGLCommandList::Destroy
* Desc: Handles destruction of this interface instance.
*/
void IKeOpenGLCommandList::Destroy()
{
	/* Delete this instance */
	delete this;
}