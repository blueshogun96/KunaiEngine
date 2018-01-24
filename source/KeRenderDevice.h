//
//  ke_renderdevice.h
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_renderdevice__
#define __ke_renderdevice__


#include "KePlatform.h"
#include "KeUnknown.h"
#include "NvFoundationMath.h"
#include "NV/NvMath.h"


/*
 * Render device types
 */
#define KE_RENDERDEVICE_D3D11       1
#define KE_RENDERDEVICE_D3D12       2
#define KE_RENDERDEVICE_OGL3        3
#define KE_RENDERDEVICE_OGL4        4
#define KE_RENDERDEVICE_OGLES2      5
#define KE_RENDERDEVICE_OGLES3      6
#define KE_RENDERDEVICE_METAL       7
#define KE_RENDERDEVICE_VULKAN      8


/*
 * Matrix types
 */
#define KE_MATRIX_WORLD         0
#define KE_MATRIX_VIEW          1
#define KE_MATRIX_PROJECTION    2


/*
 * Vertex attribute locations
 */
#define KE_VA_POSITION          0
#define KE_VA_NORMAL            1
#define KE_VA_TANGENT           2
#define KE_VA_BITANGENT         3
#define KE_VA_COLOUR            4
#define KE_VA_TEXTURE0          5
#define KE_VA_TEXTURE1          6
#define KE_VA_TEXTURE2          7
#define KE_VA_TEXTURE3          8
#define KE_VA_TEXTURE4          9
#define KE_VA_TEXTURE5          10
#define KE_VA_TEXTURE6          11
#define KE_VA_TEXTURE7          12


/*
 * Primitive types
 */
#define KE_POINTS           0
#define KE_LINES            1
#define KE_LINESTRIP        2
#define KE_LINELOOP         3
#define KE_TRIANGLES        4
#define KE_TRIANGLESTRIP    5


/*
 * Data types
 */
#define KE_BYTE             0
#define KE_UNSIGNED_BYTE    1
#define KE_SHORT            2
#define KE_UNSIGNED_SHORT   3
#define KE_INT              4
#define KE_UNSIGNED_INT     5
#define KE_FLOAT            6
#define KE_DOUBLE           7


/*
 * Buffer usage flags
 */
#define KE_USAGE_STATIC_WRITE		0	
#define KE_USAGE_STATIC_READ		1
#define KE_USAGE_STATIC_READ_WRITE	2
#define KE_USAGE_DYNAMIC_WRITE		3
#define KE_USAGE_DYNAMIC_READ		4
#define KE_USAGE_DYNAMIC_READ_WRITE	5
#define KE_USAGE_STREAM_WRITE		6
#define KE_USAGE_STREAM_READ		7
#define KE_USAGE_STREAM_READ_WRITE	8


/*
 * Buffer mapping flags
 */
#define KE_MAPBUFFER_READONLY       0x0
#define KE_MAPBUFFER_WRITEONLY      0x1
#define KE_MAPBUFFER_READWRITE      0x2
#define KE_MAPBUFFER_VERTEXDATA     0x10
#define KE_MAPBUFFER_INDEXDATA      0x20


/*
 * Render buffer types
 */
#define KE_COLOUR_BUFFER	0x1
#define KE_DEPTH_BUFFER		0x2
#define KE_STENCIL_BUFFER	0x4


/*
 * Renderstate types 
 */
#define KE_RS_DEPTHTEST     1
#define KE_RS_DEPTHFUNC     2
#define KE_RS_DEPTHMASK     3
#define KE_RS_BLENDEQUATION 4
#define KE_RS_ALPHABLEND    5
#define KE_RS_FRONTFACE     6
#define KE_RS_CULLMODE      7
#define KE_RS_POLYGONMODE   8
#define KE_RS_BLENDFUNC     9

/*
 * Texturestate types
 */
#define KE_TS_MAGFILTER     1
#define KE_TS_MINFILTER     2
#define KE_TS_WRAPU         4
#define KE_TS_WRAPV         5
#define KE_TS_WRAPW         6


