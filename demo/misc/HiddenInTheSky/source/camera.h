//
//  camera.h
//  HiddenInTheSky
//
//  Created by Shogun3D on 8/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __HiddenInTheSky__camera__
#define __HiddenInTheSky__camera__

#include "ke_platform.h"
#include <vectormath.h>

class camera_t
{
public:
    camera_t();
    
    void on_mouse_move( int x, int y );
    void update();
    Matrix4 get_view_matrix();
    
    int mouse_x, mouse_y, mouse_lx, mouse_ly;
    Vector3 eye, look, up, right;
    Matrix4 view;
};

#endif /* defined(__HiddenInTheSky__camera__) */
