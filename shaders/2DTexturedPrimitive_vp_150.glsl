#version 150

in  vec4 in_pos;		/* 2D Position (xy) 2D TexCoords (zw) */
out vec4 out_colour;	/* Output diffuse colour */
out vec2 out_tex;		/* Output 2D texture coordinates */

/* Matrices */
uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

uniform float z_value;
uniform vec4 diffuse;

void main( void )
{
	/* Multiply the vertex position by the world-view-projection matrix */
	mat4 wvp = proj * view * world;
	gl_Position = wvp * vec4( in_pos.xy, z_value, 1.0 );

	/* Passthrough texture coordinates and use white as the default colour */
	out_tex = in_pos.zw;
	out_colour = diffuse;
}