/*
 * Comparison types
 */
#define KE_NEVER        0
#define KE_LESS         1
#define KE_EQUAL        2
#define KE_LEQUAL       3
#define KE_GREATER      4
#define KE_NOTEQUAL     5
#define KE_GEQUAL       6
#define KE_ALWAYS       7


/*
 * Polygon modes
 */
#define KE_FRONT            0
#define KE_BACK             1
#define KE_FRONT_AND_BACK   2

/* 
 * Fill modes
 */
#define KE_POINT            0
#define KE_LINE             1
#define KE_SOLID            2

/*
 * Cull modes
 */
#define KE_CULL_NONE        0
#define KE_CULL_CW          1
#define KE_CULL_CCW         2

/*
 * Blend modes
 */
#define KE_ZERO						0
#define KE_ONE						1
#define KE_SRC_COLOR				2
#define KE_INV_SRC_COLOR			3
#define KE_DST_COLOR				4
#define KE_INV_DST_COLOR			5
#define KE_SRC_ALPHA				6
#define KE_INV_SRC_ALPHA			7
#define KE_DST_ALPHA				8
#define KE_INV_DST_ALPHA			9
#define KE_CONSTANT_COLOR			10
#define KE_INV_CONSTANT_COLOR		11
#define KE_CONSTANT_ALPHA			12
#define KE_INV_CONSTANT_ALPHA		13
#define KE_SRC_ALPHA_SATURATE		14
#define KE_SRC1_COLOR				15
#define KE_INV_SRC1_COLOR			16
#define KE_SRC1_ALPHA				17
#define KE_INV_SRC1_ALPHA			18

/*
 * Texture types
 */
#define KE_TEXTURE_1D       0
#define KE_TEXTURE_1D_ARRAY 1
#define KE_TEXTURE_2D       2
#define KE_TEXTURE_2D_ARRAY 3
#define KE_TEXTURE_3D       4
#define KE_TEXTURE_RECT     5

/*
 * Texture formats
 */
#define KE_TEXTUREFORMAT_RGBA   0
#define KE_TEXTUREFORMAT_BGRA   1
#define KE_TEXTUREFORMAT_R8		2
#define KE_TEXTUREFORMAT_RGB	3
#define KE_TEXTUREFORMAT_BGR	4

/*
 * Texture filtering modes
 */
#define KE_TEXTUREFILTER_NEAREST                    0
#define KE_TEXTUREFILTER_LINEAR                     1
#define KE_TEXTUREFILTER_NEAREST_MIPMAP_NEAREST     2
#define KE_TEXTUREFILTER_LINEAR_MIPMAP_NEAREST      3
#define KE_TEXTUREFILTER_NEAREST_MIPMAP_LINEAR      4
#define KE_TEXTUREFILTER_LINEAR_MIPMAP_LINEAR       5

/*
 * Texture wrap modes
 */
#define KE_REPEAT           0
#define KE_CLAMP            1
#define KE_CLAMP_TO_EDGE    2
#define KE_CLAMP_TO_BORDER  3
#define KE_MIRRORED_REPEAT  4

/*
 * Fence types
 */
#define KE_FENCE_DEFAULT    -1  /* Autodetect per platform */
#define KE_FENCE_ARB        0	/* GL_ARB_sync (core OpenGL) */
#define KE_FENCE_NV         1	/* GL_NV_fence (NVIDIA and Android) */
#define KE_FENCE_APPLE      2	/* GL_APPLE_fence (MacOSX) or GL_APPLE_sync (iOS) */
#define KE_FENCE_EGL        4	/* EGL (Android, BB10, etc.) */
#define KE_FENCE_MICROSOFT  5   /* Direct3D default (ID3D11Query for windows) */

/*
 * Shader types
 */
#define KE_VERTEX_SHADER        0
#define KE_PIXEL_SHADER         1
#define KE_GEOMETRY_SHADER      2
#define KE_TESSELATION_SHADER   3

/*
 * Sprite batch settings
 */
