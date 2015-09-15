//
//  IKeDirect3D11RenderDevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

/* DirectX Error naming collisions in dxgitype.h and winerror.h */
#pragma warning(disable:4005)
/* Annoying conversion warnings */
#pragma warning(disable:4244)

#include "KeDirect3D11RenderDevice.h"
#include "KeDebug.h"

/*
 * DirectDraw stuff
 */
#ifdef _WIN32
#define USE_DDRAW_VMEM			/* Not guaranteed to be accurate on modern hardware */
//#define USE_DDRAW_VBLANK		/* Not recommended, but here if you need it... */

#include <ddraw.h>
#endif

/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define D3D_DISPDBG( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define D3D_DISPDBG_R( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define D3D_DISPDBG_RB( a, b, c ) if(FAILED(c)) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }

/*
* Globals
*/

struct D3D11Semantic
{
	char name[16];
	int index;
};

struct D3D11MatrixBuffer
{
	float world[16];
	float view[16];
	float projection[16];
} matrix_buffer;

//glBindAttribLocation( p, 0, "in_pos" );
//    glBindAttribLocation( p, 1, "in_normal" );
//    glBindAttribLocation( p, 2, "in_tangent" );
//    glBindAttribLocation( p, 3, "in_bitangent" );
//    glBindAttribLocation( p, 4, "in_colour" );
//    glBindAttribLocation( p, 5, "in_tex0" );
//    glBindAttribLocation( p, 6, "in_tex1" );
//    glBindAttribLocation( p, 7, "in_tex2" );
//    glBindAttribLocation( p, 8, "in_tex3" );
//	glBindAttribLocation( p, 9, "in_tex4" );
//    glBindAttribLocation( p, 10, "in_tex5" );
//    glBindAttribLocation( p, 11, "in_tex6" );
//    glBindAttribLocation( p, 12, "in_tex7" );

struct D3D11Semantic semantic_list[] = 
{
	{ "POSITION", 0 },
	{ "NORMAL", 0 },
	{ "TANGENT", 0 },
	{ "BITANGENT", 0 },
	{ "COLOR", 0 },
	{ "TEXTURE0", 0 },
};



/* Direct3D11 primitive types */
D3D_PRIMITIVE_TOPOLOGY primitive_types[] =
{
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,		/* TODO: Line loop */
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

/* Direct3D11 depth/alpha test functions */
D3D11_COMPARISON_FUNC test_funcs[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

/* Direct3D11 fill modes */
D3D11_FILL_MODE fill_modes[] =
{
	D3D11_FILL_WIREFRAME,	/* Direct3D11 doesn't support point mode */
	D3D11_FILL_WIREFRAME,
	D3D11_FILL_SOLID,
};

/* Direct3D11 blend modes */
D3D11_BLEND blend_modes[] = 
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
    D3D11_BLEND_SRC_COLOR,
    D3D11_BLEND_INV_SRC_COLOR,
    D3D11_BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA,
    D3D11_BLEND_DEST_ALPHA,
    D3D11_BLEND_INV_DEST_ALPHA,
    D3D11_BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR,
    D3D11_BLEND_SRC_ALPHA_SAT,
    D3D11_BLEND_BLEND_FACTOR,
    D3D11_BLEND_INV_BLEND_FACTOR,
    D3D11_BLEND_SRC1_COLOR,
    D3D11_BLEND_INV_SRC1_COLOR,
    D3D11_BLEND_SRC1_ALPHA,
    D3D11_BLEND_INV_SRC1_ALPHA
};

DXGI_FORMAT data_types[] = 
{
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32_TYPELESS,	/* TODO: Will this work for double? */
};

/* Direct3D texture formats */
DXGI_FORMAT texture_formats[] =
{
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_B8G8R8A8_UNORM
};

#if 0

/* OpenGL texture targets */
uint32_t texture_targets[] =
{
	GL_TEXTURE_1D,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_3D,
	GL_TEXTURE_RECTANGLE,
};

/* OpenGL polygon modes */
uint32_t polygon_modes[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};


/* OpenGL cull modes */
uint32_t cull_modes[] =
{
	GL_NONE,
	GL_CW,
	GL_CCW
};
#endif

std::string KeDirect3D11FeatureLevelString( D3D_FEATURE_LEVEL feature )
{
	switch( feature )
	{
	case D3D_FEATURE_LEVEL_11_1: return "11.1";
	case D3D_FEATURE_LEVEL_11_0: return "11.0";
	case D3D_FEATURE_LEVEL_10_1: return "10.1";
	case D3D_FEATURE_LEVEL_10_0: return "10.0";
	case D3D_FEATURE_LEVEL_9_3:	 return "9.3";
	case D3D_FEATURE_LEVEL_9_2:  return "9.2";
	case D3D_FEATURE_LEVEL_9_1:  return "9.1";
	}

	return "";
}

/*
* Name: ke_initialize_default_shaders
* Desc: Initializes the default shaders to be used when there is
*       no user defined program used.
*/
bool ke_d3d11_initialize_default_shaders()
{

	return true;
}

/*
* Name: ke_uninitialize_default_shaders
* Desc:
*/
void ke_d3d11_uninitialize_default_shaders()
{
	
}

/*
* Name: IKeDirect3D11RenderDevice::IKeDirect3D11RenderDevice
* Desc: Default constructor
*/
IKeDirect3D11RenderDevice::IKeDirect3D11RenderDevice()
{
	/* TODO: Disable by making private? */
	assert(No);
}


/*
* Name: IKeDirect3D11RenderDevice::
* Desc:
*/

/*
* Name: IKeDirect3D11RenderDevice::IKeDirect3D11RenderDevice
* Desc: Appropriate constructor used for initialization of Direct3D and a window via SDL.
*/
IKeDirect3D11RenderDevice::IKeDirect3D11RenderDevice( KeRenderDeviceDesc* renderdevice_desc ) : swap_interval(0)
{
	/* Until we are finished initializing, mark this flag as false */
	initialized = false;

	/* Sanity checks */
	if( !renderdevice_desc )
		return;

	/* Save a copy of the render device description */
	device_desc = new KeRenderDeviceDesc;
	memmove( device_desc, renderdevice_desc, sizeof(KeRenderDeviceDesc));

	/* Verify device type */
	if( device_desc->device_type == KE_RENDERDEVICE_OGL3 || device_desc->device_type == KE_RENDERDEVICE_OGLES2 || 
		device_desc->device_type == KE_RENDERDEVICE_OGLES3 || device_desc->device_type == KE_RENDERDEVICE_OGL4 )
		return;

#if defined(USE_DDRAW_VMEM) || defined(USE_DDRAW_VBLANK)
	/* Create a DirectDraw object if desired */
	HRESULT hr = DirectDrawCreateEx( NULL, (void**) &dd, IID_IDirectDraw7, NULL );
	if( FAILED( hr ) )
		D3D_DISPDBG_R( KE_ERROR, "Error creating DirectDraw7 object.  Disable DirectDraw if not needed!", hr );
#endif

	/* Initialize SDL video */
	if( SDL_InitSubSystem( SDL_INIT_VIDEO ) != 0 )
		DISPDBG_R( KE_ERROR, "Error initializing SDL video sub system!" );

	/* Initialize the SDL window */
	window = SDL_CreateWindow( "Kunai Engine 0.1a", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		device_desc->width, device_desc->height, SDL_WINDOW_SHOWN );
	if( !window )
		 DISPDBG_R( KE_ERROR, "Error creating SDL window!" );

	/* Initialize Direct3D11 */
	uint32_t flags = 0;
	D3D_FEATURE_LEVEL feature_levels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	int feature_level_count = ARRAYSIZE( feature_levels );

#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	ZeroMemory( &swapchain_desc, sizeof( swapchain_desc ) );
	swapchain_desc.BufferCount = renderdevice_desc->buffer_count;
    swapchain_desc.BufferDesc.Width = renderdevice_desc->width;
    swapchain_desc.BufferDesc.Height = renderdevice_desc->height;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchain_desc.BufferDesc.RefreshRate.Numerator = renderdevice_desc->refresh_rate;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow = GetActiveWindow();
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SampleDesc.Quality = 0;
    swapchain_desc.Windowed = !renderdevice_desc->fullscreen;

	hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, feature_levels, feature_level_count, 
		D3D11_SDK_VERSION, &swapchain_desc, &dxgi_swap_chain, &d3ddevice, &feature_level, &d3ddevice_context );
	if( FAILED( hr ) )
		D3D_DISPDBG_R( KE_ERROR, "Error creating Direct3D11 device and swapchain!", hr );

	/* Create our render target view */
	ID3D11Texture2D* back_buffer = NULL;
    hr = dxgi_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&back_buffer );
    if( FAILED( hr ) )
        D3D_DISPDBG_R( KE_ERROR, "Error getting back buffer!", hr );

    hr = d3ddevice->CreateRenderTargetView( back_buffer, NULL, &d3d_render_target_view );
    back_buffer = 0;
    if( FAILED( hr ) )
        D3D_DISPDBG_R( KE_ERROR, "Error creating render target view!", hr );

    d3ddevice_context->OMSetRenderTargets( 1, &d3d_render_target_view, NULL );

    /* Setup the viewport */
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT) renderdevice_desc->width;
    vp.Height = (FLOAT) renderdevice_desc->height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    d3ddevice_context->RSSetViewports( 1, &vp );

	/* Get DXGI output */
	if( FAILED( hr = dxgi_swap_chain->GetContainingOutput( &dxgi_output ) ) )
	{
		DISPDBG( KE_WARNING, "IDXGISwapChain::GetContainingOutput returned (0x" + hr );
		dxgi_output = nullptr;
	}

	/* Set vertex attributes to their defaults */
	current_vertexattribute[0].index = 0;
	current_vertexattribute[0].size = 3;
	current_vertexattribute[0].type = KE_FLOAT;
	current_vertexattribute[0].normalize = No;
	current_vertexattribute[0].stride = 0;
	current_vertexattribute[0].offset = 0;
	current_vertexattribute[1].index = -1;

	/* Nullify current geometry buffer */
	current_geometrybuffer = NULL;

	/* Mark as initialized */
	initialized = Yes;

	/* Print Direct3D version details */
	DISPDBG( 1, "Direct3D version: " << KeDirect3D11FeatureLevelString( feature_level ) << std::endl );
}


