//
//  ke_ogl_renderdevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_ogl_renderdevice.h"
#include "nvdebug.h"


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

/*
 * Name: ke_initialize_default_shaders
 * Desc: Initializes the default shaders to be used when there is
 *       no user defined program used.
 */
bool ke_initialize_default_shaders()
{
    GLuint p, f, v;
    
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
    
	const char * vv = default_vertex_program_v150;
	const char * ff = default_fragment_program_v150;
    
	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);
    
	GLint compiled;
    
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Vertex shader not compiled.\n");
	}
    
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Fragment shader not compiled.\n");
	}
    
	p = glCreateProgram();
    
	glBindAttribLocation(p, 0, "in_pos");
    
	glAttachShader(p,v);
	glAttachShader(p,f);
    
	glLinkProgram(p);
	glUseProgram(p);
    
    glDeleteShader(v);
    glDeleteShader(f);
    
    ke_default_program = p;
    
    return true;
}

/*
 * Name: ke_uninitialize_default_shaders
 * Desc:
 */
void ke_uninitialize_default_shaders()
{
    glDeleteProgram( ke_default_program );
}

/*
 * Name: ke_ogl_renderdevice::ke_ogl_renderdevice
 * Desc: Default constructor
 */
ke_ogl_renderdevice_t::ke_ogl_renderdevice_t()
{
    /* TODO: Disable by making private? */
    assert(No);
}


/*
 * Name: ke_ogl_renderdevice::
 * Desc: 
 */

/*
 * Name: ke_ogl_renderdevice::ke_ogl_renderdevice
 * Desc: Appropriate constructor used for initialization of OpenGL via SDL.
 */
ke_ogl_renderdevice_t::ke_ogl_renderdevice_t( ke_renderdevice_desc_t* renderdevice_desc )
{
    /* Until we are finished initializing, mark this flag as false */
    initialized = false;
    
    /* Sanity checks */
    if( !renderdevice_desc )
        return;
    
    /* Save a copy of the render device description */
    device_desc = new ke_renderdevice_desc_t;
    memmove( device_desc, renderdevice_desc, sizeof( ke_renderdevice_desc_t ) );
    
    /* Verify device type */
    if( device_desc->device_type == KE_RENDERDEVICE_D3D11_1 || device_desc->device_type == KE_RENDERDEVICE_OGLES2_0 || device_desc->device_type == KE_RENDERDEVICE_OGLES3_0 )
        return;
    
    /* Initialize SDL video */
    if( SDL_InitSubSystem( SDL_INIT_VIDEO ) != 0 )
        return;
    
    /* Setup OpenGL properties */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, Yes );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, device_desc->depth_bpp );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, device_desc->stencil_bpp );
    
    /* Set the appropriate OpenGL version and profile */
    if( device_desc->device_type == KE_RENDERDEVICE_OGL4_1 )
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    }
    else if( device_desc->device_type == KE_RENDERDEVICE_OGL3_2 )
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    }
    else
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
    }
    
    /* Initialize the SDL window */
    window = SDL_CreateWindow( "Kunai Engine 0.1a",  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              device_desc->width, device_desc->height, SDL_WINDOW_OPENGL );
    if( !window )
        return;
    
    /* Create our OpenGL context */
    context = SDL_GL_CreateContext( window );
    if( !context )
        return;
    
    /* Initialize default OpenGL vertex and fragment program */
    ke_initialize_default_shaders();
    
    /* Set default OpenGL render states */
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
    
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

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
}


/*
 * Name: ke_ogl_renderdevice::~ke_ogl_renderdevice
 * Desc: Default deconstructor
 */
ke_ogl_renderdevice_t::~ke_ogl_renderdevice_t()
{
    delete device_desc;
    
    /* Kill the default vertex and fragment program */
    ke_uninitialize_default_shaders();
    
    /* Uninitialize and close OpenGL and SDL */
    SDL_GL_DeleteContext( context );
    SDL_DestroyWindow( window );
    SDL_QuitSubSystem( SDL_INIT_VIDEO );
}


/*
 * Name: ke_ogl_renderdevice::confirm_device
 * Desc: Gives confirmation that this device was successfully initialized.
 */
bool ke_ogl_renderdevice_t::confirm_device()
{
    return initialized;
}


