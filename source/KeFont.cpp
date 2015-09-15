#include "KeFont.h"



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
		1, 0, surface->w, 0,
		1, 1, surface->w, surface->h,
		0, 1, 0, surface->h,
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
		if( compiled_string->gp ) compiled_string->gp->Destroy();
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
	float viewport[4];

	/* Get our current viewport */
	renderdevice->GetViewportV( (int*) viewport );

	/* Save the four engine managed matrices before setting new ones */
	renderdevice->GetWorldMatrix( &world );
	renderdevice->GetViewMatrix( &view );
//	renderdevice->GetModelviewMatrix( &modelview );
	renderdevice->GetProjectionMatrix( &projection );
	
	/* Now setup some 2D matrices */
	nv::ortho2D( ortho, viewport[0], viewport[2], viewport[3], viewport[1] );
	nv::translation( translation, (float) x, (float) y, 0.0f );
	identity.make_identity();
	renderdevice->SetWorldMatrix( &translation );
	renderdevice->SetViewMatrix( &identity );
	renderdevice->SetProjectionMatrix( &ortho );

	/* Render this font string */
	renderdevice->SetGeometryBuffer( compiled_string->gb );
	renderdevice->SetProgram( compiled_string->gp );
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