#define KE_DEFAULT_BATCH_SIZE (32*1024*1024)


/*
 * Render device description
 */
struct KeRenderDeviceDesc
{
    int width;
    int height;
    int colour_bpp;
	int alpha_bpp;
    int depth_bpp;
    int stencil_bpp;
    int buffer_count;
    int refresh_rate;
    int device_type;
    int fullscreen;
};

/*
 * Render device capabilities
 */
struct KeRenderDeviceCaps
{
    /* General capabilities */
    int primitive_restart_supported;
    int hardware_command_buffers_supported;
    int gpu_fencing_supported;
    int instancing_supported;
    int default_fence_type;
    
    /* Texture capabilities */
    int texture_rectangles_supported;
    
    /* Shader capabilities */
    int compute_shaders_supported;
    int constant_buffers_supported;
    int uniform_constants_supported;
    int max_constant_buffer_size;
    int max_constant_buffer_bindings;
    int max_vertex_constant_buffer_blocks;
    int max_pixel_constant_buffer_blocks;
    int max_geometry_constant_buffer_blocks;
    int max_tesselation_constant_buffer_blocks;
};

/*
 * Vertex attribute structure
 */
struct KeVertexAttribute
{
    int     index;      /* Vertex attribute location */
    int     size;       /* Size per vertex */
    uint32_t type;      /* Data type */
    int     normalize;  /* Are we normalizing this vertex data? */
    int     stride;     /* Stride between vertices */
    int     offset;     /* Offset between vertex data */
};

/*
 * State structure
 */
struct KeState
{
    uint32_t state;     /* The actual render/texture state */
    uint32_t param1;    /* Render/texture state parameters */
    uint32_t param2;
    uint32_t param3;
    float    fparam;
    double   dparam;
};

struct CKeState : public KeState
{
    CKeState( uint32_t state, uint32_t param1, uint32_t param2, uint32_t param3, float fparam, double dparam )
    {
        this->state = state;
        this->param1 = param1;
        this->param2 = param2;
        this->param3 = param3;
        this->fparam = fparam;
        this->dparam = dparam;
    }
};


/* 
 * Geometry buffer description 
 */
struct KeGeometryBufferDesc
{
	uint32_t vertex_data_size;
	uint32_t index_data_size;
	uint32_t vertex_size;
	uint32_t index_type;
};

/*
 * Texture data description
 */
struct KeTextureDesc
{
    uint32_t offsetx, offsety, offsetz;
    uint32_t width, height;     /* Texture width/height */
    uint32_t pitch;             /* Buffer pitch */
    uint32_t depth;             /* Texture depth (for 3D and array textures) */
    uint32_t mipmap;            /* Mipmap level */
    uint32_t data_type;         /* Internal data type */
};

/*
 * Constant buffer data description
 */
struct KeConstantBufferDesc
{
    char        block_name[64];
    uint32_t    flags;
    uint32_t    data_size;
};


/*
 * Resource base structure
 */
struct IKeResourceBuffer : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
    
    _KEMETHOD(void*) MapData( uint32_t flags ) PURE;
    KEMETHOD UnmapData( void* ) PURE;
};


/*
 * Constant buffer structure
 */
struct IKeConstantBuffer : public IKeResourceBuffer
{
    KEMETHOD            Destroy() PURE;
    
    _KEMETHOD(void*)    MapData( uint32_t flags ) PURE;
    KEMETHOD            UnmapData( void* ) PURE;
    
    _KEMETHOD(bool)     SetConstantData( uint32_t offset, uint32_t size, void* ptr ) PURE;
    KEMETHOD            GetDesc( KeConstantBufferDesc* desc ) PURE;
};

/*
 * Geometry buffer base structure
 */
struct IKeGeometryBuffer : public IKeResourceBuffer
{
    KEMETHOD Destroy() PURE;
    
    _KEMETHOD(void*) MapData( uint32_t flags ) PURE;
	_KEMETHOD(void*) MapDataAsync( uint32_t flags ) PURE;
    KEMETHOD UnmapData( void* ) PURE;
	KEMETHOD UnmapDataAsync( void* ) PURE;
    
