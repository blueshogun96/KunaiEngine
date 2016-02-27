#version 150

out vec4 colour;
in float tex_coord;

uniform sampler1D tex0;	/* Lighting texture */

void main(void)
{
	/* Lighting value */
	colour = texture( tex0, tex_coord ).bgra;
}