/*
* Name: IKeDirect3D11RenderDevice::~IKeDirect3D11RenderDevice
* Desc: Default deconstructor
*/
IKeDirect3D11RenderDevice::~IKeDirect3D11RenderDevice()
{
	delete device_desc;

	/* Kill the default vertex and fragment program */
	ke_d3d11_uninitialize_default_shaders();

	/* Uninitialize and close Direct3D and SDL */
	
	dxgi_output = 0;
	d3ddevice_context->ClearState();
	d3d_render_target_view = 0;
	dxgi_swap_chain = 0;
	d3ddevice_context = 0;
	d3ddevice = 0;

	SDL_DestroyWindow( window );
	SDL_QuitSubSystem( SDL_INIT_VIDEO );

	/* Uninitialize DirectDraw */
#ifdef _WIN32
	dd = 0;
#endif
}


/*
* Name: IKeDirect3D11RenderDevice::confirm_device
* Desc: Gives confirmation that this device was successfully initialized.
*/
bool IKeDirect3D11RenderDevice::ConfirmDevice()
{
	return initialized;
}

/*
* Name: IKeDirect3D11RenderDevice::get_device_desc
* Desc: Returns a copy of the device description structure
*/
void IKeDirect3D11RenderDevice::GetDeviceDesc( KeRenderDeviceDesc* device_desc )
{
	memmove( device_desc, this->device_desc, sizeof( KeRenderDeviceDesc ) );
}

/*
 * Name: IKeDirect3D11RenderDevice::GetDeviceCaps
 * Desc: Returns a copy of the device capabilities structure
 */
void IKeDirect3D11RenderDevice::GetDeviceCaps( KeRenderDeviceCaps* device_caps )
{
    memmove( device_caps, this->device_caps, sizeof( KeRenderDeviceCaps ) );
}

/*
* Name: IKeDirect3D11RenderDevice::set_clear_colour_fv
* Desc: Sets the clear colour
*/
void IKeDirect3D11RenderDevice::SetClearColourFV( float* colour )
{
	memcpy( this->clear_colour, colour, sizeof(float)*4 );
}


/*
* Name: IKeDirect3D11RenderDevice::set_clear_colour_ubv
* Desc: Same as above.
*/
void IKeDirect3D11RenderDevice::SetClearColourUBV( uint8_t* colour )
{
	this->clear_colour[0] = float(colour[0] / 255);
	this->clear_colour[1] = float(colour[1] / 255);
	this->clear_colour[2] = float(colour[2] / 255);
	this->clear_colour[3] = float(colour[3] / 255);
}


/*
* Name: IKeDirect3D11RenderDevice::set_clear_depth
* Desc:
*/
void IKeDirect3D11RenderDevice::SetClearDepth( float depth )
{
	clear_depth = depth;
}


/*
 * Name: IKeDirect3D11RenderDevice::SetClearStencil
 * Desc: 
 */
void IKeDirect3D11RenderDevice::SetClearStencil( uint32_t stencil )
{
	clear_stencil = stencil;
}

/*
* Name: IKeDirect3D11RenderDevice::clear_render_buffer
* Desc: Clears only the current render buffer
*/
void IKeDirect3D11RenderDevice::ClearColourBuffer()
{
	d3ddevice_context->ClearRenderTargetView( d3d_render_target_view, clear_colour );
}


/*
* Name: IKeDirect3D11RenderDevice::clear_depth_buffer
* Desc: Clears only the current depth buffer
*/
void IKeDirect3D11RenderDevice::ClearDepthBuffer()
{
	
}


/*
* Name: IKeDirect3D11RenderDevice::clear_stencil_buffer
* Desc: Clears only the current stencil buffer
*/
void IKeDirect3D11RenderDevice::ClearStencilBuffer()
{
	
}


/*
 * Name: IKeDirect3D11RenderDevice::Clear
 * Desc: 
 */
void IKeDirect3D11RenderDevice::Clear( uint32_t buffers )
{
	if( buffers & KE_COLOUR_BUFFER )
		d3ddevice_context->ClearRenderTargetView( d3d_render_target_view, clear_colour );

	uint32_t flags = 0;

	if( buffers & KE_DEPTH_BUFFER )		flags |= 0x1;
	if( buffers & KE_STENCIL_BUFFER )	flags |= 0x2;

	if( flags )
		d3ddevice_context->ClearDepthStencilView( d3d_depth_stencil_view, (D3D11_CLEAR_FLAG) flags, clear_depth, clear_stencil );
}


/*
 * Name: IKeDirect3D11RenderDevice::swap
 * Desc: Swaps the double buffer.
 */
void IKeDirect3D11RenderDevice::Swap()
{
	HRESULT hr = dxgi_swap_chain->Present( swap_interval, 0 );
	if( FAILED( hr ) )
		DISPDBG( KE_ERROR, "IDXGISwapChain::Present(): Error = 0x" << hr << "\n" );
}