/*
 * Name: ke_ogl_renderdevice::set_clear_colour_fv
 * Desc: Sets the clear colour
 */
void ke_ogl_renderdevice_t::set_clear_colour_fv( float* colour )
{
    memcpy( this->clear_colour, clear_colour, sizeof(float)*4 );
    
    glClearColor( colour[0], colour[1], colour[2], colour[3] );
}


/*
 * Name: ke_ogl_renderdevice::set_clear_colour_ubv
 * Desc: Same as above.
 */
void ke_ogl_renderdevice_t::set_clear_colour_ubv( uint8_t* colour )
{
    this->clear_colour[0] = float(colour[0]/255);
    this->clear_colour[1] = float(colour[1]/255);
    this->clear_colour[2] = float(colour[2]/255);
    this->clear_colour[3] = float(colour[3]/255);
    
    glClearColor( this->clear_colour[0], this->clear_colour[1], this->clear_colour[2], this->clear_colour[3] );
}


/*
 * Name: ke_ogl_renderdevice::set_clear_depth
 * Desc: 
 */
void ke_ogl_renderdevice_t::set_clear_depth( float depth )
{
    glClearDepth( depth );
}


/*
 * Name: ke_ogl_renderdevice::clear_render_buffer
 * Desc: Clears only the current render buffer
 */
void ke_ogl_renderdevice_t::clear_colour_buffer()
{
    glClear( GL_COLOR_BUFFER_BIT );
}


/*
 * Name: ke_ogl_renderdevice::clear_depth_buffer
 * Desc: Clears only the current depth buffer
 */
void ke_ogl_renderdevice_t::clear_depth_buffer()
{
    glClear( GL_DEPTH_BUFFER_BIT );
}


/*
 * Name: ke_ogl_renderdevice::clear_stencil_buffer
 * Desc: Clears only the current stencil buffer
 */
void ke_ogl_renderdevice_t::clear_stencil_buffer()
{
    glClear( GL_STENCIL_BUFFER_BIT );
}


/*
 * Name: ke_ogl_renderdevice::swap
 * Desc: Swaps the double buffer.
 */
void ke_ogl_renderdevice_t::swap()
{
    SDL_GL_SwapWindow( window );
}


/*
 * Name: ke_ogl_renderdevice_t::create_geometry_buffer
 * Desc: Creates a geometry buffer based on the vertex and index data given.  Vertex and index
 *       buffers are encapsulated into one interface for easy management, however, index data
 *       input is completely optional.  Interleaved vertex data is also supported.
 */
bool ke_ogl_renderdevice_t::create_geometry_buffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, ke_vertexattribute_t* vertex_attributes, ke_geometrybuffer_t** geometry_buffer )
{
    GLenum error = glGetError();
    
    /* Sanity check(s) */
    if( !geometry_buffer )
        return false;
    if( !vertex_attributes )
        return false;
    if( !vertex_data_size )
        return false;   /* Temporary? */
 
    *geometry_buffer = new ke_ogl_geometrybuffer_t;
    ke_ogl_geometrybuffer_t* gb = static_cast<ke_ogl_geometrybuffer_t*>( *geometry_buffer );
    
    /* Create a vertex array object */
    glGenVertexArrays( 1, &gb->vao );
    error = glGetError();
    
    /* Bind this vertex array object */
    glBindVertexArray( gb->vao );
    
    /* Create the vertex buffer object */
    glGenBuffers( 1, &gb->vbo[0] );
    error = glGetError();
    
    /* Set the vertex buffer data */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW );
    error = glGetError();
    
    /* Set the vertex attributes for this geometry buffer */
    for( int i = 0; vertex_attributes[i].index != -1; i++ )
    {
        glVertexAttribPointer( vertex_attributes[i].index,
                              vertex_attributes[i].size,
                              data_types[vertex_attributes[i].type],
                              vertex_attributes[i].normalize,
                              vertex_attributes[i].stride,
                              BUFFER_OFFSET(vertex_attributes[i].offset) );
        glEnableVertexAttribArray(vertex_attributes[i].index);
    }
    error = glGetError();
    
    /* Create an index buffer if desired */
    if( index_data_size )
    {
        glGenBuffers( 1, &gb->vbo[1] );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        error = glGetError();
        
        /* Set the index buffer data */
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, GL_STATIC_DRAW );
        gb->index_type = index_data_type;
    }
    
    /* Unbind this vertex array object */
    glBindVertexArray(0);
    
    return true;
}


