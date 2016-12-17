//
//  KeMemoryPool.h
//
//  Created by Shogun3D on 11/4/16.
//  Copyright © 2016 Shogun3D. All rights reserved.
//

#ifndef KeMemoryPool_h
#define KeMemoryPool_h


/*
 * Memory pool classs
 */
class KeMemoryPool
{
public:
    KeMemoryPool(), ~KeMemoryPool();
    
public:
    bool Initialize( uint32_t chunk_size, uint32_t num_chunks );
    void Uninitialize();
    
    void* Alloc();
    void Free( void* ptr );
    uint32_t GetChunkSize() const { return m_ChunkSize; }
    
    void SetAllowResize( bool allow_resize ) { m_AllowResize = allow_resize; }
    
private:
    void Reset();
    
    bool GrowableMemoryArray();
    uint8_t* AllocateNewMemoryBlock();
    
    uint8_t* GetNext( uint8_t* block );
    void SetNext( uint8_t* block_to_change, uint8_t new_next );
    
    KeMemoryPool( const KeMemoryPool& mempool ) {}
    
public:
    uint8_t**   m_ppRawMemoryArray;
    uint8_t*    m_pHead;
    uint32_t    m_ChunkSize;
    uint32_t    m_MemArraySize;
    bool        m_AllowResize;
};

#endif /* KeMemoryPool_h */
