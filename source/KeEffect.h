#pragma once

#include "Ke.h"
#include "KeRenderDevice.h"


/*
 * Effect structure
 */
struct KeEffect;


/*
 * Effect functions
 */

/*
 * Name: KeOpenEffect
 * Desc: Open's up an effect file to create a rendering effect structure.  If successful, the
 *		 effect will contain all the necessary shaders and render/sampler states needed for the
 *       described rendering pass.		 
 */
bool KeOpenEffect( std::string effect_file, IKeRenderDevice* render_device, KeEffect* effect );

/*
 * Name: KeOpenEffectFromFile
 * Desc: Same as above, but reads data from a FILE object already loaded by the caller.
 */
bool KeOpenEffectFromFile( const uint8_t* effect_string, IKeRenderDevice* render_device, KeEffect* effect );

/*
 * Name: KeCloseEffect
 * Desc: Unintializes the GPU program and state buffer contained within the effect.
 */
void KeCloseEffect( KeEffect* effect );