//
//  ke_camera.cpp
//
//  Created by Shogun3D on 8/4/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_system.h"
#include "ke_camera.h"



void ke_default_keyboard_callback( ke_camera_t* camera, void* context )
{
    SDL_Event* input_event = static_cast<SDL_Event*>( context );
    static float rotation_speed = 1.0f;
    char k = input_event->key.state == SDL_PRESSED ? input_event->key.keysym.sym : 0;
    
	if( k == 'q' )
    {
		camera->rotation.x += rotation_speed/20.0f;
		if(camera->rotation.x > 360) camera->rotation.x -= 360;
    }
    if( k == 'z' )
    {
		camera->rotation.x -= rotation_speed/20.0f;
		if(camera->rotation.x < -360) camera->rotation.x += 360;
    }
    
	if( k == 'c' )
    {
		camera->rotation.y += rotation_speed;
		if(camera->rotation.y > 360) camera->rotation.y -= 360;
    }
    if( k == 'v' )
    {
		camera->rotation.y -= rotation_speed;
		if(camera->rotation.y < -360) camera->rotation.y += 360;
    }
    
    if( k == 'w' )
    {
		float xrotrad, yrotrad;
		yrotrad = (camera->rotation.y / 180.0f * 3.141592654f);
		xrotrad = (camera->rotation.x / 180.0f * 3.141592654f);
		camera->position.x += (float)(sin(yrotrad));
		camera->position.z -= (float)(cos(yrotrad));
		camera->position.y -= (float)(sin(xrotrad));
    }
    if( k == 's' )
    {
		float xrotrad, yrotrad;
		yrotrad = (camera->rotation.y / 180.0f * 3.141592654f);
		xrotrad = (camera->rotation.x / 180.0f * 3.141592654f);
		camera->position.x -= (float)(sin(yrotrad));
		camera->position.z += (float)(cos(yrotrad));
		camera->position.y += (float)(sin(xrotrad));
    }
    if( k == 'd' )
    {
		float yrotrad;
		yrotrad = (camera->rotation.y / 180.0f * 3.141592654f);
		camera->position.x += (float)(cos(yrotrad)) * 0.5f;
		camera->position.z += (float)(sin(yrotrad)) * 0.5f;
    }
    if( k =='a' )
    {
		float yrotrad;
		yrotrad = (camera->rotation.y / 180.0f * 3.141592654f);
		camera->position.x -= (float)(cos(yrotrad)) * 0.5f;
		camera->position.z -= (float)(sin(yrotrad)) * 0.5f;
    }
}

void ke_default_gamepad_callback( ke_camera_t* camera, void* context )
{
    SDL_Event* input_event = static_cast<SDL_Event*>( context );
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