//
//  camera.h
//  HiddenInTheSky
//
//  Created by Shogun3D on 8/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __HiddenInTheSky__camera__
#define __HiddenInTheSky__camera__

#include "ke_system.h"
#include <vectormath.h>

class camera_t
{
public:
    camera_t()
    {
        eye = vmathV3MakeFromElems_V( -8.5f, 1.8f, 11.1f );
        look = vmathV3MakeFromElems_V( 0.6f, 0.0f, -0.75f );
        up = vmathV3MakeFromElems_V( 0.0f, 1.0f, 0.0f );
        right = vmathV3MakeFromElems_V( 1.0f, 0.0f, 0.0f );
    }
    
    void    update();
    Matrix4 get_view_matrix();
    
    int     mouse_x, mouse_y, mouse_lx, mouse_ly;
    Vector3 eye, look, up, right;
    Matrix4 view;
};

#endif /* defined(__HiddenInTheSky__camera__) */
