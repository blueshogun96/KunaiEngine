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

/*
 * Name: IKeDirect3D11ConstantBuffer::MapData
 * Desc: Returns a pointer to the data contained within this constant buffer.
 */
void* IKeDirect3D11ConstantBuffer::MapData( uint32_t flags )
{
    return NULL;
}

/*
 * Name: IKeDirect3D11ConstantBufffer::UnmapData
 * Desc: Call this after you are finished with the pointer from MapData above.
 */
void IKeDirect3D11ConstantBuffer::UnmapData( void* ptr )
{
    
}

/*
 * Name: IKeDirect3D11ConstantBuffer::SetConstantData
 * Desc: Updates the constant buffer's data contents.
 */
bool IKeDirect3D11ConstantBuffer::SetConstantData( uint32_t offset, uint32_t size, void* ptr )
{   
    return true;
}

/*
 * Name: IKeDirect3D11ConstantBuffer::GetDesc
 * Desc: 
 */
void IKeDirect3D11ConstantBuffer::GetDesc( KeConstantBufferDesc* desc )
{
    if( !desc )
        return;
    
    desc->data_size = data_size;
    desc->flags = flags;
    memmove( desc->block_name, block_name, sizeof(char) * 64 );
}
