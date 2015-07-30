//
//  ke_d3d11_renderdevice.h
//
//  Created by Shogun3D on 8/24/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_d3d11_renderdevice__
#define __ke_d3d11_renderdevice__

#pragma warning(disable:4838)

#include "KeRenderDevice.h"
#ifdef __APPLE__
#error "Direct3D is not supported on non-Microsoft platforms!"
#else
#include <SDL.h>
//#include <d3d11.h>
//#include <d3d11_1.h>
#include <d3d11_2.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <comip.h>
#include <comdef.h>
#include <ddraw.h>
#endif


/*
 * Non-ATL based smart COM pointer types
 */
typedef _com_ptr_t<_com_IIID<IDirectDraw7, &IID_IDirectDraw7>>							CDirectDraw7;
typedef _com_ptr_t<_com_IIID<ID3D11Device, &IID_ID3D11Device>>							CD3D11Device;
typedef _com_ptr_t<_com_IIID<ID3D11DeviceContext, &IID_ID3D11DeviceContext>>			CD3D11DeviceContext;
typedef _com_ptr_t<_com_IIID<IDXGISwapChain, &IID_IDXGISwapChain>>						CDXGISwapChain;
typedef _com_ptr_t<_com_IIID<IDXGIOutput, &IID_IDXGIOutput>>							CDXGIOutput;
typedef _com_ptr_t<_com_IIID<ID3D11RenderTargetView, &IID_ID3D11RenderTargetView>>		CD3D11RenderTargetView;
typedef _com_ptr_t<_com_IIID<ID3D11Buffer, &IID_ID3D11Buffer>>							CD3D11Buffer;
typedef _com_ptr_t<_com_IIID<ID3D11VertexShader, &IID_ID3D11VertexShader>>				CD3D11VertexShader;
typedef _com_ptr_t<_com_IIID<ID3D11PixelShader, &IID_ID3D11PixelShader>>				CD3D11PixelShader;
typedef _com_ptr_t<_com_IIID<ID3D11GeometryShader, &IID_ID3D11GeometryShader>>			CD3D11GeometryShader;
typedef _com_ptr_t<_com_IIID<ID3D11HullShader, &IID_ID3D11HullShader>>					CD3D11HullShader;
typedef _com_ptr_t<_com_IIID<ID3D11DomainShader, &IID_ID3D11DomainShader>>				CD3D11DomainShader;
typedef _com_ptr_t<_com_IIID<ID3D11ComputeShader, &IID_ID3D11ComputeShader>>			CD3D11ComputeShader;
typedef _com_ptr_t<_com_IIID<ID3D11InputLayout, &IID_ID3D11InputLayout>>				CD3D11InputLayout;
typedef _com_ptr_t<_com_IIID<ID3D11Texture1D, &IID_ID3D11Texture1D>>					CD3D11Texture1D;
typedef _com_ptr_t<_com_IIID<ID3D11Texture2D, &IID_ID3D11Texture2D>>					CD3D11Texture2D;
typedef _com_ptr_t<_com_IIID<ID3D11Texture3D, &IID_ID3D11Texture3D>>					CD3D11Texture3D;
typedef _com_ptr_t<_com_IIID<ID3D11BlendState, &IID_ID3D11BlendState>>					CD3D11BlendState;
typedef _com_ptr_t<_com_IIID<ID3D11RasterizerState, &IID_ID3D11RasterizerState>>		CD3D11RasterizerState;
typedef _com_ptr_t<_com_IIID<ID3D11DepthStencilState, &IID_ID3D11DepthStencilState>>	CD3D11DepthStencilState;
typedef _com_ptr_t<_com_IIID<ID3D11SamplerState, &IID_ID3D11SamplerState>>				CD3D11SamplerState;
typedef _com_ptr_t<_com_IIID<ID3D11Query, &IID_ID3D11Query>>							CD3D11Query;
typedef _com_ptr_t<_com_IIID<ID3D11CommandList, &IID_ID3D11CommandList>>				CD3D11CommandList;


/*
 * Constant buffer structure
 */
struct IKeDirect3D11ConstantBuffer : public IKeConstantBuffer
{
	virtual void Destroy();
    