/*
* Name: IKeDirect3D11RenderDevice::create_geometry_buffer
* Desc: Creates a geometry buffer based on the vertex and index data given.  Vertex and index
*       buffers are encapsulated into one interface for easy management, however, index data
*       input is completely optional.  Interleaved vertex data is also supported.
*/
bool IKeDirect3D11RenderDevice::CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer )
{
	HRESULT hr = S_OK;

	/* Sanity check(s) */
    if( !geometry_buffer )
        DISPDBG_RB( KE_ERROR, "Invalid interface pointer!" );
    //if( !vertex_attributes )
      //  return false;
    if( !vertex_data_size )
        DISPDBG_RB( KE_ERROR, "(vertex_data_size == 0) condition is currently not allowed..." );   /* Temporary? */
 
    *geometry_buffer = new IKeDirect3D11GeometryBuffer;
    IKeDirect3D11GeometryBuffer* gb = static_cast<IKeDirect3D11GeometryBuffer*>( *geometry_buffer );

	/* Create a vertex buffer */
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertex_data_size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;	/* TODO */
	D3D11_SUBRESOURCE_DATA id;
	ZeroMemory( &id, sizeof(id) );
	id.pSysMem = vertex_data;

	hr = d3ddevice->CreateBuffer( &bd, &id, &gb->vb );
	if( FAILED( hr ) )
	{
		delete (*geometry_buffer);
		D3D_DISPDBG_RB( KE_ERROR, "Error creating vertex buffer!", hr );
	}

	/* Create index buffer, if desired. */
	gb->ib = NULL;
	if( index_data_size )
	{
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = index_data_size;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory( &id, sizeof(id) );
		id.pSysMem = index_data;

		hr = d3ddevice->CreateBuffer( &bd, &id, &gb->ib );
		if( FAILED( hr ) )
		{
			delete (*geometry_buffer);
			D3D_DISPDBG_RB( KE_ERROR, "Error creating index buffer!", hr );
		}

		gb->index_type = index_data_type;
	}

	return true;
}


/*
* Name: IKeDirect3D11RenderDevice::create_geometry_buffer
* Desc:
*/
void IKeDirect3D11RenderDevice::DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
	IKeDirect3D11GeometryBuffer* gb = static_cast<IKeDirect3D11GeometryBuffer*>(geometry_buffer);

	/* Release vertex and index buffers */
	gb->vb = 0;
	gb->ib = 0;

	delete geometry_buffer;
}

/*
* Name: IKeDirect3D11RenderDevice::set_vertex_buffer
* Desc: Sets the current geometry buffer to be used when rendering. Internally, binds the
*       vertex array object. If NULL, then sets the current vertex array object to 0.
*/
void IKeDirect3D11RenderDevice::SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
	current_geometrybuffer = geometry_buffer;
}

/*
* Name: IKeDirect3D11RenderDevice::create_program
* Desc: Creates a complete OpenGL program out of shaders in text form. The minimum requirements
*       are one valid vertex and fragment shader, while geometry and tesselation shaders are
*       optional.  Obviously, tesselation shaders require OpenGL 4.1+, and cannot be used with
*       OpenGL 3.2.  This function will automatically search for specific attribute locations
*       before linking it and search for pre-determined uniform names for textures and matrices
*       (see code below).
*       TODO: Allow user defined constants.
*/
bool IKeDirect3D11RenderDevice::CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program )
{
	D3D11_INPUT_ELEMENT_DESC* layout = NULL;
	int layout_size = 0;
	DXGI_FORMAT fmt;
	DWORD shader_flags = D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef _DEBUG
	shader_flags |= D3D10_SHADER_DEBUG;
#endif

	/* Allocate new GPU program */
	*gpu_program = new IKeDirect3D11GpuProgram;
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>( *gpu_program );
	ZeroMemory( gp, sizeof( IKeDirect3D11GpuProgram ) );

	/* Create Direct3D compatible vertex layout */
	while( vertex_attributes[layout_size].index != -1 )
		layout_size++;

	layout = new D3D11_INPUT_ELEMENT_DESC[layout_size];
	if( layout )
	{
		for( int i = 0; i < layout_size; i++ )
		{
			if( vertex_attributes[i].type == KE_FLOAT && vertex_attributes[i].size == 1 ) 
				fmt = DXGI_FORMAT_R32_FLOAT;
			if( vertex_attributes[i].type == KE_FLOAT && vertex_attributes[i].size == 2 ) 
				fmt = DXGI_FORMAT_R32G32_FLOAT;
			if( vertex_attributes[i].type == KE_FLOAT && vertex_attributes[i].size == 3 ) 
				fmt = DXGI_FORMAT_R32G32B32_FLOAT;
			if( vertex_attributes[i].type == KE_FLOAT && vertex_attributes[i].size == 4 ) 
				fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;

			if( !strcmp( "POSITION", semantic_list[vertex_attributes[i].index].name ) )
				layout[i].SemanticName = "POSITION";
			if( !strcmp( "COLOR", semantic_list[vertex_attributes[i].index].name ) )
				layout[i].SemanticName = "COLOR";

			layout[i].SemanticIndex = semantic_list[vertex_attributes[i].index].index;
			layout[i].Format = fmt;
			layout[i].InputSlot = 0; /* TODO */
			layout[i].AlignedByteOffset = vertex_attributes[i].offset;
			layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			layout[i].InstanceDataStepRate = 0; /* TODO */
		}

		
		/* Initialize vertex shader */
		/* TODO: Auto detect highest shader version */
		ID3D10Blob* blob_shader = NULL;
		ID3D10Blob* blob_error = NULL;

		HRESULT hr = D3DCompile( vertex_shader, lstrlenA( vertex_shader ) + 1, "vs_main", NULL, NULL, "vs_main", 
                     "vs_4_0", shader_flags, 0, &blob_shader, &blob_error );
		if( FAILED( hr ) )
		{
			if( blob_error != NULL )
			{
				DISPDBG( KE_ERROR, "Error compiling vertex shader source!\n" << (char*)blob_error->GetBufferPointer() << "\n" );
				delete[] layout;
				blob_error = 0;
				DeleteProgram(gp);
			}

			return false;
		}

		hr = d3ddevice->CreateVertexShader( blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), NULL, &gp->vs );
		if( FAILED( hr ) )
		{
			delete[] layout;
			blob_shader = 0;
			DeleteProgram(gp);
			DISPDBG( KE_ERROR, "Error creating vertex shader!\n" );
		}

		/* Create input layout */
		hr = d3ddevice->CreateInputLayout( layout, layout_size, blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), &gp->il );
		blob_shader = 0;
		delete[] layout;
		if( FAILED( hr ) )
		{
			DeleteProgram( gp );
			DISPDBG( KE_ERROR, "Error creating input layout!\n" );
		}

		/* Create pixel shader */
		hr = D3DCompile( fragment_shader, lstrlenA( fragment_shader ) + 1, "ps_main", NULL, NULL, "ps_main", 
                     "ps_4_0", shader_flags, 0, &blob_shader, &blob_error );
		if( FAILED( hr ) )
		{
			if( blob_error != NULL )
			{
				DISPDBG( KE_ERROR, "Error compiling pixel shader source!\n" << (char*)blob_error->GetBufferPointer() << "\n" );
				blob_error = 0;
				DeleteProgram(gp);
			}

			return false;
		}

		hr = d3ddevice->CreatePixelShader( blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), NULL, &gp->ps );
		if( FAILED( hr ) )
		{
			blob_shader = 0;
			DeleteProgram(gp);
			DISPDBG( KE_ERROR, "Error creating pixel shader!\n" );
		}

		blob_shader = 0;

		/* TODO: Geometry, Hull, Compute and Domain shaders */
		gp->hs = NULL;
		gp->gs = NULL;
		gp->cs = NULL;
		gp->ds = NULL;
	}

