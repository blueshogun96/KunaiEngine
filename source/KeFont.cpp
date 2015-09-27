#define STB_TRUETYPE_IMPLEMENTATION

#include "Ke.h"
#include "KeFont.h"
#include <vector>


/*
 * Font object
 */
struct KeFont
{
	TTF_Font* font;
	int size;
};

/*
 * Compiled font string
 */
struct KeCompiledFontString
{
	int width, height;
	IKeGeometryBuffer*  gb;
	IKeGpuProgram*		gp;
	IKeTexture*			tex;
};




/*
 * Name: KeInitFontLibrary
 * Desc: Initialize SDL2 ttf library
 */
bool KeInitFontLibrary()
{
	if( TTF_Init() == -1 )
	{
		DISPDBG( KE_ERROR, "Error initializing SDL2 ttf!\nError: " << TTF_GetError() << "\n" );
		return false;
	}

	return true;
}


/*
 * Name: KeUninitFontLibrary
 * Desc: Uninitialize the above
 */
void KeUninitFontLibrary()
{
	TTF_Quit();
}


/*
 * Name: KeOpenFont
 * Desc: Opens a .ttf font and generates a font map of the desired size.
 */
bool KeOpenFont( const char* fontfile, int size, KeFont** font )
{
	if( !font )
		return false;

	(*font) = new KeFont;
	(*font)->font = TTF_OpenFont( fontfile, size );
	if( !(*font)->font )
	{
		delete *font;
		DISPDBG( KE_ERROR, "Error opening font (" << fontfile << ")\n!\nError: " << TTF_GetError() << "\n" );
		return false;
	}
	(*font)->size = size;

	return true;
}


/*
 * Name: KeOpenFontFromMemory
 * Desc: Same as above, but only from a raw memory pointer.
 */
bool KeOpenFontFromMemory( void* fontbuffer, uint32_t length, int size, KeFont** font )
{
	if( !font )
		return false;

	SDL_RWops* rwops = SDL_RWFromMem( fontbuffer, length );

	(*font) = new KeFont;
	(*font)->font = TTF_OpenFontRW( rwops, No, size ); //TTF_OpenFont( fontfile, size );
	if( !(*font)->font )
	{
		delete *font;
		SDL_RWclose( rwops );
		DISPDBG( KE_ERROR, "Error opening font from memory\n!\nError: " << TTF_GetError() << "\n" );
		return false;
	}

	(*font)->size = size;

	SDL_RWclose( rwops );

	return true;
}



/*
 * Name: KeCloseFont
 * Desc: Closes this font handle
 */
void KeCloseFont( KeFont* font )
{
	if( font )
	{
		if( font->font )
			TTF_CloseFont( font->font );

		delete font;
	}
}


/*
 * Name: KeCreateCompiledFontString
 * Desc: Creates a texture with the desired string using the supplied font and program
 */
bool KeCreateCompiledFontString( const char* string, uint32_t colour, IKeGpuProgram* program, KeFont* font, KeCompiledFontString** compiled_string )
{
	/* Sanity chechs */
	if( !string && !font )
		return false;

	SDL_Colour sdl_colour = 
	{
		(colour>>16)&0xFF,
		(colour>> 8)&0xFF,
		(colour    )&0xFF,
		(colour>>24)&0xFF,
	};

	/* Generate pixel data */
	SDL_Surface* surface = TTF_RenderText_Blended( font->font, string, sdl_colour );
	if( !surface )
		DISPDBG( KE_ERROR, "Error creating SDL surface from TTF!\nError: " << TTF_GetError() << std::endl );

	/* Generate a new texture from the pixel data */
	*compiled_string = new KeCompiledFontString;
	int ret = KeGetRenderDevice()->CreateTexture2D( KE_TEXTURE_2D, surface->w, surface->h, 1, KE_TEXTUREFORMAT_RGBA, KE_UNSIGNED_BYTE, &(*compiled_string)->tex, surface->pixels );
	if( !ret )
	{
		delete *compiled_string;
		SDL_FreeSurface( surface );
		return false;
	}

	/* Generate a geometry buffer */
	/* TODO: Use indexed vertices */
	KeVertexAttribute va[] = 
	{
        { KE_VA_POSITION, 4, KE_FLOAT, No, 4*sizeof(float), 0 },
        { -1, 0, 0, 0, 0, 0 },
	};

	float vertices [] = 
	{
		0, 0, 0, 0,	/* xy uv */
		surface->w, 0, 1, 0,
		0, surface->h, 0, 1, 
		surface->w, surface->h, 1, 1, 
	};

	ret = KeGetRenderDevice()->CreateGeometryBuffer( vertices, 16*sizeof(float), NULL, 0, 0, KE_USAGE_STATIC_WRITE, va, &(*compiled_string)->gb );
	if( !ret )
	{
		delete *compiled_string;
		SDL_FreeSurface( surface );
		return false;
	}

	(*compiled_string)->width = surface->w;
	(*compiled_string)->height = surface->h;
	(*compiled_string)->gp = program;

	SDL_FreeSurface( surface );

	return true;
}


