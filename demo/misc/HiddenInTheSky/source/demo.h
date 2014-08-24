//
//  demo.h
//  HiddenInTheSky
//
//  Created by Shogun3D on 8/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __HiddenInTheSky__demo__
#define __HiddenInTheSky__demo__

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
#include "camera.h"

/* Rigid body demo class */
class hiddendemo_t
{
public:
    hiddendemo_t();
    virtual ~hiddendemo_t();
    
public:
    bool initialize();
    void uninitialize();
    void on_render();
    void on_update();
    /*static void on_keyboard( void* context, void* input_context );
    static void on_mouse( void* context, void* input_context );
    static void on_gamepad( void* context, void* input_context );*/
    
    /*ke_*/camera_t* get_camera()
    {
        return &m_camera;
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
    ke_renderdevice_t*      m_renderdevice;         /* Rendering device */
    ke_audiodevice_t*       m_audiodevice;          /* Audio device */
    
    FrameRate               m_framerate;            /* Frame rate class */
    
    ke_geometrybuffer_t*    m_sphere;               /* Sphere mesh */
    int                     m_sphere_indices;       
    
    ke_gpu_program_t*       m_default_program;      /* Default program */
    ke_gpu_program_t*       m_vertex_light_program; /* Per-vertex lighting program */
    
    /*ke_*/camera_t         m_camera;               /* Camera class */
    
    ke_physics_simulator_t* m_physics;              /* Physics engine */
};

#endif /* defined(__HiddenInTheSky__demo__) */
