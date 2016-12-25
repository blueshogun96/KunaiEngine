//
//  IKeOpenGLRenderDevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KeOpenGLRenderDevice.h"
#include "KeDebug.h"

#ifndef __MOBILE_OS__
#ifdef __APPLE__
#include "KeVideoAdapterOSX.h"
#endif
#endif

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
#define OGL_DISPDBG( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); }
#define OGL_DISPDBG_R( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return; }
#define OGL_DISPDBG_RB( a, b ) error = glGetError(); if(error) { DISPDBG( a, b << "\nError code: (" << error << ")" ); return false; }

/* Other macros */
#ifdef __MOBILE_OS__
#define glClearDepth glClearDepthf
#endif


/* GPU fencing routines */
#include "KeOpenGLFence.h"

/*
 * Program attribute locations
 */
struct KeProgramAttribute
{
    int location;
    char name[32];
} program_attributes[] =
{
    { 0, "in_pos" },
    { 1, "in_normal" },
    { 2, "in_tangent" },
    { 3, "in_bitangent" },
    { 4, "in_colour" },
    { 5, "in_tex0" },
    { 6, "in_tex1" },
    { 7, "in_tex2" },
    { 8, "in_tex3" },
    { 9, "in_tex4" },
    { 10, "in_tex5" },
    { 11, "in_tex6" },
    { 12, "in_tex7" },
};

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
#ifndef __MOBILE_OS__
    GL_DOUBLE   /* Not supported on OpenGL ES */
#else
    0
#endif
};

/* OpenGL buffer usage types */
uint32_t buffer_usage_types[] = 
{
	GL_STATIC_DRAW,
	GL_STATIC_READ,     /* OpenGL ES 3.0+ only */
	GL_STATIC_COPY,     /* OpenGL ES 3.0+ only */
	GL_DYNAMIC_DRAW,
	GL_DYNAMIC_READ,    /* OpenGL ES 3.0+ only */
	GL_DYNAMIC_COPY,    /* OpenGL ES 3.0+ only */
	GL_STREAM_DRAW,
	GL_STREAM_READ,     /* OpenGL ES 3.0+ only */
	GL_STREAM_COPY,     /* OpenGL ES 3.0+ only */
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
#ifndef __MOBILE_OS__
    GL_TEXTURE_1D,          /* Not supported on OpenGL ES */
    GL_TEXTURE_1D_ARRAY,    /* Not supported on OpenGL ES */
#else
    0, 0,
#endif
    GL_TEXTURE_2D,
    GL_TEXTURE_2D_ARRAY,    /* OpenGL ES 3.0+ only */
    GL_TEXTURE_3D,          /* OpenGL ES 3.0+ only */
#ifndef __MOBILE_OS__
    GL_TEXTURE_RECTANGLE,   /* Not supported on OpenGL ES */
#else
    0,
#endif
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
#ifndef __MOBILE_OS__
    GL_POINT,   /* Not supported on OpenGL ES */
    GL_LINE,    /* Not supported on OpenGL ES */
    GL_FILL,    /* Not supported on OpenGL ES */
#else
    0, 0, 0
#endif
};

/* OpenGL texture formats */
uint32_t texture_formats[] =
{
    GL_RGBA,
    GL_BGRA,
#ifndef __MOBILE_OS__
    GL_RED,
#else
	GL_LUMINANCE,//GL_RED_EXT,     /* OpenGL ES 3.0+ or GL_EXT_texture_rg */
#endif
	GL_RGB,
#ifndef __MOBILE_OS__
	GL_BGR,     /* Not supported on OpenGL ES */
#else
    0,
#endif
};

uint32_t internal_texture_formats[] =
{
	GL_RGBA,
	GL_RGBA,
#ifndef __MOBILE_OS__
    GL_R8,
#else
	GL_LUMINANCE, //GL_R8_EXT,      /* OpenGL ES 3.0+ or GL_EXT_texture_rg */
#endif
	GL_RGB,
	GL_RGB
};

/* OpenGL cull modes */
uint32_t cull_modes[] =
{
    0,
    GL_FRONT,
    GL_BACK
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
    GL_MIN,                     /* OpenGL ES 3.0+ only */
    GL_MAX,                     /* OpenGL ES 3.0+ only */
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
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR
};

/* OpenGL texture wrap modes */
uint32_t texture_wrap_modes[] = 
{
	GL_REPEAT,
#ifndef __APPLE__
	GL_CLAMP,
#else
    0,
#endif
	GL_CLAMP_TO_EDGE,
#ifndef __MOBILE_OS__
	GL_CLAMP_TO_BORDER,     /* Not supported on OpenGL ES */
#else
    0,
#endif
	GL_MIRRORED_REPEAT
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


/* Tells us whether a set of vertex attributes actually match another */
bool KeVertexAttributesMatch( KeVertexAttribute* va1, KeVertexAttribute* va2 )
{
    if( !va1 || !va2 )
        return false;
    
    int va_size1 = 0;
    while( va1[va_size1].index != -1 )
        va_size1++;
    
    int va_size2 = 0;
    while( va2[va_size2].index != -1 )
        va_size2++;
    
    if( va_size1 != va_size2 )
        return false;
    
    int i = va_size1-1;
    
    while( i > -1 )
    {
        if( va1[i].index == va2[i].index &&
            va1[i].normalize == va2[i].normalize &&
            va1[i].offset == va2[i].offset &&
            va1[i].size == va2[i].size &&
            va1[i].stride == va2[i].stride )
        {
            i--;
            continue;
        }
        else
            return false;
    }
    
    return true;
}

/* Disables the vertex attribute list */
void KeDisableVertexAttributes()
{
    int max_attribs = 0;
    
    /* Get the max number of vertex attributes this OpenGL implementation supports */
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &max_attribs );
    
    /* Disable all vertex attributes before setting the new ones */
    for( int i = 0; i < max_attribs; i++ )
        glDisableVertexAttribArray(i);
}

/* Sets the vertex attribute list and enables it */
void KeSetVertexAttributes( KeVertexAttribute* va )
{
    GLenum error = glGetError();
    
    /* Disable all vertex attributes */
    KeDisableVertexAttributes();
    
    /* Set the vertex attributes for this geometry buffer */
    for( int i = 0; va[i].index != -1; i++ )
    {
        glVertexAttribPointer( va[i].index,
                              va[i].size,
                              data_types[va[i].type],
                              va[i].normalize,
                              va[i].stride,
                              BUFFER_OFFSET(va[i].offset) );
        OGL_DISPDBG( KE_WARNING, "Unable to set vertex attribute #" << i );
        glEnableVertexAttribArray(va[i].index);
        OGL_DISPDBG( KE_WARNING, "Unable to enable vertex attribute #" << i );
    }
}

void KeSetVertexAttributesIM( KeVertexAttribute* va, uint8_t* vd )
{
    GLenum error = glGetError();
    
    /* Disable all vertex attributes */
    KeDisableVertexAttributes();
    
    /* Set the vertex attributes for this geometry buffer */
    for( int i = 0; va[i].index != -1; i++ )
    {
        glVertexAttribPointer( va[i].index,
                              va[i].size,
                              data_types[va[i].type],
                              va[i].normalize,
                              va[i].stride,
                              &vd[va[i].offset] );
        OGL_DISPDBG( KE_WARNING, "Unable to set vertex attribute #" << i );
        glEnableVertexAttribArray(va[i].index);
        OGL_DISPDBG( KE_WARNING, "Unable to enable vertex attribute #" << i );
    }
}

/* Returns true if this texture's dimensions are power of two, returns false otherwise. */
bool KeIsPowerOfTwo( int width, int height, int depth )
{
    uint32_t ideal_width = 0, ideal_height = 0, ideal_depth = 0;
    
    for( int v = 0; v < 32; v++ )
    {
        int mask = 1 << v;
        
        if( width & mask )
            ideal_width = mask;
        
        if( height & mask )
            ideal_height = mask;
        
        if( depth & mask )
            ideal_depth = mask;
    }
    
    if( width != ideal_width || height != ideal_height || depth != ideal_depth )
        return false;
    
    return true;
}

/* Applies sampler states to their respective texture stages */
/* TODO: Implement dirty states to prevent redundant setting of texture states */
void IKeOpenGLRenderDevice::PVT_ApplySamplerStates()
{
    /* Handle texture stages */
    for( int texture_stage = 0; texture_stage < 8; texture_stage++ )
    {
        /* Don't bother setting samplers to texture stages not being used */
        if( current_texture[texture_stage] == NULL )
            continue;
        
        /* Change the active texture unit before making changes */
        glActiveTexture( GL_TEXTURE0 + texture_stage );
        //glBindTexture( static_cast<IKeOpenGLTexture*>(current_texture[texture_stage])->target,
        //	static_cast<IKeOpenGLTexture*>(current_texture[texture_stage])->handle );
        
        IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>(current_texture[texture_stage]);
        
        int i = 0;
        while( samplers[texture_stage][i].state != -1 )
        {
            switch( samplers[texture_stage][i].state )
            {
                case KE_TS_MAGFILTER:
                    glTexParameteri( t->target, GL_TEXTURE_MAG_FILTER, texture_filter_modes[samplers[texture_stage][i].param1] );
                    break;
                    
                case KE_TS_MINFILTER:
                    glTexParameteri( t->target, GL_TEXTURE_MIN_FILTER, texture_filter_modes[samplers[texture_stage][i].param1] );
                    break;
                    
                case KE_TS_WRAPU:
                    glTexParameteri( t->target, GL_TEXTURE_WRAP_S, texture_wrap_modes[samplers[texture_stage][i].param1] );
                    break;
                    
                case KE_TS_WRAPV:
                    glTexParameteri( t->target, GL_TEXTURE_WRAP_T, texture_wrap_modes[samplers[texture_stage][i].param1] );
                    break;
                    
                case KE_TS_WRAPW:
                    glTexParameteri( t->target, GL_TEXTURE_WRAP_R, texture_wrap_modes[samplers[texture_stage][i].param1] );
                    break;
                    
                default:
                    DISPDBG( KE_WARNING, "Bad texture state!\nstate: " << samplers[texture_stage][i].state << "\n"
                            "param1: " << samplers[texture_stage][i].param1 << "\n"
                            "param2: " << samplers[texture_stage][i].param2 << "\n"
                            "param3: " << samplers[texture_stage][i].param3 << "\n"
                            "fparam: " << samplers[texture_stage][i].fparam << "\n"
                            "dparam: " << samplers[texture_stage][i].dparam << "\n" );
                    break;
            }
            
            i++;
        }
    }
}

