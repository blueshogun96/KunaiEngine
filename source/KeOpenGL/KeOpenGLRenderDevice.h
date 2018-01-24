//
//  IKeOpenGLRenderDevice.h
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __IKeOpenGLRenderDevice__
#define __IKeOpenGLRenderDevice__

#include "KeRenderDevice.h"

#ifdef __APPLE__
 #ifdef __MOBILE_OS__
  #include "SDL.h"
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES3/gl.h>
  #include <OpenGLES/ES2/glext.h>
  #include <OpenGLES/ES3/glext.h>
 #else
  #include <SDL2/SDL.h>
  #include <OpenGL/OpenGL.h>
  #include <OpenGL/gl3.h>
  #include <OpenGL/gl3ext.h>
 #endif
#else
#include <SDL.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif


/*
 * Constant buffer structure
 */
struct IKeOpenGLConstantBuffer : public IKeConstantBuffer
{
    KEMETHOD            Destroy();
    
    _KEMETHOD(void*)    MapData( uint32_t flags );
    KEMETHOD            UnmapData( void* );
    
    _KEMETHOD(bool)     SetConstantData( uint32_t offset, uint32_t size, void* ptr );
    KEMETHOD            GetDesc( KeConstantBufferDesc* desc );
    
    uint32_t    ubo;            /* Uniform buffer size */
    uint32_t    data_size;      /* Size of the data buffer */
    uint32_t    flags;          /* Buffer creation flags */
    uint32_t    block_index;    /* Block index representing the block name we are attempting to access */
    char        block_name[64]; /* Name of the block/struct within the GPU program we are writing data to */
};

/*
 * Geometry buffer structure
 */
struct IKeOpenGLGeometryBuffer : public IKeGeometryBuffer
{
    KEMETHOD Destroy();
    
    _KEMETHOD(void*) MapData( uint32_t flags );
	_KEMETHOD(void*) MapDataAsync( uint32_t flags );
    KEMETHOD UnmapData( void* );
    KEMETHOD UnmapDataAsync( void* );

    _KEMETHOD(bool) SetVertexData( uint32_t offset, uint32_t size, void* ptr );
    _KEMETHOD(bool) SetIndexData( uint32_t offset, uint32_t size, void* ptr );
	KEMETHOD GetDesc( KeGeometryBufferDesc* desc );
    
    uint32_t vbo[2];		/* Vertex and index buffer */
    uint32_t vao;			/* Vertex array object */
    uint32_t vd_length;		/* Length of vertex data (in bytes) */
	uint32_t id_length;		/* Length of index data (in bytes) */
	uint32_t vertex_size;	/* Size of each vertex (in bytes) */
    uint32_t index_type;	/* Data type for index data */
    uint32_t lock_flags;	/* Buffer lock flags */
};

/*
 * Command list structure
 */
struct IKeOpenGLCommandList : public IKeCommandList 
{
	KEMETHOD Destroy();
};

/*
 * GPU Program structure
 */
struct IKeOpenGLGpuProgram : public IKeGpuProgram
{
    KEMETHOD Destroy();
	KEMETHOD GetVertexAttributes( KeVertexAttribute* vertex_attributes );
    
    uint32_t program;       /* GPU program handle */
    uint32_t matrices[3];   /* Handles to the world, view, and projection matrices (respectively) */
	KeVertexAttribute* va;	/* Vertex attributes */
};

/*
 * Texture base structure
 */
struct IKeOpenGLTexture : public IKeTexture
{
    KEMETHOD Destroy();
    
    _KEMETHOD(void*) MapData( uint32_t flags );
    KEMETHOD UnmapData( void* );
    
    _KEMETHOD(bool) SetTextureData( KeTextureDesc* texture_data, void* pixels );
	_KEMETHOD(bool) GetTextureDesc( KeTextureDesc* texture_desc );
    
    uint32_t handle;            /* Handle to the OpenGL texture */
	uint32_t mipmap;			
    uint32_t width, height;     /* Texture width/height */
    uint32_t depth;             /* Texture depth (for 3D and array textures) */
    uint32_t depth_format;      /* See glTexImageXD */
    uint32_t internal_format;   /* See glTexImageXD */
    uint32_t data_type;         /* Internal data type */
    uint32_t target;            /* OpenGL texture target */
};

/*
 * Rendertarget base structure 
 */
struct IKeOpenGLRenderTarget : public IKeRenderTarget
{
    KEMETHOD Destroy();
    
	_KEMETHOD(void*) MapData( uint32_t flags );
	KEMETHOD UnmapData( void* );

	_KEMETHOD(bool) GetTexture( IKeTexture** texture );
	virtual IKeTexture* GetTexture2();