/*
 * Name: ke_ogl_renderdevice_t::create_geometry_buffer
 * Desc:
 */
void ke_ogl_renderdevice_t::delete_geometry_buffer( ke_geometrybuffer_t* geometry_buffer )
{
    ke_ogl_geometrybuffer_t* gb = static_cast<ke_ogl_geometrybuffer_t*>( geometry_buffer );
    
    /* Delete the VBO and VAO */
    glDeleteBuffers( 2, gb->vbo );
    glDeleteVertexArrays( 1, &gb->vao );
    
    delete geometry_buffer;
}

/*
 * Name: ke_ogl_renderdevice::set_vertex_buffer
 * Desc: Sets the current geometry buffer to be used when rendering. Internally, binds the
 *       vertex array object. If NULL, then sets the current vertex array object to 0.
 */
void ke_ogl_renderdevice_t::set_geometry_buffer( ke_geometrybuffer_t* geometry_buffer )
{
    current_geometrybuffer = geometry_buffer;
    
    if( geometry_buffer )
        glBindVertexArray( static_cast<ke_ogl_geometrybuffer_t*>( geometry_buffer )->vao );
    else
        glBindVertexArray(0);
}

/*
 * Name: ke_ogl_renderdevice_t::create_program
 * Desc: Creates a complete OpenGL program out of shaders in text form. The minimum requirements
 *       are one valid vertex and fragment shader, while geometry and tesselation shaders are
 *       optional.  Obviously, tesselation shaders require OpenGL 4.1+, and cannot be used with
 *       OpenGL 3.2.  This function will automatically search for specific attribute locations
 *       before linking it and search for pre-determined uniform names for textures and matrices
 *       (see code below).
 *       TODO: Allow user defined constants.
 */
bool ke_ogl_renderdevice_t::create_program( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, ke_gpu_program_t** gpu_program )
{
    GLuint p, f, v, t, g;
    *gpu_program = new ke_ogl_gpu_program_t;
    ke_ogl_gpu_program_t* gp = static_cast<ke_ogl_gpu_program_t*>( *gpu_program );
    GLenum error = glGetError();
    
	v = glCreateShader( GL_VERTEX_SHADER );
	f = glCreateShader( GL_FRAGMENT_SHADER );
    g = glCreateShader( GL_GEOMETRY_SHADER );
    
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
    
    gp->matrices[0] = glGetUniformLocation( p, "world" );
    error = glGetError();
    gp->matrices[1] = glGetUniformLocation( p, "view" );
    error = glGetError();
    gp->matrices[2] = glGetUniformLocation( p, "proj" );
    error = glGetError();
    
    glUniform1i( uniform_tex0, 0 );
    glUniform1i( uniform_tex1, 1 );
    glUniform1i( uniform_tex2, 2 );
    glUniform1i( uniform_tex3, 3 );

    glUseProgram(0);
    
    /* Save the handle to this newly created program */
    gp->program = p;
    
    return true;
}

/*
 * Name: ke_ogl_renderdevice_t::delete_program
 * Desc: Deletes the GPU program.
 */
void ke_ogl_renderdevice_t::delete_program( ke_gpu_program_t* gpu_program )
{
    /* Deletes the GPU program */
    if( gpu_program )
    {
        glDeleteProgram( static_cast<ke_ogl_gpu_program_t*>(gpu_program)->program );
        delete gpu_program;
    }
}

/*
 * Name: ke_ogl_renderdevice_t::set_program
 * Desc: Sets the GPU program.  If NULL, the GPU program is set to 0.
 */
