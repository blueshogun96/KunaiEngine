#version 150

in vec3 in_pos;
in vec3 in_normal;
out float tex_coord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

uniform vec4 light_dir;
uniform vec3 eye_point;
uniform float tex_offset;


void main()
{
	/* WorldViewProjection matrix */
    mat4 wvp = proj * view * world;
    
	/* Transform position */
	gl_Position = wvp * vec4( in_pos, 1.0 );

    /* Create normal matrix */
    mat4 normal_mtx = view * world;
    normal_mtx = transpose( inverse( normal_mtx ) );

	/* Transform and normalize normal */
    vec4 normal = normal_mtx * vec4( in_normal.xyz, 1 );
    normal = normalize( normal.xyzz );

	/* World space position */
	vec4 ws_pos = world * vec4( in_pos, 1.0 );

	/* Vector from point to eye */
	vec3 eye = eye_point.xyz - ws_pos.xyz;
	eye = normalize( eye );

	/* Eye dot N, compute texture coordinate */
	tex_coord = dot( eye, normal.xyz ) + tex_offset;
}


/* Original shader
; v0  -- position
; v3  -- normal
; v7  -- tex coord

;
; c0-3   -- world/view/proj matrix
; c4     -- light vector
; c5-8   -- inverse/transpose world matrix
; c9     -- {0.0, 0.5, 1.0, -1.0}
; c10    -- eye point
; c11-14 -- world matrix
; c20    -- possible tex coord offset

vs.1.0

;transform position
dp4 oPos.x, v0, c0
dp4 oPos.y, v0, c1
dp4 oPos.z, v0, c2
dp4 oPos.w, v0, c3

;transform normal
dp3 r0.x, v3, c5
dp3 r0.y, v3, c6
dp3 r0.z, v3, c7

;normalize normal
dp3 r0.w, r0, r0
rsq r0.w, r0.w
mul r0, r0, r0.w

;compute world space position
dp4 r1.x, v0, c11
dp4 r1.y, v0, c12
dp4 r1.z, v0, c13
dp4 r1.w, v0, c14

;vector from point to eye
add r2, c10, -r1

;normalize e
dp3 r2.w, r2, r2
rsq r2.w, r2.w
mul r2, r2, r2.w


; eye dot n
dp3 r1, r2, r0
add oT0.x, r1, c[20]    ; add const offset held in c[20]
                        ;  This animates tex coords without requiring an update
                        ;  of the vertex buffer data.


slt oT1, r1, r1            ; set oT1 to zero


*/