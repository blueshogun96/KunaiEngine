//
//  frustum.cpp
//
//  Created by Shogun3D on 1/9/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//


#include "ke_platform.h"
#include "vectormath.h"
#include "ke_frustum.h"



/*
 * Globals
 */

float frustum[6][4];    /* The frustum created from the supplied modelview and projection matrices */



/*
 * Name: calculate_frustum
 * Desc: Calculates the current frustum based on the supplied modelview and projection
 *       matrices.
 */
void calculate_frustum( Matrix4 modelview_matrix, Matrix4 projection_matrix )
{
    float   proj[16];
    float   modl[16];
    float   clip[16];
    float   t;
    
    memcpy( modl, &modelview_matrix.col0.x, sizeof( float ) * 16 );
    memcpy( proj, &projection_matrix.col0.x, sizeof( float ) * 16 );
    
    /* Combine the two matrices (multiply projection by modelview) */
    clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
    clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
    clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
    clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
    
    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
    clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
    clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
    clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
    
    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
    clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
    clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
    
    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
    
    /* Extract the numbers for the RIGHT plane */
    frustum[0][0] = clip[ 3] - clip[ 0];
    frustum[0][1] = clip[ 7] - clip[ 4];
    frustum[0][2] = clip[11] - clip[ 8];
    frustum[0][3] = clip[15] - clip[12];
    
    /* Normalize the result */
    t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
    frustum[0][0] /= t;
    frustum[0][1] /= t;
    frustum[0][2] /= t;
    frustum[0][3] /= t;
    
    /* Extract the numbers for the LEFT plane */
    frustum[1][0] = clip[ 3] + clip[ 0];
    frustum[1][1] = clip[ 7] + clip[ 4];
    frustum[1][2] = clip[11] + clip[ 8];
    frustum[1][3] = clip[15] + clip[12];
    
    /* Normalize the result */
    t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
    frustum[1][0] /= t;
    frustum[1][1] /= t;
    frustum[1][2] /= t;
    frustum[1][3] /= t;
    
    /* Extract the BOTTOM plane */
    frustum[2][0] = clip[ 3] + clip[ 1];
    frustum[2][1] = clip[ 7] + clip[ 5];
    frustum[2][2] = clip[11] + clip[ 9];
    frustum[2][3] = clip[15] + clip[13];
    
    /* Normalize the result */
    t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
    frustum[2][0] /= t;
    frustum[2][1] /= t;
    frustum[2][2] /= t;
    frustum[2][3] /= t;
    
    /* Extract the TOP plane */
    frustum[3][0] = clip[ 3] - clip[ 1];
    frustum[3][1] = clip[ 7] - clip[ 5];
    frustum[3][2] = clip[11] - clip[ 9];
    frustum[3][3] = clip[15] - clip[13];
    
    /* Normalize the result */
    t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
    frustum[3][0] /= t;
    frustum[3][1] /= t;
    frustum[3][2] /= t;
    frustum[3][3] /= t;
    
    /* Extract the FAR plane */
    frustum[4][0] = clip[ 3] - clip[ 2];
    frustum[4][1] = clip[ 7] - clip[ 6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];
    
    /* Normalize the result */
    t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
    frustum[4][0] /= t;
    frustum[4][1] /= t;
    frustum[4][2] /= t;
    frustum[4][3] /= t;
    
    /* Extract the NEAR plane */
    frustum[5][0] = clip[ 3] + clip[ 2];
    frustum[5][1] = clip[ 7] + clip[ 6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];
    
    /* Normalize the result */
    t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
    frustum[5][0] /= t;
    frustum[5][1] /= t;
    frustum[5][2] /= t;
    frustum[5][3] /= t;
}


/*
 * Name: point_in_frustum
 * Desc: Returns yes if this point is in the viewing frustum.
 */
bool point_in_frustum( VmathVector3 v )
{
    int p;
    
    /* Calculate the distance between this point and each plane. If the result is positive for each
       plane, then the points are inside the frustum. */
    
    for( p = 0; p < 6; p++ )
        if( frustum[p][0] * v.x + frustum[p][1] * v.y + frustum[p][2] * v.z + frustum[p][3] <= 0 )
            return No;
    
    return Yes;
}


/*
 * Name: sphere_in_frustum
 * Desc: Returns the distance between the camera and the sphere if it's within the frustum.
 */
float sphere_in_frustum( VmathVector3 v, float radius )
{
    int p;
    float d;
    
    /* Calculate the distance between this sphere and each plane. If the sphere is in front of each
       plane, then return the distance from the last plane to the sphere. If not, return 0. */
    
    for( p = 0; p < 6; p++ )
    {
        d = frustum[p][0] * v.x + frustum[p][1] * v.y + frustum[p][2] * v.z + frustum[p][3];
        if( d <= -radius )
            return 0;
    }
    
    return d + radius;
}