    uint32_t    fbo;				/* Frame buffer object handle */
    uint32_t    depth_buffer;		/* Depth render buffer */
    uint32_t	stencil_buffer;		/* Stencil buffer */
    
    IKeTexture* texture;
};

/*
 * Palette base structure
 */
struct IKeOpenGLPalette : public IKePalette
{
    KEMETHOD Destroy();
};

/*
 * GPU fence structure 
 */
struct IKeOpenGLFence : public IKeFence
{
    KEMETHOD Destroy();
    
    _KEMETHOD(bool) Insert();
    _KEMETHOD(bool) Test();
    KEMETHOD Block();
    _KEMETHOD(bool) Valid();
    
	uint32_t	fence;		/* GL_NV_fence, GL_APPLE_fence */
	GLsync		sync;		/* GL_ARB_sync, GL_APPLE_sync */
    int         vendor;     /* Fence extension vendor to use */
};

/*
 * Render/Texture state structure
 */
struct IKeOpenGLRenderStateBuffer : public IKeRenderStateBuffer
{
    KEMETHOD Destroy();
    
    KeState*    states;         /* OpenGL state values */
    int         state_count;    /* The number of states in this buffer */
};

struct IKeOpenGLTextureSamplerBuffer : public IKeTextureSamplerBuffer
{
	KEMETHOD Destroy();

	KeState*    states;         /* OpenGL state values */
	int         state_count;    /* The number of states in this buffer */
};


/*
 * Render device base class
 */
class IKeOpenGLRenderDevice : public IKeRenderDevice
{
public:
    IKeOpenGLRenderDevice();
    IKeOpenGLRenderDevice( KeRenderDeviceDesc* renderdevice_desc );
    virtual ~IKeOpenGLRenderDevice();
    
public:
    /* Misc */
    _KEMETHOD(bool) ConfirmDevice();
    KEMETHOD GetDeviceDesc( KeRenderDeviceDesc* device_desc );
    KEMETHOD GetDeviceCaps( KeRenderDeviceCaps* device_caps );
    
    /* General rendering stuff */
    KEMETHOD SetClearColourFV( float* colour );
    KEMETHOD SetClearColourUBV( uint8_t* colour );
    KEMETHOD SetClearDepth( float depth );
	KEMETHOD SetClearStencil( uint32_t stencil );
    KEMETHOD ClearColourBuffer();
    KEMETHOD ClearDepthBuffer();
    KEMETHOD ClearStencilBuffer();
	KEMETHOD Clear( uint32_t buffers );
	KEMETHOD ClearState();
    KEMETHOD Swap();
	_KEMETHOD(bool) ResizeRenderTargetAndDepthStencil( int width, int height );
    
