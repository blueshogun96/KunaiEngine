//
//  ke_d3d11_renderdevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

/* DirectX Error naming collisions in dxgitype.h and winerror.h */
#pragma warning(disable:4005)

#include "ke_d3d11_renderdevice.h"
#include "ke_debug.h"


/*
* Globals
*/

struct d3d11_semantic_t
{
	char name[16];
	int index;
};

struct d3d11_matrix_buffer_t
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

struct d3d11_semantic_t semantic_list[] = 
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

#if 0
/* OpenGL data types */
uint32_t data_types[] =
{
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_UNSIGNED_SHORT,
	GL_INT,
	GL_UNSIGNED_INT,
	GL_FLOAT,
	GL_DOUBLE
};

/* OpenGL depth/alpha test functions */
uint32_t test_funcs[] =
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

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

/* OpenGL fill modes */
uint32_t fill_modes[] =
{
	GL_POINT,
	GL_LINE,
	GL_FILL,
};

/* OpenGL texture formats */
uint32_t texture_formats[] =
{
	GL_RGBA,
	GL_BGRA
};

/* OpenGL cull modes */
uint32_t cull_modes[] =
{
	GL_NONE,
	GL_CW,
	GL_CCW
};
#endif

std::string ke_d3d11_featurelevel_string( D3D_FEATURE_LEVEL feature )
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
* Name: ke_d3d11_renderdevice::ke_d3d11_renderdevice
* Desc: Default constructor
*/
ke_d3d11_renderdevice_t::ke_d3d11_renderdevice_t()
{
	/* TODO: Disable by making private? */
	assert(No);
}


/*
* Name: ke_d3d11_renderdevice::
* Desc:
*/

/*
* Name: ke_d3d11_renderdevice::ke_d3d11_renderdevice
* Desc: Appropriate constructor used for initialization of Direct3D and a window via SDL.
*/
ke_d3d11_renderdevice_t::ke_d3d11_renderdevice_t(ke_renderdevice_desc_t* renderdevice_desc)
{
	/* Until we are finished initializing, mark this flag as false */
	initialized = false;

	/* Sanity checks */
	if( !renderdevice_desc )
		return;

	/* Save a copy of the render device description */
	device_desc = new ke_renderdevice_desc_t;
	memmove( device_desc, renderdevice_desc, sizeof(ke_renderdevice_desc_t));

	/* Verify device type */
	if( device_desc->device_type == KE_RENDERDEVICE_OGL3 || device_desc->device_type == KE_RENDERDEVICE_OGLES2 || 
		device_desc->device_type == KE_RENDERDEVICE_OGLES3 || device_desc->device_type == KE_RENDERDEVICE_OGL4 )
		return;

	/* Initialize SDL video */
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		return;

	/* Initialize the SDL window */
	window = SDL_CreateWindow("Kunai Engine 0.1a", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		device_desc->width, device_desc->height, SDL_WINDOW_SHOWN);
	if (!window)
		return;

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

	HRESULT hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, feature_levels, feature_level_count, 
		D3D11_SDK_VERSION, &swapchain_desc, &dxgi_swap_chain, &d3ddevice, &feature_level, &d3ddevice_context );
	if( FAILED( hr ) )
		return;

	/* Create our render target view */
	ID3D11Texture2D* back_buffer = NULL;
    hr = dxgi_swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&back_buffer );
    if( FAILED( hr ) )
        return;

    hr = d3ddevice->CreateRenderTargetView( back_buffer, NULL, &d3d_render_target_view );
    back_buffer->Release();
    if( FAILED( hr ) )
        return;

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

	/* Initialize default OpenGL vertex and fragment program */
	ke_d3d11_initialize_default_shaders();

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
	DISPDBG( 1, "Direct3D version: " << ke_d3d11_featurelevel_string( feature_level ) << std::endl );
}


