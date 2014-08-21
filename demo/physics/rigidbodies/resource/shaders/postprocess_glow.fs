#version 150

uniform sampler2D tex0;
in vec2 out_tex0;
out vec4 out_colour;

void main()
{
     vec4 sum = vec4(0);
     vec2 texcoord = vec2(out_tex0);
     int j;
     int i;

     for( i= -4; i < 4; i++ )
     {
         for( j = -3; j < 3; j++ )
         {
             sum += texture2D( tex0, texcoord + vec2( j, i ) * 0.004 ) * 0.25;
         }
     }
     if( texture2D( tex0, texcoord ).r < 0.3 )
     {
         out_colour = sum*sum*0.012 + texture2D( tex0, texcoord );
     }
     else
     {
         if( texture2D( tex0, texcoord ).r < 0.5 )
         {
             out_colour = sum*sum*0.009 + texture2D( tex0, texcoord );
         }
         else
         {
             out_colour = sum*sum*0.0075 + texture2D( tex0, texcoord );
         }
     }
 }