//
//  ke_camera.cpp
//  example03
//
//  Created by Shogun3D on 8/4/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_system.h"
#include "ke_camera.h"



void ke_default_keyboard_callback( ke_camera_t* camera, void* context, int down )
{
    
}

void ke_default_gamepad_callback( ke_camera_t* camera, void* context, int down )
{
    
}

ke_camera_t* ke_new_camera()
{
    /* Allocate a new camera object */
    ke_camera_t* camera = new ke_camera_t();
    if( camera )
    {
        /* Set some default keyboard and gamepad callbacks */
        camera->set_keyboard_callback( ke_default_keyboard_callback );
        camera->set_gamepad_callback( ke_default_gamepad_callback );
        
        return camera;
    }
    
    return NULL;
}