/*
* Name: ke_d3d11_renderdevice::~ke_d3d11_renderdevice
* Desc: Default deconstructor
*/
ke_d3d11_renderdevice_t::~ke_d3d11_renderdevice_t()
{
	delete device_desc;

	/* Kill the default vertex and fragment program */
	ke_d3d11_uninitialize_default_shaders();

	/* Uninitialize and close Direct3D and SDL */
	if( d3ddevice_context )
		d3ddevice_context->ClearState();
	if( d3d_render_target_view )
		d3d_render_target_view->Release();
	if( dxgi_swap_chain )
		dxgi_swap_chain->Release();
	if( d3ddevice_context )
		d3ddevice_context->Release();
	if( d3ddevice )
		d3ddevice->Release();
	SDL_DestroyWindow(window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}


/*
* Name: ke_d3d11_renderdevice::confirm_device
* Desc: Gives confirmation that this device was successfully initialized.
*/
bool ke_d3d11_renderdevice_t::confirm_device()
{
	return initialized;
}

/*
* Name: ke_d3d11_renderdevice_t::get_device_desc
* Desc: Returns a copy of the device description structure
*/
void ke_d3d11_renderdevice_t::get_device_desc(ke_renderdevice_desc_t* device_desc)
{
	memmove( device_desc, this->device_desc, sizeof( ke_renderdevice_desc_t ) );
}

/*
* Name: ke_d3d11_renderdevice::set_clear_colour_fv
* Desc: Sets the clear colour
*/
void ke_d3d11_renderdevice_t::set_clear_colour_fv(float* colour)
{
	memcpy( this->clear_colour, colour, sizeof(float)*4 );
}


/*
* Name: ke_d3d11_renderdevice::set_clear_colour_ubv
* Desc: Same as above.
*/
void ke_d3d11_renderdevice_t::set_clear_colour_ubv(uint8_t* colour)
{
	this->clear_colour[0] = float(colour[0] / 255);
	this->clear_colour[1] = float(colour[1] / 255);
	this->clear_colour[2] = float(colour[2] / 255);
	this->clear_colour[3] = float(colour[3] / 255);
}


/*
* Name: ke_d3d11_renderdevice::set_clear_depth
* Desc:
*/
void ke_d3d11_renderdevice_t::set_clear_depth(float depth)
{
	//glClearDepth(depth);
}


/*
* Name: ke_d3d11_renderdevice::clear_render_buffer
* Desc: Clears only the current render buffer
*/
void ke_d3d11_renderdevice_t::clear_colour_buffer()
{
	d3ddevice_context->ClearRenderTargetView(d3d_render_target_view, clear_colour);
	/*if( FAILED( hr ) )
		DISPDBG( 1, "ID3D11DeviceContext::ClearRenderTargetView(): Error = 0x" << hr << "\n" );*/
}


/*
* Name: ke_d3d11_renderdevice::clear_depth_buffer
* Desc: Clears only the current depth buffer
*/
void ke_d3d11_renderdevice_t::clear_depth_buffer()
{
	//d3ddevice_context->ClearDepthStencilView()
}


/*
* Name: ke_d3d11_renderdevice::clear_stencil_buffer
* Desc: Clears only the current stencil buffer
*/
void ke_d3d11_renderdevice_t::clear_stencil_buffer()
{
	//glClear(GL_STENCIL_BUFFER_BIT);
}


/*
* Name: ke_d3d11_renderdevice::swap
* Desc: Swaps the double buffer.
*/
void ke_d3d11_renderdevice_t::swap()
{
	HRESULT hr = dxgi_swap_chain->Present(0, 0);
	if( FAILED( hr ) )
		DISPDBG( KE_ERROR, "IDXGISwapChain::Present(): Error = 0x" << hr << "\n" );
}


/*
* Name: ke_d3d11_renderdevice_t::create_geometry_buffer
* Desc: Creates a geometry buffer based on the vertex and index data given.  Vertex and index
*       buffers are encapsulated into one interface for easy management, however, index data
*       input is completely optional.  Interleaved vertex data is also supported.
*/
bool ke_d3d11_renderdevice_t::create_geometry_buffer(void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, ke_vertexattribute_t* va, ke_geometrybuffer_t** geometry_buffer)
{
	HRESULT hr = S_OK;

	/* Sanity check(s) */
    if( !geometry_buffer )
        return false;
    //if( !vertex_attributes )
      //  return false;
    if( !vertex_data_size )
        return false;   /* Temporary? */
 
    *geometry_buffer = new ke_d3d11_geometrybuffer_t;
    ke_d3d11_geometrybuffer_t* gb = static_cast<ke_d3d11_geometrybuffer_t*>( *geometry_buffer );

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
		return false;
	}

	/* Create index buffer, if desired. */
	gb->ib = NULL;
	if( index_data_size )
	{
		/* TODO */
	}

	return true;
}