void ke_ogl_renderdevice_t::set_program( ke_gpu_program_t* gpu_program )
{
    /* Check for a valid pointer. If NULL, then we set the current program to 0. */
    if( gpu_program )
    {
        ke_ogl_gpu_program_t* gp = static_cast<ke_ogl_gpu_program_t*>(gpu_program);
    
        /* Save a copy of this program */
        current_gpu_program = gpu_program;
    
        glUseProgram( gp->program );
    }
    else
        glUseProgram(0);
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant_1fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_1fv( const char* location, int count, float* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform1fv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant_2fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_2fv( const char* location, int count, float* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform2fv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant_3fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_3fv( const char* location, int count, float* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform3fv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant_4fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_4fv( const char* location, int count, float* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform4fv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_1iv( const char* location, int count, int* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform1iv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_2iv( const char* location, int count, int* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform2iv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_3iv( const char* location, int count, int* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform3iv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::set_program_constant_4iv( const char* location, int count, int* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glUniform4iv( loc, count, value );
}

/*
 * Name: ke_ogl_renderdevice_t::get_program_constant_fv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::get_program_constant_fv( const char* location, float* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glGetUniformfv( p->program, loc, value );
}

/*
 * Name: ke_ogl_renderdevice_t::get_program_constant_iv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void ke_ogl_renderdevice_t::get_program_constant_iv( const char* location, int* value )
{
    ke_ogl_gpu_program_t* p = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
    glGetUniformiv( p->program, loc, value );
}

/*
 * Name: ke_ogl_renderdevice::create_texture_1d
 * Desc: Creates a 1D texture.
 */
bool ke_ogl_renderdevice_t::create_texture_1d( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    *texture = new ke_ogl_texture_t;
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->target = target;
    t->data_type = data_type;
    t->depth_format = format;
    t->internal_format = format;
    t->target = target;
    
    /* Use OpenGL to create a new 1D texture */
    glGenTextures( 1, &t->handle );
    glBindTexture( t->target, t->handle );
    error = glGetError();
    
    /* Set the initial texture attributes */
    glTexImage1D( t->target, 0, format, width, 0, format, data_type, NULL );
    error = glGetError();
    
    /* Set texture parameters */
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    error = glGetError();
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    error = glGetError();
    
    return true;
}

/*
 * Name: ke_ogl_renderdevice::create_texture_2d
 * Desc: Creates a blank 2D texture.
 */
bool ke_ogl_renderdevice_t::create_texture_2d( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    (*texture) = new ke_ogl_texture_t;
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->height = height;
    t->target = target;
    t->data_type = data_type;
    t->depth_format = format;
    t->internal_format = format;
    t->target = target;
    
    /* Use OpenGL to create a new 1D texture */
    glGenTextures( 1, &t->handle );
    glBindTexture( t->target, t->handle );
    error = glGetError();
    
    /* Set the initial texture attributes */
    glTexImage2D( t->target, 0, format, width, height, 0, format, data_type, NULL );
    error = glGetError();
    
    /* Set texture parameters */
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    error = glGetError();
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    error = glGetError();
    
    return true;
}

/*
 * Name: ke_ogl_renderdevice::create_texture_3d
 * Desc: Creates a blank 3D texture.
 */
bool ke_ogl_renderdevice_t::create_texture_3d( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, ke_texture_t** texture )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    (*texture) = new ke_ogl_texture_t;
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->height = height;
    t->depth = depth;
    t->target = target;
    t->data_type = data_type;
    t->depth_format = format;
    t->internal_format = format;
    t->target = target;
    
    /* Use OpenGL to create a new 1D texture */
    glGenTextures( 1, &t->handle );
    glBindTexture( t->target, t->handle );
    error = glGetError();
    
    /* Set the initial texture attributes */
    glTexImage3D( t->target, 0, format, width, height, depth, 0, format, data_type, NULL );
    error = glGetError();
    
    /* Set texture parameters */
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    error = glGetError();
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    error = glGetError();
    
    return true;
}

/*
 * Name: ke_ogl_renderdevice_t::delete_texture
 * Desc: Deletes a texture from memory.
 */
void ke_ogl_renderdevice_t::delete_texture( ke_texture_t* texture )
{
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( texture );
    
    if( texture )
    {
        glDeleteTextures( 1, &t->handle );
        delete texture;
    }
}


/*
 * Name: ke_ogl_renderdevice_t::set_texture_data_1d
 * Desc: Sets pixel data for a 1D texture.
 */
void ke_ogl_renderdevice_t::set_texture_data_1d( int offsetx, int width, int miplevel, void* pixels, ke_texture_t* texture )
{
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( texture );
    
    glTexSubImage1D( t->target, miplevel, offsetx, width, t->internal_format, t->data_type, pixels );
}

/*
 * Name: ke_ogl_renderdevice_t::set_texture_data_2d
 * Desc: Sets pixel data for a 1D texture.
 */
void ke_ogl_renderdevice_t::set_texture_data_2d( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, ke_texture_t* texture )
{
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( texture );
    
    glTexSubImage2D( t->target, miplevel, offsetx, offsety, width, height, t->internal_format, t->data_type, pixels );
    GLenum error = glGetError();
}

/*
 * Name: ke_ogl_renderdevice_t::set_texture_data_3d
 * Desc: Sets pixel data for a 1D texture.
 */
void ke_ogl_renderdevice_t::set_texture_data_3d( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, ke_texture_t* texture )
{
    ke_ogl_texture_t* t = static_cast<ke_ogl_texture_t*>( texture );
    
    glTexSubImage3D( t->target, miplevel, offsetx, offsety, offsetz, width, height, depth, t->internal_format, t->data_type, pixels );
}

/*
 * Name: ke_ogl_renderdevice_t::set_texture
 * Desc: Sets a texture to the desired texture stage.  If NULL, then texturing is disabled on
 *       the selected texture stage.
 */
void ke_ogl_renderdevice_t::set_texture( int stage, ke_texture_t* texture )
{
    ke_ogl_texture_t* tex = static_cast<ke_ogl_texture_t*>(texture);
    
    /* Select the currently active texture stage */
    glActiveTexture( GL_TEXTURE0 + stage );
    
    /* If this is a valid texture, set it. */
    if( texture )
    {
        glEnable( tex->target );
        glBindTexture( tex->target, tex->target );
    }
    else
    {
        glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_3D );
        glDisable( GL_TEXTURE_1D_ARRAY );
        glDisable( GL_TEXTURE_2D_ARRAY );
        glDisable( GL_TEXTURE_RECTANGLE );
    }
}

/*
 * Name: ke_ogl_renderdevice_t::set_render_states
 * Desc: Applies a list of user defined render states.
 * TODO: Allow explicit deferring of render states?
 */
void ke_ogl_renderdevice_t::set_render_states( ke_state_t* states )
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
                
            case KE_RS_CLEARDEPTH:
                glClearDepth( states[i].fparam );
                break;
                
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
                glPolygonMode( polygon_modes[states[i].param1], fill_modes[states[i].param2] );
                break;
                
            case KE_RS_BLENDFUNC:
                glBlendFunc( states[i].param1, states[i].param2 );
                break;
                
            default:
                /* TODO */
                break;
                
        }
    }
}

/*
 * Name: ke_ogl_renderdevice_t::set_sampler_states
 * Desc: Applies a list of user defined sampler states.
 * TODO: Allow explicit deferring of sampler states?
 */
void ke_ogl_renderdevice_t::set_sampler_states( ke_state_t* states )
{
    
}

/*void ke_ogl_renderdevice_t::draw_vertices_im()
{
    
}*/


/*
 * Name: ke_ogl_renderdevice::draw_vertices
 * Desc: Draws vertices from the current vertex buffer
 */
void ke_ogl_renderdevice_t::draw_vertices( uint32_t primtype, int first, int count )
{
    ke_ogl_geometrybuffer_t* gb = static_cast<ke_ogl_geometrybuffer_t*>( current_geometrybuffer );
    ke_ogl_gpu_program_t* gp = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    GLenum error = glGetError();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, &world_matrix.col0.x );
    error = glGetError();
    glUniformMatrix4fv( gp->matrices[1], 1, No, &view_matrix.col0.x );
    error = glGetError();
    glUniformMatrix4fv( gp->matrices[2], 1, No, &projection_matrix.col0.x );
    error = glGetError();
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    error = glGetError();
    
    /* Draw the vertices */
    glDrawArrays( primitive_types[primtype], first, count );
    error = glGetError();
}

/*
 * Name: ke_ogl_renderdevice::draw_indexed_vertices
 * Desc: Draws vertices from the current vertex and index buffer.
 */
void ke_ogl_renderdevice_t::draw_indexed_vertices( uint32_t primtype, int count )
{
    ke_ogl_geometrybuffer_t* gb = static_cast<ke_ogl_geometrybuffer_t*>( current_geometrybuffer );
    ke_ogl_gpu_program_t* gp = static_cast<ke_ogl_gpu_program_t*>( current_gpu_program );
    GLenum error = glGetError();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, &world_matrix.col0.x );
    glUniformMatrix4fv( gp->matrices[1], 1, No, &view_matrix.col0.x );
    glUniformMatrix4fv( gp->matrices[2], 1, No, &projection_matrix.col0.x );
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
    error = glGetError();
    //if( error != GL_NO_ERROR )
    //    DISPDBG( 1, "draw_indexed_vertices(): error binding buffers\n" );
    
    /* Draw the vertices */
    glDrawElements( primitive_types[primtype], count, data_types[gb->index_type], NULL );
    error = glGetError();
    //if( error != GL_NO_ERROR )
    //    DISPDBG( 1, "draw_indexed_vertices(): error drawing vertices\n" );
}

/*
 * Name: ke_ogl_renderdevice::set_viewport
 * Desc: Sets the viewport.
 */
void ke_ogl_renderdevice_t::set_viewport( int x, int y, int width, int height )
{
    /* Set the viewport */
    glViewport( x, y, width, height );
    
    viewport[0] = x;
    viewport[1] = y;
    viewport[2] = width;
    viewport[3] = height;
}


/*
 * Name: ke_ogl_renderdevice::set_perspective_matrix
 * Desc: Sets the projection matrix by creating a perspective matrix.
 */
void ke_ogl_renderdevice_t::set_perspective_matrix( float fov, float aspect, float near_z, float far_z )
{
    /* Set up projection matrix using the perspective method */
    projection_matrix = M4MakePerspective( fov, aspect, near_z, far_z );
}


/*
 * Name: ke_ogl_renderdevice::set_view_matrix
 * Desc:
 */
void ke_ogl_renderdevice_t::set_view_matrix( const Matrix4* view )
{
    /* Copy over the incoming view matrix */
    memmove( &view_matrix, view, sizeof( Matrix4 ) );
}


/*
 * Name: ke_ogl_renderdevice::set_world_matrix
 * Desc:
 */
void ke_ogl_renderdevice_t::set_world_matrix( const Matrix4* world )
{
    /* Copy over the incoming world matrix */
    memmove( &world_matrix, world, sizeof( Matrix4 ) );
}


/*
 * Name: ke_ogl_renderdevice::set_modelview_matrix
 * Desc:
 */
void ke_ogl_renderdevice_t::set_modelview_matrix( const Matrix4* modelview )
{
    /* Copy over the incoming modelview matrix */
    memmove( &modelview_matrix, modelview, sizeof( Matrix4 ) );
}


/*
 * Name: ke_ogl_renderdevice::set_projection_matrix
 * Desc:
 */
void ke_ogl_renderdevice_t::set_projection_matrix( const Matrix4* projection )
{
    /* Copy over the incoming projection matrix */
    memmove( &projection_matrix, projection, sizeof( Matrix4 ) );
}


/*
 * Name: ke_ogl_renderdevice_t::block_until_vertical_blank
 * Desc: Stalls the current thread for an interval equivalent to one
 *       vertical blank. This function does not sync to the actual vertical blank
 *       as I have not found a way to do this on any platform besides Windows.
 *       This is also thread safe.
 */
void ke_ogl_renderdevice_t::block_until_vertical_blank()
{
    SDL_DisplayMode display_mode;
    
    /* Get the current display mode */
    /* TODO: Get display mode based on windowed or fullscreen mode. */
    SDL_GetWindowDisplayMode( window, &display_mode );
    
    /* Stall this thread for 1000/refresh_rate milliseconds */
    SDL_Delay( 1000 / display_mode.refresh_rate );
}


/*
 * Name: ke_ogl_renderdevice_t::set_swap_interval
 * Desc: Sets the swap interval (enables/disable vertical sync). See SDL documentation on
 *       SDL_GL_SetSwapInterval for a more detailed description.
 */
void ke_ogl_renderdevice_t::set_swap_interval( int swap_interval )
{
    SDL_GL_SetSwapInterval( swap_interval );
}


/*
 * Name: ke_ogl_renderdevice_t::get_swap_interval
 * Desc: Returns the vertical sync value set above.
 */
int ke_ogl_renderdevice_t::get_swap_interval()
{
    return SDL_GL_GetSwapInterval();
}