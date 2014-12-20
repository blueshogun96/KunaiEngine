//
//  ke_d3d11_renderdevice.h
//
//  Created by Shogun3D on 8/24/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_d3d11_renderdevice__
#define __ke_d3d11_renderdevice__

#include "ke_renderdevice.h"
#ifdef __APPLE__
#error "Direct3D is not supported on non-Microsoft platforms!"
#else
#include <SDL.h>
//#include <d3d11.h>
//#include <d3d11_1.h>
#include <d3d11_2.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#endif



/*
* Constant buffer structure
*/
struct ke_d3d11_constantbuffer_t : public ke_constantbuffer_t
{
	ID3D11Buffer*	cb;		/* Constant buffer */
};

/*
* Geometry buffer structure
*/
struct ke_d3d11_geometrybuffer_t : public ke_geometrybuffer_t
{
	ID3D11Buffer*	vb;		/* Vertex buffer */
	ID3D11Buffer*	ib;		/* Index buffer */
	uint32_t stride;		/* Length of vertex data (in bytes) */
	uint32_t index_type;	/* Data type for index data */
};

/*
* GPU Program structure
*/
struct ke_d3d11_gpu_program_t : public ke_gpu_program_t
{
	ID3D11VertexShader*		vs;		/* Vertex shader */
	ID3D11PixelShader*		ps;		/* Pixel shader */
	ID3D11GeometryShader*	gs;		/* Geometry shader */
	ID3D11HullShader*		hs;		/* Hull shader */
	ID3D11DomainShader*		ds;		/* Domain shader */
	ID3D11ComputeShader*	cs;		/* Compute shader */
	ID3D11InputLayout*		il;		/* Vertex input layout */
};

/*
* Texture base structure
*/
struct ke_d3d11_texture_t : public ke_texture_t
{
	ID3D11Texture1D*	tex1d;
	ID3D11Texture2D*	tex2d;
	ID3D11Texture3D*	tex3d;
	uint32_t			flags;
};

/*
* Rendertarget base structure
*/
struct ke_d3d11_rendertarget_t : public ke_rendertarget_t
{
	uint32_t    frame_buffer_object;    /* Frame buffer object handle */
	uint32_t    depth_render_buffer;    /* Depth render buffer */
	/* TODO: Stencil? */

	ke_texture_t* texture;
};

/*
* Palette base structure
*/
struct ke_d3d11_palette_t : public ke_palette_t
{

};

/*
* GPU fence structure
*/
struct ke_d3d11_fence_t : public ke_fence_t
{
	ID3D11Query* query;
};


/*
* Render device base class
*/
class ke_d3d11_renderdevice_t : public ke_renderdevice_t
{
public:
	ke_d3d11_renderdevice_t();
	ke_d3d11_renderdevice_t(ke_renderdevice_desc_t* renderdevice_desc);
	virtual ~ke_d3d11_renderdevice_t();

public:
	/* Misc */
	virtual bool confirm_device();
	virtual void get_device_desc(ke_renderdevice_desc_t* device_desc);

	/* General rendering stuff */
	virtual void set_clear_colour_fv(float* colour);
	virtual void set_clear_colour_ubv(uint8_t* colour);
	virtual void set_clear_depth(float depth);
	virtual void clear_colour_buffer();
	virtual void clear_depth_buffer();
	virtual void clear_stencil_buffer();
	virtual void swap();