/*
 * Name: KeCloseCompiledFontString
 * Desc: Deletes the resources associated with this compiled string.
 */
void KeCloseCompiledFontString( KeCompiledFontString* compiled_string )
{
	if( compiled_string )
	{
		if( compiled_string->gb ) compiled_string->gb->Destroy();
		//if( compiled_string->gp ) compiled_string->gp->Destroy();
		if( compiled_string->tex ) compiled_string->tex->Destroy();
		
		delete compiled_string;
	}
}


/*
 * Name: KeDrawCompiledFontString
 * Desc: Draws the string to the screen using it's own 2D coordinates.
 */
void KeDrawCompiledFontString( KeCompiledFontString* compiled_string, int x, int y )
{
	nv::matrix4f ortho, translation, rotation, identity;
	nv::matrix4f world, view, modelview, projection;
	IKeRenderDevice* renderdevice = KeGetRenderDevice();
	int viewport[4];
	float colour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float z = 0.0f;

	/* Get our current viewport */
	renderdevice->GetViewportV( viewport );

	/* Save the four engine managed matrices before setting new ones */
	renderdevice->GetWorldMatrix( &world );
	renderdevice->GetViewMatrix( &view );
//	renderdevice->GetModelviewMatrix( &modelview );
	renderdevice->GetProjectionMatrix( &projection );
	
	/* Now setup some 2D matrices */
	nv::ortho2D( ortho, (float) viewport[0], (float) viewport[2], (float) viewport[3], (float) viewport[1] );
	nv::translation( translation, (float) x, (float) y, 0.0f );
	identity.make_identity();
	renderdevice->SetWorldMatrix( &identity );
	renderdevice->SetViewMatrix( &identity );
	renderdevice->SetProjectionMatrix( &ortho );

	/* Render this font string */
	renderdevice->SetProgram( compiled_string->gp );
	renderdevice->GetProgramConstantFV( "z_value", &z );
	renderdevice->SetProgramConstant4FV( "diffuse", 1, colour );
	renderdevice->SetGeometryBuffer( compiled_string->gb );
	renderdevice->SetTexture( 0, compiled_string->tex );
	renderdevice->DrawVertices( KE_TRIANGLESTRIP, sizeof(float)*4, 0, 4 );

	/* Reset the matrices back to their previous states */
	renderdevice->SetWorldMatrix( &world );
	renderdevice->SetViewMatrix( &view );
	renderdevice->SetProjectionMatrix( &projection );
}

#if 0
void RenderText(std::string message, SDL_Color color, int x, int y, int size) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  gluOrtho2D(0, m_Width, 0, m_Height); // m_Width and m_Height is the resolution of window
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  TTF_Font * font = TTF_OpenFont("pathToFont.ttf", size);
  SDL_Surface * sFont = TTF_RenderText_Blended(font, message, color);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

  glBegin(GL_QUADS);
  {
    glTexCoord2f(0,0); glVertex2f(x, y);
    glTexCoord2f(1,0); glVertex2f(x + sFont->w, y);
    glTexCoord2f(1,1); glVertex2f(x + sFont->w, y + sFont->h);
    glTexCoord2f(0,1); glVertex2f(x, y + sFont->h);
  }
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glDeleteTextures(1, &texture);
  TTF_CloseFont(font);
  SDL_FreeSurface(sFont);
}

