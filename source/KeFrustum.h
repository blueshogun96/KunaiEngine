//
//  frustum.h
//
//  Created by Shogun3D on 1/9/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __frustum__
#define __frustum__


/*
 * Name: KeCalculateFrustum
 * Desc: Calculates the current frustum based on the supplied modelview and projection
 *       matrices.
 */
void KeCalculateFrustum( nv::matrix4f modelview_matrix, nv::matrix4f projection_matrix );

/*
 * Name: KePointInFrustum
 * Desc: Returns yes if this point is in the viewing frustum.
 */
bool KePointInFrustum( nv::vec3f v );

/*
 * Name: KeSphereInFrustum
 * Desc: Returns the distance between the camera and the sphere if it's within the frustum.
 */
float KeSphereInFrustum( nv::vec3f v, float radius );

/*
 * Name: KeCubeInFrustum
 * Desc: Checks each corner of the cube if it is within our viewing frustum.
 */
bool KeCubeInFrustum( nv::vec3f v, float size );

/*
 * Name: KeSphereInFrustum2
 * Desc: Same as the previous, except also capable of telling if the sphere is partly visible.
 */
int KeSphereInFrustum2( nv::vec3f v, float radius );

/*
 * Name: KeCubeInFrustum2
 * Desc: Same as the previous, except also capaoble of telling us if the cube is partly visible.
 */
int KeCubeInFrustum2( nv::vec3f v, float size );

/*
 * Name: KePolygonInFrustum
 * Desc: Tests a polygon for visibility.
 */
bool KePolygonInFrustum( int num_points, nv::vec3f* point_list );

/*
 * Name: KeProjectVertex
 * Desc: Takes a point in 3D space and returns a 2D screen coordinate. This func-
 *		 ion is useful when drawing 2D shapes over 3D objects, picking, etc.
 *		 Keep in mind that this code does not work for orthographic projections
 *		 and assumes the depth range is 0.0-1.0f.  Also, if win_coord.z's value is
 *		 greater than 1.0, then the object is behind the camera.
 */
int KeProjectVertex( nv::vec3f obj, nv::matrix4f modelview, nv::matrix4f projection, int* viewport, nv::vec3f* win_coord );

int KeUnProjectVertex( nv::matrix4f& modelview, nv::matrix4f& projection, int* viewport, nv::vec3f* win_coord, nv::vec3f* obj );


/*
 * Namespaced API
 */
namespace Ke
{
    namespace Frustum
    {
        void    (*Calculate)( nv::matrix4f, nv::matrix4f ) = KeCalculateFrustum;
        bool    (*PointVisible)( nv::vec3f ) = KePointInFrustum;
        float   (*SphereVisible)( nv::vec3f, float ) = KeSphereInFrustum;
        bool    (*CubeVisible)( nv::vec3f, float ) = KeCubeInFrustum;
        int     (*SphereVisibleEx)( nv::vec3f, float ) = KeSphereInFrustum2;
        int     (*CubeVisibleEx)( nv::vec3f, float ) = KeCubeInFrustum2;
        bool    (*PolygonVisible)( int , nv::vec3f* ) = KePolygonInFrustum;
        int     (*ProjectVertex)( nv::vec3f, nv::matrix4f, nv::matrix4f, int*, nv::vec3f* ) = KeProjectVertex;
        int     (*UnprojectVertex)( nv::matrix4f&, nv::matrix4f&, int*, nv::vec3f*, nv::vec3f* ) = KeUnProjectVertex;
    }
}
#endif /* defined(__frustum__) */