    _KEMETHOD(bool) SetVertexData( uint32_t offset, uint32_t size, void* ptr ) PURE;
    _KEMETHOD(bool) SetIndexData( uint32_t offset, uint32_t size, void* ptr ) PURE;
	KEMETHOD GetDesc( KeGeometryBufferDesc* desc ) PURE;
};

/*
 * Command list base structure
 */
struct IKeCommandList : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
};

/*
 * GPU Program base structure
 */
struct IKeGpuProgram : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
	KEMETHOD GetVertexAttributes( KeVertexAttribute* vertex_attributes ) PURE;
};

/*
 * Texture base structure
 */
struct IKeTexture : public IKeResourceBuffer
{
    KEMETHOD Destroy() PURE;
    
    _KEMETHOD(void*) MapData( uint32_t flags ) PURE;
    KEMETHOD UnmapData( void* ) PURE;
    
    _KEMETHOD(bool) SetTextureData( KeTextureDesc* texture_data, void* pixels ) PURE;
	_KEMETHOD(bool) GetTextureDesc( KeTextureDesc* texture_desc ) PURE;
};


/*
 * Rendertarget base structure
 */
struct IKeRenderTarget : public IKeResourceBuffer
{
    KEMETHOD Destroy() PURE;
    
    _KEMETHOD(void*) MapData( uint32_t flags ) PURE;
    KEMETHOD UnmapData( void* ) PURE;

	_KEMETHOD(bool) GetTexture( IKeTexture** texture ) PURE;
	virtual IKeTexture* GetTexture2() PURE;
};

/*
 * Palette base structure
 */
struct IKePalette : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
};

/*
 * Fence base structure
 */
struct IKeFence : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
    
    _KEMETHOD(bool) Insert() PURE;
    _KEMETHOD(bool) Test() PURE;
    KEMETHOD Block() PURE;
    _KEMETHOD(bool) Valid() PURE;
};

/*
 * Render and texture state structure
 */
struct IKeRenderStateBuffer : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
};

struct IKeTextureSamplerBuffer : public IKeUnknown
{
	KEMETHOD Destroy() PURE;
};

/*
 * Occlusion query base structure
 */
struct IKeOcclusionQuery : public IKeUnknown
{
    KEMETHOD Destroy() PURE;
};


/*
 * Render device base class
 */
class IKeRenderDevice
{
public:
    IKeRenderDevice() { /*assert(0);*/ }
    IKeRenderDevice( KeRenderDeviceDesc* renderdevice_desc ) { assert(0); }
    virtual ~IKeRenderDevice() { /*assert(0);*/ }
    
public:
    /* Misc */
    _KEMETHOD(bool) ConfirmDevice() PURE;
    KEMETHOD GetDeviceDesc( KeRenderDeviceDesc* device_desc ) PURE;
    KEMETHOD GetDeviceCaps( KeRenderDeviceCaps* device_caps ) PURE;
    
    /* General rendering stuff */
    KEMETHOD SetClearColourFV( float* colour ) PURE;
    KEMETHOD SetClearColourUBV( uint8_t* colour ) PURE;
    KEMETHOD SetClearDepth( float depth ) PURE;
	KEMETHOD SetClearStencil( uint32_t stencil ) PURE;
    KEMETHOD ClearColourBuffer() PURE;
    KEMETHOD ClearDepthBuffer() PURE;
    KEMETHOD ClearStencilBuffer() PURE;
	KEMETHOD Clear( uint32_t buffers ) PURE;
	KEMETHOD ClearState() PURE;
    KEMETHOD Swap() PURE;
	_KEMETHOD(bool) ResizeRenderTargetAndDepthStencil( int width, int height ) PURE;
    
