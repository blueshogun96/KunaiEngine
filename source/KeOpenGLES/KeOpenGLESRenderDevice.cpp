//
//  IKeOpenGLESRenderDevice.cpp
//
//  Created by Shogun3D on 6/18/15.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeOpenGLESRenderDevice.h"
#include "KeDebug.h"


/*
 * Debugging macros
 */
#define DISPDBG_R( a, b ) { DISPDBG( a, b ); return; }
#define DISPDBG_RB( a, b ) { DISPDBG( a, b ); return false; }
#define OGL_DISPDBG( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); }
#define OGL_DISPDBG_R( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return; }
#define OGL_DISPDBG_RB( a, b, c ) if(c) { DISPDBG( a, b << "\nError code: (" << c << ")" ); return false; }


/* GPU fencing routines */
#include "KeOpenGLESFence.h"


/*
 * Globals
 */

const char default_vertex_program_v150[] =
"#version 150\n"
"in  vec3 in_Position;\n"
"void main(void)\n"
"{\n"
"    gl_Position = vec4(in_Position, 1.0);\n"
"}\n";

const char default_fragment_program_v150[] =
"#version 150\n"
"out vec4 colour;\n"

"void main(void)\n"
"{\n"
"	colour = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";

uint32_t ke_default_program;

/* OpenGL primitive types */
uint32_t primitive_types[] =
{
    GL_POINTS,
    GL_LINES,
    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP
};

/* OpenGL data types */
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
    0, //GL_DOUBLE
};

/* OpenGL buffer usage types */
uint32_t buffer_usage_types[] =
{
    GL_STATIC_DRAW,
    0, //GL_STATIC_READ,
    0, //GL_STATIC_COPY,
    GL_DYNAMIC_DRAW,
    0, //GL_DYNAMIC_READ,
    0, //GL_DYNAMIC_COPY,
    GL_STREAM_DRAW,
    0, //GL_STREAM_READ,
    0, //GL_STREAM_COPY,
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
    0, //GL_TEXTURE_1D,
    0, //GL_TEXTURE_1D_ARRAY,
    GL_TEXTURE_2D,
    0, //GL_TEXTURE_2D_ARRAY,
    0, //GL_TEXTURE_3D,
    0, //GL_TEXTURE_RECTANGLE,
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
    0, //GL_POINT,
    0, //GL_LINE,
    0, //GL_FILL,
};

/* OpenGL texture formats */
uint32_t texture_formats[] =
{
    GL_RGBA,
    GL_BGRA,
	0, //GL_RED
};
uint32_t internal_texture_formats[] = 
{
	GL_RGBA,
	GL_BGRA,
	0, //GL_R8,
};

/* OpenGL cull modes */
uint32_t cull_modes[] =
{
    GL_NONE,
    GL_CW,
    GL_CCW
};

/* OpenGL blend modes */
uint32_t blend_modes[] = 
{
	 GL_ZERO, 
	 GL_ONE, 
	 GL_SRC_COLOR, 
	 GL_ONE_MINUS_SRC_COLOR, 
	 GL_DST_COLOR, 
	 GL_ONE_MINUS_DST_COLOR, 
	 GL_SRC_ALPHA, 
	 GL_ONE_MINUS_SRC_ALPHA, 
	 GL_DST_ALPHA, 
	 GL_ONE_MINUS_DST_ALPHA, 
	 GL_CONSTANT_COLOR, 
	 GL_ONE_MINUS_CONSTANT_COLOR, 
	 GL_CONSTANT_ALPHA, 
	 GL_ONE_MINUS_CONSTANT_ALPHA, 
	 GL_SRC_ALPHA_SATURATE,
#ifndef __APPLE__
	 GL_SRC1_COLOR,
	 GL_ONE_MINUS_SRC1_COLOR, 
	 GL_SRC1_ALPHA, 
	 GL_ONE_MINUS_SRC1_ALPHA
#else
    0, 0, 0, 0
#endif
};

/* OpenGL blend equations */
uint32_t blend_equations[]
{
    GL_FUNC_ADD,
    0, //GL_MIN,
    0, //GL_MAX,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT
};

/* OpenGL texture filtering modes */
uint32_t texture_filter_modes[] =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR
};







/*
 * Name: IKeOpenGLESESRenderDevice::IKeOpenGLESRenderDevice
 * Desc: Default constructor
 */
IKeOpenGLESRenderDevice::IKeOpenGLESRenderDevice()
{
    /* TODO: Disable by making private? */
    assert(No);
}


/*
 * Name: IKeOpenGLESRenderDevice::
 * Desc: 
 */

/*
 * Name: IKeOpenGLESRenderDevice::IKeOpenGLESRenderDevice
 * Desc: Appropriate constructor used for initialization of OpenGL via SDL.
 */
IKeOpenGLESRenderDevice::IKeOpenGLESRenderDevice( KeRenderDeviceDesc* renderdevice_desc ) : fence_vendor( KE_FENCE_ARB ), dd(NULL)
{
    /* Until we are finished initializing, mark this flag as false */
    initialized = false;
    
    /* Sanity checks */
    if( !renderdevice_desc )
        return;
    
    /* Save a copy of the render device description */
    device_desc = new KeRenderDeviceDesc;
    memmove( device_desc, renderdevice_desc, sizeof( KeRenderDeviceDesc ) );
    
    /* Verify device type */
    if( device_desc->device_type != KE_RENDERDEVICE_OGLES2 )
        return;
    
    /* Initialize SDL video */
    if( SDL_InitSubSystem( SDL_INIT_VIDEO ) != 0 )
        return;
    
    SDL_LogSetAllPriority( SDL_LOG_PRIORITY_WARN );
    
    /* Setup OpenGL properties */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, Yes );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, device_desc->depth_bpp );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, device_desc->stencil_bpp );
    
    /* Set the appropriate OpenGL version and profile */
    major_version = 2;
    minor_version = 0;
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_version );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_version );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    
    /* TEST */
    int landscape = 1;
    int modes = SDL_GetNumDisplayModes(0);
    int sx = 0, sy = 0;
    for (int i = 0; i < modes; i++)
    {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, i, &mode);
        if (landscape ? mode.w > sx : mode.h > sy)
        {
            sx = mode.w;
            sy = mode.h;
        }
    }
    
    /* Initialize the SDL window */
    window = SDL_CreateWindow( "Kunai Engine 0.1a",  0, 0,
                              device_desc->width, device_desc->height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS );
    if( !window )
        return;
    
    /* Create our OpenGL context. */
    context = SDL_GL_CreateContext( window );
    
    /* Verify that we have a valid context */
    if( !context )
        DISPDBG( KE_ERROR, "Error creating OpenGL context!" );
    
    /* Set default OpenGL render states */
    glClearDepthf( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    
    glDisable( GL_BLEND );
    glDisable( GL_CULL_FACE );
    glDisable( GL_TEXTURE_2D );
    
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
    
    /* Print OpenGL driver/implementation details */
    DISPDBG( 1, "\n\tOpenGL Vendor: " << glGetString( GL_VENDOR ) <<
            "\n\tOpenGL Version: " << glGetString( GL_VERSION ) <<
            "\n\tOpenGL Renderer: " << glGetString( GL_RENDERER ) << "\n" );


	/* TODO: Determine which fencing version to use based on vendor if needed */
}