/*
 * Name: cube_in_frustum
 * Desc: Checks each corner of the cube if it is within our viewing frustum.
 */
bool cube_in_frustum( VmathVector3 v, float size )
{
    int p;
    
    for( p = 0; p < 6; p++ )
    {
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            continue;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            continue;
        return No;
    }
    
    return Yes;
}


/*
 * Name: sphere_in_frustum2
 * Desc: Same as the previous, except also capable of telling if the sphere is partly visible.
 */
int sphere_in_frustum2( VmathVector3 v, float radius )
{
    int p;
    int c = 0;
    float d;
    
    for( p = 0; p < 6; p++ )
    {
        d = frustum[p][0] * v.x + frustum[p][1] * v.y + frustum[p][2] * v.z + frustum[p][3];
        if( d <= -radius )
            return 0;
        if( d > radius )
            c++;
    }
    
    return (c == 6) ? 2 : 1;
}


/*
 * Name: cube_in_frustum2
 * Desc: Same as the previous, except also capable of telling us if the cube is partly visible.
 */
int cube_in_frustum2( VmathVector3 v, float size )
{
    int p;
    int c;
    int c2 = 0;
    
    for( p = 0; p < 6; p++ )
    {
        c = 0;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z - size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y - size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x - size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            c++;
        if( frustum[p][0] * (v.x + size) + frustum[p][1] * (v.y + size) + frustum[p][2] * (v.z + size) + frustum[p][3] > 0 )
            c++;
        if( c == 0 )
            return 0;
        if( c == 8 )
            c2++;
    }
    
    return (c2 == 6) ? 2 : 1;
}


/*
 * Name: polygon_in_frustum
 * Desc: Tests a polygon for visibility.
 */
bool polygon_in_frustum( int num_points, VmathVector3* point_list )
{
    int f, p;
    
    for( f = 0; f < 6; f++ )
    {
        for( p = 0; p < num_points; p++ )
        {
            if( frustum[f][0] * point_list[p].x + frustum[f][1] * point_list[p].y + frustum[f][2] * point_list[p].z + frustum[f][3] > 0 )
                break;
        }
        
        if( p == num_points )
            return false;
    }
    
    return true;
}


/*
 * Name: project_vertex
 * Desc: Takes a point in 3D space and returns a 2D screen coordinate. This func-
 *		 ion is useful when drawing 2D shapes over 3D objects, picking, etc.
 *		 Keep in mind that this code does not work for orthographic projections
 *		 and assumes the depth range is 0.0-1.0f.  Also, if win_coord.z's value is
 *		 greater than 1.0, then the object is behind the camera.
 */
int project_vertex( VmathVector3* obj, float* modelview, float* projection, int* viewport, VmathVector3* win_coord )
{
	/* Transformation vectors */
	VmathVector4 tv1, tv2;
    
	/* Modelview transformation */
	tv1.x = modelview[0]*obj->x+modelview[4]*obj->y+modelview[8]*obj->z+modelview[12];
	tv1.y = modelview[1]*obj->x+modelview[5]*obj->y+modelview[9]*obj->z+modelview[13];
	tv1.z = modelview[2]*obj->x+modelview[6]*obj->y+modelview[10]*obj->z+modelview[14];
	tv1.w = modelview[3]*obj->x+modelview[7]*obj->y+modelview[11]*obj->z+modelview[15];
    
	/* Projection transformation */
	tv2.x = projection[0]*tv1.x+projection[4]*tv1.y+projection[8]*tv1.z+projection[12]*tv1.w;
	tv2.y = projection[1]*tv1.x+projection[5]*tv1.y+projection[9]*tv1.z+projection[13]*tv1.w;
	tv2.z = projection[2]*tv1.x+projection[6]*tv1.y+projection[10]*tv1.z+projection[14]*tv1.w;
	tv2.w = -tv1.z;
    
	/* Normalize result between -1 and 1 */
	if( tv2.w == 0.0f )
		return 0;
    
	tv2.w = 1.0f / tv2.w;
    
	/* Perspective division */
	tv2.x *= tv2.w;
	tv2.y *= tv2.w;
	tv2.z *= tv2.w;
    
	/* Calculate window coordinates */
	win_coord->x = (tv2.x*0.5f+0.5f)*viewport[2]+viewport[0];
	win_coord->y = (tv2.y*0.5f+0.5f)*viewport[3]+viewport[1];
	win_coord->z = (1.0f+tv2.z)*0.5f;
    
	return 1;
}