/*
* Name: ke_d3d11_renderdevice_t::create_geometry_buffer
* Desc:
*/
void ke_d3d11_renderdevice_t::delete_geometry_buffer(ke_geometrybuffer_t* geometry_buffer)
{
	ke_d3d11_geometrybuffer_t* gb = static_cast<ke_d3d11_geometrybuffer_t*>(geometry_buffer);

	/* Release vertex and index buffers */
	if( gb->vb )
		gb->vb->Release();
	if( gb->ib )
		gb->ib->Release();

	delete geometry_buffer;
}

/*
* Name: ke_d3d11_renderdevice::set_vertex_buffer
* Desc: Sets the current geometry buffer to be used when rendering. Internally, binds the
*       vertex array object. If NULL, then sets the current vertex array object to 0.
*/
void ke_d3d11_renderdevice_t::set_geometry_buffer(ke_geometrybuffer_t* geometry_buffer)
{
	current_geometrybuffer = geometry_buffer;
}

/*
* Name: ke_d3d11_renderdevice_t::create_program
* Desc: Creates a complete OpenGL program out of shaders in text form. The minimum requirements
*       are one valid vertex and fragment shader, while geometry and tesselation shaders are
*       optional.  Obviously, tesselation shaders require OpenGL 4.1+, and cannot be used with
*       OpenGL 3.2.  This function will automatically search for specific attribute locations
*       before linking it and search for pre-determined uniform names for textures and matrices
*       (see code below).
*       TODO: Allow user defined constants.
*/
bool ke_d3d11_renderdevice_t::create_program(const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, ke_vertexattribute_t* vertex_attributes, ke_gpu_program_t** gpu_program)
{
	D3D11_INPUT_ELEMENT_DESC* layout = NULL;
	int layout_size = 0;
	DXGI_FORMAT fmt;
	DWORD shader_flags = D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef _DEBUG
	shader_flags |= D3D10_SHADER_DEBUG;
#endif

	/* Allocate new GPU program */
	*gpu_program = new ke_d3d11_gpu_program_t;
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>( *gpu_program );
	ZeroMemory( gp, sizeof( ke_d3d11_gpu_program_t ) );

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
				blob_error->Release();
				delete_program(gp);
			}

			return false;
		}

		hr = d3ddevice->CreateVertexShader( blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), NULL, &gp->vs );
		if( FAILED( hr ) )
		{
			delete[] layout;
			blob_shader->Release();
			delete_program(gp);
			DISPDBG( KE_ERROR, "Error creating vertex shader!\n" );
		}

		/* Create input layout */
		hr = d3ddevice->CreateInputLayout( layout, layout_size, blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), &gp->il );
		blob_shader->Release();
		delete[] layout;
		if( FAILED( hr ) )
		{
			delete_program( gp );
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
				blob_error->Release();
				delete_program(gp);
			}

			return false;
		}

		hr = d3ddevice->CreatePixelShader( blob_shader->GetBufferPointer(), blob_shader->GetBufferSize(), NULL, &gp->ps );
		if( FAILED( hr ) )
		{
			blob_shader->Release();
			delete_program(gp);
			DISPDBG( KE_ERROR, "Error creating pixel shader!\n" );
		}

		blob_shader->Release();

		/* TODO: Geometry, Hull, Compute and Domain shaders */
		gp->hs = NULL;
		gp->gs = NULL;
		gp->cs = NULL;
		gp->ds = NULL;
	}

	return true;
}

