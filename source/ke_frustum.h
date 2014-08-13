//
//  frustum.h
//
//  Created by Shogun3D on 1/9/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __frustum__
#define __frustum__


/*
 * Name: ke_calculate_frustum
 * Desc: Calculates the current frustum based on the supplied modelview and projection
 *       matrices.
 */
void ke_calculate_frustum( Matrix4 modelview_matrix, Matrix4 projection_matrix );

/*
 * Name: ke_point_in_frustum
 * Desc: Returns yes if this point is in the viewing frustum.
 */
bool ke_point_in_frustum( VmathVector3 v );

/*
 * Name: ke_sphere_in_frustum
 * Desc: Returns the distance between the camera and the sphere if it's within the frustum.
 */
float ke_sphere_in_frustum( VmathVector3 v, float radius );

/*
 * Name: ke_cube_in_frustum
 * Desc: Checks each corner of the cube if it is within our viewing frustum.
 */
bool ke_cube_in_frustum( VmathVector3 v, float size );

/*
 * Name: ke_sphere_in_frustum2
 * Desc: Same as the previous, except also capable of telling if the sphere is partly visible.
 */
int ke_sphere_in_frustum2( VmathVector3 v, float radius );

/*
 * Name: ke_cube_in_frustum2
 * Desc: Same as the previous, except also capaoble of telling us if the cube is partly visible.
 */
int ke_cube_in_frustum2( VmathVector3 v, float size );

/*
 * Name: ke_polygon_in_frustum
 * Desc: Tests a polygon for visibility.
 */
bool ke_polygon_in_frustum( int num_points, VmathVector3* point_list );

/*
 * Name: ke_project_vertex
 * Desc: Takes a point in 3D space and returns a 2D screen coordinate. This func-
 *		 ion is useful when drawing 2D shapes over 3D objects, picking, etc.
 *		 Keep in mind that this code does not work for orthographic projections
 *		 and assumes the depth range is 0.0-1.0f.  Also, if win_coord.z's value is
 *		 greater than 1.0, then the object is behind the camera.
 */
int ke_project_vertex( VmathVector3* obj, float* modelview, float* projection, int* viewport, VmathVector3* win_coord );

#endif /* defined(__frustum__) */