#if 1
	/* Copy vertex attributes */
	int va_size = 0;
	while( vertex_attributes[va_size].index != -1 )
		va_size++;

	gp->va = new KeVertexAttribute[va_size+1];
	memmove( gp->va, vertex_attributes, sizeof( KeVertexAttribute ) * (va_size+1) );
#endif

	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::DeleteProgram
* Desc: Deletes the GPU program.
*/
void IKeDirect3D11RenderDevice::DeleteProgram( IKeGpuProgram* gpu_program )
{
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>( gpu_program );

	/* Deletes the GPU program */
	if(gp)
	{
		if( gp->il ) gp->il = 0;
		if( gp->hs ) gp->hs = 0;
		if( gp->cs ) gp->cs = 0;
		if( gp->ds ) gp->ds = 0;
		if( gp->gs ) gp->gs = 0;
		if( gp->ps ) gp->ps = 0;
		if( gp->vs ) gp->vs = 0;

		delete gpu_program;
	}
}

/*
* Name: IKeDirect3D11RenderDevice::set_program
* Desc: Sets the GPU program.  If NULL, the GPU program is set to 0.
*/
void IKeDirect3D11RenderDevice::SetProgram( IKeGpuProgram* gpu_program )
{
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>( gpu_program );

	/* Set input layout */
	if(gp)
		d3ddevice_context->IASetInputLayout( gp->il );
	else
		d3ddevice_context->IASetInputLayout( NULL );

	/* Set shaders */
	if(gp)
	{
		d3ddevice_context->VSSetShader( gp->vs, NULL, 0 );
		d3ddevice_context->PSSetShader( gp->ps, NULL, 0 );
		d3ddevice_context->GSSetShader( gp->gs, NULL, 0 );
		d3ddevice_context->HSSetShader( gp->hs, NULL, 0 );
		d3ddevice_context->DSSetShader( gp->ds, NULL, 0 );
		d3ddevice_context->CSSetShader( gp->cs, NULL, 0 );
	}
	else
	{
		d3ddevice_context->VSSetShader( NULL, NULL, 0 );
		d3ddevice_context->PSSetShader( NULL, NULL, 0 );
		d3ddevice_context->GSSetShader( NULL, NULL, 0 );
		d3ddevice_context->HSSetShader( NULL, NULL, 0 );
		d3ddevice_context->DSSetShader( NULL, NULL, 0 );
		d3ddevice_context->CSSetShader( NULL, NULL, 0 );
	}
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant_1fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant1FV( const char* location, int count, float* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);

	
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant_2fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant2FV( const char* location, int count, float* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);

	
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant_3fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant3FV( const char* location, int count, float* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant_4fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant4FV( const char* location, int count, float* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant1IV( const char* location, int count, int* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant2IV( const char* location, int count, int* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant3IV( const char* location, int count, int* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::SetProgramConstant4IV( const char* location, int count, int* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::get_program_constant_fv
* Desc: Gets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::GetProgramConstantFV( const char* location, float* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
* Name: IKeDirect3D11RenderDevice::get_program_constant_iv
* Desc: Gets program constants (do your research on GLSL uniforms)
*/
void IKeDirect3D11RenderDevice::GetProgramConstantIV( const char* location, int* value )
{
	IKeDirect3D11GpuProgram* p = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);
}

/*
 * Name: IKeDirect3D11RenderDevice::create_constant_buffer
 * Desc: Creates a constant buffer used for storing/setting constants in one call instead of one by one.
 *		 If supported, this is the recommended method for setting constants.
 */
bool IKeDirect3D11RenderDevice::CreateConstantBuffer( uint32_t buffer_size, IKeConstantBuffer** constant_buffer )
{
	*constant_buffer = new IKeDirect3D11ConstantBuffer;
	IKeDirect3D11ConstantBuffer* cb = static_cast<IKeDirect3D11ConstantBuffer*>( *constant_buffer );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = buffer_size;
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = d3ddevice->CreateBuffer( &bd, NULL, &cb->cb );
	if( FAILED( hr ) )
	{
		delete *constant_buffer;
		DISPDBG( KE_ERROR, "Error creating constant buffer (0x" << hr << ")!\n" );
		return false;
	}

	return true;
}

/*
 * Name: IKeDirect3D11RenderDevice::delete_constant_buffer
 * Desc: Deletes a constant buffer.
 */
void IKeDirect3D11RenderDevice::DeleteConstantBuffer( IKeConstantBuffer* constant_buffer )
{
	IKeDirect3D11ConstantBuffer* cb = static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer );

	if( cb )
	{
		if( cb->cb ) cb->cb = 0;
		delete constant_buffer;
	}
}

/*
 * Name: IKeDirect3D11RenderDevice::set_constant_buffer_data
 * Desc: 
 */
bool IKeDirect3D11RenderDevice::SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer )
{
	if( !constant_buffer )
		return false;

	d3ddevice_context->UpdateSubresource( static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer )->cb, 0, NULL, data, 0, 0 );

	return true;
}

/*
 * Name: ke_ogl_renderdevice_t::set_vertex_shader_constant_buffer
 * Desc: 
 */
void IKeDirect3D11RenderDevice::SetVertexShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{
	d3ddevice_context->VSSetConstantBuffers( slot, 1, &static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer )->cb );
}


/*
 * Name: IKeDirect3D11RenderDevice::set_pixel_shader_constant_buffer
 * Desc: 
 */
void IKeDirect3D11RenderDevice::SetPixelShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{
	d3ddevice_context->PSSetConstantBuffers( slot, 1, &static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer )->cb );
}

/*
 * Name: IKeDirect3D11RenderDevice::set_geometry_shader_constant_buffer
 * Desc: 
 */
void IKeDirect3D11RenderDevice::SetGeometryShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{
	d3ddevice_context->GSSetConstantBuffers( slot, 1, &static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer )->cb );
}

/*
 * Name: IKeDirect3D11RenderDevice::set_tesselation_shader_constant_buffer
 * Desc: 
 */
void IKeDirect3D11RenderDevice::SetTesselationShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{
	d3ddevice_context->HSSetConstantBuffers( slot, 1, &static_cast<IKeDirect3D11ConstantBuffer*>( constant_buffer )->cb );
}

/*
* Name: IKeDirect3D11RenderDevice::create_texture_1d
* Desc: Creates a 1D texture.
*/
bool IKeDirect3D11RenderDevice::CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
	/* Allocate a new texture */
	*texture = new IKeDirect3D11Texture;
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(*texture);

	/* Set texture attributes */
	/*t->width = width;
	t->target = target;
	t->data_type = data_types[data_type];
	t->depth_format = texture_formats[format];
	t->internal_format = texture_formats[format];
	t->target = target;*/
	
	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::create_texture_2d
* Desc: Creates a blank 2D texture.
*/
bool IKeDirect3D11RenderDevice::CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
	/* Allocate a new texture */
	(*texture) = new IKeDirect3D11Texture;
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(*texture);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = mipmaps;
	desc.ArraySize = 1;  /* TODO */
	desc.Format = texture_formats[format];
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;	// TODO 
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	HRESULT hr = d3ddevice->CreateTexture2D( &desc, NULL, &t->tex2d );
	D3D_DISPDBG_RB( KE_ERROR, "Error creating 2D texture!", hr );

	if( pixels )
	{
		D3D11_MAPPED_SUBRESOURCE res;
		hr = d3ddevice_context->Map( t->tex2d, 0, D3D11_MAP_WRITE_DISCARD, 0, &res );
		if( SUCCEEDED( hr ) )
		{
			memcpy( res.pData, pixels, width*height*4 );	// TODO
			d3ddevice_context->Unmap( t->tex2d, 0 );
		}
	}

	/* Set texture attributes */
	t->width = width;
	t->height = height;
	t->target = target;
	t->data_type = data_types[data_type];
	t->depth_format = texture_formats[format];
	t->internal_format = texture_formats[format];
	t->target = target;

	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::create_texture_3d
* Desc: Creates a blank 3D texture.
*/
bool IKeDirect3D11RenderDevice::CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
	/* Allocate a new texture */
	(*texture) = new IKeDirect3D11Texture;
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(*texture);

	/* Set texture attributes */
	/*t->width = width;
	t->height = height;
	t->depth = depth;
	t->target = target;
	t->data_type = data_types[data_type];
	t->depth_format = texture_formats[format];
	t->internal_format = texture_formats[format];
	t->target = target;*/

	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::delete_texture
* Desc: Deletes a texture from memory.
*/
void IKeDirect3D11RenderDevice::DeleteTexture( IKeTexture* texture )
{
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(texture);

	if( texture )
	{
		delete texture;
	}
}


/*
* Name: IKeDirect3D11RenderDevice::set_texture_data_1d
* Desc: Sets pixel data for a 1D texture.
*/
void IKeDirect3D11RenderDevice::SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture )
{
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(texture);
}

