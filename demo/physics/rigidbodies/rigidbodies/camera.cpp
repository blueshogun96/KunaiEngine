//
//  camera.cpp
//  rigidbodies
//
//  Created by Shogun3D on 4/21/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#include "camera.h"


fp_camera::fp_camera()
{
    m_eye = nv::vec3f( 0, 0, 0 );
    m_lookat = nv::vec3f( 0, 1, 0.5 );
    m_up = nv::vec3f( 0, 1, 0 );
}

void fp_camera::look_at( nv::vec3f eye, nv::vec3f lookat, nv::vec3f up )
{
    m_eye = eye;
    m_lookat = lookat;
    m_up = up;
}

void fp_camera::rotate( float angle, float x, float y, float z )
{
    nv::vec3f view, new_view;
    
    view = m_lookat - m_eye;
    
    float angle_cos = cosf( angle );
    float angle_sin = sinf( angle );
    
    // Find the new x position for the new rotated point
    new_view.x  = (angle_cos + (1 - angle_cos) * x * x)		* view.x;
    new_view.x += ((1 - angle_cos) * x * y - z * angle_sin)	* view.y;
    new_view.x += ((1 - angle_cos) * x * z + y * angle_sin)	* view.z;
    
    // Find the new y position for the new rotated point
    new_view.y  = ((1 - angle_cos) * x * y + z * angle_sin)	* view.x;
    new_view.y += (angle_cos + (1 - angle_cos) * y * y)		* view.y;
    new_view.y += ((1 - angle_cos) * y * z - x * angle_sin)	* view.z;
    
    // Find the new z position for the new rotated point
    new_view.z  = ((1 - angle_cos) * x * z - y * angle_sin)	* view.x;
    new_view.z += ((1 - angle_cos) * y * z + x * angle_sin)	* view.y;
    new_view.z += (angle_cos + (1 - angle_cos) * z * z)		* view.z;
    
    m_lookat = m_eye + new_view;
}

void fp_camera::move( float speed )
{
    nv::vec3f vec = nv::vec3f( 0, 0, 0 );
    
    vec = m_lookat - m_eye;
    
    m_eye.x += vec.x * speed;
    m_eye.z += vec.z * speed;
    
    m_lookat.x += vec.x * speed;
    m_lookat.z += vec.z * speed;
}


nv::matrix4f fp_camera::get_view_matrix() const
{
    nv::matrix4f view;
    
    return nv::lookAt( view, m_eye, m_lookat, m_up );
}

