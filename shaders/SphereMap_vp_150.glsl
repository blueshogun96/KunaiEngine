#version 150

in  vec3 in_pos;
in  vec3 in_normal;
in  vec2 in_tex0;
out vec2 out_tex0;
out vec2 out_tex1;
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

	/* Pass though stage 0 texture coordinates */
	out_tex0 = in_tex0;

	/* Do spheremap calulations */
	mat4 modelview = view * world;
	vec3 u = normalize( vec3( modelview * vec4( in_pos, 1 ) ) );
	vec3 n = normal.xyz;
	vec3 r = reflect( u, n );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	out_tex1.x = r.x/m + 0.5;
	out_tex1.y = r.y/m + 0.5;
}

/*
for(i=0; i<total; i++) {
   myEyeVertex = MatrixTimesVector(ModelviewMatrix, myVertex[i]);
   myEyeVertex.x -= ModelviewMatrix._41;
   myEyeVertex.y -= ModelviewMatrix._42;
   myEyeVertex.z -= ModelviewMatrix._43;
   myEyeNormal = Normalize(VectorTimesMatrix(myNormal[i], InverseModelviewMatrix));
   reflectionVector = myEyeVertex - myEyeNormal * 2.0 * dot3D(myEyeVertex, myEyeNormal);
   reflectionVector.z -= 1.0;
   m = 1.0 / (2.0 * sqrt(dot3D(reflectionVector, reflectionVector)));
   myTexCoord[i].s = -(reflectionVector.x * m + 0.5);
   myTexCoord[i].t = -(reflectionVector.y * m + 0.5);
 }*/