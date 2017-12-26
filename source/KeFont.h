#pragma once

#include "Ke.h"
#include "KeRenderDevice.h"

#if defined(__APPLE__) && !defined(__MOBILE_OS__)
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif
#include <stb_truetype.h>


/*
 * Font object
 */
struct KeFont;

/*
 * Compiled font string
 */
struct KeCompiledFontString;

/*
 * Dynamic font string
 */
struct KeDynamicFontString
{
public:
	KeDynamicFontString( const char* ttffont, float font_size, int tex_width = 1024, int tex_height = 1024 );
	KeDynamicFontString( void* ttffont_buffer, uint32_t buffer_length, float font_size, int tex_width = 1024, int tex_height = 1024 );
	virtual ~KeDynamicFontString();

	void SetProgram( IKeGpuProgram* program );
	void Print( const char* text, nv::vec2f position, bool center, float* colour );
	void Printw( const wchar_t* text, nv::vec2f position, bool center, float* colour );

private:
    int					width, height;
    stbtt_bakedchar     cdata[224];
	IKeTexture*			tex;
    IKeGpuProgram*	    program;
	IKeGeometryBuffer*	gb;
};

#if 0
class ttf_font_t
{
public:
    ttf_font_t() {};
    ttf_font_t( std::string ttffont, float font_size, int tex_width = 1024, int tex_height = 1024 )
    {
#if 0
        /* Get a pointer to the file */
        FILE* fp = fopen( ttffont.c_str(), "rb" );

        /* Get the ttf file size */
        fseek( fp, 0, SEEK_END );
        long len = ftell(fp);
        fseek(fp, 0, SEEK_SET );
#endif

        /* Allocate space for the file and texture */
		uint32_t len;
        unsigned char* ttf_buffer;// = (unsigned char*) malloc( len );
        unsigned char* temp_bitmap = (unsigned char*) malloc( tex_width*tex_height );

        /* Read in the actual file data */
        //fread(ttf_buffer, 1, len, fp);
		rez_read( ttffont.c_str(), &ttf_buffer, &len );

        /* Create a bitmap font from the ttf file */
        stbtt_BakeFontBitmap(ttf_buffer,0, font_size, temp_bitmap,tex_width,tex_height, 32,224, cdata); // no guarantee this fits!

        /* Free the allocated buffer and file pointer */
        free(ttf_buffer);
        //fclose(fp);

        /* Create a 8-bit alpha only texture */
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex_width, tex_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free(temp_bitmap);

        tex_dim.set( tex_width, tex_height );
    }

    virtual ~ttf_font_t()
    {
        glDeleteTextures( 1, &tex );
    }

public:
    void set_program( struct program_t* program )
    {
        this->program = program;
    }

    void set_ogldrv( ogldrv_t* drv )
    {
        ogldrv = drv;
    }

    void print( const char* text, struct vec2_t<float> p, bool center, struct vec4_t<float> colour = vec4_t<float>(0,0,0,1) )
    {
        float x = p.v[0];
        float y = p.v[1];
        float sx = x;

        /* If we are centering this, get the length of the string and halv it */
        if( center )
        {
            int len = (int) strlen(text);
            int i = 0;

            while( i < len )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], text[i]-32, &x,&y,&q,1);
                i++;
            }

            float dx = x-sx;
            x -= dx*1.5f;
        }

        //glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        // assume orthographic projection with units = screen pixels, origin at top left
        glBindTexture(GL_TEXTURE_2D, tex);
        glUseProgram( program->handle );
        glUniform4fv( program->uniform[0], 1, colour.v );
        glUniform1i( program->uniform[1], 0 );
        glUniformMatrix4fv( program->uniform[3], 1, 0, &ogldrv->m_modelview.col0.x );
        glUniformMatrix4fv( program->uniform[4], 1, 0, &ogldrv->m_projection.col0.x );
        glEnableVertexAttribArray( program->attrib[0] );
        glEnableVertexAttribArray( program->attrib[1] );

        while (*text) {
            //if (*text >= 32 && *text < 128) {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], *text-32, &x,&y,&q,1);//1=opengl,0=old d3d

                float t[4][2] = { {q.s0,q.t1}, {q.s1,q.t1}, {q.s1,q.t0}, {q.s0,q.t0} };
                float v[4][2] = { {q.x0,q.y1}, {q.x1,q.y1}, {q.x1,q.y0}, {q.x0,q.y0} };

                glVertexAttribPointer( program->attrib[1], 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, t );
                glVertexAttribPointer( program->attrib[0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, v );
                glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
                ogldrv->add_primitives_per_frame(2);
            //}
            ++text;
        }

        glDisableVertexAttribArray( program->attrib[0] );
        glDisableVertexAttribArray( program->attrib[1] );
        glUseProgram(0);

        glDisable( GL_BLEND );
        //glDisable( GL_TEXTURE_2D );
    }

    void printw( const wchar_t* text, struct vec2_t<float> p, bool center, struct vec4_t<float> colour = vec4_t<float>(0,0,0,1) )
    {
#if 0   /* TODO */
        float x = p.v[0];
        float y = p.v[1];
        float sx = x;
        float sy = y;

        /* If we are centering this, get the length of the string and halv it */
        if( center )
        {
            int len = wcslen(text);
            int i = 0;

            while( i < len )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], text[i]-32, &x,&y,&q,1);
                i++;
            }

            float dx = x-sx;
            x -= dx*1.5f;
        }

        //glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        // assume orthographic projection with units = screen pixels, origin at top left
        glBindTexture(GL_TEXTURE_2D, tex);
        glBegin(GL_QUADS);
        while (*text) {
            if (*text >= 32 && *text < 128) {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], *text-32, &x,&y,&q,1);//1=opengl,0=old d3d
                glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
                glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
                glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
                glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
            }
            ++text;
        }
        glEnd();
        glDisable( GL_BLEND );
        //glDisable( GL_TEXTURE_2D );
