//
//  frustum.cpp
//
//  Created by Shogun3D on 1/9/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//


#include "KePlatform.h"
#include "NV/NvMath.h"
#include "KeFrustum.h"



/*
 * Globals
 */

float frustum[6][4];    /* The frustum created from the supplied modelview and projection matrices */



/*
 * Name: KeCalculateFrustum
 * Desc: Calculates the current frustum based on the supplied modelview and projection
 *       matrices.
 */
void KeCalculateFrustum( nv::matrix4f modelview_matrix, nv::matrix4f projection_matrix )
{
    float   proj[16];
    float   modl[16];
    float   clip[16];
    float   t;
    
    memcpy( modl, modelview_matrix._array, sizeof( float ) * 16 );
    memcpy( proj, projection_matrix._array, sizeof( float ) * 16 );
    
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
 * Name: KePointInFrustum
 * Desc: Returns yes if this point is in the viewing frustum.
 */
bool KePointInFrustum( nv::vec3f v )
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
 * Name: KeSphereInFrustum
 * Desc: Returns the distance between the camera and the sphere if it's within the frustum.
 */
float KeSphereInFrustum( nv::vec3f v, float radius )
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
 * Name: KeCubeInFrustum
 * Desc: Checks each corner of the cube if it is within our viewing frustum.
 */
bool KeCubeInFrustum( nv::vec3f v, float size )
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
 * Name: KeSphereInFrustum2
 * Desc: Same as the previous, except also capable of telling if the sphere is partly visible.
 */
int KeSphereInFrustum2( nv::vec3f v, float radius )
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
 * Name: KeCubeInFrustum2
 * Desc: Same as the previous, except also capable of telling us if the cube is partly visible.
 */
int KeCubeInFrustum2( nv::vec3f v, float size )
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
 * Name: KePolygonInFrustum
 * Desc: Tests a polygon for visibility.
 */
bool KePolygonInFrustum( int num_points, nv::vec3f* point_list )
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
 * Name: KeProjectVertex
 * Desc: Takes a point in 3D space and returns a 2D screen coordinate. This func-
 *		 ion is useful when drawing 2D shapes over 3D objects, picking, etc.
 *		 Keep in mind that this code does not work for orthographic projections
 *		 and assumes the depth range is 0.0-1.0f.  Also, if win_coord.z's value is
 *		 greater than 1.0, then the object is behind the camera.
 */
int KeProjectVertex( nv::vec3f obj, nv::matrix4f modelview, nv::matrix4f projection, int* viewport, nv::vec3f* win_coord )
{
	/* Transformation vectors */
    nv::vec4f tv1, tv2;
    
#if 1
	/* Modelview transformation */
	tv1.x = modelview._array[0]*obj.x+modelview._array[4]*obj.y+modelview._array[8]*obj.z+modelview._array[12];
	tv1.y = modelview._array[1]*obj.x+modelview._array[5]*obj.y+modelview._array[9]*obj.z+modelview._array[13];
	tv1.z = modelview._array[2]*obj.x+modelview._array[6]*obj.y+modelview._array[10]*obj.z+modelview._array[14];
	tv1.w = modelview._array[3]*obj.x+modelview._array[7]*obj.y+modelview._array[11]*obj.z+modelview._array[15];
    
	/* Projection transformation */
	tv2.x = projection._array[0]*tv1.x+projection._array[4]*tv1.y+projection._array[8]*tv1.z+projection._array[12]*tv1.w;
	tv2.y = projection._array[1]*tv1.x+projection._array[5]*tv1.y+projection._array[9]*tv1.z+projection._array[13]*tv1.w;
	tv2.z = projection._array[2]*tv1.x+projection._array[6]*tv1.y+projection._array[10]*tv1.z+projection._array[14]*tv1.w;
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
	win_coord->y = viewport[3] - ((tv2.y*0.5f+0.5f)*viewport[3]+viewport[1]);
	win_coord->z = (1.0f+tv2.z)*0.5f;
#else
    memmove( tv1._array, obj._array, sizeof(float)*3 );
    tv1.w = 1.0f;
    
    /* Modelview and projeection transformation */
    tv2 = modelview * tv1;
    tv1 = projection * tv2;
    
    if( tv1.w == 0.0f )
        return 0;
    
    /* Perspective division */
    tv1.x /= tv1.w;
    tv1.y /= tv1.w;
    tv1.z /= tv1.w;
    
    /* Map xyz to 0-1 range */
    tv1.x = tv1.x * 0.5f + 0.5f;
    tv1.y = tv1.y * 0.5f + 0.5f;
    tv1.z = tv1.z * 0.5f + 0.5f;
    
    /* Map xy to viewport */
    tv1.x = tv1.x * float(viewport[2]) + float(viewport[0]);
    tv1.y = viewport[3] - (tv1.y * float(viewport[3]) + float(viewport[1]));
    
    /* Final coordinates */
    win_coord->x = tv1.x;
    win_coord->y = tv1.y;
    win_coord->z = tv1.z;
#endif
    
	return 1;
}


int KeUnProjectVertex( nv::matrix4f& modelview, nv::matrix4f& projection, int* viewport, nv::vec3f* win_coord, nv::vec3f* obj )
{
    //Transformation matrices
    nv::matrix4f m, A;
    nv::vec4f in, out;
    
    //Calculation for inverting a matrix, compute projection x modelview
    //and store in A[16]
    A = projection * modelview;
    
    //Now compute the inverse of matrix A
    m = nv::inverse(A);
    
    //Transformation of normalized coordinates between -1 and 1
    in.x = ( win_coord->x - (float)viewport[0] ) / (float)viewport[2] * 2.0f - 1.0f;
    in.y = ( win_coord->y - (float)viewport[1] ) / (float)viewport[3] * 2.0f - 1.0f;
    in.z = 2.0f * win_coord->z - 1.0f;
    in.w = 1.0f;
    
    //Objects coordinates
    out = m * in;
    if( out.w == 0.0f )
        return 0;
    
    out.w = 1.0f / out.w;
    obj->x = out.x * out.w;
    obj->y = out.y * out.w;
    obj->z = out.z * out.w;
    
    return 1;
}

/*
 int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
 {
 //Transformation matrices
 float m[16], A[16];
 float in[4], out[4];
 //Calculation for inverting a matrix, compute projection x modelview
 //and store in A[16]
 MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
 //Now compute the inverse of matrix A
 if(glhInvertMatrixf2(A, m)==0)
 return 0;
 //Transformation of normalized coordinates between -1 and 1
 in[0]=(winx-(float)viewport[0])/(float)viewport[2]*2.0-1.0;
 in[1]=(winy-(float)viewport[1])/(float)viewport[3]*2.0-1.0;
 in[2]=2.0*winz-1.0;
 in[3]=1.0;
 //Objects coordinates
 MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
 if(out[3]==0.0)
 return 0;
 out[3]=1.0/out[3];
 objectCoordinate[0]=out[0]*out[3];
 objectCoordinate[1]=out[1]*out[3];
 objectCoordinate[2]=out[2]*out[3];
 return 1;
 }
 */
