#include "KeRenderDevice.h"
#include "KeEffect.h"

#include <yaml.h>



/* Effect structure definition */
struct KeEffect
{
	IKeGpuProgram*				program;
	IKeRenderStateBuffer*		renderstate;
	IKeTextureSamplerBuffer*	texturesampler;
};




/*
 * Name: KeOpenEffect
 * Desc: Open's up an effect file to create a rendering effect structure.  If successful, the
 *		 effect will contain all the necessary shaders and render/sampler states needed for the
 *       described rendering pass.
 */
bool KeOpenEffect( std::string effect_file, IKeRenderDevice* render_device, KeEffect* effect )
{
	/* Sanity checks */
	if( !render_device )
		return false;

	if( !effect )
		return false;

	/* Open the YAML based effect file */
	FILE* fp = fopen( effect_file.c_str(), "r" );
	if( !fp )
		return false;

	/* Initialize the YAML parser */
	yaml_parser_t parser;

	yaml_parser_initialize( &parser );
	yaml_parser_set_input_file( &parser, fp );

	yaml_parser_delete( &parser );

	return true;
}

/*
 * Name: KeOpenEffectFromFile
 * Desc: Same as above, but reads data from a FILE object already loaded by the caller.
 */
bool KeOpenEffectFromFile( const uint8_t* effect_string, IKeRenderDevice* render_device, KeEffect* effect )
{
	return true;
}

/*
 * Name: KeCloseEffect
 * Desc: Unintializes the GPU program and state buffer contained within the effect.
 */
void KeCloseEffect( KeEffect* effect )
{
	if( effect )
	{
		if( effect->program )
			effect->program->Destroy();
		if( effect->renderstate )
			effect->renderstate->Destroy();
		if( effect->texturesampler )
			effect->texturesampler->Destroy();
	}
}