/.../

int main() {

/.../ Render 3D stuff here

  // Prints out "Hello World" at location (5,10) at font size 12!
  SDL_Color color = {255, 0, 0, 0}; // Red
  RenderText("Hello World", color, 5, 10, 12); 

/.../

  return 0;
}
#endif



KeDynamicFontString::KeDynamicFontString( const char* ttffont, float font_size, int tex_width, int tex_height )
{
	 /* Get a pointer to the file */
    FILE* fp = fopen( ttffont, "rb" );

    /* Get the ttf file size */
    fseek( fp, 0, SEEK_END );
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET );

    /* Allocate space for the file and texture */
    unsigned char* ttf_buffer = (unsigned char*) malloc( len );
    unsigned char* temp_bitmap = (unsigned char*) malloc( tex_width*tex_height );

    /* Read in the actual file data */
    fread( ttf_buffer, 1, len, fp );

    /* Create a bitmap font from the ttf file */
    stbtt_BakeFontBitmap( ttf_buffer, 0, font_size, temp_bitmap,tex_width,tex_height, 32,224, cdata ); // no guarantee this fits!

    /* Free the allocated buffer and file pointer */
    free(ttf_buffer);
    fclose(fp);

    /* Create a 8-bit alpha only texture */
	KeGetRenderDevice()->CreateTexture2D( KE_TEXTURE_2D, tex_width, tex_height, 1, KE_TEXTUREFORMAT_R8, KE_UNSIGNED_BYTE, &tex, temp_bitmap );

    free(temp_bitmap);

    width = tex_width, height = tex_height;

	/* Create a dynamic geometry buffer */
	KeVertexAttribute va[] = 
	{
        { KE_VA_POSITION, 4, KE_FLOAT, No, 4*sizeof(float), 0 },
        { -1, 0, 0, 0, 0, 0 },
	};

	/* TODO: Index vertices */

	bool ret = KeGetRenderDevice()->CreateGeometryBuffer( NULL, sizeof(nv::vec4f)*400, NULL, sizeof(uint16_t)*400*6, KE_UNSIGNED_SHORT, KE_USAGE_DYNAMIC_WRITE, va, &gb );
}

KeDynamicFontString::KeDynamicFontString( void* ttffont_buffer, uint32_t buffer_length, float font_size, int tex_width, int tex_height )
{
    /* Allocate space for the file and texture */
    unsigned char* temp_bitmap = (unsigned char*) malloc( tex_width*tex_height );

    /* Create a bitmap font from the ttf file */
    stbtt_BakeFontBitmap( (const unsigned char*) ttffont_buffer, 0, font_size, temp_bitmap,tex_width,tex_height, 32,224, cdata ); // no guarantee this fits!

    /* Create a 8-bit alpha only texture */
	KeGetRenderDevice()->CreateTexture2D( KE_TEXTURE_2D, tex_width, tex_height, 1, KE_TEXTUREFORMAT_R8, KE_UNSIGNED_BYTE, &tex, temp_bitmap );

    free(temp_bitmap);

    width = tex_width, height = tex_height;

	/* Create a dynamic geometry buffer */
	KeVertexAttribute va[] = 
	{
        { KE_VA_POSITION, 4, KE_FLOAT, No, 4*sizeof(float), 0 },
        { -1, 0, 0, 0, 0, 0 },
	};

	/* TODO: Index vertices */

	bool ret = KeGetRenderDevice()->CreateGeometryBuffer( NULL, sizeof(nv::vec4f)*400, NULL, sizeof(uint16_t)*400*6, KE_UNSIGNED_SHORT, KE_USAGE_DYNAMIC_WRITE, va, &gb );
}

KeDynamicFontString::~KeDynamicFontString()
{
	if( gb )
		gb->Destroy();
	if( tex )
		tex->Destroy();
}

void KeDynamicFontString::SetProgram( IKeGpuProgram* program )
{
	this->program = program;
}

