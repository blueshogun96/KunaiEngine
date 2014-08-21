#version 150

in vec2 in_pos;
out vec2 out_tex0;

void main(void)
{
    gl_Position = vec4( in_pos, 0.0, 1.0 );
    out_tex0 = ( in_pos + 1.0 ) / 2.0;
}