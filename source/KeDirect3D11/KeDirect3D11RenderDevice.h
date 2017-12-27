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
#ifndef _WIN32
#error "Direct3D is not supported on non-Microsoft platforms!"
#else
#include <SDL.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <comip.h>
#include <comdef.h>
#include <ddraw.h>
#endif


/*
 * Non-ATL based smart COM pointer types
 */
#define _ComPtr(_interface)	_com_ptr_t<_com_IIID<_interface, __uuidof(_interface)>>

#ifndef _UWP	/* For Desktop Windows only */
typedef _com_ptr_t<_com_IIID<IDirectDraw7, &IID_IDirectDraw7>>							CDirectDraw7;
#endif

typedef _com_ptr_t<_com_IIID<ID3D11Device, &IID_ID3D11Device>>							CD3D11Device;
typedef _com_ptr_t<_com_IIID<ID3D11DeviceContext, &IID_ID3D11DeviceContext>>			CD3D11DeviceContext;
typedef _com_ptr_t<_com_IIID<IDXGISwapChain, &IID_IDXGISwapChain>>						CDXGISwapChain;
typedef _com_ptr_t<_com_IIID<IDXGIOutput, &IID_IDXGIOutput>>							CDXGIOutput;
typedef _com_ptr_t<_com_IIID<IDXGIDevice, &IID_IDXGIDevice>>							CDXGIDevice;
typedef _com_ptr_t<_com_IIID<IDXGIDevice1, &IID_IDXGIDevice1>>							CDXGIDevice1;
typedef _com_ptr_t<_com_IIID<IDXGIAdapter, &IID_IDXGIAdapter>>							CDXGIAdapter;
typedef _com_ptr_t<_com_IIID<IDXGIFactory, &IID_IDXGIFactory>>							CDXGIFactory;
typedef _com_ptr_t<_com_IIID<IDXGIFactory1, &IID_IDXGIFactory1>>						CDXGIFactory1;
typedef _com_ptr_t<_com_IIID<ID3D11RenderTargetView, &IID_ID3D11RenderTargetView>>		CD3D11RenderTargetView;
typedef _com_ptr_t<_com_IIID<ID3D11DepthStencilView, &IID_ID3D11DepthStencilView>>		CD3D11DepthStencilView;
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
typedef _com_ptr_t<_com_IIID<ID3D11ShaderResourceView, &IID_ID3D11ShaderResourceView>>	CD3D11ShaderResourceView;
typedef _com_ptr_t<_com_IIID<ID3D10Blob, &IID_ID3D10Blob>>								CD3D10Blob;

#ifdef __d3d11_1_h__
typedef _com_ptr_t<_com_IIID<ID3D11Device1, &IID_ID3D11Device1>>						CD3D11Device1;
typedef _com_ptr_t<_com_IIID<ID3D11DeviceContext1, &IID_ID3D11DeviceContext1>>			CD3D11DeviceContext1;
typedef _com_ptr_t<_com_IIID<IDXGISwapChain1, &IID_IDXGISwapChain1>>					CDXGISwapChain1;
typedef _com_ptr_t<_com_IIID<IDXGIOutput1, &IID_IDXGIOutput1>>							CDXGIOutput1;
typedef _com_ptr_t<_com_IIID<IDXGIDevice2, &IID_IDXGIDevice2>>							CDXGIDevice2;
typedef _com_ptr_t<_com_IIID<IDXGIFactory2, &IID_IDXGIFactory2>>						CDXGIFactory2;
typedef _com_ptr_t<_com_IIID<ID3D11BlendState1, &IID_ID3D11BlendState1>>				CD3D11BlendState1;
typedef _com_ptr_t<_com_IIID<ID3D11RasterizerState1, &IID_ID3D11RasterizerState1>>		CD3D11RasterizerState1;
#endif

#ifdef __d3d11_2_h__
typedef _com_ptr_t<_com_IIID<ID3D11Device2, &IID_ID3D11Device2>>						CD3D11Device2;
typedef _com_ptr_t<_com_IIID<ID3D11DeviceContext2, &IID_ID3D11DeviceContext2>>			CD3D11DeviceContext2;
typedef _com_ptr_t<_com_IIID<IDXGISwapChain2, &IID_IDXGISwapChain2>>					CDXGISwapChain2;
typedef _com_ptr_t<_com_IIID<IDXGISwapChainMedia, &IID_IDXGISwapChainMedia>>			CDXGISwapChainMedia;
typedef _com_ptr_t<_com_IIID<IDXGIDevice3, &IID_IDXGIDevice3>>							CDXGIDevice3;
#endif

/*
 * Constant buffer structure
 */
struct IKeDirect3D11ConstantBuffer : public IKeConstantBuffer
{
	KEMETHOD            Destroy();
    
    _KEMETHOD(void*)    MapData( uint32_t flags );
    KEMETHOD            UnmapData( void* );
    
    _KEMETHOD(bool)     SetConstantData( uint32_t offset, uint32_t size, void* ptr );
    KEMETHOD            GetDesc( KeConstantBufferDesc* desc );
    