#endif
        float x = p.v[0];
        float y = p.v[1];
        float sx = x;

        /* If we are centering this, get the length of the string and halv it */
        if( center )
        {
            int len = (int) std::wcslen(text);
            int i = 0;

            while( i < len )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], text[i]-32, &x,&y,&q,1);
                i++;
            }

            float dx = x-sx;
            x -= dx*1.5f;
        }

        //glEnable( GL_TEXTURE_2D );
        glEnable( GL_BLEND );
        // assume orthographic projection with units = screen pixels, origin at top left
        glBindTexture(GL_TEXTURE_2D, tex);
        glUseProgram( program->handle );
        glUniform4fv( program->uniform[0], 1, colour.v );
        glUniform1i( program->uniform[1], 0 );
        glUniformMatrix4fv( program->uniform[3], 1, 0, &ogldrv->m_modelview.col0.x );
        glUniformMatrix4fv( program->uniform[4], 1, 0, &ogldrv->m_projection.col0.x );
        glEnableVertexAttribArray( program->attrib[0] );
        glEnableVertexAttribArray( program->attrib[1] );

        while (*text) {
            //if (*text >= 32 && *text < 128) {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, tex_dim.v[0],tex_dim.v[1], *text-32, &x,&y,&q,1);//1=opengl,0=old d3d

                float t[4][2] = { {q.s0,q.t1}, {q.s1,q.t1}, {q.s1,q.t0}, {q.s0,q.t0} };
                float v[4][2] = { {q.x0,q.y1}, {q.x1,q.y1}, {q.x1,q.y0}, {q.x0,q.y0} };

                glVertexAttribPointer( program->attrib[1], 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, t );
                glVertexAttribPointer( program->attrib[0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, v );
                glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
                ogldrv->add_primitives_per_frame(2);
           // }
            ++text;
        }

        glDisableVertexAttribArray( program->attrib[0] );
        glDisableVertexAttribArray( program->attrib[1] );
        glUseProgram(0);

        glDisable( GL_BLEND );
        //glDisable( GL_TEXTURE_2D );
    }

protected:
    unsigned int        tex;
    struct vec2_t<int>  tex_dim;
    stbtt_bakedchar     cdata[224];
    struct program_t*   program;
    ogldrv_t*           ogldrv;
};
#endif



/*
 * Name: KeInitFontLibrary
 * Desc: Initialize SDL2 ttf library
 */
bool KeInitFontLibrary();


/*
 * Name: KeUninitFontLibrary
 * Desc: Uninitialize the above
 */
void KeUninitFontLibrary();


/*
 * Name: KeOpenFont
 * Desc: Opens a .ttf font and generates a font map of the desired size.
 */
bool KeOpenFont( const char* fontfile, int size, KeFont** font );


/*
 * Name: KeOpenFontFromMemory
 * Desc: Same as above, but only from a raw memory pointer.
 */
bool KeOpenFontFromMemory( void* fontbuffer, uint32_t length, int size, KeFont** font );

/*
 * Name: KeSetFontOutline
 * Desc: Sets the width of the font outline in pixels (use 0 to set to normal)
 */
bool KeSetFontOutline( int outline, KeFont** font );

/*
 * Name: KeCloseFont
 * Desc: Closes this font handle
 */
void KeCloseFont( KeFont* font );


/*
 * Name: KeCreateBakedFontTexture
 * Desc: Creates a texture with the desired text using the supplied font
 */
bool KeCreateBakedFontTexture( const char* string, uint32_t colour, int outline, KeFont* font, IKeTexture** texture );


/*
 * Name: KeCreateCompiledFontString
 * Desc: Creates a texture with the desired string using the supplied font and program
 */
bool KeCreateCompiledFontString( const char* string, uint32_t colour, int outline, IKeGpuProgram* program, KeFont* font, KeCompiledFontString** compiled_string );


/*
 * Name: KeCloseCompiledFontString
 * Desc: Deletes the resources associated with this compiled string.
 */
void KeCloseCompiledFontString( KeCompiledFontString* compiled_string );


/*
 * Name: KeDrawCompiledFontString
 * Desc: Draws the string to the screen using it's own 2D coordinates.
 */
void KeDrawCompiledFontString( KeCompiledFontString* compiled_string, int x, int y );


/*
 * Namespaced API
 */
namespace Ke
{
    namespace FontLibrary
    {
        static bool (*Init)() = KeInitFontLibrary;
        static void (*Uninit)() = KeUninitFontLibrary;
        static bool (*OpenFont)( const char*, int, KeFont** ) = KeOpenFont;
        static bool (*OpenFontFromMemory)( void*, uint32_t, int, KeFont** ) = KeOpenFontFromMemory;
        static void (*CloseFont)( KeFont* ) = KeCloseFont;
        static bool (*CreateBakedFontTexture)( const char*, uint32_t, int, KeFont* font, IKeTexture** ) = KeCreateBakedFontTexture;
        static  bool (*CreateCompiledFontString)( const char*, uint32_t, int, IKeGpuProgram*, KeFont*, KeCompiledFontString** ) = KeCreateCompiledFontString;
        static void (*CloseCompiledFontString)( KeCompiledFontString* ) = KeCloseCompiledFontString;
        static void (*DrawCompiledFontString)( KeCompiledFontString*, int, int ) = KeDrawCompiledFontString;
    }
}