    KEMETHOD SetIMCacheSize( uint32_t cache_size ) PURE;
    _KEMETHOD(bool) CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer ) PURE;
    KEMETHOD DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer ) PURE;
    KEMETHOD SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer ) PURE;
	_KEMETHOD(bool) CreateCommandList( IKeCommandList** command_list ) PURE;
	_KEMETHOD(bool) BeginCommandList( IKeCommandList* command_list ) PURE;
	_KEMETHOD(bool) EndCommandList( IKeCommandList** command_list, int restore_state ) PURE;
	_KEMETHOD(bool) ExecuteCommandList( IKeCommandList* command_list, int restore_state ) PURE;
	KEMETHOD RestoreImmediateContext() PURE;
    _KEMETHOD(bool) CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program ) PURE;
    KEMETHOD DeleteProgram( IKeGpuProgram* gpu_program ) PURE;
    KEMETHOD SetProgram( IKeGpuProgram* gpu_program ) PURE;
    KEMETHOD SetProgramConstant1FV( const char* location, int count, float* value ) PURE;
    KEMETHOD SetProgramConstant2FV( const char* location, int count, float* value ) PURE;
    KEMETHOD SetProgramConstant3FV( const char* location, int count, float* value ) PURE;
    KEMETHOD SetProgramConstant4FV( const char* location, int count, float* value ) PURE;
    KEMETHOD SetProgramConstant1IV( const char* location, int count, int* value ) PURE;
    KEMETHOD SetProgramConstant2IV( const char* location, int count, int* value ) PURE;
    KEMETHOD SetProgramConstant3IV( const char* location, int count, int* value ) PURE;
    KEMETHOD SetProgramConstant4IV( const char* location, int count, int* value ) PURE;
    KEMETHOD GetProgramConstantFV( const char* location, float* value ) PURE;
    KEMETHOD GetProgramConstantIV( const char* location, int* value ) PURE;
	_KEMETHOD(bool) CreateConstantBuffer( KeConstantBufferDesc* desc, IKeConstantBuffer** constant_buffer, void* data = NULL ) PURE;
	KEMETHOD DeleteConstantBuffer( IKeConstantBuffer* constant_buffer ) PURE;
	_KEMETHOD(bool) SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer ) PURE;
	KEMETHOD SetConstantBuffer( int slot, int shader_type, IKeConstantBuffer* constant_buffer ) PURE;
    _KEMETHOD(bool) CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    _KEMETHOD(bool) CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    _KEMETHOD(bool) CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    KEMETHOD DeleteTexture( IKeTexture* texture ) PURE;
    KEMETHOD SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    KEMETHOD SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    KEMETHOD SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    _KEMETHOD(bool) CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget ) PURE;
    KEMETHOD DeleteRenderTarget( IKeRenderTarget* rendertarget ) PURE;
    KEMETHOD BindRenderTarget( IKeRenderTarget* rendertarget ) PURE;
    KEMETHOD SetTexture( int stage, IKeTexture* texture ) PURE;
    _KEMETHOD(bool) CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer ) PURE;
	_KEMETHOD(bool) CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer ) PURE;
    _KEMETHOD(bool) SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer ) PURE;
	_KEMETHOD(bool) SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer ) PURE;
    KEMETHOD SetRenderStates( KeState* states ) PURE;
    KEMETHOD SetSamplerStates( int stage, KeState* states ) PURE;
    KEMETHOD DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, void* vertex_data ) PURE;
    KEMETHOD DrawIndexedVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int count, void* vertex_data, void* index_data ) PURE;
    KEMETHOD DrawIndexedVerticesRangeIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int start, int end, int count, void* vertex_data, void* index_data ) PURE;
    KEMETHOD DrawVertices( uint32_t primtype, uint32_t stride, int first, int count ) PURE;
    KEMETHOD DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count ) PURE;
    KEMETHOD DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count ) PURE;
    
    _KEMETHOD(bool) GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels ) PURE;
    
    /* Matrix/viewport related */
    KEMETHOD SetViewport( int x, int y, int width, int height ) PURE;
    KEMETHOD SetViewportV( int* viewport ) PURE;
    KEMETHOD GetViewport( int* x, int* y, int* width, int* height ) PURE;
    KEMETHOD GetViewportV( int* viewport ) PURE;
    KEMETHOD SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z ) PURE;
    KEMETHOD SetViewMatrix( const nv::matrix4f* view ) PURE;
    KEMETHOD SetWorldMatrix( const nv::matrix4f* world ) PURE;
    KEMETHOD SetModelviewMatrix( const nv::matrix4f* modelview ) PURE;
    KEMETHOD SetProjectionMatrix( const nv::matrix4f* projection ) PURE;
    KEMETHOD GetViewMatrix( nv::matrix4f* view ) PURE;
    KEMETHOD GetWorldMatrix( nv::matrix4f* world ) PURE;
    KEMETHOD GetModelviewMatrix( nv::matrix4f* modelview ) PURE;
    KEMETHOD GetProjectionMatrix( nv::matrix4f* projection ) PURE;
    
    /* Synchronization */
    KEMETHOD BlockUntilVerticalBlank() PURE;
    KEMETHOD SetSwapInterval( int swap_interval ) PURE;
    virtual int GetSwapInterval() PURE;
	KEMETHOD BlockUntilIdle() PURE;
	KEMETHOD Kick() PURE;
    _KEMETHOD(bool) CreateFence( IKeFence** fence, uint32_t flags ) PURE;