/*
* Name: IKeDirect3D11RenderDevice::set_texture_data_2d
* Desc: Sets pixel data for a 1D texture.
*/
void IKeDirect3D11RenderDevice::SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture )
{
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(texture);
}

/*
* Name: IKeDirect3D11RenderDevice::set_texture_data_3d
* Desc: Sets pixel data for a 1D texture.
*/
void IKeDirect3D11RenderDevice::SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture )
{
	IKeDirect3D11Texture* t = static_cast<IKeDirect3D11Texture*>(texture);
}

/*
* Name: IKeDirect3D11RenderDevice::create_render_target
* Desc: Creates a seperate render target (FBO), typically used for rendering to a texture.
*       Creates a colour, depth and stencil buffer (if desired) and can be set as a texture.
*/
bool IKeDirect3D11RenderDevice::CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget )
{
	IKeDirect3D11RenderTarget* rt = static_cast<IKeDirect3D11RenderTarget*>(*rendertarget);

	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::delete_render_target
* Desc: Deletes the render target resources used.
*/
void IKeDirect3D11RenderDevice::DeleteRenderTarget( IKeRenderTarget* rendertarget )
{
	IKeDirect3D11RenderTarget* rt = static_cast<IKeDirect3D11RenderTarget*>(rendertarget);

	delete rendertarget;
}

/*
* Name: IKeDirect3D11RenderDevice::bind_render_target
* Desc: Binds the render target to OpenGL.  You set the texture to the appropriate  texture
*       stage yourself using ::set_texture().
*/
void IKeDirect3D11RenderDevice::BindRenderTarget( IKeRenderTarget* rendertarget )
{
	IKeDirect3D11RenderTarget* rt = static_cast<IKeDirect3D11RenderTarget*>(rendertarget);
}

/*
* Name: IKeDirect3D11RenderDevice::set_texture
* Desc: Sets a texture to the desired texture stage.  If NULL, then texturing is disabled on
*       the selected texture stage.
*/
void IKeDirect3D11RenderDevice::SetTexture( int stage, IKeTexture* texture )
{
	IKeDirect3D11Texture* tex = static_cast<IKeDirect3D11Texture*>(texture);
}

#if 0
/*
 * Name: IKeDirect3D11RenderDevice::CreateState
 * Desc: Creates a compiled buffer of render and texture states.  At the time of writing, OpenGL
 *       does not have an equivalent to Direct3D's state buffer, we have to create a faux state buffer
 *       and save/set each state individually.
 */
bool IKeDirect3D11RenderDevice::CreateStateBuffer( KeState* state_params, int state_count, IKeStateBuffer** state_buffer )
{
	D3D11_BLEND_DESC blend_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	D3D11_SAMPLER_DESC sampler_desc;
	HRESULT hr;
	bool use_blend = false, use_raster = false, use_depth_stencil = false, use_sampler = false;

	ZeroMemory( &blend_desc, sizeof( blend_desc ) );
	ZeroMemory( &raster_desc, sizeof( raster_desc ) );
	ZeroMemory( &depth_stencil_desc, sizeof( depth_stencil_desc ) );
	ZeroMemory( &sampler_desc, sizeof( sampler_desc ) );

    /* Create a new state buffer interface */
    (*state_buffer) = new IKeDirect3D11StateBuffer;
    IKeDirect3D11StateBuffer* sb = static_cast<IKeDirect3D11StateBuffer*>( *state_buffer );
    
	/* Apply each render state in the list */
	int i = 0;
    while( i != state_count )
    {
        switch( state_params[i].state )
        {
            case KE_RS_DEPTHTEST:
				use_depth_stencil = true;
				depth_stencil_desc.DepthEnable = state_params[i].param1 ? Yes : No;
                break;
                
            case KE_RS_DEPTHFUNC:
				use_depth_stencil = true;
				depth_stencil_desc.DepthFunc = test_funcs[state_params[i].param1];
                break;
                
            case KE_RS_DEPTHMASK:
				use_depth_stencil = true;
                depth_stencil_desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK) state_params[i].param1;
                break;
                
            /*case KE_RS_CLEARDEPTH:
                glClearDepth( state_params[i].fparam );
                break;*/
                
            case KE_RS_ALPHABLEND:
				use_blend = true;
				blend_desc.RenderTarget[0].BlendEnable = state_params[i].param1;
                break;
                
            case KE_RS_FRONTFACE:
                /* TODO */
				//raster_desc.FrontCounterClockwise = FALSE;
                break;
                
            case KE_RS_POLYGONMODE:
				use_raster = true;
				raster_desc.FillMode = (state_params[i].param2 == 0) ? (D3D11_FILL_MODE) 1 : fill_modes[state_params[i].param2];
                //glPolygonMode( polygon_modes[state_params[i].param1], fill_modes[state_params[i].param2] );
                break;
                
            case KE_RS_BLENDFUNC:
				use_blend = true;
				blend_desc.RenderTarget[0].SrcBlend = blend_modes[state_params[i].param1];
				blend_desc.RenderTarget[0].DestBlend = blend_modes[state_params[i].param2];
                break;
                
            case KE_RS_CULLMODE:
				use_raster = true;
				raster_desc.CullMode = (D3D11_CULL_MODE) state_params[i].param2;
                break;
                
            default:
                DISPDBG( KE_WARNING, "Bad render or texture state!\nstate: " << state_params[i].state << "\n"
                        "param1: " << state_params[i].param1 << "\n"
                        "param2: " << state_params[i].param2 << "\n"
                        "param3: " << state_params[i].param3 << "\n"
                        "fparam: " << state_params[i].fparam << "\n"
                        "dparam: " << state_params[i].dparam << "\n" );
                break;
        }
        
        i++;
    }

	/* Create our state buffers */
	if( use_blend )
	{
		hr = d3ddevice->CreateBlendState( &blend_desc, &sb->bs );
		D3D_DISPDBG( KE_ERROR, "Error creating blend state!", hr );
	}

	if( use_depth_stencil )
	{
		hr = d3ddevice->CreateDepthStencilState( &depth_stencil_desc, &sb->dss );
		D3D_DISPDBG( KE_ERROR, "Error creating depth stencil state!", hr );
	}

	if( use_raster )
	{
		hr = d3ddevice->CreateRasterizerState( &raster_desc, &sb->rs );
		D3D_DISPDBG( KE_ERROR, "Error creating rasterizer state!", hr );
	}

	if( use_sampler )
	{
		hr = d3ddevice->CreateSamplerState( &sampler_desc, &sb->ss );
		D3D_DISPDBG( KE_ERROR, "Error creating sampler state!", hr );
	}

    return true;
}
#endif