	CD3D11Buffer	cb;		/* Constant buffer */
};

/*
 * Geometry buffer structure
 */
struct IKeDirect3D11GeometryBuffer : public IKeGeometryBuffer
{
	virtual void Destroy();
    
    virtual void* MapData( uint32_t flags );
    virtual void UnmapData( void* );

	virtual bool SetVertexData( uint32_t offset, uint32_t size, void* ptr );
    virtual bool SetIndexData( uint32_t offset, uint32_t size, void* ptr );

	CD3D11Buffer	vb;		/* Vertex buffer */
	CD3D11Buffer	ib;		/* Index buffer */
	uint32_t stride;		/* Length of vertex data (in bytes) */
	uint32_t index_type;	/* Data type for index data */
};

/*
 * Command list structure
 */
struct IKeDirect3D11CommandList : public IKeCommandList
{
	virtual void Destroy();

	CD3D11CommandList cl;
};

/*
 * GPU Program structure
 */
struct IKeDirect3D11GpuProgram : public IKeGpuProgram
{
	virtual void Destroy();
    
	CD3D11VertexShader		vs;		/* Vertex shader */
	CD3D11PixelShader		ps;		/* Pixel shader */
	CD3D11GeometryShader	gs;		/* Geometry shader */
	CD3D11HullShader		hs;		/* Hull shader */
	CD3D11DomainShader		ds;		/* Domain shader */
	CD3D11ComputeShader		cs;		/* Compute shader */
	CD3D11InputLayout		il;		/* Vertex input layout */
};

/*
 * Texture base structure
 */
struct IKeDirect3D11Texture : public IKeTexture
{
	virtual void Destroy();

    virtual void* MapData( uint32_t flags );
    virtual void UnmapData( void* );

	virtual bool SetTextureData( KeTextureDesc* texture_data, void* pixels );

	CD3D11Texture1D		tex1d;
	CD3D11Texture2D		tex2d;
	CD3D11Texture3D		tex3d;
	uint32_t			flags;
};

/*
 * Rendertarget base structure
 */
struct IKeDirect3D11RenderTarget : public IKeRenderTarget
{
	virtual void Destroy();
    
    virtual void* MapData( uint32_t flags );
    virtual void UnmapData( void* );

	virtual bool GetTexture( IKeTexture** texture );
	virtual IKeTexture* GetTexture2();

	uint32_t    frame_buffer_object;    /* Frame buffer object handle */
	uint32_t    depth_render_buffer;    /* Depth render buffer */
	/* TODO: Stencil? */

	IKeDirect3D11Texture* texture;
};

/*
 * Palette base structure
 */
struct IKeDirect3D11Palette : public IKePalette
{

};

/*
 * GPU fence structure
 */
struct IKeDirect3D11Fence : public IKeFence
{
	virtual void Destroy();
    
	CD3D11Query query;
};

/*
 * Render/Texture state structure
 */
struct IKeDirect3D11RenderStateBuffer : public IKeRenderStateBuffer
{
    virtual void Destroy();

	CD3D11BlendState			bs;
	CD3D11RasterizerState		rs;
	CD3D11DepthStencilState		dss;
};

struct IKeDirect3D11TextureSamplerBuffer : public IKeTextureSamplerBuffer
{
	virtual void Destroy();

	CD3D11SamplerState			ss;
};


/*
 * Render device base class
 */
class IKeDirect3D11RenderDevice : public IKeRenderDevice
{
public:
	IKeDirect3D11RenderDevice();
	IKeDirect3D11RenderDevice( KeRenderDeviceDesc* renderdevice_desc );
	virtual ~IKeDirect3D11RenderDevice();

public:
	/* Misc */
    virtual bool ConfirmDevice();
    virtual void GetDeviceDesc( KeRenderDeviceDesc* device_desc );
	virtual void GetDeviceCaps( KeRenderDeviceCaps* device_caps );
    
    /* General rendering stuff */
    virtual void SetClearColourFV( float* colour );
    virtual void SetClearColourUBV( uint8_t* colour );
    virtual void SetClearDepth( float depth );
    virtual void ClearColourBuffer();
    virtual void ClearDepthBuffer();
    virtual void ClearStencilBuffer();
    virtual void Swap();
    
