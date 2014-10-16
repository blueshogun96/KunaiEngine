#version 150

in  vec3 in_pos;
in  vec3 in_normal;
in vec2 in_tex0;
out vec4 out_colour;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;
uniform vec4 diffuse;

void main( void )
{
    /* WorldViewProjection matrix */
    mat4 wvp = proj * view * world;
    
    /* Transform vertex */
    gl_Position = wvp * vec4(in_pos, 1.0);
    
    /* Set constant vertex colour */
    out_colour = diffuse;
}