/*
 * Name: IKeDirect3D11RenderDevice::CreateRenderStateBuffer
 * Desc: Creates a compiled buffer of render states.
 */
bool IKeDirect3D11RenderDevice::CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer )
{
	D3D11_BLEND_DESC blend_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	HRESULT hr;
	bool use_blend = false, use_raster = false, use_depth_stencil = false, use_sampler = false;

	ZeroMemory( &blend_desc, sizeof( blend_desc ) );
	ZeroMemory( &raster_desc, sizeof( raster_desc ) );
	ZeroMemory( &depth_stencil_desc, sizeof( depth_stencil_desc ) );

	/* Create a new render state buffer interface */
	(*state_buffer) = new IKeDirect3D11RenderStateBuffer;
	IKeDirect3D11RenderStateBuffer* sb = static_cast<IKeDirect3D11RenderStateBuffer*>(*state_buffer);

	/* Apply each render state in the list */
	int i = 0;
	while (i != state_count)
	{
		switch (state_params[i].state)
		{
		case KE_RS_DEPTHTEST:
			use_depth_stencil = true;
			depth_stencil_desc.DepthEnable = state_params[i].param1 ? Yes : No;
			break;

		case KE_RS_DEPTHFUNC:
			use_depth_stencil = true;
			depth_stencil_desc.DepthFunc = test_funcs[state_params[i].param1];
			break;

		case KE_RS_DEPTHMASK:
			use_depth_stencil = true;
			depth_stencil_desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)state_params[i].param1;
			break;

			/*case KE_RS_CLEARDEPTH:
			glClearDepth( state_params[i].fparam );
			break;*/

		case KE_RS_ALPHABLEND:
			use_blend = true;
			blend_desc.RenderTarget[0].BlendEnable = state_params[i].param1;
			break;

		case KE_RS_FRONTFACE:
			/* TODO */
			//raster_desc.FrontCounterClockwise = FALSE;
			break;

		case KE_RS_POLYGONMODE:
			use_raster = true;
			raster_desc.FillMode = (state_params[i].param2 == 0) ? (D3D11_FILL_MODE)1 : fill_modes[state_params[i].param2];
			//glPolygonMode( polygon_modes[state_params[i].param1], fill_modes[state_params[i].param2] );
			break;

		case KE_RS_BLENDFUNC:
			use_blend = true;
			blend_desc.RenderTarget[0].SrcBlend = blend_modes[state_params[i].param1];
			blend_desc.RenderTarget[0].DestBlend = blend_modes[state_params[i].param2];
			break;

		case KE_RS_CULLMODE:
			use_raster = true;
			raster_desc.CullMode = (D3D11_CULL_MODE)state_params[i].param2;
			break;

		default:
			DISPDBG( KE_WARNING, "Bad render state!\nstate: " << state_params[i].state << "\n"
				"param1: " << state_params[i].param1 << "\n"
				"param2: " << state_params[i].param2 << "\n"
				"param3: " << state_params[i].param3 << "\n"
				"fparam: " << state_params[i].fparam << "\n"
				"dparam: " << state_params[i].dparam << "\n" );
			break;
		}

		i++;
	}

	/* Create our state buffers */
	if( use_blend )
	{
		hr = d3ddevice->CreateBlendState( &blend_desc, &sb->bs );
		D3D_DISPDBG( KE_ERROR, "Error creating blend state!", hr );
	}

	if( use_depth_stencil )
	{
		hr = d3ddevice->CreateDepthStencilState( &depth_stencil_desc, &sb->dss );
		D3D_DISPDBG( KE_ERROR, "Error creating depth stencil state!", hr );
	}

	if( use_raster )
	{
		hr = d3ddevice->CreateRasterizerState( &raster_desc, &sb->rs );
		D3D_DISPDBG( KE_ERROR, "Error creating rasterizer state!", hr );
	}

	return true;
}


/*
 * Name: IKeDirect3D11RenderDevice::CreateTextureSamplerBuffer
 * Desc: Creates a compiled buffer of texture sampler states.
 */
bool IKeDirect3D11RenderDevice::CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer )
{
	D3D11_SAMPLER_DESC sampler_desc;
	HRESULT hr;

	ZeroMemory( &sampler_desc, sizeof( sampler_desc ) );

	/* Create a new state buffer interface */
	(*state_buffer) = new IKeDirect3D11TextureSamplerBuffer;
	IKeDirect3D11TextureSamplerBuffer* sb = static_cast<IKeDirect3D11TextureSamplerBuffer*>(*state_buffer);

	/* Apply each render state in the list */
	int i = 0;
	while( i != state_count )
	{
		switch( state_params[i].state )
		{
		default:
			DISPDBG( KE_WARNING, "Bad texture sampler state!\nstate: " << state_params[i].state << "\n"
				"param1: " << state_params[i].param1 << "\n"
				"param2: " << state_params[i].param2 << "\n"
				"param3: " << state_params[i].param3 << "\n"
				"fparam: " << state_params[i].fparam << "\n"
				"dparam: " << state_params[i].dparam << "\n" );
			break;
		}

		i++;
	}

	/* Create our sampler state buffers */
	hr = d3ddevice->CreateSamplerState( &sampler_desc, &sb->ss );
	D3D_DISPDBG( KE_ERROR, "Error creating sampler state!", hr );

	return true;
}

bool IKeDirect3D11RenderDevice::SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer )
{
    int i = 0;
	float blend_factor[4] = { 0, 0, 0, 0 };
	uint32_t sample_mask = 0xFFFFFFFF;

    /* Sanity check */
    if( !state_buffer )
        return false;
    
	/* Set the state buffers */
    IKeDirect3D11RenderStateBuffer* sb = static_cast<IKeDirect3D11RenderStateBuffer*>(state_buffer);

	d3ddevice_context->RSSetState( sb->rs );
	d3ddevice_context->OMSetDepthStencilState( sb->dss, 1 );
	d3ddevice_context->OMSetBlendState( sb->bs, blend_factor, sample_mask );
    
    return true;
}


bool IKeDirect3D11RenderDevice::SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer )
{
	int i = 0;
	float blend_factor[4] = { 0, 0, 0, 0 };
	uint32_t sample_mask = 0xFFFFFFFF;

	/* Sanity check */
	if (!state_buffer)
		return false;

	/* Set the texture sampler to it's proper slot */
	IKeDirect3D11TextureSamplerBuffer* sb = static_cast<IKeDirect3D11TextureSamplerBuffer*>(state_buffer);

	d3ddevice_context->CSSetSamplers( stage, 1, &sb->ss );

	return true;
}

/*
 * Name: IKeDirect3D11RenderDevice::set_render_states
 * Desc: Applies a list of user defined render states.
 * TODO: Allow explicit deferring of render states?
 */
