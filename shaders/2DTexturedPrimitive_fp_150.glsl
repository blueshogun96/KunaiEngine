#version 150

out vec4 colour;
in vec4 out_colour;
in vec2 out_tex;

uniform sampler2D tex0;	/* Sprite texture */

void main(void)
{
	/* 32-bit texel value */
	vec4 texel = texture2D( tex0, out_tex );

    /* Modulate final colour with vertex colour */
	colour = texel * out_colour;
}