/*
 * Name: IKeOpenGLESRenderDevice::~IKeOpenGLESRenderDevice
 * Desc: Default deconstructor
 */
IKeOpenGLESRenderDevice::~IKeOpenGLESRenderDevice()
{
    delete device_desc;
    
    /* Kill the default vertex and fragment program */
   // ke_uninitialize_default_shaders();
    
    /* Uninitialize and close OpenGL and SDL */
    SDL_GL_DeleteContext( context );
    SDL_DestroyWindow( window );
    SDL_QuitSubSystem( SDL_INIT_VIDEO );

	/* Uninitialize DirectDraw */
#ifdef _WIN32
	if( dd )
		reinterpret_cast<IDirectDraw7*>( dd )->Release();
#endif
}


/*
 * Name: IKeOpenGLESRenderDevice::confirm_device
 * Desc: Gives confirmation that this device was successfully initialized.
 */
bool IKeOpenGLESRenderDevice::ConfirmDevice()
{
    return initialized;
}

/*
 * Name: IKeOpenGLESRenderDevice::get_device_desc
 * Desc: Returns a copy of the device description structure
 */
void IKeOpenGLESRenderDevice::GetDeviceDesc( KeRenderDeviceDesc* device_desc )
{
    memmove( device_desc, this->device_desc, sizeof( KeRenderDeviceDesc ) );
}

/*
 * Name: IKeOpenGLESRenderDevice::GetDeviceCaps
 * Desc: Returns a copy of the device capabilities structure
 */