void IKeDirect3D11RenderDevice::SetRenderStates( KeState* states )
{
	int i = 0;

	/* Apply each render state in the list */
	while( states[i].state != -1 )
	{
		switch( states[i].state )
		{
		default:
			DISPDBG( KE_WARNING, "Bad render state!\nstate: " << states[i].state << "\n"
				"param1: " << states[i].param1 << "\n"
				"param2: " << states[i].param2 << "\n"
				"param3: " << states[i].param3 << "\n"
				"fparam: " << states[i].fparam << "\n"
				"dparam: " << states[i].dparam << "\n" );
			break;
		}

		i++;
	}
}

/*
* Name: IKeDirect3D11RenderDevice::set_sampler_states
* Desc: Applies a list of user defined sampler states.
* TODO: Allow explicit deferring of sampler states?
*/
void IKeDirect3D11RenderDevice::SetSamplerStates( int stage, KeState* states )
{
	
}

void IKeDirect3D11RenderDevice::DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, uint8_t* vertex_data )
{
	DISPDBG( KE_ERROR, "Functionality not yet implemented!" );
}


/*
* Name: IKeDirect3D11RenderDevice::draw_vertices
* Desc: Draws vertices from the current vertex buffer
*/
void IKeDirect3D11RenderDevice::DrawVertices( uint32_t primtype, uint32_t stride, int first, int count )
{
	IKeDirect3D11GeometryBuffer* gb = static_cast<IKeDirect3D11GeometryBuffer*>(current_geometrybuffer);
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);

	uint32_t offset = 0;		/* TODO: Allow user to specify this */
	d3ddevice_context->IASetVertexBuffers( 0, 1, &gb->vb, &stride, &offset );
	d3ddevice_context->IASetPrimitiveTopology( primitive_types[primtype] );
	d3ddevice_context->Draw( count, first );
}

/*
* Name: IKeDirect3D11RenderDevice::draw_indexed_vertices
* Desc: Draws vertices from the current vertex and index buffer.
*/
void IKeDirect3D11RenderDevice::DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count )
{
	IKeDirect3D11GeometryBuffer* gb = static_cast<IKeDirect3D11GeometryBuffer*>(current_geometrybuffer);
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);

	uint32_t offset = 0;		/* TODO: Allow user to specify this */
	d3ddevice_context->IASetVertexBuffers( 0, 1, &gb->vb, &stride, &offset );
	d3ddevice_context->IASetIndexBuffer( gb->ib, data_types[gb->index_type], 0 );
	d3ddevice_context->IASetPrimitiveTopology( primitive_types[primtype] );
	d3ddevice_context->DrawIndexed( count, 0, 0 );
}

/*
* Name: IKeDirect3D11RenderDevice::draw_indexed_vertices_range
* Desc: Same as above, but allows the user to specify the start/end vertex.
*/
void IKeDirect3D11RenderDevice::DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count )
{
	IKeDirect3D11GeometryBuffer* gb = static_cast<IKeDirect3D11GeometryBuffer*>(current_geometrybuffer);
	IKeDirect3D11GpuProgram* gp = static_cast<IKeDirect3D11GpuProgram*>(current_gpu_program);

	uint32_t offset = 0;		/* TODO: Allow user to specify this */
	d3ddevice_context->IASetVertexBuffers( 0, 1, &gb->vb, &stride, &offset );
	d3ddevice_context->IASetIndexBuffer( gb->ib, data_types[gb->index_type], 0 );
	d3ddevice_context->IASetPrimitiveTopology( primitive_types[primtype] );
	d3ddevice_context->DrawIndexed( count, start, 0 );
}

/*
* Name: IKeDirect3D11RenderDevice::get_framebuffer_region
* Desc: Returns a pointer filled with pixels of the given region of the current framebuffer.
* TODO: Determine bit depth, allow reading from depth buffers, etc.
*/
bool IKeDirect3D11RenderDevice::GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels )
{
	int buffer_bpp = device_desc->colour_bpp;

	/* Return the bit depth of this framebuffer */
	*bpp = buffer_bpp;

	/* Allocate pointer to hold the pixel data */
	(*pixels) = new uint8_t[(width - x)*(height - y)*(buffer_bpp / 8)];
	if (!(*pixels))
		return false;

	return true;
}

/*
* Name: IKeDirect3D11RenderDevice::set_viewport
* Desc: Sets the viewport.
*/
void IKeDirect3D11RenderDevice::SetViewport( int x, int y, int width, int height )
{
	viewport[0] = x;
	viewport[1] = y;
	viewport[2] = width;
	viewport[3] = height;

	/* Setup the viewport */
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT) width;
    vp.Height = (FLOAT) height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = (FLOAT) x;
    vp.TopLeftY = (FLOAT) y;
    d3ddevice_context->RSSetViewports( 1, &vp );
}

/*
 * Name: IKeDirect3D11RenderDevice::set_viewport
 * Desc: Sets the viewport.
 */
void IKeDirect3D11RenderDevice::SetViewportV( int* viewport )
{
	/* Setup the viewport */
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT) viewport[2];
    vp.Height = (FLOAT) viewport[3];
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = (FLOAT) viewport[0];
    vp.TopLeftY = (FLOAT) viewport[1];
    d3ddevice_context->RSSetViewports( 1, &vp );
    
    memmove( this->viewport, viewport, sizeof(int)*4 );
}

/*
 * Name: IKeDirect3D11RenderDevice::GetViewport
 * Desc: Gets the viewport.
 */
void IKeDirect3D11RenderDevice::GetViewport( int* x, int* y, int* width, int* height )
{
	uint32_t viewport_count = 0;
	D3D11_VIEWPORT vp;

    /* Get the viewport */
    d3ddevice_context->RSGetViewports( &viewport_count, &vp );
    
    *x = vp.TopLeftX;
    *y = vp.TopLeftY;
    *width = vp.Width;
    *height = vp.Height;
}

/*
 * Name: IKeDirect3D11RenderDevice::set_viewport
 * Desc: Gets the viewport.
 */
void IKeDirect3D11RenderDevice::GetViewportV( int* viewport )
{
	uint32_t viewport_count = 0;
	D3D11_VIEWPORT vp;

    /* Get the viewport */
    d3ddevice_context->RSGetViewports( &viewport_count, &vp );
    
    viewport[0] = vp.TopLeftX;
	viewport[1] = vp.TopLeftY;
	viewport[2] = vp.Width;
  	viewport[3] = vp.Height;
}

/*
* Name: IKeDirect3D11RenderDevice::set_perspective_matrix
* Desc: Sets the projection matrix by creating a perspective matrix.
*/
void IKeDirect3D11RenderDevice::SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z )
{
	/* Set up projection matrix using the perspective method */
	//projection_matrix = M4MakePerspective( fov, aspect, near_z, far_z );
	nv::perspective( projection_matrix, fov, aspect, near_z, far_z );
}


/*
* Name: IKeDirect3D11RenderDevice::set_view_matrix
* Desc:
*/
void IKeDirect3D11RenderDevice::SetViewMatrix( const nv::matrix4f* view )
{
	/* Copy over the incoming view matrix */
	//memmove( &view_matrix, view, sizeof( nv::matrix4f ) );
	memmove( view_matrix._array, view->_array, sizeof( float ) * 16 );
}


/*
* Name: IKeDirect3D11RenderDevice::set_world_matrix
* Desc:
*/
void IKeDirect3D11RenderDevice::SetWorldMatrix( const nv::matrix4f* world )
{
	/* Copy over the incoming world matrix */
	//memmove( &world_matrix, world, sizeof( nv::matrix4f ) );
	memmove( world_matrix._array, world->_array, sizeof( float ) * 16 );
}


