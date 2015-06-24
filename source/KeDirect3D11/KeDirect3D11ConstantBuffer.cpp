//
//  KeDirect3D11ConstantBuffer.cpp
//
//  Created by Shogun3D on 4/22/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Name: IKeDirect3D11ConstantBuffer::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11ConstantBuffer::Destroy()
{
    /* Delete this instance */
    delete this;
}