    KEMETHOD SetIMCacheSize( uint32_t cache_size );
    _KEMETHOD(bool) CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer );
    KEMETHOD DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
    KEMETHOD SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
	_KEMETHOD(bool) CreateCommandList( IKeCommandList** command_list );
	_KEMETHOD(bool) BeginCommandList( IKeCommandList* command_list );
	_KEMETHOD(bool) EndCommandList( IKeCommandList** command_list, int restore_state );
	_KEMETHOD(bool) ExecuteCommandList( IKeCommandList* command_list, int restore_state );
	KEMETHOD RestoreImmediateContext();
    _KEMETHOD(bool) CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program );
    KEMETHOD DeleteProgram( IKeGpuProgram* gpu_program );
    KEMETHOD SetProgram( IKeGpuProgram* gpu_program );
    KEMETHOD SetProgramConstant1FV( const char* location, int count, float* value );
    KEMETHOD SetProgramConstant2FV( const char* location, int count, float* value );
    KEMETHOD SetProgramConstant3FV( const char* location, int count, float* value );
    KEMETHOD SetProgramConstant4FV( const char* location, int count, float* value );
    KEMETHOD SetProgramConstant1IV( const char* location, int count, int* value );
    KEMETHOD SetProgramConstant2IV( const char* location, int count, int* value );
    KEMETHOD SetProgramConstant3IV( const char* location, int count, int* value );
    KEMETHOD SetProgramConstant4IV( const char* location, int count, int* value );
    KEMETHOD GetProgramConstantFV( const char* location, float* value );
    KEMETHOD GetProgramConstantIV( const char* location, int* value );
	_KEMETHOD(bool) CreateConstantBuffer( KeConstantBufferDesc* desc, IKeConstantBuffer** constant_buffer, void* data = NULL );
	KEMETHOD DeleteConstantBuffer( IKeConstantBuffer* constant_buffer );
	_KEMETHOD(bool) SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer );
	KEMETHOD SetConstantBuffer( int slot, int shader_type, IKeConstantBuffer* constant_buffer );
    _KEMETHOD(bool) CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    _KEMETHOD(bool) CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    _KEMETHOD(bool) CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    KEMETHOD DeleteTexture( IKeTexture* texture );
    KEMETHOD SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture );
    KEMETHOD SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture );
    KEMETHOD SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture );
    _KEMETHOD(bool) CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget );
    KEMETHOD DeleteRenderTarget( IKeRenderTarget* rendertarget );
    KEMETHOD BindRenderTarget( IKeRenderTarget* rendertarget );
    KEMETHOD SetTexture( int stage, IKeTexture* texture );
	_KEMETHOD(bool) CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer );
	_KEMETHOD(bool) CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer );
	_KEMETHOD(bool) SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer );
	_KEMETHOD(bool) SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer );
	KEMETHOD SetRenderStates( KeState* states );
	KEMETHOD SetSamplerStates( int stage, KeState* states );
    KEMETHOD DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, void* vertex_data );
    KEMETHOD DrawIndexedVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int count, void* vertex_data, void* index_data );
    KEMETHOD DrawIndexedVerticesRangeIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int start, int end, int count, void* vertex_data, void* index_data );
    KEMETHOD DrawVertices( uint32_t primtype, uint32_t stride, int first, int count );
    KEMETHOD DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count );
    KEMETHOD DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count );
    
    _KEMETHOD(bool) GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels );
    
    /* Matrix/viewport related */
    KEMETHOD SetViewport( int x, int y, int width, int height );
    KEMETHOD SetViewportV( int* viewport );
    KEMETHOD GetViewport( int* x, int* y, int* width, int* height );
    KEMETHOD GetViewportV( int* viewport );
    KEMETHOD SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z );
    KEMETHOD SetViewMatrix( const nv::matrix4f* view );
    KEMETHOD SetWorldMatrix( const nv::matrix4f* world );
    KEMETHOD SetModelviewMatrix( const nv::matrix4f* modelview );
    KEMETHOD SetProjectionMatrix( const nv::matrix4f* projection );
	KEMETHOD GetViewMatrix( nv::matrix4f* view );
    KEMETHOD GetWorldMatrix( nv::matrix4f* world );
    KEMETHOD GetModelviewMatrix( nv::matrix4f* modelview );
    KEMETHOD GetProjectionMatrix( nv::matrix4f* projection );
    
    /* Synchronization */
    KEMETHOD BlockUntilVerticalBlank();
    KEMETHOD SetSwapInterval( int swap_interval );
    virtual int GetSwapInterval();
	KEMETHOD BlockUntilIdle();
	KEMETHOD Kick();
    _KEMETHOD(bool) CreateFence( IKeFence** fence, uint32_t flags );
#if 0
	_KEMETHOD(bool) InsertFence( IKeFence** fence );
	_KEMETHOD(bool) TestFence( IKeFence* fence );
	KEMETHOD BlockOnFence( IKeFence* fence );
	KEMETHOD DeleteFence( IKeFence* fence );
	_KEMETHOD(bool) IsFence( IKeFence* fence );
#endif
    
    /* Misc */
    KEMETHOD GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory );
	KEMETHOD Trim();
    
private:    /* Private, internal use only */
    void PVT_ApplySamplerStates();
    void PVT_SetWorldViewProjectionMatrices();
	bool PVT_InititalizeDriverHooks();
	void PVT_BlockUntilVerticalBlankDDraw();
	void PVT_BlockUntilVerticalBlankD3DKMT();
	void PVT_BlockUntilVerticalBlankDefault();
    
protected:
    SDL_GLContext context;
    SDL_Window* window;
	int			major_version;
	int			minor_version;
	int			fence_vendor;
	KeState		samplers[8][16];
	int			dirty_samplers[8][16];
    uint32_t    im_cache_size;
    IKeGeometryBuffer* im_gb;
};


/*
 * Sprite factory class
 */
class IKeOpenGLSpriteFactory : public IKeSpriteFactory
{
public:
    IKeOpenGLSpriteFactory() {}
    IKeOpenGLSpriteFactory( IKeRenderDevice* renderdevice, KeVertexAttribute* vertex_attributes, uint32_t batch_size = KE_DEFAULT_BATCH_SIZE );
    virtual ~IKeOpenGLSpriteFactory();
    
public:
    _KEMETHOD(bool) Initialized();
    //KEMETHOD SetStates( IKeStateBuffer* state );
    KEMETHOD SetProgram( IKeGpuProgram* program );
    KEMETHOD SetTexture( IKeTexture* texture );
    KEMETHOD SetVertexData( void* vertex_data, uint32_t offset, uint32_t vertex_data_size );
    _KEMETHOD(bool) Lock( void** data );
    KEMETHOD Unlock( void* data );
    
    KEMETHOD Draw();
};

#endif /* defined(__IKeOpenGLRenderDevice__) */
