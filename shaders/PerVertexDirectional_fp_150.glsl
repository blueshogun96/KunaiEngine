#version 150

out vec4 colour;
in  vec4 out_colour;

void main(void)
{
    /* Pass through fragment colour */
	colour = out_colour; 
}