void KeDynamicFontString::Print( const char* text, nv::vec2f position, bool center, float* colour )
{
	float x = position.x;
    float y = position.y;
	float z = 0;
    float sx = x;
	std::vector<nv::vec4f> vertices;
	std::vector<uint16_t> indices;
	IKeRenderDevice* renderdevice = KeGetRenderDevice();
	nv::matrix4f ortho, translation, rotation, identity;
	nv::matrix4f world, view, modelview, projection;
	int viewport[4];

	/* Get our current viewport */
	renderdevice->GetViewportV( viewport );

	/* Save the four engine managed matrices before setting new ones */
	renderdevice->GetWorldMatrix( &world );
	renderdevice->GetViewMatrix( &view );
//	renderdevice->GetModelviewMatrix( &modelview );
	renderdevice->GetProjectionMatrix( &projection );
	
	/* Now setup some 2D matrices */
	nv::ortho2D( ortho, (float) viewport[0], (float) viewport[2], (float) viewport[3], (float) viewport[1] );
	nv::translation( translation, (float) x, (float) y, 0.0f );
	identity.make_identity();
	renderdevice->SetWorldMatrix( &identity );
	renderdevice->SetViewMatrix( &identity );
	renderdevice->SetProjectionMatrix( &ortho );

    /* If we are centering this, get the length of the string and halv it */
    if( center )
    {
        int len = (int) strlen(text);
        int i = 0;

        while( i < len )
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad( cdata, position.x, position.y, text[i]-32, &x, &y, &q, 1 );
            i++;
        }

        float dx = x-sx;
        x -= dx*1.5f;
    }

	renderdevice->SetTexture( 0, tex );
	renderdevice->SetProgram( program );
	renderdevice->SetProgramConstant1FV( "z_value", 1, &z );
	renderdevice->SetProgramConstant4FV( "diffuse", 1, colour );
	
	uint16_t i = 0;

	/* Reserve memory the size of the dynamic geometry buffer.  Commenting out this line 
	   results in a significant framerate drop. */
	/* TODO: Indices */
	vertices.reserve(400);
	//indices.reserve(400*6);
    while (*text) {
        //if (*text >= 32 && *text < 128) {
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad( cdata, width, height, *text-32, &x, &y, &q, 1 );//1=opengl,0=old d3d

        //float t[4][2] = { {q.s0,q.t1}, {q.s1,q.t1}, {q.s1,q.t0}, {q.s0,q.t0} };
        //float v[4][2] = { {q.x0,q.y1}, {q.x1,q.y1}, {q.x1,q.y0}, {q.x0,q.y0} };
		nv::vec4f v[4] = { {q.x0,q.y1,q.s0,q.t1}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y0,q.s1,q.t0} };
		nv::vec4f v2[6] = { {q.x0,q.y1,q.s0,q.t1}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y0,q.s1,q.t0} };

		/*vertices.push_back(v[0]);
		vertices.push_back(v[1]);
		vertices.push_back(v[2]);
		vertices.push_back(v[3]);
		indices.push_back(i+0);
		indices.push_back(i+1);
		indices.push_back(i+2);
		indices.push_back(i+1);
		indices.push_back(i+2);
		indices.push_back(i+3);
		i+=6;*/

		vertices.push_back(v2[0]);
		vertices.push_back(v2[1]);
		vertices.push_back(v2[2]);
		vertices.push_back(v2[3]);
		vertices.push_back(v2[4]);
		vertices.push_back(v2[5]);

        ++text;
    }

	gb->SetVertexData( 0, sizeof(nv::vec4f)*vertices.size(), vertices.data() );
	//gb->SetIndexData( 0, sizeof(uint16_t)*indices.size(), indices.data() );
	renderdevice->SetGeometryBuffer(gb);
	renderdevice->DrawVertices( KE_TRIANGLES, sizeof( nv::vec4f ), 0, vertices.size() );
	//renderdevice->DrawIndexedVertices( KE_TRIANGLES, sizeof( nv::vec4f ), indices.size() );
	renderdevice->SetGeometryBuffer(NULL);

	renderdevice->SetProgram(NULL);
	
	/* Reset the matrices back to their previous states */
	renderdevice->SetWorldMatrix( &world );
	renderdevice->SetViewMatrix( &view );
	renderdevice->SetProjectionMatrix( &projection );
}

