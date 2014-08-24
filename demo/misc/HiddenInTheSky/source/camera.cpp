//
//  camera.cpp
//  HiddenInTheSky
//
//  Created by Shogun3D on 8/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "camera.h"


void camera_t::update()
{
    ke_mouse_t mouse;
    Matrix4 rotation;
    Vector4 temp_look, temp_up;
    
    /* Get mouse position */
    ke_get_mouse_state( &mouse );
    
    mouse_x = mouse.x;
    mouse_y = mouse.y;
    
    if( mouse.button[0] )
    {
        int diffx = mouse_x - mouse_lx;
        int diffy = mouse_y - mouse_ly;
        
        if( diffy != 0 )
        {
            float rad = (float(diffy) / 3.0f) * (M_PI/180.0f);
            rotation = vmathM4MakeRotationAxis_V( -rad, right );
            temp_look = vmathM4MulV3_V( rotation, look );
            temp_up = vmathM4MulV3_V( rotation, up );
            
            memmove( &look, &temp_look, sizeof(Vector3) );
            memmove( &up, &temp_up, sizeof(Vector3) );
        }
        
        if( diffx != 0 )
        {
            Vector3 vup = { 0, 1, 0 };
            
            float rad = (float(diffx) / 3.0f) * (M_PI/180.0f);
            rotation = vmathM4MakeRotationAxis_V( -rad, vup );
            temp_look = vmathM4MulV3_V( rotation, look );
            temp_up = vmathM4MulV3_V( rotation, up );
            
            memmove( &look, &temp_look, sizeof(Vector3) );
            memmove( &up, &temp_up, sizeof(Vector3) );
        }
    }
    
    mouse_lx = mouse_x;
    mouse_ly = mouse_y;
    
    /* Get keyboard input */
    ke_button_t keys[256];
    
    ke_get_key_state( keys );
    Vector3 tlook = look, tright = right;
    Vector3 speed = { 0.5f, 0.5f, 0.5f };
    
    if( keys['w'].pressed )
    {
        tlook = vmathV3MulPerElem_V( tlook, vmathV3Neg_V(speed) );
        eye = vmathV3Sub_V( eye, tlook );
    }
    
    if( keys['s'].pressed )
    {
        tlook = vmathV3MulPerElem_V( tlook, vmathV3Neg_V(speed) );
        eye = vmathV3Add_V( eye, tlook );
    }
    
    if( keys['a'].pressed )
    {
        tright = vmathV3MulPerElem_V( tright, speed );
        eye = vmathV3Sub_V( eye, tright );
    }
    
    if( keys['d'].pressed )
    {
        tright = vmathV3MulPerElem_V( tright, speed );
        eye = vmathV3Add_V( eye, tright );
    }
}

Matrix4 camera_t::get_view_matrix()
{
    Matrix4 ret;
    float temp[16];
    
    vmathM4MakeIdentity( (Matrix4*) temp );
    
    look = vmathV3Normalize_V( look );
    
    right = vmathV3Cross_V( look, up );
    right = vmathV3Normalize_V( right );
    
    up = vmathV3Cross_V( right, look );
    up = vmathV3Normalize_V( up );
    
    temp[0] = right.x;
    temp[1] = up.x;
    temp[2] = -look.x;
    temp[3] = 0.0f;
    
    temp[4] = right.y;
    temp[5] = up.y;
    temp[6] = -look.y;
    temp[7] = 0.0f;
    
    temp[8] = right.z;
    temp[9] = up.z;
    temp[10] = -look.z;
    temp[11] = 0.0f;
    
    temp[12] = -vmathV3Dot_V( right, eye );
    temp[13] = -vmathV3Dot_V( up, eye );
    temp[14] = vmathV3Dot_V( look, eye );
    temp[15] = 1.0f;
    
    memmove( &ret, temp, sizeof( Matrix4 ) );
    
    return ret;
}