    virtual bool CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer );
    virtual void DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
    virtual void SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer );
    virtual bool CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program );
    virtual void DeleteProgram( IKeGpuProgram* gpu_program );
    virtual void SetProgram( IKeGpuProgram* gpu_program );
    virtual void SetProgramConstant1FV( const char* location, int count, float* value );
    virtual void SetProgramConstant2FV( const char* location, int count, float* value );
    virtual void SetProgramConstant3FV( const char* location, int count, float* value );
    virtual void SetProgramConstant4FV( const char* location, int count, float* value );
    virtual void SetProgramConstant1IV( const char* location, int count, int* value );
    virtual void SetProgramConstant2IV( const char* location, int count, int* value );
    virtual void SetProgramConstant3IV( const char* location, int count, int* value );
    virtual void SetProgramConstant4IV( const char* location, int count, int* value );
    virtual void GetProgramConstantFV( const char* location, float* value );
    virtual void GetProgramConstantIV( const char* location, int* value );
	virtual bool CreateConstantBuffer( uint32_t buffer_size, IKeConstantBuffer** constant_buffer );
	virtual void DeleteConstantBuffer( IKeConstantBuffer* constant_buffer );
	virtual bool SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer );
	virtual void SetVertexShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetPixelShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetGeometryShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
	virtual void SetTesselationShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer );
    virtual bool CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual bool CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual bool CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels = NULL );
    virtual void DeleteTexture( IKeTexture* texture );
    virtual void SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture );
    virtual void SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture );
    virtual void SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture );
    virtual bool CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget );
    virtual void DeleteRenderTarget( IKeRenderTarget* rendertarget );
    virtual void BindRenderTarget( IKeRenderTarget* rendertarget );
    virtual void SetTexture( int stage, IKeTexture* texture );
	virtual bool CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer );
	virtual bool CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer );
	virtual bool SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer );
	virtual bool SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer );
	virtual void SetRenderStates( KeState* states );
	virtual void SetSamplerStates( int stage, KeState* states );
	virtual void DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, uint8_t* vertex_data );
    virtual void DrawVertices( uint32_t primtype, uint32_t stride, int first, int count );
    virtual void DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count );
    virtual void DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count );
    
    virtual bool GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels );
    
    /* Matrix/viewport related */
    virtual void SetViewport( int x, int y, int width, int height );
    virtual void SetViewportV( int* viewport );
    virtual void GetViewport( int* x, int* y, int* width, int* height );
    virtual void GetViewportV( int* viewport );
    virtual void SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z );
    virtual void SetViewMatrix( const nv::matrix4f* view );
    virtual void SetWorldMatrix( const nv::matrix4f* world );
    virtual void SetModelviewMatrix( const nv::matrix4f* modelview );
    virtual void SetProjectionMatrix( const nv::matrix4f* projection );
    
    /* Synchronization */
    virtual void BlockUntilVerticalBlank();
    virtual void SetSwapInterval( int swap_interval );
    virtual int GetSwapInterval();
	virtual void BlockUntilIdle();
	virtual void Kick();
	virtual bool InsertFence( IKeFence** fence );
	virtual bool TestFence( IKeFence* fence );
	virtual void BlockOnFence( IKeFence* fence );
	virtual void DeleteFence( IKeFence* fence );
	virtual bool IsFence( IKeFence* fence );
    
    /* Misc */
    virtual void GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory );

protected:
	SDL_Window*						window;
	D3D_DRIVER_TYPE					driver_type;
	D3D_FEATURE_LEVEL				feature_level;
	CD3D11Device					d3ddevice;
	CD3D11DeviceContext				d3ddevice_context;
	CDXGISwapChain					dxgi_swap_chain; 
	CDXGIOutput						dxgi_output;
	CD3D11RenderTargetView			d3d_render_target_view;
	DXGI_SWAP_CHAIN_DESC			swapchain_desc;
	int								swap_interval;
	CDirectDraw7					dd;
};

#endif /* defined(__ke_d3d11_renderdevice__) */