	virtual bool create_geometry_buffer(void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, ke_vertexattribute_t* va, ke_geometrybuffer_t** geometry_buffer);
	virtual void delete_geometry_buffer(ke_geometrybuffer_t* geometry_buffer);
	virtual void set_geometry_buffer(ke_geometrybuffer_t* geometry_buffer);
	virtual bool create_program(const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, ke_vertexattribute_t* vertex_attributes, ke_gpu_program_t** gpu_program);
	virtual void delete_program(ke_gpu_program_t* gpu_program);
	virtual void set_program(ke_gpu_program_t* gpu_program);
	virtual void set_program_constant_1fv(const char* location, int count, float* value);
	virtual void set_program_constant_2fv(const char* location, int count, float* value);
	virtual void set_program_constant_3fv(const char* location, int count, float* value);
	virtual void set_program_constant_4fv(const char* location, int count, float* value);
	virtual void set_program_constant_1iv(const char* location, int count, int* value);
	virtual void set_program_constant_2iv(const char* location, int count, int* value);
	virtual void set_program_constant_3iv(const char* location, int count, int* value);
	virtual void set_program_constant_4iv(const char* location, int count, int* value);
	virtual void get_program_constant_fv(const char* location, float* value);
	virtual void get_program_constant_iv(const char* location, int* value);
	virtual bool create_constant_buffer( uint32_t buffer_size, ke_constantbuffer_t** constant_buffer );
	virtual void delete_constant_buffer( ke_constantbuffer_t* constant_buffer );
	virtual bool set_constant_buffer_data( void* data, ke_constantbuffer_t* constant_buffer );
	virtual void set_vertex_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer );
	virtual void set_pixel_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer );
	virtual void set_geometry_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer );
	virtual void set_tesselation_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer );
	virtual bool create_texture_1d(uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture, void* pixels = NULL);
	virtual bool create_texture_2d(uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture, void* pixels = NULL);
	virtual bool create_texture_3d(uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture, void* pixels = NULL);
	virtual void delete_texture(ke_texture_t* texture);
	virtual void set_texture_data_1d(int offsetx, int width, int miplevel, void* pixels, ke_texture_t* texture);
	virtual void set_texture_data_2d(int offsetx, int offsety, int width, int height, int miplevel, void* pixels, ke_texture_t* texture);
	virtual void set_texture_data_3d(int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, ke_texture_t* texture);
	virtual bool create_render_target(int width, int height, int depth, uint32_t flags, ke_rendertarget_t** rendertarget);
	virtual void delete_render_target(ke_rendertarget_t* rendertarget);
	virtual void bind_render_target(ke_rendertarget_t* rendertarget);
	virtual void set_texture(int stage, ke_texture_t* texture);
	virtual void set_render_states(ke_state_t* states);
	virtual void set_sampler_states(ke_state_t* states);
	//virtual void draw_vertices_im();
	virtual void draw_vertices(uint32_t primtype, uint32_t stride, int first, int count);
	virtual void draw_indexed_vertices(uint32_t primtype, uint32_t stride, int count);
	virtual void draw_indexed_vertices_range(uint32_t primtype, uint32_t stride, int start, int end, int count);

	virtual bool get_framebuffer_region(int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels);

	/* Matrix/viewport related */
	virtual void set_viewport(int x, int y, int width, int height);
	virtual void set_perspective_matrix(float fov, float aspect, float near_z, float far_z);
	virtual void set_view_matrix(const Matrix4* view);
	virtual void set_world_matrix(const Matrix4* world);
	virtual void set_modelview_matrix(const Matrix4* modelview);
	virtual void set_projection_matrix(const Matrix4* projection);

	/* Synchronization */
	virtual void block_until_vertical_blank();
	virtual void set_swap_interval(int swap_interval);
	virtual int get_swap_interval();
	virtual void block_until_idle();
	virtual void kick();
	virtual bool insert_fence( ke_fence_t** fence );
	virtual bool test_fence( ke_fence_t* fence );
	virtual void block_on_fence( ke_fence_t* fence );
	virtual void delete_fence( ke_fence_t* fence );
    virtual bool is_fence( ke_fence_t* fence );

	/* Misc */
    virtual void gpu_memory_info( uint32_t* total_memory, uint32_t* free_memory );

protected:
	SDL_Window*				window;
	D3D_DRIVER_TYPE			driver_type;
	D3D_FEATURE_LEVEL		feature_level;
	ID3D11Device*			d3ddevice;
	ID3D11DeviceContext*	d3ddevice_context;
	IDXGISwapChain*			dxgi_swap_chain; 
	IDXGIOutput*			dxgi_output;
	ID3D11RenderTargetView* d3d_render_target_view;
	DXGI_SWAP_CHAIN_DESC	swapchain_desc;
	int						swap_interval;
};

#endif /* defined(__ke_d3d11_renderdevice__) */