void KeDynamicFontString::Printw( const wchar_t* text, nv::vec2f position, bool center, float* colour )
{
	float x = position.x;
    float y = position.y;
	float z = 0;
    float sx = x;
	std::vector<nv::vec4f> vertices;
	std::vector<uint16_t> indices;
	IKeRenderDevice* renderdevice = KeGetRenderDevice();
	nv::matrix4f ortho, translation, rotation, identity;
	nv::matrix4f world, view, modelview, projection;
	int viewport[4];

	/* Get our current viewport */
	renderdevice->GetViewportV( viewport );

	/* Save the four engine managed matrices before setting new ones */
	renderdevice->GetWorldMatrix( &world );
	renderdevice->GetViewMatrix( &view );
//	renderdevice->GetModelviewMatrix( &modelview );
	renderdevice->GetProjectionMatrix( &projection );
	
	/* Now setup some 2D matrices */
	nv::ortho2D( ortho, (float) viewport[0], (float) viewport[2], (float) viewport[3], (float) viewport[1] );
	nv::translation( translation, (float) x, (float) y, 0.0f );
	identity.make_identity();
	renderdevice->SetWorldMatrix( &identity );
	renderdevice->SetViewMatrix( &identity );
	renderdevice->SetProjectionMatrix( &ortho );

    /* If we are centering this, get the length of the string and halv it */
    if( center )
    {
        int len = (int) std::wcslen(text);
        int i = 0;

        while( i < len )
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad( cdata, position.x, position.y, text[i]-32, &x, &y, &q, 1 );
            i++;
        }

        float dx = x-sx;
        x -= dx*1.5f;
    }

	renderdevice->SetTexture( 0, tex );
	renderdevice->SetProgram( program );
	renderdevice->SetProgramConstant1FV( "z_value", 1, &z );
	renderdevice->SetProgramConstant4FV( "diffuse", 1, colour );
	
	uint16_t i = 0;

	/* Reserve memory the size of the dynamic geometry buffer.  Commenting out this line 
	   results in a significant framerate drop. */
	/* TODO: Indices */
	vertices.reserve(400);
	//indices.reserve(400*6);
    while (*text) {
        //if (*text >= 32 && *text < 128) {
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad( cdata, width, height, *text-32, &x, &y, &q, 1 );//1=opengl,0=old d3d

		nv::vec4f v[4] = { {q.x0,q.y1,q.s0,q.t1}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y0,q.s1,q.t0} };
		nv::vec4f v2[6] = { {q.x0,q.y1,q.s0,q.t1}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y1,q.s1,q.t1}, {q.x0,q.y0,q.s0,q.t0}, {q.x1,q.y0,q.s1,q.t0} };

		/* TODO: Not only are indexed vertices for font rendering broken atm, they seem to give me less 
		   performance (for core OpenGL).  Fix this, and try it on Direct3D11. */

		/*vertices.push_back(v[0]);
		vertices.push_back(v[1]);
		vertices.push_back(v[2]);
		vertices.push_back(v[3]);
		indices.push_back(i+0);
		indices.push_back(i+1);
		indices.push_back(i+2);
		indices.push_back(i+1);
		indices.push_back(i+2);
		indices.push_back(i+3);
		i+=6;*/

		vertices.push_back(v2[0]);
		vertices.push_back(v2[1]);
		vertices.push_back(v2[2]);
		vertices.push_back(v2[3]);
		vertices.push_back(v2[4]);
		vertices.push_back(v2[5]);

        ++text;
    }

	gb->SetVertexData( 0, sizeof(nv::vec4f)*vertices.size(), vertices.data() );
	//gb->SetIndexData( 0, sizeof(uint16_t)*indices.size(), indices.data() );
	renderdevice->SetGeometryBuffer(gb);
	renderdevice->DrawVertices( KE_TRIANGLES, sizeof( nv::vec4f ), 0, vertices.size() );
	//renderdevice->DrawIndexedVertices( KE_TRIANGLES, sizeof( nv::vec4f ), indices.size() );
	renderdevice->SetGeometryBuffer(NULL);

	renderdevice->SetProgram(NULL);
	
	/* Reset the matrices back to their previous states */
	renderdevice->SetWorldMatrix( &world );
	renderdevice->SetViewMatrix( &view );
	renderdevice->SetProjectionMatrix( &projection );
}