void IKeOpenGLESRenderDevice::GetDeviceCaps( KeRenderDeviceCaps* device_caps )
{
    memmove( device_caps, this->device_caps, sizeof( KeRenderDeviceCaps ) );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_clear_colour_fv
 * Desc: Sets the clear colour
 */
void IKeOpenGLESRenderDevice::SetClearColourFV( float* colour )
{
    memcpy( this->clear_colour, clear_colour, sizeof(float)*4 );
    
    glClearColor( colour[0], colour[1], colour[2], colour[3] );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_clear_colour_ubv
 * Desc: Same as above.
 */
void IKeOpenGLESRenderDevice::SetClearColourUBV( uint8_t* colour )
{
    this->clear_colour[0] = float(colour[0]/255);
    this->clear_colour[1] = float(colour[1]/255);
    this->clear_colour[2] = float(colour[2]/255);
    this->clear_colour[3] = float(colour[3]/255);
    
    glClearColor( this->clear_colour[0], this->clear_colour[1], this->clear_colour[2], this->clear_colour[3] );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_clear_depth
 * Desc: 
 */
void IKeOpenGLESRenderDevice::SetClearDepth( float depth )
{
    glClearDepthf( depth );
}


/*
 * Name: IKeOpenGLESRenderDevice::clear_render_buffer
 * Desc: Clears only the current render buffer
 */
void IKeOpenGLESRenderDevice::ClearColourBuffer()
{
    glClear( GL_COLOR_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLESRenderDevice::clear_depth_buffer
 * Desc: Clears only the current depth buffer
 */
void IKeOpenGLESRenderDevice::ClearDepthBuffer()
{
    glClear( GL_DEPTH_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLESRenderDevice::clear_stencil_buffer
 * Desc: Clears only the current stencil buffer
 */
void IKeOpenGLESRenderDevice::ClearStencilBuffer()
{
    glClear( GL_STENCIL_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLESRenderDevice::swap
 * Desc: Swaps the double buffer.
 */
void IKeOpenGLESRenderDevice::Swap()
{
    SDL_GL_SwapWindow( window );
}


/*
 * Name: IKeOpenGLESRenderDevice::create_geometry_buffer
 * Desc: Creates a geometry buffer based on the vertex and index data given.  Vertex and index
 *       buffers are encapsulated into one interface for easy management, however, index data
 *       input is completely optional.  Interleaved vertex data is also supported.
 */
bool IKeOpenGLESRenderDevice::CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer )
{
    GLenum error = glGetError();

    /* Sanity check(s) */
    if( !geometry_buffer )
        DISPDBG_RB( KE_ERROR, "Invalid interface pointer!" );
    //if( !vertex_attributes )
      //  return false;
    if( !vertex_data_size )
        DISPDBG_RB( KE_ERROR, "(vertex_data_size == 0) condition is currently not allowed..." );   /* Temporary? */
 
    *geometry_buffer = new IKeOpenGLESGeometryBuffer;
    IKeOpenGLESGeometryBuffer* gb = static_cast<IKeOpenGLESGeometryBuffer*>( *geometry_buffer );
    
	/* Enumerate buffer usage flags */

    /* Create a vertex array object */
    glGenVertexArraysOES( 1, &gb->vao );
    OGL_DISPDBG_RB( KE_ERROR, "Error creating vertex array object!", glGetError() ); //error = glGetError();
    
    /* Bind this vertex array object */
    glBindVertexArrayOES( gb->vao );
    
    /* Create the vertex buffer object */
    glGenBuffers( 1, &gb->vbo[0] );
    OGL_DISPDBG_RB( KE_ERROR, "Error generating vertex buffer object!", glGetError() );
    
    /* Set the vertex buffer data */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, vertex_data_size, vertex_data, buffer_usage_types[flags] );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting vertex buffer data!", glGetError() );
     
	/* Set the vertex attributes for this geometry buffer */
	for( int i = 0; va[i].index != -1; i++ )
	{
		glVertexAttribPointer( va[i].index,
								va[i].size,
								data_types[va[i].type],
								va[i].normalize,
								va[i].stride,
								BUFFER_OFFSET(va[i].offset) );
		glEnableVertexAttribArray(va[i].index);
	}

    /* Create an index buffer if desired */
    if( index_data_size )
    {
        glGenBuffers( 1, &gb->vbo[1] );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        OGL_DISPDBG_RB( KE_ERROR, "Error creating index buffer object!", glGetError() );
        
        /* Set the index buffer data */
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, buffer_usage_types[flags] );
        gb->index_type = index_data_type;
		OGL_DISPDBG_RB( KE_ERROR, "Error setting index buffer data!", glGetError() );
    }
    else
        gb->vbo[1] = 0;
    
    /* Unbind this vertex array object */
    glBindVertexArrayOES(0);
    
    return true;
}


/*
 * Name: IKeOpenGLESRenderDevice::delete_geometry_buffer
 * Desc:
 */
void IKeOpenGLESRenderDevice::DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
    IKeOpenGLESGeometryBuffer* gb = static_cast<IKeOpenGLESGeometryBuffer*>( geometry_buffer );
    
    /* Delete the VBO and VAO */
    glDeleteBuffers( 2, gb->vbo );
    glDeleteVertexArraysOES( 1, &gb->vao );
    
    delete geometry_buffer;
}

/*
 * Name: IKeOpenGLESRenderDevice::set_vertex_buffer
 * Desc: Sets the current geometry buffer to be used when rendering. Internally, binds the
 *       vertex array object. If NULL, then sets the current vertex array object to 0.
 */
void IKeOpenGLESRenderDevice::SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
    current_geometrybuffer = geometry_buffer;
    
    if( geometry_buffer )
        glBindVertexArrayOES( static_cast<IKeOpenGLESGeometryBuffer*>( geometry_buffer )->vao );
    else
        glBindVertexArrayOES(0);
}

/*
 * Name: IKeOpenGLESRenderDevice::create_program
 * Desc: Creates a complete OpenGL program out of shaders in text form. The minimum requirements
 *       are one valid vertex and fragment shader, while geometry and tesselation shaders are
 *       optional.  Obviously, tesselation shaders require OpenGL 4.1+, and cannot be used with
 *       OpenGL 3.2.  This function will automatically search for specific attribute locations
 *       before linking it and search for pre-determined uniform names for textures and matrices
 *       (see code below).
 *       TODO: Allow user defined constants.
 */
bool IKeOpenGLESRenderDevice::CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program )
{
    GLuint p, f, v, t = 0, g;
    *gpu_program = new IKeOpenGLESGpuProgram;
    IKeOpenGLESGpuProgram* gp = static_cast<IKeOpenGLESGpuProgram*>( *gpu_program );
    GLenum error = glGetError();
    
	v = glCreateShader( GL_VERTEX_SHADER );
	f = glCreateShader( GL_FRAGMENT_SHADER );
    //g = glCreateShader( GL_GEOMETRY_SHADER );
    
	const char* vv = vertex_shader;
	const char* ff = fragment_shader;
    const char* gg = geometry_shader;
    const char* tt = tesselation_shader;
    
	glShaderSource( v, 1, &vv, NULL );
	glShaderSource( f, 1, &ff, NULL );
    
	GLint compiled;
    
	glCompileShader(v);
	glGetShaderiv( v, GL_COMPILE_STATUS, &compiled );
	if( !compiled )
	{
        char str[2048];
        int len = 0;
        
        glGetShaderInfoLog( v, 2048, &len, str );
		printf("Vertex shader not compiled.\n%s\n", str);
	}
    
	glCompileShader(f);
	glGetShaderiv( f, GL_COMPILE_STATUS, &compiled );
	if( !compiled )
	{
        char str[2048];
        int len = 0;
        
        glGetShaderInfoLog( f, 2048, &len, str );
		printf("Fragment shader not compiled.\n%s\n", str);
	}
    
	p = glCreateProgram();
    
	glBindAttribLocation( p, 0, "in_pos" );
    glBindAttribLocation( p, 1, "in_normal" );
    glBindAttribLocation( p, 2, "in_tangent" );
    glBindAttribLocation( p, 3, "in_bitangent" );
    glBindAttribLocation( p, 4, "in_colour" );
    glBindAttribLocation( p, 5, "in_tex0" );
    glBindAttribLocation( p, 6, "in_tex1" );
    glBindAttribLocation( p, 7, "in_tex2" );
    glBindAttribLocation( p, 8, "in_tex3" );
	glBindAttribLocation( p, 9, "in_tex4" );
    glBindAttribLocation( p, 10, "in_tex5" );
    glBindAttribLocation( p, 11, "in_tex6" );
    glBindAttribLocation( p, 12, "in_tex7" );
    
	glAttachShader( p, v );
	glAttachShader( p, f );
    
	glLinkProgram(p);
	glUseProgram(p);
    
    glDeleteShader(v);
    glDeleteShader(f);
    glDeleteShader(g);
    
    GLuint uniform_tex0 = glGetUniformLocation( p, "tex0" );
    GLuint uniform_tex1 = glGetUniformLocation( p, "tex1" );
    GLuint uniform_tex2 = glGetUniformLocation( p, "tex2" );
    GLuint uniform_tex3 = glGetUniformLocation( p, "tex3" );
	GLuint uniform_tex4 = glGetUniformLocation( p, "tex4" );
    GLuint uniform_tex5 = glGetUniformLocation( p, "tex5" );
    GLuint uniform_tex6 = glGetUniformLocation( p, "tex6" );
    GLuint uniform_tex7 = glGetUniformLocation( p, "tex7" );
    
    gp->matrices[0] = glGetUniformLocation( p, "world" );
    gp->matrices[1] = glGetUniformLocation( p, "view" );
    gp->matrices[2] = glGetUniformLocation( p, "proj" );
    
    glUniform1i( uniform_tex0, 0 );
    glUniform1i( uniform_tex1, 1 );
    glUniform1i( uniform_tex2, 2 );
    glUniform1i( uniform_tex3, 3 );
	glUniform1i( uniform_tex4, 4 );
    glUniform1i( uniform_tex5, 5 );
    glUniform1i( uniform_tex6, 6 );
    glUniform1i( uniform_tex7, 7 );

    glUseProgram(0);
    
    /* Save the handle to this newly created program */
    gp->program = p;

#if 0
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
 * Name: IKeOpenGLESRenderDevice::delete_program
 * Desc: Deletes the GPU program.
 */
void IKeOpenGLESRenderDevice::DeleteProgram( IKeGpuProgram* gpu_program )
{
    /* Deletes the GPU program */
    if( gpu_program )
    {
        glDeleteProgram( static_cast<IKeOpenGLESGpuProgram*>(gpu_program)->program );
		//delete[] static_cast<IKeOpenGLESGpuProgram*>(gpu_program)->va;
        delete gpu_program;
    }
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program
 * Desc: Sets the GPU program.  If NULL, the GPU program is set to 0.
 */
void IKeOpenGLESRenderDevice::SetProgram( IKeGpuProgram* gpu_program )
{
	GLenum error = glGetError();

    /* Check for a valid pointer. If NULL, then we set the current program to 0. */
    if( gpu_program )
    {
        IKeOpenGLESGpuProgram* gp = static_cast<IKeOpenGLESGpuProgram*>(gpu_program);
    
        /* Save a copy of this program */
        current_gpu_program = gpu_program;
    
        glUseProgram( gp->program );
		OGL_DISPDBG_R( KE_ERROR, "Invalid GPU program!", glGetError() );
    }
    else
        glUseProgram(0);
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant_1fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant1FV( const char* location, int count, float* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform1fv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant_2fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant2FV( const char* location, int count, float* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform2fv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant_3fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant3FV( const char* location, int count, float* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform3fv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant_4fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant4FV( const char* location, int count, float* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform4fv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant1IV( const char* location, int count, int* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform1iv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant2IV( const char* location, int count, int* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform2iv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant3IV( const char* location, int count, int* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform3iv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::SetProgramConstant4IV( const char* location, int count, int* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform4iv( loc, count, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::get_program_constant_fv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::GetProgramConstantFV( const char* location, float* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glGetUniformfv( p->program, loc, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::get_program_constant_iv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLESRenderDevice::GetProgramConstantIV( const char* location, int* value )
{
    IKeOpenGLESGpuProgram* p = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glGetUniformiv( p->program, loc, value );
}

/*
 * Name: IKeOpenGLESRenderDevice::create_constant_buffer
 * Desc: Creates a constant buffer.
 * TODO: Support for OpenGL.
 */
bool IKeOpenGLESRenderDevice::CreateConstantBuffer( uint32_t buffer_size, IKeConstantBuffer** constant_buffer )
{
	OGL_DISPDBG_RB( KE_ERROR, "Functionality not supported for OpenGL ES 2.0!", glGetError() );
	return false;
}

/*
 * Name: IKeOpenGLESRenderDevice::delete_constant_buffer
 * Desc: 
 */
void IKeOpenGLESRenderDevice::DeleteConstantBuffer( IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLESRenderDevice::set_constant_buffer_data
 * Desc: 
 */
bool IKeOpenGLESRenderDevice::SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer )
{
	return true;
}

/*
 * Name: IKeOpenGLESRenderDevice::set_vertex_shader_constant_buffer
 * Desc: 
 */
void IKeOpenGLESRenderDevice::SetVertexShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLESRenderDevice::set_pixel_shader_constant_buffer
 * Desc: 
 */
void IKeOpenGLESRenderDevice::SetPixelShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLESRenderDevice::set_geometry_shader_constant_buffer
 * Desc: 
 */
void IKeOpenGLESRenderDevice::SetGeometryShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLESRenderDevice::set_tesselation_shader_constant_buffer
 * Desc: 
 */
void IKeOpenGLESRenderDevice::SetTesselationShaderConstantBuffer( int slot, IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLESRenderDevice::create_texture_1d
 * Desc: Creates a 1D texture.
 */
bool IKeOpenGLESRenderDevice::CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
    DISPDBG_RB( KE_ERROR, "1D textures are not supported for OpenGL ES!" );
}

/*
 * Name: IKeOpenGLESRenderDevice::create_texture_2d
 * Desc: Creates a blank 2D texture.
 */
bool IKeOpenGLESRenderDevice::CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    (*texture) = new IKeOpenGLESTexture;
    IKeOpenGLESTexture* t = static_cast<IKeOpenGLESTexture*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->height = height;
    t->target = texture_targets[target];
    t->data_type = data_types[data_type];
    t->depth_format = texture_formats[format];
    t->internal_format = internal_texture_formats[format];
    
    /* Use OpenGL to create a new 2D texture */
    glGenTextures( 1, &t->handle );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating texture!", glGetError() );
    glBindTexture( t->target, t->handle );
    OGL_DISPDBG_RB( KE_ERROR, "Error binding texture!", glGetError() );
    
    /* Set the initial texture attributes */
    glTexImage2D( t->target, 0, internal_texture_formats[format], width, height, 0, texture_formats[format], data_types[data_type], pixels );
    OGL_DISPDBG_RB( KE_ERROR, "Error initializing texture attributes!", glGetError() );
    
    /* Set texture parameters */
    glTexParameteri( t->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( t->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
    return true;
}

/*
 * Name: IKeOpenGLESRenderDevice::create_texture_3d
 * Desc: Creates a blank 3D texture.
 */
bool IKeOpenGLESRenderDevice::CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
    DISPDBG_RB( KE_ERROR, "3D textures are not supported for OpenGL ES 2.0" );
}

/*
 * Name: IKeOpenGLESRenderDevice::delete_texture
 * Desc: Deletes a texture from memory.
 */
void IKeOpenGLESRenderDevice::DeleteTexture( IKeTexture* texture )
{
    IKeOpenGLESTexture* t = static_cast<IKeOpenGLESTexture*>( texture );
    
    if( texture )
    {
        glDeleteTextures( 1, &t->handle );
        delete texture;
    }
}


/*
 * Name: IKeOpenGLESRenderDevice::set_texture_data_1d
 * Desc: Sets pixel data for a 1D texture.
 */
void IKeOpenGLESRenderDevice::SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture )
{
    DISPDBG_R( KE_ERROR, "1D textures are not supported for OpenGL ES" );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_texture_data_2d
 * Desc: Sets pixel data for a 2D texture.
 */
void IKeOpenGLESRenderDevice::SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture )
{
	GLenum error = glGetError( );
    IKeOpenGLESTexture* t = static_cast<IKeOpenGLESTexture*>( texture );
    
	//glEnable( t->target );
	glBindTexture( t->target, t->handle );
    glTexSubImage2D( t->target, miplevel, offsetx, offsety, width, height, t->depth_format, t->data_type, pixels );
	OGL_DISPDBG( KE_ERROR, "Error setting texture data!", glGetError() );
	glBindTexture( t->target, 0 );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_texture_data_3d
 * Desc: Sets pixel data for a 3D texture.
 */
void IKeOpenGLESRenderDevice::SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture )
{
    DISPDBG_R( KE_ERROR, "3D textures are not supported for OpenGL ES" );
}

/*
 * Name: IKeOpenGLESRenderDevice::create_render_target
 * Desc: Creates a seperate render target (FBO), typically used for rendering to a texture.
 *       Creates a colour, depth and stencil buffer (if desired) and can be set as a texture.
 */
bool IKeOpenGLESRenderDevice::CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget )
{
#if 0
    GLenum error = glGetError();
    IKeOpenGLESRenderTarget* rt = static_cast<IKeOpenGLESRenderTarget*>( *rendertarget );
    
    /* Generate frame buffer object */
    glGenFramebuffers( 1, &rt->frame_buffer_object );
    OGL_DISPDBG_RB( 1, "Error creating FBO!", glGetError() );
    
    /* Bind the FBO */
    glBindFramebuffer( GL_FRAMEBUFFER, rt->frame_buffer_object );
    
    /* Create a texture to render this FBO to */
    this->CreateTexture2D( KE_TEXTURE_2D, width, height, 0, KE_TEXTUREFORMAT_RGBA, KE_UNSIGNED_BYTE, &rt->texture );
    
    /* Use nearest point filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    /* Create the depth buffer */
    glGenRenderbuffers( 1, &rt->depth_render_buffer );
    OGL_DISPDBG_RB( 1, "Error creating depth buffer!", glGetError() );
    
    /* Set the depth buffer attributes */
    IKeOpenGLESTexture* tex = static_cast<IKeOpenGLESTexture*>( rt->texture );
    
    glBindRenderbuffer( GL_RENDERBUFFER, rt->depth_render_buffer );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex->width, tex->height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rt->depth_render_buffer );
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex->height, 0 );
    
    GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers( 1, draw_buffers );
    
    /* Check the framebuffer status */
    if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        error = glGetError();
        DISPDBG_RB( 1, "Error during rendertarget creation!" );
    }
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "Render targets are not yet supported." );
#endif
}

/*
 * Name: IKeOpenGLESRenderDevice::delete_render_target
 * Desc: Deletes the render target resources used.
 */
void IKeOpenGLESRenderDevice::DeleteRenderTarget( IKeRenderTarget* rendertarget )
{
    IKeOpenGLESRenderTarget* rt = static_cast<IKeOpenGLESRenderTarget*>( rendertarget );
    
    /* Delete the texture */
    this->DeleteTexture( static_cast<IKeOpenGLESTexture*>( rt->texture ) );
    
    /* Delete the render target */
    glDeleteRenderbuffers( 1, &rt->depth_render_buffer );
    glDeleteFramebuffers( 1, &rt->frame_buffer_object );

	delete rendertarget;
}

/*
 * Name: IKeOpenGLESRenderDevice::bind_render_target
 * Desc: Binds the render target to OpenGL.  You set the texture to the appropriate  texture
 *       stage yourself using ::set_texture().
 */
void IKeOpenGLESRenderDevice::BindRenderTarget( IKeRenderTarget* rendertarget )
{
    GLenum error = glGetError();
    IKeOpenGLESRenderTarget* rt = static_cast<IKeOpenGLESRenderTarget*>( rendertarget );
    
    /* Bind the FBO */
    glBindFramebuffer( GL_FRAMEBUFFER, rt->frame_buffer_object );
    error = glGetError();
    if( error != GL_NO_ERROR )
        DISPDBG( 1, "IKeOpenGLESRenderDevice::bind_render_target(): Error binding rendertarget! (error=0x" << error << ")\n" );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_texture
 * Desc: Sets a texture to the desired texture stage.  If NULL, then texturing is disabled on
 *       the selected texture stage.
 */
void IKeOpenGLESRenderDevice::SetTexture( int stage, IKeTexture* texture )
{
    IKeOpenGLESTexture* tex = static_cast<IKeOpenGLESTexture*>(texture);
    
	/* Save this texture as the active texture for this unit */
	current_texture[stage] = tex;

    /* Select the currently active texture stage */
    glActiveTexture( GL_TEXTURE0 + stage );
    
    /* If this is a valid texture, set it. */
    if( texture )
    {
        glEnable( tex->target );
        glBindTexture( tex->target, tex->handle );
    }
    else
    {
        //glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        //glDisable( GL_TEXTURE_3D );
        //glDisable( GL_TEXTURE_1D_ARRAY );
        //glDisable( GL_TEXTURE_2D_ARRAY );
        //glDisable( GL_TEXTURE_RECTANGLE );
    }
}

/*
 * Name: IKeOpenGLESRenderDevice::CreateState
 * Desc: Creates a compiled buffer of render and texture states.  At the time of writing, OpenGL
 *       does not have an equivalent to Direct3D's state buffer, we have to create a faux state buffer
 *       and save/set each state individually.
 */
bool IKeOpenGLESRenderDevice::CreateStateBuffer( KeState* state_params, int state_count, IKeStateBuffer** state_buffer )
{
    /* Create a new state buffer interface */
    (*state_buffer) = new IKeOpenGLESStateBuffer;
    IKeOpenGLESStateBuffer* sb = static_cast<IKeOpenGLESStateBuffer*>( *state_buffer );
    
    /* Allocate faux state buffer */
    sb->states = new KeState[state_count];
    
    /* Copy the supplied state parameters over */
    memmove( sb->states, state_params, sizeof( KeState ) * state_count );
    sb->state_count = state_count;
    
    return true;
}

bool IKeOpenGLESRenderDevice::SetStateBuffer( IKeStateBuffer* state_buffer )
{
    int i = 0;
    
    /* Sanity check */
    if( !state_buffer )
        return false;
    
    IKeOpenGLESStateBuffer* sb = static_cast<IKeOpenGLESStateBuffer*>(state_buffer);
    
    /* Apply each render state in the list */
    while( i != sb->state_count )
    {
        switch( sb->states[i].state )
        {
            case KE_RS_DEPTHTEST:
                if( sb->states[i].param1 )
                    glEnable( GL_DEPTH_TEST );
                else
                    glDisable( GL_DEPTH_TEST );
                break;
                
            case KE_RS_DEPTHFUNC:
                glDepthFunc( test_funcs[sb->states[i].param1] );
                break;
                
            case KE_RS_DEPTHMASK:
                if( sb->states[i].param1 )
                    glEnable( GL_DEPTH_WRITEMASK );
                else
                    glDisable( GL_DEPTH_WRITEMASK );
                break;
                
            /*case KE_RS_CLEARDEPTH:
                glClearDepth( sb->states[i].fparam );
                break;*/
                
            case KE_RS_ALPHABLEND:
                if( sb->states[i].param1 )
                    glEnable( GL_BLEND );
                else
                    glDisable( GL_BLEND );
                break;
                
            case KE_RS_FRONTFACE:
                /* TODO */
                break;
                
            case KE_RS_POLYGONMODE:
                //glPolygonMode( polygon_modes[sb->states[i].param1], fill_modes[sb->states[i].param2] );
                DISPDBG( KE_WARNING, "You cannot set the polygon mode for OpenGL ES" );
                break;
                
            case KE_RS_BLENDFUNC:
                glBlendFunc( blend_modes[sb->states[i].param1], blend_modes[sb->states[i].param2] );
                break;
                
            case KE_RS_CULLMODE:
                if( sb->states[i].param1 )
                    glEnable( GL_CULL_FACE );
                else
                    glDisable( GL_CULL_FACE );
                glCullFace( cull_modes[sb->states[i].param2] );
                break;
                
            default:
                DISPDBG( KE_WARNING, "Bad render or texture state!\nstate: " << sb->states[i].state << "\n"
                        "param1: " << sb->states[i].param1 << "\n"
                        "param2: " << sb->states[i].param2 << "\n"
                        "param3: " << sb->states[i].param3 << "\n"
                        "fparam: " << sb->states[i].fparam << "\n"
                        "dparam: " << sb->states[i].dparam << "\n" );
                break;
        }
        
        i++;
    }
    
    return true;
}

/*
 * Name: IKeOpenGLESRenderDevice::set_render_states
 * Desc: Applies a list of user defined render states.
 * TODO: Allow explicit deferring of render states?
 */
void IKeOpenGLESRenderDevice::SetRenderStates( KeState* states )
{
    int i = 0;
    
    /* Apply each render state in the list */
    while( states[i].state != -1 )
    {
        switch( states[i].state )
        {
            case KE_RS_DEPTHTEST:
                if( states[i].param1 )
                    glEnable( GL_DEPTH_TEST );
                else
                    glDisable( GL_DEPTH_TEST );
                break;
                
            case KE_RS_DEPTHFUNC:
                glDepthFunc( test_funcs[states[i].param1] );
                break;
                
            case KE_RS_DEPTHMASK:
                if( states[i].param1 )
                    glEnable( GL_DEPTH_WRITEMASK );
                else
                    glDisable( GL_DEPTH_WRITEMASK );
                break;
                
            /*case KE_RS_CLEARDEPTH:
                glClearDepth( states[i].fparam );
                break;*/
                
            case KE_RS_ALPHABLEND:
                if( states[i].param1 )
                    glEnable( GL_BLEND );
                else
                    glDisable( GL_BLEND );
                break;
                
            case KE_RS_FRONTFACE:
                /* TODO */
                break;
                
            case KE_RS_POLYGONMODE:
                //glPolygonMode( polygon_modes[states[i].param1], fill_modes[states[i].param2] );
                DISPDBG( KE_WARNING, "You cannot set the polygon mode for OpenGL ES" );
                break;
                
            case KE_RS_BLENDFUNC:
                glBlendFunc( blend_modes[states[i].param1], blend_modes[states[i].param2] );
                break;
                
            case KE_RS_CULLMODE:
                if( states[i].param1 )
                    glEnable( GL_CULL_FACE );
                else
                    glDisable( GL_CULL_FACE );
                glCullFace( cull_modes[states[i].param2] );
                break;
                
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
 * Name: IKeOpenGLESRenderDevice::set_sampler_states
 * Desc: Applies a list of user defined sampler states.
 * TODO: Allow explicit deferring of sampler states?
 */
void IKeOpenGLESRenderDevice::SetSamplerStates( KeState* states )
{
    int i = 0;
    
    while( states[i].state != -1 )
    {
        switch( states[i].state )
        {
            case KE_TS_MAGFILTER:
                glTexParameteri( texture_targets[states[i].param1], GL_TEXTURE_MAG_FILTER, texture_filter_modes[states[i].param2] );
                break;
                
            case KE_TS_MINFILTER:
                glTexParameteri( texture_targets[states[i].param1], GL_TEXTURE_MIN_FILTER, texture_filter_modes[states[i].param2] );
                break;
                
            default:
                DISPDBG( KE_WARNING, "Bad texture state!\nstate: " << states[i].state << "\n"
                        "param1: " << states[i].param1 << "\n"
                        "param2: " << states[i].param2 << "\n"
                        "param3: " << states[i].param3 << "\n"
                        "fparam: " << states[i].fparam << "\n"
                        "dparam: " << states[i].dparam << "\n" );
                break;
        }
    }
}

/*void IKeOpenGLESRenderDevice::draw_vertices_im()
{
    
}*/


/*
 * Name: IKeOpenGLESRenderDevice::draw_vertices
 * Desc: Draws vertices from the current vertex buffer
 */
void IKeOpenGLESRenderDevice::DrawVertices( uint32_t primtype, uint32_t stride, int first, int count )
{
    IKeOpenGLESGeometryBuffer* gb = static_cast<IKeOpenGLESGeometryBuffer*>( current_geometrybuffer );
    IKeOpenGLESGpuProgram* gp = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
   
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, world_matrix._array );
    error = glGetError();
    glUniformMatrix4fv( gp->matrices[1], 1, No, view_matrix._array );
    error = glGetError();
    glUniformMatrix4fv( gp->matrices[2], 1, No, projection_matrix._array );
    error = glGetError();
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!", glGetError() );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    /* Draw the vertices */
    glDrawArrays( primitive_types[primtype], first, count );
    OGL_DISPDBG_R( KE_ERROR, "Vertex buffer rendering error (glDrawArrays)!", glGetError() );
}

/*
 * Name: IKeOpenGLESRenderDevice::draw_indexed_vertices
 * Desc: Draws vertices from the current vertex and index buffer.
 */
void IKeOpenGLESRenderDevice::DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count )
{
    IKeOpenGLESGeometryBuffer* gb = static_cast<IKeOpenGLESGeometryBuffer*>( current_geometrybuffer );
    IKeOpenGLESGpuProgram* gp = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, world_matrix._array );
    glUniformMatrix4fv( gp->matrices[1], 1, No, view_matrix._array );
    glUniformMatrix4fv( gp->matrices[2], 1, No, projection_matrix._array );
    
    /* Bind the vertex and index buffer objects */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!", glGetError() );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding index buffer!", glGetError() );
   
    /* Draw the vertices */
    glDrawElements( primitive_types[primtype], count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "Indexed geometry rendering error (glDrawElements)!", glGetError() );
}

/*
 * Name: IKeOpenGLESRenderDevice::draw_indexed_vertices_range
 * Desc: Same as above, but allows the user to specify the start/end vertex.
 */
void IKeOpenGLESRenderDevice::DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count )
{
#if 0
    IKeOpenGLESGeometryBuffer* gb = static_cast<IKeOpenGLESGeometryBuffer*>( current_geometrybuffer );
    IKeOpenGLESGpuProgram* gp = static_cast<IKeOpenGLESGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
   
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, world_matrix._array );
    glUniformMatrix4fv( gp->matrices[1], 1, No, view_matrix._array );
    glUniformMatrix4fv( gp->matrices[2], 1, No, projection_matrix._array );
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!", glGetError() );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding index buffer!", glGetError() );
    
    /* Draw the vertices */
    glDrawRangeElements( primitive_types[primtype], start, end, count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "Indexed geometry rendering error (glDrawRangeElements)!", glGetError() );
#else
    DISPDBG( KE_ERROR, "This rendering API is not yet supported for OpenGL ES 2.0" );
#endif
}

/*
 * Name: IKeOpenGLESRenderDevice::get_framebuffer_region
 * Desc: Returns a pointer filled with pixels of the given region of the current framebuffer.
 * TODO: Determine bit depth, allow reading from depth buffers, etc.
 */
bool IKeOpenGLESRenderDevice::GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels )
{
    int buffer_bpp = device_desc->colour_bpp;
    
    /* Return the bit depth of this framebuffer */
    *bpp = buffer_bpp;
    
    /* Allocate pointer to hold the pixel data */
    (*pixels) = new uint8_t[(width-x)*(height-y)*(buffer_bpp/8)];
    if( !(*pixels) )
        DISPDBG_RB( KE_ERROR, "Could not allocate buffer!" );
    
    /* Read from the current framebuffer */
    glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, *pixels );
    
    return true;
}

/*
 * Name: IKeOpenGLESRenderDevice::set_viewport
 * Desc: Sets the viewport.
 */
void IKeOpenGLESRenderDevice::SetViewport( int x, int y, int width, int height )
{
    /* Set the viewport */
    glViewport( x, y, width, height );
    
    viewport[0] = x;
    viewport[1] = y;
    viewport[2] = width;
    viewport[3] = height;
}

/*
 * Name: IKeOpenGLESRenderDevice::set_viewport
 * Desc: Sets the viewport.
 */
void IKeOpenGLESRenderDevice::SetViewportV( int* viewport )
{
    /* Set the viewport */
    glViewport( viewport[0], viewport[1], viewport[2], viewport[3] );
    
    memmove( this->viewport, viewport, sizeof(int)*4 );
}

/*
 * Name: IKeOpenGLESRenderDevice::GetViewport
 * Desc: Gets the viewport.
 */
void IKeOpenGLESRenderDevice::GetViewport( int* x, int* y, int* width, int* height )
{
    /* Get the viewport */
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    *x = viewport[0];
    *y = viewport[1];
    *width = viewport[2];
    *height = viewport[3];
}

/*
 * Name: IKeOpenGLESRenderDevice::set_viewport
 * Desc: Gets the viewport.
 */
void IKeOpenGLESRenderDevice::GetViewportV( int* viewport )
{
    /* Get the viewport */
    glGetIntegerv( GL_VIEWPORT, this->viewport );
    
    memmove( viewport, this->viewport, sizeof(int)*4 );
}

/*
 * Name: IKeOpenGLESRenderDevice::set_perspective_matrix
 * Desc: Sets the projection matrix by creating a perspective matrix.
 */
void IKeOpenGLESRenderDevice::SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z )
{
    /* Set up projection matrix using the perspective method */
//    projection_matrix = M4MakePerspective( fov, aspect, near_z, far_z );
    nv::perspective( projection_matrix, fov, aspect, near_z, far_z );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_view_matrix
 * Desc:
 */
void IKeOpenGLESRenderDevice::SetViewMatrix( const nv::matrix4f* view )
{
    /* Copy over the incoming view matrix */
    memmove( view_matrix._array, view->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_world_matrix
 * Desc:
 */
void IKeOpenGLESRenderDevice::SetWorldMatrix( const nv::matrix4f* world )
{
    /* Copy over the incoming world matrix */
    memmove( world_matrix._array, world->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_modelview_matrix
 * Desc:
 */
void IKeOpenGLESRenderDevice::SetModelviewMatrix( const nv::matrix4f* modelview )
{
    /* Copy over the incoming modelview matrix */
    memmove( modelview_matrix._array, modelview->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_projection_matrix
 * Desc:
 */
void IKeOpenGLESRenderDevice::SetProjectionMatrix( const nv::matrix4f* projection )
{
    /* Copy over the incoming projection matrix */
    memmove( projection_matrix._array, projection->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLESRenderDevice::block_until_vertical_blank
 * Desc: Stalls the current thread for an interval equivalent to one
 *       vertical blank. This function does not sync to the actual vertical blank
 *       as I have not found a way to do this on any platform besides Windows.
 *       This is also thread safe.
 */
void IKeOpenGLESRenderDevice::BlockUntilVerticalBlank()
{
    SDL_DisplayMode display_mode;
    
    /* Get the current display mode */
    /* TODO: Get display mode based on windowed or fullscreen mode. */
    SDL_GetWindowDisplayMode( window, &display_mode );
    
    /* Stall this thread for 1000/refresh_rate milliseconds */
    SDL_Delay( 1000 / display_mode.refresh_rate );
}


/*
 * Name: IKeOpenGLESRenderDevice::set_swap_interval
 * Desc: Sets the swap interval (enables/disable vertical sync). See SDL documentation on
 *       SDL_GL_SetSwapInterval for a more detailed description.
 */
void IKeOpenGLESRenderDevice::SetSwapInterval( int swap_interval )
{
    SDL_GL_SetSwapInterval( swap_interval );
}


/*
 * Name: IKeOpenGLESRenderDevice::get_swap_interval
 * Desc: Returns the vertical sync value set above.
 */
int IKeOpenGLESRenderDevice::GetSwapInterval()
{
    return SDL_GL_GetSwapInterval();
}

/*
 * Name: IKeOpenGLESRenderDevice::block_until_idle
 * Desc: Stalls the current thread until the GPU is no longer busy.
 */
void IKeOpenGLESRenderDevice::BlockUntilIdle()
{
	glFinish();
}


/*
 * Name: IKeOpenGLESRenderDevice::kick
 * Desc: Sends all pending GPU commands to the pipeline.
 */
void IKeOpenGLESRenderDevice::Kick()
{
	glFlush();
}


/*
 * Name: IKeOpenGLESRenderDevice::insert_fence
 * Desc: Creates a new GPU fence object and sets it in place.
 */
bool IKeOpenGLESRenderDevice::InsertFence( IKeFence** fence )
{
	/* Sanity check */
	if( !fence )
		return false;

	/* Allocate and initialize a new fence */
	(*fence) = new IKeOpenGLESFence;

	return KeOpenGLInsertFence[fence_vendor]( (IKeOpenGLESFence**) fence );
}


/*
 * Name: IKeOpenGLESRenderDevice::test_fence
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeOpenGLESRenderDevice::TestFence( IKeFence* fence )
{
	if( !fence )
		return false;

	return KeOpenGLTestFence[fence_vendor]( static_cast<IKeOpenGLESFence*>(fence) );
}


/*
 * Name: IKeOpenGLESRenderDevice::block_on_fence
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeOpenGLESRenderDevice::BlockOnFence( IKeFence* fence )
{
	KeOpenGLBlockOnFence[fence_vendor]( static_cast<IKeOpenGLESFence*>(fence) );
}


/*
 * Name: IKeOpenGLESRenderDevice::delete_fence
 * Desc: Deletes a GPU fence object.
 */
void IKeOpenGLESRenderDevice::DeleteFence( IKeFence* fence )
{
	if( !fence )
		return;

	IKeOpenGLESFence* f = static_cast<IKeOpenGLESFence*>( fence );
	KeOpenGLDeleteFence[fence_vendor](f);

	delete fence;
}


/*
 * Name: IKeOpenGLESRenderDevice::is_fence
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeOpenGLESRenderDevice::IsFence( IKeFence* fence )
{
	if( !fence )
		return false;

	return KeOpenGLIsFence[fence_vendor]( static_cast<IKeOpenGLESFence*>(fence) );;
}

/*
 * Name: IKeOpenGLESRenderDevice::gpu_memory_info
 * Desc: Returns the amound of available and total video memory of this machine.
 * NOTE: The OpenGL version of this API is dependent on the availability of certain extensions
 *       that expose this.  This is not guaranteed to be supported for all platforms.
 */
void IKeOpenGLESRenderDevice::GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory )
{
#ifdef _WIN32
 #ifdef USE_DDRAW_VMEM
	DDSCAPS2 caps;
	reinterpret_cast<IDirectDraw7*>( dd )->GetAvailableVidMem( &caps, (DWORD*) total_memory, (DWORD*) free_memory );
 #endif
#endif

#ifndef __APPLE__
#endif
}


