//
//  KeDirect3D11State.cpp
//
//  Created by Shogun3D on 4/22/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "Ke.h"
#include "KeRenderDevice.h"
#include "KeDirect3D11RenderDevice.h"


/*
 * Name: IKeDirect3D11RenderState::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11RenderStateBuffer::Destroy()
{
    /* Delete this instance */
    delete this;
}

/*
 * Name: IKeDirect3D11TextureSamler::Destroy
 * Desc: Handles destruction of this interface instance.
 */
void IKeDirect3D11TextureSamplerBuffer::Destroy()
{
	/* Delete this instance */
	delete this;
}