#if 0
	_KEMETHOD(bool) InsertFence( IKeFence** fence ) PURE;
	_KEMETHOD(bool) TestFence( IKeFence* fence ) PURE;
	KEMETHOD BlockOnFence( IKeFence* fence ) PURE;
	KEMETHOD DeleteFence( IKeFence* fence ) PURE;
	_KEMETHOD(bool) IsFence( IKeFence* fence ) PURE;
#endif
    
    /* Misc */
    KEMETHOD GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory ) PURE;
	KEMETHOD Trim() PURE;
    
protected:
    bool                    initialized;
    float                   clear_colour[4];
    float                   clear_depth;
	uint32_t				clear_stencil;
    int                     viewport[4];
    KeRenderDeviceDesc*		device_desc;
    KeRenderDeviceCaps*		device_caps;
    nv::matrix4f            world_matrix;
    nv::matrix4f            view_matrix;
    nv::matrix4f            modelview_matrix;
    nv::matrix4f            projection_matrix;
    IKeGeometryBuffer*		current_geometrybuffer;
    IKeGpuProgram*			current_gpu_program;
	IKeTexture*				current_texture[8];
    KeVertexAttribute		current_vertexattribute[32];
};


/*
 * Sprite factory class
 */
class IKeSpriteFactory
{
public:
    IKeSpriteFactory() {}
    IKeSpriteFactory( IKeRenderDevice* renderdevice, KeVertexAttribute* vertex_attributes, uint32_t batch_size = KE_DEFAULT_BATCH_SIZE ) {}
    virtual ~IKeSpriteFactory() {}
    
public:
    _KEMETHOD(bool) Initialized() PURE;
    //KEMETHOD SetStates( IKeStateBuffer* state ) PURE;
    KEMETHOD SetProgram( IKeGpuProgram* program ) PURE;
    KEMETHOD SetTexture( IKeTexture* texture ) PURE;
    KEMETHOD SetVertexData( void* vertex_data, uint32_t offset, uint32_t vertex_data_size ) PURE;
    _KEMETHOD(bool) Lock( void** data ) PURE;
    KEMETHOD Unlock( void* data ) PURE;
    
    KEMETHOD Draw() PURE;
    
protected:
    IKeRenderDevice*        renderdevice;
    IKeGeometryBuffer*      dynamic_geometrybuffer;
    uint32_t                batch_size;
    bool                    initialized;
};

/*
 * Render device creation function
 */
IKeRenderDevice* KeCreateRenderDevice( KeRenderDeviceDesc* renderdevice_desc );

/*
 * Sprite batch creation function
 */
IKeSpriteFactory* KeCreateSpriteFactory( IKeRenderDevice* renderdevice );

#endif /* defined(__ke_renderdevice__) */
