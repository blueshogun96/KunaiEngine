#version 150

out vec4 colour;
in  vec2 out_tex0;
in  vec2 out_tex1;
in  vec4 out_colour;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{
    /* Compute final fragment ( (tex0+tex1)*diffuse ) */
	colour = ( /*texture( tex0, out_tex0 ) +*/ texture( tex1, out_tex1 ) ) * out_colour; 
}