	CD3D11Buffer	cb;				/* Constant buffer */
	uint32_t		data_size;      /* Size of the data buffer */
    uint32_t		flags;          /* Buffer creation flags */
    uint32_t		block_index;    /* Block index representing the block name we are attempting to access */
    char			block_name[64]; /* Name of the block/struct within the GPU program we are writing data to */
};

/*
 * Geometry buffer structure
 */
struct IKeDirect3D11GeometryBuffer : public IKeGeometryBuffer
{
	virtual void Destroy();
    
    virtual void* MapData( uint32_t flags );
	virtual void* MapDataAsync( uint32_t flags );
    virtual void UnmapData( void* );
	virtual void UnmapDataAsync( void* );

	virtual bool SetVertexData( uint32_t offset, uint32_t size, void* ptr );
    virtual bool SetIndexData( uint32_t offset, uint32_t size, void* ptr );
	virtual void GetDesc( KeGeometryBufferDesc* desc );

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
	virtual void GetVertexAttributes( KeVertexAttribute* vertex_attributes );
    
	CD3D11VertexShader		vs;		/* Vertex shader */
	CD3D11PixelShader		ps;		/* Pixel shader */
	CD3D11GeometryShader	gs;		/* Geometry shader */
	CD3D11HullShader		hs;		/* Hull shader */
	CD3D11DomainShader		ds;		/* Domain shader */
	CD3D11ComputeShader		cs;		/* Compute shader */
	CD3D11InputLayout		il;		/* Vertex input layout */
	KeVertexAttribute*		va;
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
	virtual bool GetTextureDesc( KeTextureDesc* texture_desc );

	CD3D11Texture1D		tex1d;
	CD3D11Texture2D		tex2d;
	CD3D11Texture3D		tex3d;
	uint32_t			flags;

    uint32_t width, height;     /* Texture width/height */
    uint32_t depth;             /* Texture depth (for 3D and array textures) */
    uint32_t depth_format;      /* See glTexImageXD */
    uint32_t internal_format;   /* See glTexImageXD */
    uint32_t data_type;         /* Internal data type */
    uint32_t target;            /* Direct3D11 texture type */
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
    
	virtual bool Insert();
    virtual bool Test();
    virtual void Block();
    virtual bool Valid();

	CD3D11Query			query;
	CD3D11DeviceContext context;	/* Copy of the render device's context */
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
	virtual void SetClearStencil( uint32_t stencil );
    virtual void ClearColourBuffer();
    virtual void ClearDepthBuffer();
    virtual void ClearStencilBuffer();
	virtual void Clear( uint32_t buffers );
    virtual void Swap();
    
	virtual void SetIMCacheSize( uint32_t cache_size );
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
	virtual bool CreateConstantBuffer( KeConstantBufferDesc* desc, IKeConstantBuffer** constant_buffer, void* data = NULL );
	virtual void DeleteConstantBuffer( IKeConstantBuffer* constant_buffer );
	virtual bool SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer );
	virtual void SetConstantBuffer( int slot, int shader_type, IKeConstantBuffer* constant_buffer );
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
	virtual void DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, void* vertex_data );
    virtual void DrawIndexedVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int count, void* vertex_data, void* index_data );
    virtual void DrawIndexedVerticesRangeIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int start, int end, int count, void* vertex_data, void* index_data );
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
	virtual void GetViewMatrix( nv::matrix4f* view );
    virtual void GetWorldMatrix( nv::matrix4f* world );
    virtual void GetModelviewMatrix( nv::matrix4f* modelview );
    virtual void GetProjectionMatrix( nv::matrix4f* projection );
    
    /* Synchronization */
    virtual void BlockUntilVerticalBlank();
    virtual void SetSwapInterval( int swap_interval );
    virtual int GetSwapInterval();
	virtual void BlockUntilIdle();
	virtual void Kick();
	virtual bool CreateFence( IKeFence** fence, uint32_t flags );
    
    /* Misc */
    virtual void GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory );

protected:
#ifdef _UWP
	bool PVT_InitializeDirect3DUWP();
#else
	bool PVT_InitializeDirect3DWin32();
#endif

protected:
	SDL_Window*						window;
	D3D_DRIVER_TYPE					driver_type;
	D3D_FEATURE_LEVEL				feature_level;
	CD3D11Device					d3ddevice;
	CD3D11DeviceContext				d3ddevice_context;
#ifdef _UWP
	CDXGISwapChain1					dxgi_swap_chain;
#else
	CDXGISwapChain					dxgi_swap_chain; 
#endif
	CDXGIOutput						dxgi_output;
	CD3D11RenderTargetView			d3d_render_target_view;
	CD3D11Texture2D					d3d_depth_stencil_buffer;
	CD3D11DepthStencilView			d3d_depth_stencil_view;
#ifdef _UWP
	DXGI_SWAP_CHAIN_DESC1			swapchain_desc;
#else
	DXGI_SWAP_CHAIN_DESC			swapchain_desc;
#endif
	int								swap_interval;
#ifndef _UWP
	CDirectDraw7					dd;
#else
	CDXGIDevice3					dxgi_device;
	CDXGIAdapter					dxgi_adapter;
	CDXGIFactory2					dxgi_factory;
#endif
	uint32_t						im_cache_size;
    IKeGeometryBuffer*				im_gb;
};

#endif /* defined(__ke_d3d11_renderdevice__) */
