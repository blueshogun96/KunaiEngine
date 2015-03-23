//
//  ke_renderdevice.h
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_renderdevice__
#define __ke_renderdevice__


#include "KePlatform.h"
#include "vectormath.h"
#include "NvFoundationMath.h"
#include "NV/NvMath.h"


/*
 * Render device types
 */
#define KE_RENDERDEVICE_D3D11     1
#define KE_RENDERDEVICE_OGL3      2
#define KE_RENDERDEVICE_OGL4      3
#define KE_RENDERDEVICE_OGLES2    4
#define KE_RENDERDEVICE_OGLES3    5


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
 * Renderstate types 
 */
#define KE_RS_DEPTHTEST     1
#define KE_RS_DEPTHFUNC     2
#define KE_RS_DEPTHMASK     3
#define KE_RS_CLEARDEPTH    4
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
 * Constant buffer structure
 */
struct IKeConstantBuffer {};

/*
 * Geometry buffer base structure
 */
struct IKeGeometryBuffer {};

/*
 * Push buffer base structure
 */
struct IKePushBuffer {};

/*
 * Universal shader base structure
 */
struct IKeShader {};

/*
 * GPU Program base structure
 */
struct IKeGpuProgram {};

/*
 * Texture base structure
 */
struct IKeTexture {};

/*
 * Rendertarget base structure
 */
struct IKeRenderTarget {};

/*
 * Palette base structure
 */
struct IKePalette {};

/*
 * Fence base structure
 */
struct IKeFence {};


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
    virtual void SetRenderStates( KeState* states ) PURE;
    virtual void SetSamplerStates( KeState* states ) PURE;
//    virtual void draw_vertices_im() PURE;
    virtual void DrawVertices( uint32_t primtype, uint32_t stride, int first, int count ) PURE;
    virtual void DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count ) PURE;
    virtual void DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count ) PURE;
    
    virtual bool GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels ) PURE;
    
    /* Matrix/viewport related */
    virtual void SetViewport( int x, int y, int width, int height ) PURE;
    virtual void SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z ) PURE;
    virtual void SetViewMatrix( const Matrix4* view ) PURE;
    virtual void SetWorldMatrix( const Matrix4* world ) PURE;
    virtual void SetModelviewMatrix( const Matrix4* modelview ) PURE;
    virtual void SetProjectionMatrix( const Matrix4* projection ) PURE;
    
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
    Matrix4                 world_matrix;
    Matrix4                 view_matrix;
    Matrix4                 modelview_matrix;
    Matrix4                 projection_matrix;
    IKeGeometryBuffer*		current_geometrybuffer;
    IKeGpuProgram*			current_gpu_program;
	IKeTexture*				current_texture[8];
    KeVertexAttribute		current_vertexattribute[32];
};


/*
 * Render device creation function
 */
IKeRenderDevice* KeCreateRenderDevice( KeRenderDeviceDesc* renderdevice_desc );


#endif /* defined(__ke_renderdevice__) */
