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
 * Font library functions
 */
bool KeInitFontLibrary();
void KeUninitFontLibrary();