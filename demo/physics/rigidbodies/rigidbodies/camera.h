//
//  camera.h
//  rigidbodies
//
//  Created by Shogun3D on 4/21/15.
//  Copyright (c) 2015 Shogun3D. All rights reserved.
//

#ifndef __rigidbodies__camera__
#define __rigidbodies__camera__

#include <NV/NvMath.h>

/* First person camera class */
class fp_camera
{
public:
    fp_camera();
    
    void look_at( nv::vec3f eye, nv::vec3f lookat, nv::vec3f up );
    void rotate( float angle, float x, float y, float z );
    void move( float speed );
    nv::matrix4f get_view_matrix() const;
    
    nv::vec3f m_eye, m_lookat, m_up;
};

#endif /* defined(__rigidbodies__camera__) */
