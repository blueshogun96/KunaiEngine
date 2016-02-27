//
//  KeDirect3D11CommandList.cpp
//
//  Created by Shogun3D on 7/4/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
* Name: IKeDirect3D11CommandList::Destroy
* Desc: Handles destruction of this interface instance.
*/
void IKeDirect3D11CommandList::Destroy()
{
	/* Delete this instance */
	delete this;
}