void IKeOpenGLRenderDevice::PVT_SetWorldViewProjectionMatrices()
{
    IKeOpenGLGpuProgram* gp = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    
    GLenum error = glGetError();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    glUniformMatrix4fv( gp->matrices[0], 1, No, world_matrix._array );
    OGL_DISPDBG( KE_DBGLVL(1), "Could not set world matrix..." );
    glUniformMatrix4fv( gp->matrices[1], 1, No, view_matrix._array );
    OGL_DISPDBG( KE_DBGLVL(1), "Could not set projection matrix..." );
    glUniformMatrix4fv( gp->matrices[2], 1, No, projection_matrix._array );
    OGL_DISPDBG( KE_DBGLVL(1), "Could not set projection matrix..." );
}


/*
 * Name: IKeOpenGLRenderDevice::IKeOpenGLRenderDevice
 * Desc: Default constructor
 */
IKeOpenGLRenderDevice::IKeOpenGLRenderDevice()
{
    /* TODO: Disable by making private? */
    assert(No);
}


/*
 * Name: IKeOpenGLRenderDevice::
 * Desc: 
 */

/*
 * Name: IKeOpenGLRenderDevice::IKeOpenGLRenderDevice
 * Desc: Appropriate constructor used for initialization of OpenGL via SDL.
 */
IKeOpenGLRenderDevice::IKeOpenGLRenderDevice( KeRenderDeviceDesc* renderdevice_desc ) : fence_vendor( KE_FENCE_ARB ), dd(NULL), im_gb(NULL), im_cache_size(0)
{
    /* Until we are finished initializing, mark this flag as false */
    initialized = false;
    
    /* Sanity checks */
    if( !renderdevice_desc )
		DISPDBG_R( KE_ERROR, "Invalid render device description!" );
    
    /* Do we have a valid window width/height? */
    if( renderdevice_desc->width < 1 || renderdevice_desc->height < 1 )
        DISPDBG_R( KE_ERROR, "Invalid render target resolution!" );
    
    /* Save a copy of the render device description */
    device_desc = new KeRenderDeviceDesc;
    memmove( device_desc, renderdevice_desc, sizeof( KeRenderDeviceDesc ) );
    
    /* Verify device type */
    if( device_desc->device_type == KE_RENDERDEVICE_D3D11 )
        DISPDBG_R( KE_ERROR, "Invalid rendering device type specified!" );
    
#if defined(USE_DDRAW_VMEM) || defined(USE_DDRAW_VBLANK)
	/* Create a DirectDraw object if desired */
	HRESULT hr = DirectDrawCreateEx( NULL, &dd, IID_IDirectDraw7, NULL );
	if( FAILED( hr ) )
		DISPDBG_R( KE_ERROR, "Error creating DirectDraw7 object.  Disable DirectDraw if not needed!" );
#endif

    /* Initialize SDL video */
    if( SDL_InitSubSystem( SDL_INIT_VIDEO ) != 0 )
        DISPDBG_R( KE_ERROR, "Error initializing SDL video sub system!" );
    
    /* Setup OpenGL properties */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, Yes );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, device_desc->depth_bpp );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, device_desc->stencil_bpp );
    
    /* Set the appropriate OpenGL version and profile */
    if( device_desc->device_type == KE_RENDERDEVICE_OGL4 )
    {
		major_version = 4;
		minor_version = 5;
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    }
    else if( device_desc->device_type == KE_RENDERDEVICE_OGL3 )
    {
		major_version = 3;
		minor_version = 3;
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    }
    else if( device_desc->device_type == KE_RENDERDEVICE_OGLES2 )
    {
        major_version = 2;
        minor_version = 0;
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    }
    else if( device_desc->device_type == KE_RENDERDEVICE_OGLES3 )
    {
        major_version = 3;
        minor_version = 1;
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, major_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minor_version );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
    }
    else
    {
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
    }
    
    /* Initialize the SDL window */
    window = SDL_CreateWindow( "Kunai Engine",  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              device_desc->width, device_desc->height, SDL_WINDOW_OPENGL );
    if( !window )
        DISPDBG_R( KE_ERROR, "Error creating SDL window!" );
    
    /* Create our OpenGL context. */
    context = SDL_GL_CreateContext( window );
	if( !context )
	{
		/* If it fails the first time around, lower the minor version until it succeeds. */
		while( minor_version > -1 )
		{
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, --minor_version );
			context = SDL_GL_CreateContext( window );
			if( context )
				break;
		}
	}

	/* Verify that we have a valid context */
	if( !context )
		DISPDBG_R( KE_ERROR, "Error creating core OpenGL context!" );
    
	/* It's not wise to assume that just because we have a valid SDL OpenGL context that we have
	   an actual core OpenGL context to use.  SDL appears to be just giving us the highest profile
	   version available if core OpenGL is not supported, so we'll have to check for ourselves if
	   we have a real core OpenGL context.  If the major/minor version returned is 0, then we have
	   a legacy OpenGL context. */
	int real_major_version = 0, real_minor_version = 0;
	glGetIntegerv( GL_MAJOR_VERSION, &real_major_version );
	glGetIntegerv( GL_MINOR_VERSION, &real_minor_version );

	if( device_desc->device_type == KE_RENDERDEVICE_OGL4 )
	{
		if( real_major_version < 4 )
			DISPDBG_R( KE_ERROR, "A core OpenGL 4.x context was not created!" );
	}
	if( device_desc->device_type == KE_RENDERDEVICE_OGL3 )
	{
		if( real_major_version < 3 )
			DISPDBG_R( KE_ERROR, "A core OpenGL 3.x context was not created!" );
	}

	/* Initialize GLEW for non-Apple platforms */
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if( error != GLEW_NO_ERROR )
	{
		DISPDBG_R( KE_ERROR, "Error initializing glew!\n" );
	}
#endif
    
    /* Set default OpenGL render states */
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
    
#ifndef __MOBILE_OS__
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
#endif

	glDisable( GL_BLEND );
	glDisable( GL_CULL_FACE );
    glDisable( GL_TEXTURE_2D );
    
    /* Set vertex attributes to their defaults */
    ZeroMemory( current_vertexattribute, sizeof( KeVertexAttribute ) * 32 );
    current_vertexattribute[0].index = 0;
    current_vertexattribute[0].size = 3;
    current_vertexattribute[0].type = KE_FLOAT;
    current_vertexattribute[0].normalize = No;
    current_vertexattribute[0].stride = 0;
    current_vertexattribute[0].offset = 0;
    current_vertexattribute[1].index = -1;
    
    /* Nullify current geometry buffer */
    current_geometrybuffer = NULL;
    
	/* Nullify texture stages */
	current_texture[0] = NULL;
	current_texture[1] = NULL;
	current_texture[2] = NULL;
	current_texture[3] = NULL;
	current_texture[4] = NULL;
	current_texture[5] = NULL;
	current_texture[6] = NULL;
	current_texture[7] = NULL;

	/* Clear dirty sampler state flags */
	KeState empty = { static_cast<uint32_t>(-1), 0, 0, 0, 0, 0 };

	for( int i = 0; i < 8; i++ )
		memmove( &samplers[i], &empty, sizeof( KeState ) );
	ZeroMemory( dirty_samplers, sizeof( int ) * 8 * 16 );

    /* Setup default viewport */
    this->SetViewport( 0, 0, renderdevice_desc->width, renderdevice_desc->height );
    
    /* Mark as initialized */
    initialized = Yes;
    
    /* Print OpenGL driver/implementation details */
    DISPDBG( KE_DBGLVL(0), "\n\tOpenGL Vendor: " << glGetString( GL_VENDOR ) << 
		"\n\tOpenGL Version: " << glGetString( GL_VERSION ) << 
		"\n\tOpenGL Renderer: " << glGetString( GL_RENDERER ) <<
        "\n\tGLSL Version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n" );
    
    /* Print a list of available OpenGL extensions for this OpenGL implementation */
    int extension_count = 0, i = 0;
#ifdef __MOBILE_OS__
    char* extension_string = (char*) glGetString( GL_EXTENSIONS );
    /*size_t length = strlen( extension_string );
    
    while( i < length )
    {
        if( extension_string[i] == ' ' )
        {
            extension_string[i] = '\n';
            extension_count++;
        }
        
        i++;
    }*/
    
    std::vector<GLchar> ext;
    
    ext.push_back('\t');
    ext.push_back('\t');
    
    /* Count extensions */
    for( int i = 0; i < strlen( extension_string )+1; i++ )
    {
        if( extension_string[i] == ' ' || extension_string[i] == '\0' )
        {
            extension_count++;
            
            ext.push_back('\n');
            ext.push_back('\t');
            ext.push_back('\t');
        }
        else
        ext.push_back(extension_string[i]);
    }
    
    ext.push_back('\n\0');
    
    std::stringstream sstr;
    sstr << extension_count;
    std::string ext_str = "\n\tOpenGL Extensions (" + sstr.str() + "):\n";
    ext_str += ext.data();
#else
	glGetIntegerv( GL_NUM_EXTENSIONS, &extension_count );

	std::stringstream sstr;
	sstr << extension_count;
    std::string ext_str = "\n\tOpenGL Extensions (" + sstr.str() + "):\n";
    
	while( i < extension_count )
    {
		ext_str += "\t\t";
        ext_str += (const char*) glGetStringi( GL_EXTENSIONS, i );
		ext_str += "\n";
        i++;
    }