/*
* Name: ke_d3d11_renderdevice_t::delete_program
* Desc: Deletes the GPU program.
*/
void ke_d3d11_renderdevice_t::delete_program( ke_gpu_program_t* gpu_program )
{
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>( gpu_program );

	/* Deletes the GPU program */
	if(gp)
	{
		if( gp->il ) gp->il->Release();
		if( gp->hs ) gp->hs->Release();
		if( gp->cs ) gp->cs->Release();
		if( gp->ds ) gp->ds->Release();
		if( gp->gs ) gp->gs->Release();
		if( gp->ps ) gp->ps->Release();
		if( gp->vs ) gp->vs->Release();

		delete gpu_program;
	}
}

/*
* Name: ke_d3d11_renderdevice_t::set_program
* Desc: Sets the GPU program.  If NULL, the GPU program is set to 0.
*/
void ke_d3d11_renderdevice_t::set_program( ke_gpu_program_t* gpu_program )
{
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>( gpu_program );

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
* Name: ke_d3d11_renderdevice_t::set_program_constant_1fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_1fv( const char* location, int count, float* value )
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);

	
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant_2fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_2fv(const char* location, int count, float* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);

	
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant_3fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_3fv(const char* location, int count, float* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant_4fv
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_4fv(const char* location, int count, float* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_1iv(const char* location, int count, int* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_2iv(const char* location, int count, int* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_3iv(const char* location, int count, int* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::set_program_constant
* Desc: Sets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::set_program_constant_4iv(const char* location, int count, int* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::get_program_constant_fv
* Desc: Gets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::get_program_constant_fv(const char* location, float* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::get_program_constant_iv
* Desc: Gets program constants (do your research on GLSL uniforms)
*/
void ke_d3d11_renderdevice_t::get_program_constant_iv(const char* location, int* value)
{
	ke_d3d11_gpu_program_t* p = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
 * Name: ke_d3d11_renderdevice_t::create_constant_buffer
 * Desc: Creates a constant buffer used for storing/setting constants in one call instead of one by one.
 *		 If supported, this is the recommended method for setting constants.
 */
bool ke_d3d11_renderdevice_t::create_constant_buffer( uint32_t buffer_size, ke_constantbuffer_t** constant_buffer )
{
	*constant_buffer = new ke_d3d11_constantbuffer_t;
	ke_d3d11_constantbuffer_t* cb = static_cast<ke_d3d11_constantbuffer_t*>( *constant_buffer );

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
 * Name: ke_d3d11_renderdevice_t::delete_constant_buffer
 * Desc: Deletes a constant buffer.
 */
void ke_d3d11_renderdevice_t::delete_constant_buffer( ke_constantbuffer_t* constant_buffer )
{
	ke_d3d11_constantbuffer_t* cb = static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer );

	if( cb )
	{
		if( cb->cb ) cb->cb->Release();
		delete constant_buffer;
	}
}

/*
 * Name: ke_d3d11_renderdevice_t::set_constant_buffer_data
 * Desc: 
 */
bool ke_d3d11_renderdevice_t::set_constant_buffer_data( void* data, ke_constantbuffer_t* constant_buffer )
{
	if( !constant_buffer )
		return false;

	d3ddevice_context->UpdateSubresource( static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer )->cb, 0, NULL, data, 0, 0 );

	return true;
}

/*
 * Name: ke_ogl_renderdevice_t::set_vertex_shader_constant_buffer
 * Desc: 
 */
void ke_d3d11_renderdevice_t::set_vertex_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer )
{
	d3ddevice_context->VSSetConstantBuffers( slot, 1, &static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer )->cb );
}


/*
 * Name: ke_d3d11_renderdevice_t::set_pixel_shader_constant_buffer
 * Desc: 
 */
void ke_d3d11_renderdevice_t::set_pixel_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer )
{
	d3ddevice_context->PSSetConstantBuffers( slot, 1, &static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer )->cb );
}

/*
 * Name: ke_d3d11_renderdevice_t::set_geometry_shader_constant_buffer
 * Desc: 
 */
void ke_d3d11_renderdevice_t::set_geometry_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer )
{
	d3ddevice_context->GSSetConstantBuffers( slot, 1, &static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer )->cb );
}

/*
 * Name: ke_d3d11_renderdevice_t::set_tesselation_shader_constant_buffer
 * Desc: 
 */
void ke_d3d11_renderdevice_t::set_tesselation_shader_constant_buffer( int slot, ke_constantbuffer_t* constant_buffer )
{
	d3ddevice_context->HSSetConstantBuffers( slot, 1, &static_cast<ke_d3d11_constantbuffer_t*>( constant_buffer )->cb );
}

/*
* Name: ke_d3d11_renderdevice::create_texture_1d
* Desc: Creates a 1D texture.
*/
bool ke_d3d11_renderdevice_t::create_texture_1d(uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture)
{
	/* Allocate a new texture */
	*texture = new ke_d3d11_texture_t;
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(*texture);

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
* Name: ke_d3d11_renderdevice::create_texture_2d
* Desc: Creates a blank 2D texture.
*/
bool ke_d3d11_renderdevice_t::create_texture_2d(uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture)
{
	/* Allocate a new texture */
	(*texture) = new ke_d3d11_texture_t;
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(*texture);

	/* Set texture attributes */
	/*t->width = width;
	t->height = height;
	t->target = target;
	t->data_type = data_types[data_type];
	t->depth_format = texture_formats[format];
	t->internal_format = texture_formats[format];
	t->target = target;*/

	return true;
}

/*
* Name: ke_d3d11_renderdevice::create_texture_3d
* Desc: Creates a blank 3D texture.
*/
bool ke_d3d11_renderdevice_t::create_texture_3d(uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture)
{
	/* Allocate a new texture */
	(*texture) = new ke_d3d11_texture_t;
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(*texture);

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
* Name: ke_d3d11_renderdevice_t::delete_texture
* Desc: Deletes a texture from memory.
*/
void ke_d3d11_renderdevice_t::delete_texture(ke_texture_t* texture)
{
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(texture);

	if (texture)
	{
		delete texture;
	}
}


/*
* Name: ke_d3d11_renderdevice_t::set_texture_data_1d
* Desc: Sets pixel data for a 1D texture.
*/
void ke_d3d11_renderdevice_t::set_texture_data_1d(int offsetx, int width, int miplevel, void* pixels, ke_texture_t* texture)
{
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(texture);
}

/*
* Name: ke_d3d11_renderdevice_t::set_texture_data_2d
* Desc: Sets pixel data for a 1D texture.
*/
void ke_d3d11_renderdevice_t::set_texture_data_2d(int offsetx, int offsety, int width, int height, int miplevel, void* pixels, ke_texture_t* texture)
{
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(texture);
}

/*
* Name: ke_d3d11_renderdevice_t::set_texture_data_3d
* Desc: Sets pixel data for a 1D texture.
*/
void ke_d3d11_renderdevice_t::set_texture_data_3d(int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, ke_texture_t* texture)
{
	ke_d3d11_texture_t* t = static_cast<ke_d3d11_texture_t*>(texture);
}

/*
* Name: ke_d3d11_renderdevice_t::create_render_target
* Desc: Creates a seperate render target (FBO), typically used for rendering to a texture.
*       Creates a colour, depth and stencil buffer (if desired) and can be set as a texture.
*/
bool ke_d3d11_renderdevice_t::create_render_target(int width, int height, int depth, uint32_t flags, ke_rendertarget_t** rendertarget)
{
	ke_d3d11_rendertarget_t* rt = static_cast<ke_d3d11_rendertarget_t*>(*rendertarget);

	return true;
}

/*
* Name: ke_d3d11_renderdevice_t::delete_render_target
* Desc: Deletes the render target resources used.
*/
void ke_d3d11_renderdevice_t::delete_render_target(ke_rendertarget_t* rendertarget)
{
	ke_d3d11_rendertarget_t* rt = static_cast<ke_d3d11_rendertarget_t*>(rendertarget);

	delete rendertarget;
}

/*
* Name: ke_d3d11_renderdevice_t::bind_render_target
* Desc: Binds the render target to OpenGL.  You set the texture to the appropriate  texture
*       stage yourself using ::set_texture().
*/
void ke_d3d11_renderdevice_t::bind_render_target(ke_rendertarget_t* rendertarget)
{
	ke_d3d11_rendertarget_t* rt = static_cast<ke_d3d11_rendertarget_t*>(rendertarget);
}

/*
* Name: ke_d3d11_renderdevice_t::set_texture
* Desc: Sets a texture to the desired texture stage.  If NULL, then texturing is disabled on
*       the selected texture stage.
*/
void ke_d3d11_renderdevice_t::set_texture(int stage, ke_texture_t* texture)
{
	ke_d3d11_texture_t* tex = static_cast<ke_d3d11_texture_t*>(texture);
}

/*
* Name: ke_d3d11_renderdevice_t::set_render_states
* Desc: Applies a list of user defined render states.
* TODO: Allow explicit deferring of render states?
*/
void ke_d3d11_renderdevice_t::set_render_states(ke_state_t* states)
{
	int i = 0;

	/* Apply each render state in the list */
	while (states[i].state != -1)
	{
		switch (states[i].state)
		{
		default:
			DISPDBG(KE_WARNING, "Bad render state!\nstate: " << states[i].state << "\n"
				"param1: " << states[i].param1 << "\n"
				"param2: " << states[i].param2 << "\n"
				"param3: " << states[i].param3 << "\n"
				"fparam: " << states[i].fparam << "\n"
				"dparam: " << states[i].dparam << "\n");
			break;
		}

		i++;
	}
}

/*
* Name: ke_d3d11_renderdevice_t::set_sampler_states
* Desc: Applies a list of user defined sampler states.
* TODO: Allow explicit deferring of sampler states?
*/
void ke_d3d11_renderdevice_t::set_sampler_states(ke_state_t* states)
{

}

/*void ke_d3d11_renderdevice_t::draw_vertices_im()
{

}*/


/*
* Name: ke_d3d11_renderdevice::draw_vertices
* Desc: Draws vertices from the current vertex buffer
*/
void ke_d3d11_renderdevice_t::draw_vertices(uint32_t primtype, uint32_t stride, int first, int count)
{
	ke_d3d11_geometrybuffer_t* gb = static_cast<ke_d3d11_geometrybuffer_t*>(current_geometrybuffer);
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);

	uint32_t offset = 0;		/* TODO: Allow user to specify this */
	d3ddevice_context->IASetVertexBuffers( 0, 1, &gb->vb, &stride, &offset );
	d3ddevice_context->IASetPrimitiveTopology( primitive_types[primtype] );
	d3ddevice_context->Draw( count, first );
}

/*
* Name: ke_d3d11_renderdevice::draw_indexed_vertices
* Desc: Draws vertices from the current vertex and index buffer.
*/
void ke_d3d11_renderdevice_t::draw_indexed_vertices(uint32_t primtype, uint32_t stride, int count)
{
	ke_d3d11_geometrybuffer_t* gb = static_cast<ke_d3d11_geometrybuffer_t*>(current_geometrybuffer);
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::draw_indexed_vertices_range
* Desc: Same as above, but allows the user to specify the start/end vertex.
*/
void ke_d3d11_renderdevice_t::draw_indexed_vertices_range(uint32_t primtype, uint32_t stride, int start, int end, int count)
{
	ke_d3d11_geometrybuffer_t* gb = static_cast<ke_d3d11_geometrybuffer_t*>(current_geometrybuffer);
	ke_d3d11_gpu_program_t* gp = static_cast<ke_d3d11_gpu_program_t*>(current_gpu_program);
}

/*
* Name: ke_d3d11_renderdevice_t::get_framebuffer_region
* Desc: Returns a pointer filled with pixels of the given region of the current framebuffer.
* TODO: Determine bit depth, allow reading from depth buffers, etc.
*/
bool ke_d3d11_renderdevice_t::get_framebuffer_region(int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels)
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
* Name: ke_d3d11_renderdevice::set_viewport
* Desc: Sets the viewport.
*/
void ke_d3d11_renderdevice_t::set_viewport(int x, int y, int width, int height)
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
* Name: ke_d3d11_renderdevice::set_perspective_matrix
* Desc: Sets the projection matrix by creating a perspective matrix.
*/
void ke_d3d11_renderdevice_t::set_perspective_matrix(float fov, float aspect, float near_z, float far_z)
{
	/* Set up projection matrix using the perspective method */
	projection_matrix = M4MakePerspective(fov, aspect, near_z, far_z);
}


/*
* Name: ke_d3d11_renderdevice::set_view_matrix
* Desc:
*/
void ke_d3d11_renderdevice_t::set_view_matrix(const Matrix4* view)
{
	/* Copy over the incoming view matrix */
	memmove(&view_matrix, view, sizeof(Matrix4));
}


/*
* Name: ke_d3d11_renderdevice::set_world_matrix
* Desc:
*/
void ke_d3d11_renderdevice_t::set_world_matrix(const Matrix4* world)
{
	/* Copy over the incoming world matrix */
	memmove(&world_matrix, world, sizeof(Matrix4));
}


/*
* Name: ke_d3d11_renderdevice::set_modelview_matrix
* Desc:
*/
void ke_d3d11_renderdevice_t::set_modelview_matrix(const Matrix4* modelview)
{
	/* Copy over the incoming modelview matrix */
	memmove(&modelview_matrix, modelview, sizeof(Matrix4));
}


/*
* Name: ke_d3d11_renderdevice::set_projection_matrix
* Desc:
*/
void ke_d3d11_renderdevice_t::set_projection_matrix(const Matrix4* projection)
{
	/* Copy over the incoming projection matrix */
	memmove(&projection_matrix, projection, sizeof(Matrix4));
}


/*
* Name: ke_d3d11_renderdevice_t::block_until_vertical_blank
* Desc: Stalls the current thread for an interval equivalent to one
*       vertical blank. This function does not sync to the actual vertical blank
*       as I have not found a way to do this on any platform besides Windows.
*       This is also thread safe.
*/
void ke_d3d11_renderdevice_t::block_until_vertical_blank()
{
	SDL_DisplayMode display_mode;

	/* Get the current display mode */
	/* TODO: Get display mode based on windowed or fullscreen mode. */
	SDL_GetWindowDisplayMode(window, &display_mode);

	/* Stall this thread for 1000/refresh_rate milliseconds */
	SDL_Delay(1000 / display_mode.refresh_rate);
}


/*
* Name: ke_d3d11_renderdevice_t::set_swap_interval
* Desc: Sets the swap interval (enables/disable vertical sync). See SDL documentation on
*       SDL_GL_SetSwapInterval for a more detailed description.
*/
void ke_d3d11_renderdevice_t::set_swap_interval(int swap_interval)
{
	//SDL_GL_SetSwapInterval(swap_interval);
}


/*
* Name: ke_d3d11_renderdevice_t::get_swap_interval
* Desc: Returns the vertical sync value set above.
*/
int ke_d3d11_renderdevice_t::get_swap_interval()
{
	return 1;// SDL_GL_GetSwapInterval();
}
