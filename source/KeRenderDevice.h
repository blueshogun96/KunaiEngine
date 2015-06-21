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
//#include "vectormath.h"
#include <NvFoundationMath.h>
#include <NV/NvMath.h>


/*
 * Render device types
 */
#define KE_RENDERDEVICE_D3D11       1
#define KE_RENDERDEVICE_OGL3        2
#define KE_RENDERDEVICE_OGL4        3
#define KE_RENDERDEVICE_OGLES2      4
#define KE_RENDERDEVICE_OGLES3      5
#define KE_RENDERDEVICE_METAL       6


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
#define KE_VA_TEXTURE4          8
#define KE_VA_TEXTURE5          8
#define KE_VA_TEXTURE6          8
#define KE_VA_TEXTURE7          8


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
 * Renderstate types 
 */
#define KE_RS_DEPTHTEST     1
#define KE_RS_DEPTHFUNC     2
#define KE_RS_DEPTHMASK     3
//#define KE_RS_CLEARDEPTH    4
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
    
    /* Texture capabilities */
    int texture_rectangles_supported;
    
    /* Shader capabilities */
    int compute_shaders_supported;
    int constant_buffers_supported;
    int single_constants_supported;
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
 * Resource base structure
 */
struct IKeResourceBuffer : public IKeUnknown
{
    virtual void Destroy() PURE;
    
    virtual void* MapData( uint32_t flags ) PURE;
    virtual void UnmapData( void* ) PURE;
};


/*
 * Constant buffer structure
 */
struct IKeConstantBuffer : public IKeUnknown
{
    virtual void Destroy() PURE;
};

/*
 * Geometry buffer base structure
 */
struct IKeGeometryBuffer : public IKeResourceBuffer
{
    virtual void Destroy() PURE;
    
    virtual void* MapData( uint32_t flags ) PURE;
    virtual void UnmapData( void* ) PURE;
    
    virtual bool SetVertexData( uint32_t offset, uint32_t size, void* ptr ) PURE;
    virtual bool SetIndexData( uint32_t offset, uint32_t size, void* ptr ) PURE;
};

/*
 * Push buffer base structure
 */
struct IKePushBuffer : public IKeResourceBuffer
{
    virtual void Destroy() PURE;
    
    virtual void* MapData( uint32_t flags ) PURE;
    virtual void UnmapData( void* ) PURE;
};

/*
 * GPU Program base structure
 */
struct IKeGpuProgram : public IKeUnknown
{
    virtual void Destroy() PURE;
};

/*
 * Texture base structure
 */
struct IKeTexture : public IKeResourceBuffer
{
    virtual void Destroy() PURE;
    
    virtual void* MapData( uint32_t flags ) PURE;
    virtual void UnmapData( void* ) PURE;
    
    virtual bool SetTextureData( KeTextureDesc* texture_data, void* pixels ) PURE;
};


/*
 * Rendertarget base structure
 */
struct IKeRenderTarget : public IKeResourceBuffer
{
    virtual void Destroy() PURE;
    
    virtual void* MapData( uint32_t flags ) PURE;
    virtual void UnmapData( void* ) PURE;
};

/*
 * Palette base structure
 */
struct IKePalette : public IKeUnknown
{
    virtual void Destroy() PURE;
};

/*
 * Fence base structure
 */
struct IKeFence : public IKeUnknown
{
    virtual void Destroy() PURE;
};

/*
 * Render and texture state structure
 */
struct IKeStateBuffer : public IKeUnknown
{
    virtual void Destroy() PURE;
};

/*
 * Occlusion query base structure
 */
struct IKeOcclusionQuery : public IKeUnknown
{
    virtual void Destroy() PURE;
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
    virtual bool ConfirmDevice() PURE;
    virtual void GetDeviceDesc( KeRenderDeviceDesc* device_desc ) PURE;
    virtual void GetDeviceCaps( KeRenderDeviceCaps* device_caps ) PURE;
    
    /* General rendering stuff */
    virtual void SetClearColourFV( float* colour ) PURE;
    virtual void SetClearColourUBV( uint8_t* colour ) PURE;
    virtual void SetClearDepth( float depth ) PURE;
    virtual void ClearColourBuffer() PURE;
    virtual void ClearDepthBuffer() PURE;
    virtual void ClearStencilBuffer() PURE;
    virtual void Swap() PURE;
    
