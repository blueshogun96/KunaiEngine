//
//  rigidbodydemo.h
//  rigidbodies
//
//  Created by Shogun3D on 8/13/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __rigidbodies__rigidbodydemo__
#define __rigidbodies__rigidbodydemo__

#include "ke.h"
#include "ke_system.h"
#include "ke_renderdevice.h"
#include "ke_audiodevice.h"
#include "ke_frustum.h"
#include "ke_camera.h"
#include "ke_mesh.h"
#include "ke_physics.h"
#include "ke_process.hpp"
#include "linkedlist.h"
#include "ke_capture.h"


/* Rigid body demo class */
class rigidbodydemo_t
{
public:
    rigidbodydemo_t();
    virtual ~rigidbodydemo_t();
    
public:
    bool initialize();
    void uninitialize();
    void on_render();
    void on_update();
    static void on_keyboard( void* context, void* input_context );
    static void on_mouse( void* context, void* input_context );
    static void on_gamepad( void* context, void* input_context );

    ke_camera_t* get_camera()
    {
        return m_camera;
    }

    ke_physics_simulator_t* get_physics()
    {
        return m_physics;
    }

    int main_loop();

protected:
    bool create_geometry();
    void delete_geometry();
    bool create_programs();
    void delete_programs();
    bool start_physics();
    void stop_physics();

    std::string get_shader_string( std::string );

protected:
    ke_renderdevice_t*      m_renderdevice;
    ke_audiodevice_t*       m_audiodevice;
    FrameRate               m_framerate;
    ke_geometrybuffer_t*    m_box;
    ke_geometrybuffer_t*    m_ball;
    ke_gpu_program_t*       m_default_program;
    ke_gpu_program_t*       m_vertex_light_program;
    ke_camera_t*            m_camera;
    ke_physics_simulator_t* m_physics;
    uint32_t                m_rigid_body_id[5], m_animated_body_id;
    int                     m_width, m_height;

    int m_box_indices, m_ball_indices;
};

#endif /* defined(__rigidbodies__rigidbodydemo__) */