/*
* Name: IKeDirect3D11RenderDevice::set_modelview_matrix
* Desc:
*/
void IKeDirect3D11RenderDevice::SetModelviewMatrix( const nv::matrix4f* modelview )
{
	/* Copy over the incoming modelview matrix */
	//memmove( &modelview_matrix, modelview, sizeof( nv::matrix4f ) );
	memmove( modelview_matrix._array, modelview->_array, sizeof( float ) * 16 );
}


/*
* Name: IKeDirect3D11RenderDevice::set_projection_matrix
* Desc:
*/
void IKeDirect3D11RenderDevice::SetProjectionMatrix( const nv::matrix4f* projection )
{
	/* Copy over the incoming projection matrix */
	//memmove( &projection_matrix, projection, sizeof( nv::matrix4f ) );
	memmove( projection_matrix._array, projection->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeDirect3D11RenderDevice::set_view_matrix
 * Desc:
 */
void IKeDirect3D11RenderDevice::GetViewMatrix( nv::matrix4f* view )
{
    /* Copy over the incoming view matrix */
    memmove( view->_array, view_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeDirect3D11RenderDevice::set_world_matrix
 * Desc:
 */
void IKeDirect3D11RenderDevice::GetWorldMatrix( nv::matrix4f* world )
{
    /* Copy over the incoming world matrix */
    memmove( world->_array, world_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeDirect3D11RenderDevice::set_modelview_matrix
 * Desc:
 */
void IKeDirect3D11RenderDevice::GetModelviewMatrix( nv::matrix4f* modelview )
{
    /* Copy over the incoming modelview matrix */
    memmove( modelview->_array, modelview_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeDirect3D11RenderDevice::set_projection_matrix
 * Desc:
 */
void IKeDirect3D11RenderDevice::GetProjectionMatrix( nv::matrix4f* projection )
{
    /* Copy over the incoming projection matrix */
    memmove( projection->_array, projection_matrix._array, sizeof( float ) * 16 );
}


/*
* Name: IKeDirect3D11RenderDevice::block_until_vertical_blank
* Desc: Stalls the current thread for an interval equivalent to one
*       vertical blank. This function does not sync to the actual vertical blank
*       as I have not found a way to do this on any platform besides Windows.
*       This is also thread safe.
*/
void IKeDirect3D11RenderDevice::BlockUntilVerticalBlank()
{
#ifdef _WIN32
 #ifdef USE_DDRAW_VBLANK
	dd->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
	return;
 #endif
#endif

	SDL_DisplayMode display_mode;

	/* Get the current display mode */
	/* TODO: Get display mode based on windowed or fullscreen mode. */
	SDL_GetWindowDisplayMode(window, &display_mode);

	/* Stall this thread for 1000/refresh_rate milliseconds */
	SDL_Delay( 1000 / display_mode.refresh_rate );
}


/*
* Name: IKeDirect3D11RenderDevice::set_swap_interval
* Desc: Sets the swap interval (enables/disable vertical sync). See SDL documentation on
*       SDL_GL_SetSwapInterval for a more detailed description.
*/
void IKeDirect3D11RenderDevice::SetSwapInterval(int swap_interval)
{
	this->swap_interval = swap_interval;
}


/*
* Name: IKeDirect3D11RenderDevice::get_swap_interval
* Desc: Returns the vertical sync value set above.
*/
int IKeDirect3D11RenderDevice::GetSwapInterval()
{
	return swap_interval;
}


/*
 * Name: IKeDirect3D11RenderDevice::block_until_idle
 * Desc: Stalls the current thread until the GPU is no longer busy.
 */
void IKeDirect3D11RenderDevice::BlockUntilIdle()
{
	IKeFence* fence = NULL;

	/* Insert a new fence into the pipeline. */
	if( InsertFence( &fence ) )
	{
		/* Wait until the fence has been crossed */
		BlockOnFence( fence );

		/* Delete this fence */
		DeleteFence( fence );

		return;
	}

	DISPDBG( KE_ERROR, "Unable to create new fence!" );
}


/*
 * Name: IKeDirect3D11RenderDevice::Kick
 * Desc: Sends all pending GPU commands to the pipeline.
 */
void IKeDirect3D11RenderDevice::Kick()
{
	/* TODO */
}


/*
 * Name: IKeDirect3D11RenderDevice::insert_fence
 * Desc: Creates a new GPU fence object and sets it in place.
 */
bool IKeDirect3D11RenderDevice::InsertFence( IKeFence** fence )
{
	/* Sanity check */
	if( !fence )
		return false;

	/* Create a query object */
	HRESULT hr = S_OK;
    D3D11_QUERY_DESC query_desc;
    query_desc.Query = D3D11_QUERY_EVENT;
    query_desc.MiscFlags = 0;

	(*fence) = new IKeDirect3D11Fence;
	IKeDirect3D11Fence* f = static_cast<IKeDirect3D11Fence*>( *fence );

	hr = d3ddevice->CreateQuery( &query_desc, &f->query );
	if( FAILED( hr ) )
	{
		DeleteFence(f);
		D3D_DISPDBG_RB( KE_ERROR, "Error creating GPU fence!", hr );;
	}

	/* Set query object */
	d3ddevice_context->End( f->query );

	return true;
}


/*
 * Name: IKeDirect3D11RenderDevice::test_fence
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeDirect3D11RenderDevice::TestFence( IKeFence* fence )
{
	/* Test this query object */
	HRESULT hr = d3ddevice_context->GetData( static_cast<IKeDirect3D11Fence*>( fence )->query, NULL, 0, 0 );
	if( FAILED( hr ) )
	{
		/* Test result */
		if( hr == S_FALSE )
		{
			/* GPU commands are still pending */
			return false;
		}
		else
		{
			/* Some other error */
			D3D_DISPDBG_RB( KE_ERROR, "Unexpected error!", hr );
		}
	}

	/* GPU commands all complete */
	return true;
}


/*
 * Name: IKeDirect3D11RenderDevice::block_on_fence
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeDirect3D11RenderDevice::BlockOnFence( IKeFence* fence )
{
	HRESULT hr;
	do
	{
		hr = d3ddevice_context->GetData( static_cast<IKeDirect3D11Fence*>( fence )->query, NULL, 0, 0 );
	}
	while( hr == S_FALSE );
}


/*
 * Name: IKeDirect3D11RenderDevice::DeleteFence
 * Desc: Deletes a GPU fence object.
 */
void IKeDirect3D11RenderDevice::DeleteFence( IKeFence* fence )
{
	if( !fence )
		return;

	IKeDirect3D11Fence* f = static_cast<IKeDirect3D11Fence*>( fence );

	if( f->query )
	{
		f->query = 0;
	}

	delete fence;
}


/*
 * Name: IKeDirect3D11RenderDevice::is_fence
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeDirect3D11RenderDevice::IsFence( IKeFence* fence )
{
	if( !fence )
		return false;

	/* Was this query created already?  If so, assume it's a valid fence. */
	IKeDirect3D11Fence* f = static_cast<IKeDirect3D11Fence*>( fence );
	if( !f->query )
		return false;

	return true;
}


/*
 * Name: IKeDirect3D11RenderDevice::gpu_memory_info
 * Desc: Returns the amound of available and total video memory of this machine.
 */
void IKeDirect3D11RenderDevice::GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory )
{
#ifdef _WIN32
 #ifdef USE_DDRAW_VMEM
	DDSCAPS2 caps;
	dd->GetAvailableVidMem( &caps, (DWORD*) total_memory, (DWORD*) free_memory );
 #endif
#endif
}