    virtual bool CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer ) PURE;
    virtual void DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer ) PURE;
    virtual void SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer ) PURE;
    virtual bool CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program ) PURE;
    virtual void DeleteProgram( IKeGpuProgram* gpu_program ) PURE;
    virtual void SetProgram( IKeGpuProgram* gpu_program ) PURE;
    virtual void SetProgramConstant1FV( const char* location, int count, float* value ) PURE;
    virtual void SetProgramConstant2FV( const char* location, int count, float* value ) PURE;
    virtual void SetProgramConstant3FV( const char* location, int count, float* value ) PURE;
    virtual void SetProgramConstant4FV( const char* location, int count, float* value ) PURE;
    virtual void SetProgramConstant1IV( const char* location, int count, int* value ) PURE;
    virtual void SetProgramConstant2IV( const char* location, int count, int* value ) PURE;
    virtual void SetProgramConstant3IV( const char* location, int count, int* value ) PURE;
    virtual void SetProgramConstant4IV( const char* location, int count, int* value ) PURE;
    virtual void GetProgramConstantFV( const char* location, float* value ) PURE;
    virtual void GetProgramConstantIV( const char* location, int* value ) PURE;
	virtual bool CreateConstantBuffer( uint32_t buffer_size, IKeConstantBuffer** constant_buffer ) PURE;
	virtual void DeleteConstantBuffer( IKeConstantBuffer* constant_buffer ) PURE;
	virtual bool SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer ) PURE;
	virtual void SetVertexShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer ) PURE;
	virtual void SetPixelShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer ) PURE;
	virtual void SetGeometryShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer ) PURE;
	virtual void SetTesselationShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer ) PURE;
    virtual bool CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    virtual bool CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    virtual bool CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL ) PURE;
    virtual void DeleteTexture( IKeTexture* texture ) PURE;
    virtual void SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    virtual void SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    virtual void SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture ) PURE;
    virtual bool CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget ) PURE;
    virtual void DeleteRenderTarget( IKeRenderTarget* rendertarget ) PURE;
    virtual void BindRenderTarget( IKeRenderTarget* rendertarget ) PURE;
    virtual void SetTexture( int stage, IKeTexture* texture ) PURE;
    virtual bool CreateStateBuffer( KeState* state_params, int state_count, IKeStateBuffer** state_buffer ) PURE;
    virtual bool SetStateBuffer( IKeStateBuffer* state_buffer ) PURE;
    virtual void SetRenderStates( KeState* states ) PURE;
    virtual void SetSamplerStates( KeState* states ) PURE;
//    virtual void draw_vertices_im() PURE;
    virtual void DrawVertices( uint32_t primtype, uint32_t stride, int first, int count ) PURE;
    virtual void DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count ) PURE;
    virtual void DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count ) PURE;
    
    virtual bool GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels ) PURE;
    
    /* Matrix/viewport related */
    virtual void SetViewport( int x, int y, int width, int height ) PURE;
    virtual void SetViewportV( int* viewport ) PURE;
    virtual void GetViewport( int* x, int* y, int* width, int* height ) PURE;
    virtual void GetViewportV( int* viewport ) PURE;
    virtual void SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z ) PURE;
    virtual void SetViewMatrix( const nv::matrix4f* view ) PURE;
    virtual void SetWorldMatrix( const nv::matrix4f* world ) PURE;
    virtual void SetModelviewMatrix( const nv::matrix4f* modelview ) PURE;
    virtual void SetProjectionMatrix( const nv::matrix4f* projection ) PURE;
    
    /* Synchronization */
    virtual void BlockUntilVerticalBlank() PURE;
    virtual void SetSwapInterval( int swap_interval ) PURE;
    virtual int GetSwapInterval() PURE;
	virtual void BlockUntilIdle() PURE;
	virtual void Kick() PURE;
	virtual bool InsertFence( IKeFence** fence ) PURE;
	virtual bool TestFence( IKeFence* fence ) PURE;
	virtual void BlockOnFence( IKeFence* fence ) PURE;
	virtual void DeleteFence( IKeFence* fence ) PURE;
	virtual bool IsFence( IKeFence* fence ) PURE;
    
    /* Misc */
    virtual void GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory ) PURE;
    
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
    virtual bool Initialized() PURE;
    virtual void SetStates( IKeStateBuffer* state ) PURE;
    virtual void SetProgram( IKeGpuProgram* program ) PURE;
    virtual void SetTexture( IKeTexture* texture ) PURE;
    virtual void SetVertexData( void* vertex_data, uint32_t offset, uint32_t vertex_data_size ) PURE;
    virtual bool Lock( void** data ) PURE;
    virtual void Unlock( void* data ) PURE;
    
    virtual void Draw() PURE;
    
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
