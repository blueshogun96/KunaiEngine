//
//  ke_renderdevice.h
//
//  Created by Shogun3D on 5/22/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_renderdevice__
#define __ke_renderdevice__


#include "ke_platform.h"
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



/*
 * Render device description
 */
struct ke_renderdevice_desc_t
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
struct ke_vertexattribute_t
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
struct ke_state_t
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
struct ke_constantbuffer_t {};

/*
 * Geometry buffer base structure
 */
struct ke_geometrybuffer_t {};

/*
 * Universal shader base structure
 */
struct ke_shader_t {};

/*
 * GPU Program base structure
 */
struct ke_gpu_program_t {};

/*
 * Texture base structure
 */
struct ke_texture_t {};

/*
 * Rendertarget base structure
 */
struct ke_rendertarget_t {};

/*
 * Palette base structure
 */
struct ke_palette_t {};

/*
 * Fence base structure
 */
struct ke_fence_t {};


/*
 * Render device base class
 */
class ke_renderdevice_t
{
public:
    ke_renderdevice_t() { /*assert(0);*/ }
    ke_renderdevice_t( ke_renderdevice_desc_t* renderdevice_desc ) { assert(0); }
    virtual ~ke_renderdevice_t() { /*assert(0);*/ }
    
public:
    /* Misc */
    virtual bool confirm_device() PURE;
    virtual void get_device_desc( ke_renderdevice_desc_t* device_desc ) PURE;
    
    /* General rendering stuff */
    virtual void set_clear_colour_fv( float* colour ) PURE;
    virtual void set_clear_colour_ubv( uint8_t* colour ) PURE;
    virtual void set_clear_depth( float depth ) PURE;
    virtual void clear_colour_buffer() PURE;
    virtual void clear_depth_buffer() PURE;
    virtual void clear_stencil_buffer() PURE;
    virtual void swap() PURE;
    
    virtual bool create_geometry_buffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, ke_vertexattribute_t* va, ke_geometrybuffer_t** geometry_buffer ) PURE;
    virtual void delete_geometry_buffer( ke_geometrybuffer_t* geometry_buffer ) PURE;
    virtual void set_geometry_buffer( ke_geometrybuffer_t* geometry_buffer ) PURE;
    virtual bool create_program( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, ke_vertexattribute_t* vertex_attributes, ke_gpu_program_t** gpu_program ) PURE;
    virtual void delete_program( ke_gpu_program_t* gpu_program ) PURE;
    virtual void set_program( ke_gpu_program_t* gpu_program ) PURE;
    virtual void set_program_constant_1fv( const char* location, int count, float* value ) PURE;
    virtual void set_program_constant_2fv( const char* location, int count, float* value ) PURE;
    virtual void set_program_constant_3fv( const char* location, int count, float* value ) PURE;
    virtual void set_program_constant_4fv( const char* location, int count, float* value ) PURE;
    virtual void set_program_constant_1iv( const char* location, int count, int* value ) PURE;
    virtual void set_program_constant_2iv( const char* location, int count, int* value ) PURE;
    virtual void set_program_constant_3iv( const char* location, int count, int* value ) PURE;
    virtual void set_program_constant_4iv( const char* location, int count, int* value ) PURE;
    virtual void get_program_constant_fv( const char* location, float* value ) PURE;
    virtual void get_program_constant_iv( const char* location, int* value ) PURE;
	virtual bool create_constant_buffer( uint32_t buffer_size, ke_constantbuffer_t** constant_buffer ) PURE;
	virtual void delete_constant_buffer( ke_constantbuffer_t* constant_buffer ) PURE;
	virtual bool set_constant_buffer_data( void* data, ke_constantbuffer_t* constant_buffer ) PURE;
	virtual void set_vertex_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer ) PURE;
	virtual void set_pixel_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer ) PURE;
	virtual void set_geometry_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer ) PURE;
	virtual void set_tesselation_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer ) PURE;
    virtual bool create_texture_1d( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture ) PURE;
    virtual bool create_texture_2d( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture ) PURE;
    virtual bool create_texture_3d( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture ) PURE;
    virtual void delete_texture( ke_texture_t* texture ) PURE;
    virtual void set_texture_data_1d( int offsetx, int width, int miplevel, void* pixels, ke_texture_t* texture ) PURE;
    virtual void set_texture_data_2d( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, ke_texture_t* texture ) PURE;
    virtual void set_texture_data_3d( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, ke_texture_t* texture ) PURE;
    virtual bool create_render_target( int width, int height, int depth, uint32_t flags, ke_rendertarget_t** rendertarget ) PURE;
    virtual void delete_render_target( ke_rendertarget_t* rendertarget ) PURE;
    virtual void bind_render_target( ke_rendertarget_t* rendertarget ) PURE;
    virtual void set_texture( int stage, ke_texture_t* texture ) PURE;
    virtual void set_render_states( ke_state_t* states ) PURE;
    virtual void set_sampler_states( ke_state_t* states ) PURE;
//    virtual void draw_vertices_im() PURE;
    virtual void draw_vertices( uint32_t primtype, uint32_t stride, int first, int count ) PURE;
    virtual void draw_indexed_vertices( uint32_t primtype, uint32_t stride, int count ) PURE;
    virtual void draw_indexed_vertices_range( uint32_t primtype, uint32_t stride, int start, int end, int count ) PURE;
    
    virtual bool get_framebuffer_region( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels ) PURE;
    
    /* Matrix/viewport related */
    virtual void set_viewport( int x, int y, int width, int height ) PURE;
    virtual void set_perspective_matrix( float fov, float aspect, float near_z, float far_z ) PURE;
    virtual void set_view_matrix( const Matrix4* view ) PURE;
    virtual void set_world_matrix( const Matrix4* world ) PURE;
    virtual void set_modelview_matrix( const Matrix4* modelview ) PURE;
    virtual void set_projection_matrix( const Matrix4* projection ) PURE;
    
    /* Synchronization */
    virtual void block_until_vertical_blank() PURE;
    virtual void set_swap_interval( int swap_interval ) PURE;
    virtual int get_swap_interval() PURE;
    
protected:
    bool                    initialized;
    float                   clear_colour[4];
    float                   clear_depth;
	uint32_t				clear_stencil;
    int                     viewport[4];
    ke_renderdevice_desc_t* device_desc;
    Matrix4                 world_matrix;
    Matrix4                 view_matrix;
    Matrix4                 modelview_matrix;
    Matrix4                 projection_matrix;
    ke_geometrybuffer_t*    current_geometrybuffer;
    ke_gpu_program_t*       current_gpu_program;
    ke_vertexattribute_t    current_vertexattribute[32];
};


/*
 * Render device creation function
 */
ke_renderdevice_t* ke_create_renderdevice( ke_renderdevice_desc_t* renderdevice_desc );


#endif /* defined(__ke_renderdevice__) */
