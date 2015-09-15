#pragma once

#include "Ke.h"
#include "KeRenderDevice.h"

#include <SDL_ttf.h>


/*
 * Font object
 */
struct KeFont;

/*
 * Compiled font string
 */
struct KeCompiledFontString;



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
 * Name: KeCloseFont
 * Desc: Closes this font handle
 */
void KeCloseFont( KeFont* font );


/*
 * Name: KeCreateCompiledFontString
 * Desc: Creates a texture with the desired string using the supplied font and program
 */
bool KeCreateCompiledFontString( const char* string, uint32_t colour, IKeGpuProgram* program, KeFont* font, KeCompiledFontString** compiled_string );


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