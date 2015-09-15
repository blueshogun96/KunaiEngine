#version 150

in  vec3 in_pos;
in  vec3 in_normal;
out vec4 out_colour;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 light_dir;

void main(void)
{
    /* WorldViewProjection matrix */
    mat4 wvp = proj * view * world;
    
    /* Create normal matrix */
    mat4 normal_mtx = view * world;
    normal_mtx = transpose( inverse( normal_mtx ) );
    
    /* Transform normal and normalize */
    vec4 normal = normal_mtx * vec4( in_normal.xyz, 1 );
    normal = normalize( normal.xyzz );
    
    /* Calculate light direction */
    float nl = dot( normal, light_dir );
    
    /* Transform vertex */
    gl_Position = wvp * vec4(in_pos, 1.0);
    
    /* Calculate vertex colour */
    out_colour = ( diffuse * nl ) + ambient;
}