#endif
    
	DISPDBG( KE_DBGLVL(0), ext_str );

    /* Print out more information on the video card we just initialized OpenGL for */
#ifndef __MOBILE_OS__
#ifdef __APPLE__
    KeVideoAdapterOSX adapter;
    KeVideoCardInfo video_card_info;
    
    KeGetCurrentVideoAdapterInformationOSX( &adapter );
    KeGetVideoCardInfo( &video_card_info );
    
    DISPDBG( KE_DBGLVL(0), "\n\tVideo Memory: " << adapter.video_memory << "\n"
            "\tTexture Memory: " << adapter.texture_memory << "\n"
            "\tAdapter ID: " << adapter.adapter_id << "\n"
            "\tRenderer ID: " << adapter.renderer_id << "\n" );
#endif
#endif
    sstr.clear();
    
	/* TODO: Determine which fencing version to use based on vendor if needed */
}


/*
 * Name: IKeOpenGLRenderDevice::~IKeOpenGLRenderDevice
 * Desc: Default deconstructor
 */
IKeOpenGLRenderDevice::~IKeOpenGLRenderDevice()
{
    delete device_desc;
    
    /* Kill the default vertex and fragment program */
    ke_uninitialize_default_shaders();
    
    /* Destroy the immediate mode geometry buffer if it exists */
    if( im_gb )
    {
        im_gb->Destroy();
        im_gb = nullptr;
    }
    
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
 * Name: IKeOpenGLRenderDevice::confirm_device
 * Desc: Gives confirmation that this device was successfully initialized.
 */
bool IKeOpenGLRenderDevice::ConfirmDevice()
{
    return initialized;
}

/*
 * Name: IKeOpenGLRenderDevice::get_device_desc
 * Desc: Returns a copy of the device description structure
 */
void IKeOpenGLRenderDevice::GetDeviceDesc( KeRenderDeviceDesc* device_desc )
{
    memmove( device_desc, this->device_desc, sizeof( KeRenderDeviceDesc ) );
}

/*
 * Name: IKeOpenGLRenderDevice::GetDeviceCaps
 * Desc: Returns a copy of the device capabilities structure
 */
void IKeOpenGLRenderDevice::GetDeviceCaps( KeRenderDeviceCaps* device_caps )
{
    memmove( device_caps, this->device_caps, sizeof( KeRenderDeviceCaps ) );
}

/*
 * Name: IKeOpenGLRenderDevice::set_clear_colour_fv
 * Desc: Sets the clear colour
 */
void IKeOpenGLRenderDevice::SetClearColourFV( float* colour )
{
    memcpy( this->clear_colour, clear_colour, sizeof(float)*4 );
    
    glClearColor( colour[0], colour[1], colour[2], colour[3] );
}


/*
 * Name: IKeOpenGLRenderDevice::set_clear_colour_ubv
 * Desc: Same as above.
 */
void IKeOpenGLRenderDevice::SetClearColourUBV( uint8_t* colour )
{
    this->clear_colour[0] = float(colour[0]/255);
    this->clear_colour[1] = float(colour[1]/255);
    this->clear_colour[2] = float(colour[2]/255);
    this->clear_colour[3] = float(colour[3]/255);
    
    glClearColor( this->clear_colour[0], this->clear_colour[1], this->clear_colour[2], this->clear_colour[3] );
}


/*
 * Name: IKeOpenGLRenderDevice::set_clear_depth
 * Desc: 
 */
void IKeOpenGLRenderDevice::SetClearDepth( float depth )
{
    glClearDepth( depth );
}


/*
 * Name: IKeOpenGLRenderDevice::SetClearStencil
 * Desc: 
 */
void IKeOpenGLRenderDevice::SetClearStencil( uint32_t stencil )
{
    glStencilMask( stencil );
}


/*
 * Name: IKeOpenGLRenderDevice::clear_render_buffer
 * Desc: Clears only the current render buffer
 */
void IKeOpenGLRenderDevice::ClearColourBuffer()
{
    glClear( GL_COLOR_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLRenderDevice::clear_depth_buffer
 * Desc: Clears only the current depth buffer
 */
void IKeOpenGLRenderDevice::ClearDepthBuffer()
{
    glClear( GL_DEPTH_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLRenderDevice::clear_stencil_buffer
 * Desc: Clears only the current stencil buffer
 */
void IKeOpenGLRenderDevice::ClearStencilBuffer()
{
    glClear( GL_STENCIL_BUFFER_BIT );
}


/*
 * Name: IKeOpenGLRenderDevice::Clear
 * Desc: Clears all of the specified buffers
 */
void IKeOpenGLRenderDevice::Clear( uint32_t buffers )
{
	uint32_t flags = 0;

	if( buffers & KE_COLOUR_BUFFER )
		flags |= GL_COLOR_BUFFER_BIT;
	if( buffers & KE_DEPTH_BUFFER )
		flags |= GL_DEPTH_BUFFER_BIT;
	if( buffers & KE_STENCIL_BUFFER )
		flags |= GL_STENCIL_BUFFER_BIT;

	glClear( flags );
}


/*
 * Name: IKeOpenGLRenderDevice::swap
 * Desc: Swaps the double buffer.
 */
void IKeOpenGLRenderDevice::Swap()
{
    SDL_GL_SwapWindow( window );
}


/* 
 * Name: IKeOpenGLRenderDevice::SetIMCacheSize
 * Desc: Sets the size of the internally managed geometry buffer (in bytes) used for immediate
 *       mode rendering.
 * NOTE: When changing the cache size, this function checks whether the immediate mode geometry-
 *       buffer was already created.  If it exists and the size of the buffer size requested differs
 *       from the curent size, the buffer will be destroyed and recreated.  To avoid performance issues,
 *       avoid calling this function every frame.
 *
 *       This function is currently not necessary to be called on embedded platforms.  A dynamic VBO for
 *       OpenGL ES has proven to be too slow in many cases (at the time of writing) so immediate mode
 *       for OpenGL ES uses client side vertex arrays instead.
 */
void IKeOpenGLRenderDevice::SetIMCacheSize( uint32_t cache_size )
{
    /* A dummy vertex attribute; this will be changed by the user later on */
    KeVertexAttribute dummy_va[] =
    {
        { KE_VA_POSITION, 3, KE_FLOAT, No, sizeof(float)*3, 0 },
        { -1, 0, 0, 0, 0, 0 },
    };
    
    /* Does this buffer already exist? */
    if( im_gb )
    {
        /* If so, does the buffer size requested actually match the current buffer size or not? */
        if( cache_size != this->im_cache_size )
        {
            DISPDBG( KE_WARNING, "Changing IM Cache size from " << im_cache_size << " bytes to " << cache_size << " bytes..." );
        }
        
        /* Destroy this geometry buffer... */
        im_gb->Destroy();
    }
    
	im_cache_size = cache_size;

    /* And create a new one */
    bool res = CreateGeometryBuffer( NULL, cache_size, NULL, cache_size, KE_UNSIGNED_SHORT, KE_USAGE_DYNAMIC_WRITE, dummy_va, &im_gb );
    if( !res )
    {
        DISPDBG( KE_ERROR, "Error resizing IM geometry buffer!" );
    }
}

/*
 * Name: IKeOpenGLRenderDevice::create_geometry_buffer
 * Desc: Creates a geometry buffer based on the vertex and index data given.  Vertex and index
 *       buffers are encapsulated into one interface for easy management, however, index data
 *       input is completely optional.  Interleaved vertex data is also supported.
 */
bool IKeOpenGLRenderDevice::CreateGeometryBuffer( void* vertex_data, uint32_t vertex_data_size, void* index_data, uint32_t index_data_size, uint32_t index_data_type, uint32_t flags, KeVertexAttribute* va, IKeGeometryBuffer** geometry_buffer )
{
    GLenum error = glGetError();

    /* Sanity check(s) */
    if( !geometry_buffer )
        DISPDBG_RB( KE_ERROR, "Invalid interface pointer!" );
    //if( !vertex_attributes )
      //  return false;
    if( !vertex_data_size )
        DISPDBG_RB( KE_ERROR, "(vertex_data_size == 0) condition is currently not allowed..." );   /* Temporary? */
 
    *geometry_buffer = new IKeOpenGLGeometryBuffer;
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( *geometry_buffer );
    
	/* Enumerate buffer usage flags */

    /* Create a vertex array object */
    glGenVertexArrays( 1, &gb->vao );
    OGL_DISPDBG_RB( KE_ERROR, "Error creating vertex array object!" ); //error = glGetError();
    
    /* Bind this vertex array object */
    glBindVertexArray( gb->vao );
    
    /* Create the vertex buffer object */
    glGenBuffers( 1, &gb->vbo[0] );
    OGL_DISPDBG_RB( KE_ERROR, "Error generating vertex buffer object!" );
    
    /* Set the vertex buffer data */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, vertex_data_size, vertex_data, buffer_usage_types[flags] );
    OGL_DISPDBG_RB( KE_ERROR, "Error setting vertex buffer data!" );
     
	/* Set the vertex attributes for this geometry buffer */
    KeSetVertexAttributes(va);

	/* Save vertex data size and vertex stride */
	gb->vd_length = vertex_data_size;
	gb->vertex_size = va[0].stride;

    /* Create an index buffer if desired */
    if( index_data_size )
    {
        glGenBuffers( 1, &gb->vbo[1] );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        OGL_DISPDBG_RB( KE_ERROR, "Error creating index buffer object!" );
        
        /* Set the index buffer data */
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, buffer_usage_types[flags] );
        gb->index_type = index_data_type;
		OGL_DISPDBG_RB( KE_ERROR, "Error setting index buffer data!" );

		/* Save index data size */
		gb->id_length = index_data_size;
    }
    else
        gb->vbo[1] = 0;
    
    /* Unbind this vertex array object */
    glBindVertexArray(0);
    
    return true;
}


/*
 * Name: IKeOpenGLRenderDevice::delete_geometry_buffer
 * Desc:
 */
void IKeOpenGLRenderDevice::DeleteGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( geometry_buffer );
    
    /* Delete the VBO and VAO */
    glDeleteBuffers( 2, gb->vbo );
    glDeleteVertexArrays( 1, &gb->vao );
    
    delete geometry_buffer;
}

/*
 * Name: IKeOpenGLRenderDevice::set_vertex_buffer
 * Desc: Sets the current geometry buffer to be used when rendering. Internally, binds the
 *       vertex array object. If NULL, then sets the current vertex array object to 0.
 */
void IKeOpenGLRenderDevice::SetGeometryBuffer( IKeGeometryBuffer* geometry_buffer )
{
    current_geometrybuffer = geometry_buffer;
    
    if( geometry_buffer )
        glBindVertexArray( static_cast<IKeOpenGLGeometryBuffer*>( geometry_buffer )->vao );
    else
        glBindVertexArray(0);
}

/*
 * Name: IKeOpenGLRenderDevice::create_program
 * Desc: Creates a complete OpenGL program out of shaders in text form. The minimum requirements
 *       are one valid vertex and fragment shader, while geometry and tesselation shaders are
 *       optional.  Obviously, tesselation shaders require OpenGL 4.1+, and cannot be used with
 *       OpenGL 3.2.  This function will automatically search for specific attribute locations
 *       before linking it and search for pre-determined uniform names for textures and matrices
 *       (see code below).
 *       TODO: Allow user defined constants.
 */
bool IKeOpenGLRenderDevice::CreateProgram( const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tesselation_shader, KeVertexAttribute* vertex_attributes, IKeGpuProgram** gpu_program )
{
    GLuint p, f, v, t = 0, g;
    *gpu_program = new IKeOpenGLGpuProgram;
    IKeOpenGLGpuProgram* gp = static_cast<IKeOpenGLGpuProgram*>( *gpu_program );
    GLenum error = glGetError();
	int Fail = No;
    
    /* Nullify vertex attribute list */
    gp->va = NULL;
    
	v = glCreateShader( GL_VERTEX_SHADER );
	f = glCreateShader( GL_FRAGMENT_SHADER );
    
#ifndef __MOBILE_OS__
    g = glCreateShader( GL_GEOMETRY_SHADER );
#endif
    
	glShaderSource( v, 1, &vertex_shader, NULL );
	glShaderSource( f, 1, &fragment_shader, NULL );
    
	GLint compiled;
    
	glCompileShader(v);
	glGetShaderiv( v, GL_COMPILE_STATUS, &compiled );
	if( !compiled )
	{
        char str[2048];
        int len = 0;
        
        glGetShaderInfoLog( v, 2048, &len, str );
		DISPDBG( KE_ERROR, "Vertex shader not compiled.\n" << str );
		Fail = Yes;
	}
    
	glCompileShader(f);
	glGetShaderiv( f, GL_COMPILE_STATUS, &compiled );
	if( !compiled )
	{
        char str[2048];
        int len = 0;
        
        glGetShaderInfoLog( f, 2048, &len, str );
		DISPDBG( KE_ERROR, "Fragment shader not compiled.\n" << str );
		Fail = Yes;
	}
    
	if( Fail )
	{
		glDeleteShader(v);
		glDeleteShader(f);
		glDeleteShader(g);
		gp->Destroy();
		gp = NULL;

		DISPDBG_RB( KE_ERROR, "An error occured building this GPU program!" );
	}

	p = glCreateProgram();
    
    /*glBindAttribLocation( p, 0, "in_pos" );
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
    glBindAttribLocation( p, 12, "in_tex7" );*/
    
    /* Bind all attributes found within the list of vertex attributes */
    int index = 0;
    while( vertex_attributes[index].index != -1 )
    {
        glBindAttribLocation
        (
         p, program_attributes[vertex_attributes[index].index].location,
         program_attributes[vertex_attributes[index].index].name
        );
        
        OGL_DISPDBG( KE_WARNING, "Could not bind attribute! (location: " <<
                    program_attributes[vertex_attributes[index].index].location <<
                    ", name: " << program_attributes[vertex_attributes[index].index].name << ")" );
        index++;
    }
    
	glAttachShader( p, v );
	glAttachShader( p, f );
    
	GLint status = 0;
	glLinkProgram(p);
	glGetProgramiv( p, GL_LINK_STATUS, &status );
	if (status == GL_FALSE)
	{
		char str[2048];
		int len = 0;

		glGetProgramInfoLog( p, 2048, &len, str );
		DISPDBG( KE_ERROR, "Error linking program.\n" << str << "\n" );
	}

	glValidateProgram( p );
	glGetProgramiv( p, GL_VALIDATE_STATUS, &status );
	if (status == GL_FALSE)
	{
		char str[2048];
		int len = 0;

		glGetProgramInfoLog( p, 2048, &len, str );
		DISPDBG( KE_ERROR, "Error validating program.\n" << str << "\n" );
	}
    
	glUseProgram(p);
    
    glDeleteShader(v);
    glDeleteShader(f);
#ifndef __MOBILE_OS__
    glDeleteShader(g);
#endif
    
    GLuint uniform_tex0 = glGetUniformLocation( p, "tex0" );
    GLuint uniform_tex1 = glGetUniformLocation( p, "tex1" );
    GLuint uniform_tex2 = glGetUniformLocation( p, "tex2" );
    GLuint uniform_tex3 = glGetUniformLocation( p, "tex3" );
	GLuint uniform_tex4 = glGetUniformLocation( p, "tex4" );
    GLuint uniform_tex5 = glGetUniformLocation( p, "tex5" );
    GLuint uniform_tex6 = glGetUniformLocation( p, "tex6" );
    GLuint uniform_tex7 = glGetUniformLocation( p, "tex7" );
    
    error = glGetError();
    
    gp->matrices[0] = glGetUniformLocation( p, "world" );
	OGL_DISPDBG( KE_WARNING, "Could not find the world matrix uniform location..." );
    gp->matrices[1] = glGetUniformLocation( p, "view" );
	OGL_DISPDBG( KE_WARNING, "Could not find the view matrix uniform location..." );
    gp->matrices[2] = glGetUniformLocation( p, "proj" );
	OGL_DISPDBG( KE_WARNING, "Could not find the projection matrix uniform location..." );
    
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
 * Name: IKeOpenGLRenderDevice::delete_program
 * Desc: Deletes the GPU program.
 */
void IKeOpenGLRenderDevice::DeleteProgram( IKeGpuProgram* gpu_program )
{
    /* Deletes the GPU program */
    if( gpu_program )
    {
        glDeleteProgram( static_cast<IKeOpenGLGpuProgram*>(gpu_program)->program );
		//delete[] static_cast<IKeOpenGLGpuProgram*>(gpu_program)->va;
        delete gpu_program;
    }
}

/*
 * Name: IKeOpenGLRenderDevice::set_program
 * Desc: Sets the GPU program.  If NULL, the GPU program is set to 0.
 */
void IKeOpenGLRenderDevice::SetProgram( IKeGpuProgram* gpu_program )
{
	GLenum error = glGetError();
    
    /* Save a copy of this program */
    current_gpu_program = gpu_program;

    /* Check for a valid pointer. If NULL, then we set the current program to 0. */
    if( gpu_program )
    {
        IKeOpenGLGpuProgram* gp = static_cast<IKeOpenGLGpuProgram*>(gpu_program);
        
        glUseProgram( gp->program );
		OGL_DISPDBG_R( KE_ERROR, "Invalid GPU program!" );
    }
    else
        glUseProgram(0);
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant_1fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant1FV( const char* location, int count, float* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform1fv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant_2fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant2FV( const char* location, int count, float* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform2fv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant_3fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant3FV( const char* location, int count, float* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform3fv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant_4fv
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant4FV( const char* location, int count, float* value )
{
	GLenum error = glGetError();
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    
    int loc = glGetUniformLocation( p->program, location );
	OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform4fv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant1IV( const char* location, int count, int* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform1iv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant2IV( const char* location, int count, int* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform2iv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant3IV( const char* location, int count, int* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform3iv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::set_program_constant
 * Desc: Sets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::SetProgramConstant4IV( const char* location, int count, int* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glUniform4iv( loc, count, value );
}

/*
 * Name: IKeOpenGLRenderDevice::get_program_constant_fv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::GetProgramConstantFV( const char* location, float* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glGetUniformfv( p->program, loc, value );
}

/*
 * Name: IKeOpenGLRenderDevice::get_program_constant_iv
 * Desc: Gets program constants (do your research on GLSL uniforms)
 */
void IKeOpenGLRenderDevice::GetProgramConstantIV( const char* location, int* value )
{
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    GLenum error = glGetError();
    
    int loc = glGetUniformLocation( p->program, location );
    OGL_DISPDBG( KE_ERROR, "Error getting uniform location for \"" << location << "\"" << std::endl );
    glGetUniformiv( p->program, loc, value );
}

/*
 * Name: IKeOpenGLRenderDevice::create_constant_buffer
 * Desc: Creates a constant buffer and initializes it with the chosen contents.
 * NOTE: Constant buffers use Unified Buffer Objects (UBOs) and are only supported by core OpenGL 3.1+
 *       and OpenGL ES 3.0+.
 */
bool IKeOpenGLRenderDevice::CreateConstantBuffer( KeConstantBufferDesc* desc, IKeConstantBuffer** constant_buffer, void* data )
{
    GLenum error = glGetError();
    
    /* Sanity check */
    if( !desc )
        return false;
    
    /* Allocate a new constant buffer interface */
    *constant_buffer = new IKeOpenGLConstantBuffer;
    IKeOpenGLConstantBuffer* cb = static_cast<IKeOpenGLConstantBuffer*>( *constant_buffer );
    
    /* Generate a new UBO */
    glGenBuffers( 1, &cb->ubo );
    OGL_DISPDBG_RB( KE_ERROR, "Error generating new UBO!" );
    
    /* Bind the UBO and setup the initial contents */
    glBindBuffer( GL_UNIFORM_BUFFER, cb->ubo );
    glBufferData( GL_UNIFORM_BUFFER, desc->data_size, data, buffer_usage_types[desc->flags] );
    OGL_DISPDBG( KE_ERROR, "Error setting UBO data!" );
    
    /* Unbind the UBO */
    glBindBuffer( GL_UNIFORM_BUFFER, 0 );
    
    /* Copy important attributes */
    memcpy( cb->block_name, desc->block_name, sizeof( char ) * 64 );
    cb->data_size = desc->data_size;
    cb->flags = desc->flags;
    
    return true;
}

/*
 * Name: IKeOpenGLRenderDevice::delete_constant_buffer
 * Desc: 
 */
void IKeOpenGLRenderDevice::DeleteConstantBuffer( IKeConstantBuffer* constant_buffer )
{

}

/*
 * Name: IKeOpenGLRenderDevice::set_constant_buffer_data
 * Desc: 
 */
bool IKeOpenGLRenderDevice::SetConstantBufferData( void* data, IKeConstantBuffer* constant_buffer )
{
	return true;
}

/*
 * Name: IKeOpenGLRenderDevice::SetConstantBuffer
 * Desc: Sets the constant buffer into the desired binding index if available.
 * NOTE: For OpenGL, you are limited to the number of available uniform buffer bindings.
 */
void IKeOpenGLRenderDevice::SetConstantBuffer( int slot, int shader_type, IKeConstantBuffer* constant_buffer )
{
    GLenum error = glGetError();
    
    IKeOpenGLConstantBuffer* cb = static_cast<IKeOpenGLConstantBuffer*>( constant_buffer );
    IKeOpenGLGpuProgram* p = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    
    if( !current_gpu_program )
        DISPDBG_R( KE_ERROR, "A valid GPU program must be set before setting a constant buffer!" );
    
    /* Locate the block index so we can bind the UBO to the desired index */
    uint32_t block_index = glGetUniformBlockIndex( p->program, cb->block_name );
    OGL_DISPDBG_R( KE_ERROR, "Error getting uniform block index (" << cb->block_name << ")!" );
    
    /* Bind the UBO to an index */
    glBindBufferBase( GL_UNIFORM_BUFFER, slot, cb->ubo );
    OGL_DISPDBG_R( KE_ERROR, "Error binding UBO to index #" << slot << "!" );
    
    /* Bind the uniform block */
    glUniformBlockBinding( p->program, block_index, slot );
}


/*
 * Name: IKeOpenGLRenderDevice::create_texture_1d
 * Desc: Creates a 1D texture.
 */
bool IKeOpenGLRenderDevice::CreateTexture1D( uint32_t target, int width, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
#ifndef __MOBILE_OS__
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    *texture = new IKeOpenGLTexture;
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->mipmap = mipmaps;
    t->target = texture_targets[target];
    t->data_type = data_types[data_type];
    t->depth_format = texture_formats[format];
    t->internal_format = internal_texture_formats[format];
    
    /* Use OpenGL to create a new 1D texture */
    glGenTextures( 1, &t->handle );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating texture!" );
    glBindTexture( t->target, t->handle );
    OGL_DISPDBG_RB( KE_ERROR, "Error binding texture!" );
    
    /* Set the initial texture attributes */
    glTexImage1D( t->target, 0, internal_texture_formats[format], width, mipmaps, texture_formats[format], data_types[data_type], pixels );
    OGL_DISPDBG_RB( KE_ERROR, "Error initializing texture attributes!" );
    
    /* Set texture parameters */
    glTexParameteri( t->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( t->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
    return true;
#else
    DISPDBG_RB( KE_ERROR, "1D textures are not supported for OpenGL ES!" );
#endif
}

/*
 * Name: IKeOpenGLRenderDevice::create_texture_2d
 * Desc: Creates a blank 2D texture.
 */
bool IKeOpenGLRenderDevice::CreateTexture2D( uint32_t target, int width, int height, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    (*texture) = new IKeOpenGLTexture;
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->height = height;
    t->mipmap = mipmaps;
    t->target = texture_targets[target];
    t->data_type = data_types[data_type];
    t->depth_format = texture_formats[format];
    t->internal_format = internal_texture_formats[format];
    
    /* Use OpenGL to create a new 2D texture */
    glGenTextures( 1, &t->handle );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating texture!" );
    glBindTexture( t->target, t->handle );
    OGL_DISPDBG_RB( KE_ERROR, "Error binding texture!" );
    
    /* Set the initial texture attributes */
    glTexImage2D( t->target, 0, internal_texture_formats[format], width, height, mipmaps, texture_formats[format], data_types[data_type], pixels );
    //glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, 1024, 1024, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels );
    OGL_DISPDBG_RB( KE_ERROR, "Error initializing texture attributes!" );
    
    /* Set texture parameters */
    glTexParameteri( t->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( t->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
#ifdef GL_ES_VERSION_2_0
    /* For OpenGL ES, textures need to have GL_CLAMP_TO_EDGE set if they are not power of two. 
       For now, we'll set it automatically, then output a warning that this was done for 
       compatibility. The developer can change it later if necessary. */
    
    if( !KeIsPowerOfTwo( width, height, 0 ) )
    {
        glTexParameteri( t->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( t->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        
        DISPDBG( KE_WARNING, "Non-pow2 texture detected; Setting texture wrap mode to GL_CLAMP_TO_EDGE by default." );
    }
#endif
    
	/* Unbind texture */
	glBindTexture( t->target, 0 );

    return true;
}

/*
 * Name: IKeOpenGLRenderDevice::create_texture_3d
 * Desc: Creates a blank 3D texture.
 * NOTE: Only suppotted for core and ES 3.0+
 */
bool IKeOpenGLRenderDevice::CreateTexture3D( uint32_t target, int width, int height, int depth, int mipmaps, uint32_t format, uint32_t data_type, IKeTexture** texture, void* pixels )
{
    GLenum error = glGetError();
    
    /* Allocate a new texture */
    (*texture) = new IKeOpenGLTexture;
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( *texture );
    
    /* Set texture attributes */
    t->width = width;
    t->height = height;
    t->depth = depth;
    t->mipmap = mipmaps;
    t->target = texture_targets[target];
    t->data_type = data_types[data_type];
    t->depth_format = texture_formats[format];
    t->internal_format = internal_texture_formats[format];
    
    /* Use OpenGL to create a new 3D texture */
    glGenTextures( 1, &t->handle );
	OGL_DISPDBG_RB( KE_ERROR, "Error generating texture!" );
    glBindTexture( t->target, t->handle );
    OGL_DISPDBG_RB( KE_ERROR, "Error binding texture!" );
    
    /* Set the initial texture attributes */
    glTexImage3D( t->target, 0, internal_texture_formats[format], width, height, depth, mipmaps, texture_formats[format], data_types[data_type], pixels );
    OGL_DISPDBG_RB( KE_ERROR, "Error initializing texture attributes!" );
    
    /* Set texture parameters */
    glTexParameteri( t->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( t->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
#ifdef GL_ES_VERSION_2_0
    /* For OpenGL ES, textures need to have GL_CLAMP_TO_EDGE set if they are not power of two.
     For now, we'll set it automatically, then output a warning that this was done for
     compatibility. The developer can change it later if necessary. */
    
    if( !KeIsPowerOfTwo( width, height, depth ) )
    {
        glTexParameteri( t->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( t->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( t->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        
        DISPDBG( KE_WARNING, "Non-pow2 texture detected; Setting texture wrap mode to GL_CLAMP_TO_EDGE by default." );
    }
#endif
    
    return true;
}

/*
 * Name: IKeOpenGLRenderDevice::delete_texture
 * Desc: Deletes a texture from memory.
 */
void IKeOpenGLRenderDevice::DeleteTexture( IKeTexture* texture )
{
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( texture );
    
    if( texture )
    {
        glDeleteTextures( 1, &t->handle );
        delete texture;
    }
}


/*
 * Name: IKeOpenGLRenderDevice::set_texture_data_1d
 * Desc: Sets pixel data for a 1D texture.
 */
void IKeOpenGLRenderDevice::SetTextureData1D( int offsetx, int width, int miplevel, void* pixels, IKeTexture* texture )
{
#ifndef __MOBILE_OS__
    GLenum error = glGetError();
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( texture );

    glBindTexture( t->target, t->handle );
    glTexSubImage1D( t->target, miplevel, offsetx, width, t->internal_format, t->data_type, pixels );
    OGL_DISPDBG( KE_ERROR, "Error setting texture data!" );
    glBindTexture( t->target, 0 );
#else
    DISPDBG( KE_ERROR, "1D textures are not supported for OpenGL ES!" );
#endif
}

/*
 * Name: IKeOpenGLRenderDevice::set_texture_data_2d
 * Desc: Sets pixel data for a 2D texture.
 */
void IKeOpenGLRenderDevice::SetTextureData2D( int offsetx, int offsety, int width, int height, int miplevel, void* pixels, IKeTexture* texture )
{
	GLenum error = glGetError();
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( texture );
    
	//glEnable( t->target );
	glBindTexture( t->target, t->handle );
    glTexSubImage2D( t->target, miplevel, offsetx, offsety, width, height, t->depth_format, t->data_type, pixels );
	OGL_DISPDBG( KE_ERROR, "Error setting texture data!" );
	glBindTexture( t->target, 0 );
}

/*
 * Name: IKeOpenGLRenderDevice::set_texture_data_3d
 * Desc: Sets pixel data for a 3D texture.
 */
void IKeOpenGLRenderDevice::SetTextureData3D( int offsetx, int offsety, int offsetz, int width, int height, int depth, int miplevel, void* pixels, IKeTexture* texture )
{
    GLenum error = glGetError();
    IKeOpenGLTexture* t = static_cast<IKeOpenGLTexture*>( texture );
    
    glBindTexture( t->target, t->handle );
    glTexSubImage3D( t->target, miplevel, offsetx, offsety, offsetz, width, height, depth, t->internal_format, t->data_type, pixels );
    OGL_DISPDBG( KE_ERROR, "Error setting texture data!" );
    glBindTexture( t->target, 0 );
}

/*
 * Name: IKeOpenGLRenderDevice::create_render_target
 * Desc: Creates a seperate render target (FBO), typically used for rendering to a texture.
 *       Creates a colour, depth and stencil buffer (if desired) and can be set as a texture.
 */
bool IKeOpenGLRenderDevice::CreateRenderTarget( int width, int height, int depth, uint32_t flags, IKeRenderTarget** rendertarget )
{
    GLenum error = glGetError();
	(*rendertarget) = new IKeOpenGLRenderTarget;
    IKeOpenGLRenderTarget* rt = static_cast<IKeOpenGLRenderTarget*>( *rendertarget );
    
#if 1
	/* Generate a texture as our colour buffer */
	if( !this->CreateTexture2D( KE_TEXTURE_2D, width, height, 0, KE_TEXTUREFORMAT_RGBA, KE_UNSIGNED_BYTE, &rt->texture ) )
	{
		DISPDBG_RB( KE_ERROR, "Error creating render target texture!" );
	}

	/* Get OpenGL texture handle */
	uint32_t texture_handle = static_cast<IKeOpenGLTexture*>( rt->texture )->handle;

	/* Depth buffer */
	glGenRenderbuffers( 1, &rt->depth_buffer );
	glBindRenderbuffer( GL_RENDERBUFFER, rt->depth_buffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	/* Framebuffer to link everything together */
	glGenFramebuffers( 1, &rt->fbo);
	glBindFramebuffer( GL_FRAMEBUFFER, rt->fbo );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_handle, 0 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rt->depth_buffer );
	
	/* Did it work? */
	GLenum status;
	if( ( status = glCheckFramebufferStatus( GL_FRAMEBUFFER ) ) != GL_FRAMEBUFFER_COMPLETE )
	{
		OGL_DISPDBG( KE_ERROR, "Error linking framebuffer (status: " << status << ")" );
		return 0;
	}

	/* Unbind the fbo */
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
#else
    /* Generate frame buffer object */
    glGenFramebuffers( 1, &rt->frame_buffer_object );
    OGL_DISPDBG_RB( 1, "Error creating FBO!" );
    
    /* Bind the FBO */
    glBindFramebuffer( GL_FRAMEBUFFER, rt->frame_buffer_object );
    
    /* Create a texture to render this FBO to */
    this->CreateTexture2D( KE_TEXTURE_2D, width, height, 0, KE_TEXTUREFORMAT_RGBA, KE_UNSIGNED_BYTE, &rt->texture );
    
    /* Use nearest point filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    /* Create the depth buffer */
    glGenRenderbuffers( 1, &rt->depth_render_buffer );
    OGL_DISPDBG_RB( 1, "Error creating depth buffer!" );
    
    /* Set the depth buffer attributes */
    IKeOpenGLTexture* tex = static_cast<IKeOpenGLTexture*>( rt->texture );
    
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
#endif
    
    return true;
}

/*
 * Name: IKeOpenGLRenderDevice::delete_render_target
 * Desc: Deletes the render target resources used.
 */
void IKeOpenGLRenderDevice::DeleteRenderTarget( IKeRenderTarget* rendertarget )
{
    IKeOpenGLRenderTarget* rt = static_cast<IKeOpenGLRenderTarget*>( rendertarget );
    
    /* Delete the texture */
    this->DeleteTexture( static_cast<IKeOpenGLTexture*>( rt->texture ) );
    
    /* Delete the render target */
    glDeleteRenderbuffers( 1, &rt->depth_buffer );
    glDeleteFramebuffers( 1, &rt->fbo );

	delete rendertarget;
}

/*
 * Name: IKeOpenGLRenderDevice::bind_render_target
 * Desc: Binds the render target to OpenGL.  You set the texture to the appropriate  texture
 *       stage yourself using ::set_texture().
 */
void IKeOpenGLRenderDevice::BindRenderTarget( IKeRenderTarget* rendertarget )
{
    GLenum error = glGetError();
    IKeOpenGLRenderTarget* rt = static_cast<IKeOpenGLRenderTarget*>( rendertarget );
    
    /* Bind the FBO */
	if( rt )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, rt->fbo );
		error = glGetError();
		if( error != GL_NO_ERROR )
			DISPDBG( 1, "Error binding rendertarget! (error=0x" << error << ")\n" );
	}
	else
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

/*
 * Name: IKeOpenGLRenderDevice::set_texture
 * Desc: Sets a texture to the desired texture stage.  If NULL, then texturing is disabled on
 *       the selected texture stage.
 */
void IKeOpenGLRenderDevice::SetTexture( int stage, IKeTexture* texture )
{
    IKeOpenGLTexture* tex = static_cast<IKeOpenGLTexture*>(texture);
    
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
        /*glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_3D );
        glDisable( GL_TEXTURE_1D_ARRAY );
        glDisable( GL_TEXTURE_2D_ARRAY );
        glDisable( GL_TEXTURE_RECTANGLE );*/
        
#ifndef __MOBILE_OS__
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_1D_ARRAY, 0 );
        glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
#endif
        glBindTexture( GL_TEXTURE_2D, 0 );
        glBindTexture( GL_TEXTURE_3D, 0 );
        glBindTexture( GL_TEXTURE_2D_ARRAY, 0 );
    }
}


/*
 * Name: IKeOpenGLRenderDevice::CreateRenderStateBuffer
 * Desc: Creates a compiled buffer of render and texture states.  At the time of writing, OpenGL
 *       does not have an equivalent to Direct3D's state buffer, we have to create a faux state buffer
 *       and save/set each state individually.
 */
bool IKeOpenGLRenderDevice::CreateRenderStateBuffer( KeState* state_params, int state_count, IKeRenderStateBuffer** state_buffer )
{
    /* Create a new state buffer interface */
    (*state_buffer) = new IKeOpenGLRenderStateBuffer;
    IKeOpenGLRenderStateBuffer* sb = static_cast<IKeOpenGLRenderStateBuffer*>( *state_buffer );
    
    /* Allocate faux state buffer */
    sb->states = new KeState[state_count];
    
    /* Copy the supplied state parameters over */
    memmove( sb->states, state_params, sizeof( KeState ) * state_count );
    sb->state_count = state_count;
    
    return true;
}

/*
 * Name: IKeOpenGLRenderDevice::CreateTextureSamplerBuffer
 * Desc: Creates a compiled buffer of texture states.  At the time of writing, OpenGL
 *       does not have an equivalent to Direct3D's state buffer, we have to create a faux state buffer
 *       and save/set each state individually.
 */
bool IKeOpenGLRenderDevice::CreateTextureSamplerBuffer( KeState* state_params, int state_count, IKeTextureSamplerBuffer** state_buffer )
{
	/* Create a new state buffer interface */
	(*state_buffer) = new IKeOpenGLTextureSamplerBuffer;
	IKeOpenGLTextureSamplerBuffer* sb = static_cast<IKeOpenGLTextureSamplerBuffer*>(*state_buffer);

	/* Allocate faux state buffer */
	sb->states = new KeState[state_count];

	/* Copy the supplied state parameters over */
	memmove( sb->states, state_params, sizeof( KeState ) * state_count );
	sb->state_count = state_count;

	return true;
}

bool IKeOpenGLRenderDevice::SetRenderStateBuffer( IKeRenderStateBuffer* state_buffer )
{
    int i = 0;
    
    /* Sanity check */
    if( !state_buffer )
        return false;
    
    IKeOpenGLRenderStateBuffer* sb = static_cast<IKeOpenGLRenderStateBuffer*>(state_buffer);
    
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
#ifndef __MOBILE_OS__
                glPolygonMode( polygon_modes[sb->states[i].param1], fill_modes[sb->states[i].param2] );
#endif
                break;
                
            case KE_RS_BLENDFUNC:
                glBlendFunc( blend_modes[sb->states[i].param1], blend_modes[sb->states[i].param2] );
                break;
                
            case KE_RS_CULLMODE:
                if( sb->states[i].param1 )
                {
                    glEnable( GL_CULL_FACE );
                    glCullFace( cull_modes[sb->states[i].param2] );
                }
                else
                    glDisable( GL_CULL_FACE );
                break;
                
            default:
                DISPDBG( KE_WARNING, "Bad render state!\nstate: " << sb->states[i].state << "\n"
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


bool IKeOpenGLRenderDevice::SetTextureSamplerBuffer( int stage, IKeTextureSamplerBuffer* state_buffer )
{
	int i = 0;

	/* Sanity check */
	if (!state_buffer)
		return false;

	IKeOpenGLTextureSamplerBuffer* sb = static_cast<IKeOpenGLTextureSamplerBuffer*>(state_buffer);

#if 0
	/* Apply each render state in the list */
	while (i != sb->state_count)
	{
		switch (sb->states[i].state)
		{
		case KE_TS_MAGFILTER:
			glTexParameteri( texture_targets[sb->states[i].param1], GL_TEXTURE_MAG_FILTER, texture_filter_modes[sb->states[i].param2] );
			break;

		case KE_TS_MINFILTER:
			glTexParameteri( texture_targets[sb->states[i].param1], GL_TEXTURE_MIN_FILTER, texture_filter_modes[sb->states[i].param2] );
			break;

		default:
			DISPDBG( KE_WARNING, "Bad texture state!\nstate: " << sb->states[i].state << "\n"
				"param1: " << sb->states[i].param1 << "\n"
				"param2: " << sb->states[i].param2 << "\n"
				"param3: " << sb->states[i].param3 << "\n"
				"fparam: " << sb->states[i].fparam << "\n"
				"dparam: " << sb->states[i].dparam << "\n" );
			break;
		}

		i++;
	}
#else
	KeState empty = { static_cast<uint32_t>(-1), 0, 0, 0, 0, 0 };

	/* Copy this sampler state to the texture unit slot */
	memcpy( &samplers[stage], sb->states, sizeof( KeState ) * sb->state_count );
	memcpy( &samplers[stage][sb->state_count], &empty, sizeof( KeState ) );
#endif

	return true;
}

/*
 * Name: IKeOpenGLRenderDevice::set_render_states
 * Desc: Applies a list of user defined render states.
 * TODO: Allow explicit deferring of render states?
 */
void IKeOpenGLRenderDevice::SetRenderStates( KeState* states )
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
#ifndef __MOBILE_OS__
                glPolygonMode( polygon_modes[states[i].param1], fill_modes[states[i].param2] );
#endif
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
 * Name: IKeOpenGLRenderDevice::set_sampler_states
 * Desc: Applies a list of user defined sampler states.
 * TODO: Allow explicit deferring of sampler states?
 */
void IKeOpenGLRenderDevice::SetSamplerStates( int stage, KeState* states )
{
    int i = 0;
	int previous_texture_unit;

#if 0
	/* Save the previous active texture unit before changing it */
	/* TODO: Is this necessary? */
	glGetIntegerv( GL_ACTIVE_TEXTURE, &previous_texture_unit );
	glActiveTexture( GL_TEXTURE0 + state );
    
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

		i++;
    }

	/* Restore the previous texture unit */
	glGetIntegerv( GL_ACTIVE_TEXTURE, &previous_texture_unit );
#else
	while( states[i].state != -1 )
	{
		memmove( &samplers[stage][i], &states[i], sizeof( KeState ) );
		i++;
	}
	memmove( &samplers[stage][i], &states[i], sizeof( KeState ) );
#endif
}

/*
 * Name: IKeOpenGLRenderDevice::DrawVerticesIM
 * Desc: Draws raw vertices without the need for a geometry buffer.
 */
void IKeOpenGLRenderDevice::DrawVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int first, int count, void* vertex_data )
{
    IKeOpenGLGpuProgram* gp = static_cast<IKeOpenGLGpuProgram*>( current_gpu_program );
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
    
    /* If the IM geometry buffer was never even created, then exit now */
    if( !im_gb )
    {
        DISPDBG_R( KE_ERROR, "The IM Geometry Buffer was not created! Call IKeRenderDevice::SetIMCacheSize first!" );
    }
    
    /* Unbind any VBO or IBO bound */
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
#ifndef __MOBILE_OS__
    /* Set the VAO of the IM geometry buffer now */
    glBindVertexArray( static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vao );
    OGL_DISPDBG( KE_ERROR, "Error binding the IM vertex array object!" );
#else
    /* For OpenGL ES, let's unbind the VAO */
    glBindVertexArray(0);
#endif
    
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
#ifndef __MOBILE_OS__
    /* Now bind the VBO that is designated for immediate mode rendering */
    glBindBuffer( GL_ARRAY_BUFFER, static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vbo[0] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding IM vertex buffer!" );
#endif
    
#ifndef __MOBILE_OS__
    /* Set the vertex attributes for this geometry buffer now that the VAO and VBO are in place */
    KeSetVertexAttributes( vertex_attributes );
    
    /* Set the vertex data into the immediate mode geometry buffer */
    im_gb->SetVertexData( 0, stride*count, vertex_data );
#else
    /* Set vertex data and vertex attributes without a dynamic VBO for OpenGL ES,
       this has been more efficient */
    KeSetVertexAttributesIM( vertex_attributes, (uint8_t*) vertex_data );
#endif
    
    /* Draw the vertices */
    glDrawArrays( primitive_types[primtype], first, count );
    OGL_DISPDBG_R( KE_ERROR, "IM Vertex array rendering error (glDrawArrays)!" );
    
    /* Re-bind the previous geometry buffer if it existed */
    if( gb )
    {
        glBindVertexArray( gb->vao );
        OGL_DISPDBG( KE_ERROR, "Error re-binding vertex array object!" );
        glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding vertex buffer!" );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding index buffer!" );
    }
}


/*
 * Name: IKeOpenGLRenderDevice::DrawIndexedVerticesIM
 * Desc: Draws raw indexed vertices without the need for a geometry buffer.
 */
void IKeOpenGLRenderDevice::DrawIndexedVerticesIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int count, void* vertex_data, void* index_data )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
    
    /* If the IM geometry buffer was never even created, then exit now */
    if( !im_gb )
    {
        DISPDBG_R( KE_ERROR, "The IM Geometry Buffer was not created! Call IKeRenderDevice::SetIMCacheSize first!" );
    }
    
    /* Unbind any VBO or IBO bound */
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    /* Set the VAO of the IM geometry buffer now */
    glBindVertexArray( static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vao );
    OGL_DISPDBG( KE_ERROR, "Error binding the IM vertex array object!" );
    
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
    /* Now bind the VBO and IBO that is designated for immediate mode rendering */
    glBindBuffer( GL_ARRAY_BUFFER, static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vbo[0] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding IM vertex buffer!" );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding IM index buffer!" );
    
    /* Set the vertex attributes for this geometry buffer now that the VAO and VBO are in place */
    KeSetVertexAttributes( vertex_attributes );
    
    /* Set the vertex data into the immediate mode geometry buffer */
    im_gb->SetVertexData( 0, stride*count, vertex_data );
    im_gb->SetIndexData( 0, count*sizeof(uint16_t), index_data );
    
    /* Draw the vertices */
    glDrawElements( primitive_types[primtype], count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "IM Indexed geometry rendering error (glDrawElements)!" );
    
    /* Re-bind the previous geometry buffer if it existed */
    if( gb )
    {
        glBindVertexArray( gb->vao );
        OGL_DISPDBG( KE_ERROR, "Error re-binding vertex array object!" );
        glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding vertex buffer!" );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding index buffer!" );
    }
}

/*
 * Name: IKeOpenGLRenderDevice::DrawIndexedVerticesRangeIM
 * Desc: Draws raw indexed vertices without the need for a geometry buffer.
 */
void IKeOpenGLRenderDevice::DrawIndexedVerticesRangeIM( uint32_t primtype, uint32_t stride, KeVertexAttribute* vertex_attributes, int start, int end, int count, void* vertex_data, void* index_data )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
    
    /* If the IM geometry buffer was never even created, then exit now */
    if( !im_gb )
    {
        DISPDBG_R( KE_ERROR, "The IM Geometry Buffer was not created! Call IKeRenderDevice::SetIMCacheSize first!" );
    }
    
    /* Unbind any VBO or IBO bound */
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    /* Set the VAO of the IM geometry buffer now */
    glBindVertexArray( static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vao );
    OGL_DISPDBG( KE_ERROR, "Error binding the IM vertex array object!" );
    
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
    /* Now bind the VBO and IBO that is designated for immediate mode rendering */
    glBindBuffer( GL_ARRAY_BUFFER, static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vbo[0] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding IM vertex buffer!" );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, static_cast<IKeOpenGLGeometryBuffer*>(im_gb)->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding IM index buffer!" );
    
    /* Set the vertex attributes for this geometry buffer now that the VAO and VBO are in place */
    KeSetVertexAttributes( vertex_attributes );
    
    /* Set the vertex data into the immediate mode geometry buffer */
    im_gb->SetVertexData( 0, stride*count, vertex_data );
    im_gb->SetIndexData( 0, count*sizeof(uint16_t), index_data );
    
    /* Draw the vertices */
    glDrawRangeElements( primitive_types[primtype], start, end, count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "Indexed geometry rendering error (glDrawRangeElements)!" );
    
    /* Re-bind the previous geometry buffer if it existed */
    if( gb )
    {
        glBindVertexArray( gb->vao );
        OGL_DISPDBG( KE_ERROR, "Error re-binding vertex array object!" );
        glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding vertex buffer!" );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
        OGL_DISPDBG_R( KE_ERROR, "Error re-binding index buffer!" );
    }
}


/*
 * Name: IKeOpenGLRenderDevice::draw_vertices
 * Desc: Draws vertices from the current vertex buffer
 */
void IKeOpenGLRenderDevice::DrawVertices( uint32_t primtype, uint32_t stride, int first, int count )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
   
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!" );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    /* Draw the vertices */
    glDrawArrays( primitive_types[primtype], first, count );
    OGL_DISPDBG_R( KE_ERROR, "Vertex buffer rendering error (glDrawArrays)!" );
}

/*
 * Name: IKeOpenGLRenderDevice::draw_indexed_vertices
 * Desc: Draws vertices from the current vertex and index buffer.
 */
void IKeOpenGLRenderDevice::DrawIndexedVertices( uint32_t primtype, uint32_t stride, int count )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
    
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
    /* Bind the vertex and index buffer objects */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!" );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding index buffer!" );
   
    /* Draw the vertices */
    glDrawElements( primitive_types[primtype], count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "Indexed geometry rendering error (glDrawElements)!" );
}

/*
 * Name: IKeOpenGLRenderDevice::draw_indexed_vertices_range
 * Desc: Same as above, but allows the user to specify the start/end vertex.
 */
void IKeOpenGLRenderDevice::DrawIndexedVerticesRange( uint32_t primtype, uint32_t stride, int start, int end, int count )
{
    IKeOpenGLGeometryBuffer* gb = static_cast<IKeOpenGLGeometryBuffer*>( current_geometrybuffer );
    GLenum error = glGetError();
   
    /* Apply sampler states */
    PVT_ApplySamplerStates();
    
    /* Assuming there is already a GPU program bound, attempt to set the current matrices */
    PVT_SetWorldViewProjectionMatrices();
    
    /* Bind the vertex buffer object, but not the index buffer object */
    glBindBuffer( GL_ARRAY_BUFFER, gb->vbo[0] );
	OGL_DISPDBG_R( KE_ERROR, "Error binding vertex buffer!" );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gb->vbo[1] );
    OGL_DISPDBG_R( KE_ERROR, "Error binding index buffer!" );
    
    /* Draw the vertices */
    glDrawRangeElements( primitive_types[primtype], start, end, count, data_types[gb->index_type], NULL );
    OGL_DISPDBG_R( KE_ERROR, "Indexed geometry rendering error (glDrawRangeElements)!" );
}

/*
 * Name: IKeOpenGLRenderDevice::get_framebuffer_region
 * Desc: Returns a pointer filled with pixels of the given region of the current framebuffer.
 * TODO: Determine bit depth, allow reading from depth buffers, etc.
 */
bool IKeOpenGLRenderDevice::GetFramebufferRegion( int x, int y, int width, int height, uint32_t flags, int* bpp, void** pixels )
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
 * Name: IKeOpenGLRenderDevice::set_viewport
 * Desc: Sets the viewport.
 */
void IKeOpenGLRenderDevice::SetViewport( int x, int y, int width, int height )
{
    /* Set the viewport */
    glViewport( x, y, width, height );
    
    viewport[0] = x;
    viewport[1] = y;
    viewport[2] = width;
    viewport[3] = height;
}

/*
 * Name: IKeOpenGLRenderDevice::set_viewport
 * Desc: Sets the viewport.
 */
void IKeOpenGLRenderDevice::SetViewportV( int* viewport )
{
    /* Set the viewport */
    glViewport( viewport[0], viewport[1], viewport[2], viewport[3] );
    
    memmove( this->viewport, viewport, sizeof(int)*4 );
}

/*
 * Name: IKeOpenGLRenderDevice::GetViewport
 * Desc: Gets the viewport.
 */
void IKeOpenGLRenderDevice::GetViewport( int* x, int* y, int* width, int* height )
{
    /* Get the viewport */
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    *x = viewport[0];
    *y = viewport[1];
    *width = viewport[2];
    *height = viewport[3];
}

/*
 * Name: IKeOpenGLRenderDevice::set_viewport
 * Desc: Gets the viewport.
 */
void IKeOpenGLRenderDevice::GetViewportV( int* viewport )
{
    /* Get the viewport */
    glGetIntegerv( GL_VIEWPORT, this->viewport );
    
    memmove( viewport, this->viewport, sizeof(int)*4 );
}

/*
 * Name: IKeOpenGLRenderDevice::set_perspective_matrix
 * Desc: Sets the projection matrix by creating a perspective matrix.
 */
void IKeOpenGLRenderDevice::SetPerspectiveMatrix( float fov, float aspect, float near_z, float far_z )
{
    /* Set up projection matrix using the perspective method */
//    projection_matrix = M4MakePerspective( fov, aspect, near_z, far_z );
    nv::perspective( projection_matrix, fov, aspect, near_z, far_z );
}


/*
 * Name: IKeOpenGLRenderDevice::set_view_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::SetViewMatrix( const nv::matrix4f* view )
{
    /* Copy over the incoming view matrix */
    memmove( view_matrix._array, view->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_world_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::SetWorldMatrix( const nv::matrix4f* world )
{
    /* Copy over the incoming world matrix */
    memmove( world_matrix._array, world->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_modelview_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::SetModelviewMatrix( const nv::matrix4f* modelview )
{
    /* Copy over the incoming modelview matrix */
    memmove( modelview_matrix._array, modelview->_array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_projection_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::SetProjectionMatrix( const nv::matrix4f* projection )
{
    /* Copy over the incoming projection matrix */
    memmove( projection_matrix._array, projection->_array, sizeof( float ) * 16 );
}

/*
 * Name: IKeOpenGLRenderDevice::set_view_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::GetViewMatrix( nv::matrix4f* view )
{
    /* Copy over the incoming view matrix */
    memmove( view->_array, view_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_world_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::GetWorldMatrix( nv::matrix4f* world )
{
    /* Copy over the incoming world matrix */
    memmove( world->_array, world_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_modelview_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::GetModelviewMatrix( nv::matrix4f* modelview )
{
    /* Copy over the incoming modelview matrix */
    memmove( modelview->_array, modelview_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::set_projection_matrix
 * Desc:
 */
void IKeOpenGLRenderDevice::GetProjectionMatrix( nv::matrix4f* projection )
{
    /* Copy over the incoming projection matrix */
    memmove( projection->_array, projection_matrix._array, sizeof( float ) * 16 );
}


/*
 * Name: IKeOpenGLRenderDevice::block_until_vertical_blank
 * Desc: Stalls the current thread for an interval equivalent to one
 *       vertical blank. This function does not sync to the actual vertical blank
 *       as I have not found a way to do this on any platform besides Windows.
 *       This is also thread safe.
 */
void IKeOpenGLRenderDevice::BlockUntilVerticalBlank()
{
#ifdef _WIN32
 #ifdef USE_DDRAW_VBLANK
	reinterpret_cast<IDirectDraw7*>(dd)->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
	return;
 #endif
#endif

    SDL_DisplayMode display_mode;
    
    /* Get the current display mode */
    /* TODO: Get display mode based on windowed or fullscreen mode. */
    SDL_GetWindowDisplayMode( window, &display_mode );
    
    /* Stall this thread for 1000/refresh_rate milliseconds */
    SDL_Delay( 1000 / display_mode.refresh_rate );
}


/*
 * Name: IKeOpenGLRenderDevice::SetSwapInterval
 * Desc: Sets the swap interval (enables/disable vertical sync). See SDL documentation on
 *       SDL_GL_SetSwapInterval for a more detailed description.
 */
void IKeOpenGLRenderDevice::SetSwapInterval( int swap_interval )
{
    SDL_GL_SetSwapInterval( swap_interval );
}


/*
 * Name: IKeOpenGLRenderDevice::GetSwapInterval
 * Desc: Returns the vertical sync value set above.
 */
int IKeOpenGLRenderDevice::GetSwapInterval()
{
    return SDL_GL_GetSwapInterval();
}

/*
 * Name: IKeOpenGLRenderDevice::BlockUntilIdle
 * Desc: Stalls the current thread until the GPU is no longer busy.
 */
void IKeOpenGLRenderDevice::BlockUntilIdle()
{
	glFinish();
}


/*
 * Name: IKeOpenGLRenderDevice::Kick
 * Desc: Sends all pending GPU commands to the pipeline.
 */
void IKeOpenGLRenderDevice::Kick()
{
	glFlush();
}

/*
 * Name: IKeOpenGLRenderDevice::CreateFence
 * Desc: Creates a new GPU fence object based on the vendor's fencing implementation.
 * NOTE: With sync objects (not fence objects), the fence is inserted the moment it is created.  
 *       So if this is the chosen extension, then the fence will not literally be created until 
 *       the time of it's insertion.
 */
bool IKeOpenGLRenderDevice::CreateFence( IKeFence** fence, uint32_t flags )
{
    if( !fence )
        return false;
    
    /* Create a new fence */
    (*fence) = new IKeOpenGLFence();
    IKeOpenGLFence* f = static_cast<IKeOpenGLFence*>( *fence );
    
    /* Set the device's chosen fence vendor */
    f->vendor = flags == KE_FENCE_DEFAULT ? fence_vendor : flags;
    
    if( !KeOpenGLCreateFence[f->vendor]( &f ) )
    {
        f->Destroy();
        return false;
    }
    
    return true;
}

#if 0
/*
 * Name: IKeOpenGLRenderDevice::insert_fence
 * Desc: Creates a new GPU fence object and sets it in place.
 */
bool IKeOpenGLRenderDevice::InsertFence( IKeFence** fence )
{
	/* Sanity check */
	if( !fence )
		return false;

	/* Allocate and initialize a new fence */
	(*fence) = new IKeOpenGLFence;

	return KeOpenGLInsertFence[fence_vendor]( (IKeOpenGLFence**) fence );
}


/*
 * Name: IKeOpenGLRenderDevice::test_fence
 * Desc: Returns true if this all GPU commands have been completed since
 *		 this fence was set.  If there are still GPU commands pending,
 *		 returns false.
 */
bool IKeOpenGLRenderDevice::TestFence( IKeFence* fence )
{
	if( !fence )
		return false;

	return KeOpenGLTestFence[fence_vendor]( static_cast<IKeOpenGLFence*>(fence) );
}


/*
 * Name: IKeOpenGLRenderDevice::block_on_fence
 * Desc: Stalls the current thread until the fence has been crossed.
 */
void IKeOpenGLRenderDevice::BlockOnFence( IKeFence* fence )
{
	KeOpenGLBlockOnFence[fence_vendor]( static_cast<IKeOpenGLFence*>(fence) );
}


/*
 * Name: IKeOpenGLRenderDevice::delete_fence
 * Desc: Deletes a GPU fence object.
 */
void IKeOpenGLRenderDevice::DeleteFence( IKeFence* fence )
{
	if( !fence )
		return;

	IKeOpenGLFence* f = static_cast<IKeOpenGLFence*>( fence );
	KeOpenGLDeleteFence[fence_vendor](f);

    f->Destroy();
}


/*
 * Name: IKeOpenGLRenderDevice::is_fence
 * Desc: Tests this fence object for a valid fence.
 */
bool IKeOpenGLRenderDevice::IsFence( IKeFence* fence )
{
	if( !fence )
		return false;

	return KeOpenGLIsFence[fence_vendor]( static_cast<IKeOpenGLFence*>(fence) );;
}
#endif


/*
 * Name: IKeOpenGLRenderDevice::gpu_memory_info
 * Desc: Returns the amound of available and total video memory of this machine.
 * NOTE: The OpenGL version of this API is dependent on the availability of certain extensions
 *       that expose this.  This is not guaranteed to be supported for all platforms.
 */
void IKeOpenGLRenderDevice::GpuMemoryInfo( uint32_t* total_memory, uint32_t* free_memory )
{
#ifdef _WIN32
 #ifdef USE_DDRAW_VMEM
	DDSCAPS2 caps;
	reinterpret_cast<IDirectDraw7*>( dd )->GetAvailableVidMem( &caps, (DWORD*) total_memory, (DWORD*) free_memory );
 #endif
#endif

#ifndef __MOBILE_OS__
#ifdef __APPLE__
    KeVideoAdapterOSX adapter;
    
    KeGetCurrentVideoAdapterInformationOSX( &adapter );
    *total_memory = adapter.video_memory;
    *free_memory = adapter.texture_memory;
#endif
#endif
}


