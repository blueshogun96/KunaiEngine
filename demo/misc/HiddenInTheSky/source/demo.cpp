//
//  demo.cpp
//  HiddenInTheSky
//
//  Created by Shogun3D on 8/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "demo.h"




hiddendemo_t::hiddendemo_t(): m_renderdevice(NULL), m_audiodevice(NULL)
{
    /* Initialize the frame rate counter */
    m_framerate.StartFPSClock();
}

hiddendemo_t::~hiddendemo_t()
{
    
}

bool hiddendemo_t::initialize()
{
    /* Set the callback context and functions for input */
    ke_set_context_pointer( this );
    /*ke_set_keyboard_callback( hiddendemo_t::on_keyboard );
    ke_set_mouse_callback( hiddendemo_t::on_mouse );
    ke_set_gamepad_callback( hiddendemo_t::on_gamepad );*/
    
    /* Now, intialize the engine */
    bool ret = ke_initialize();
    if( !ret )
        return false;
    
    /* Create our rendering device */
    ke_renderdevice_desc_t renderdevice_desc;
    renderdevice_desc.width = 640;
    renderdevice_desc.height = 480;
    renderdevice_desc.colour_bpp = 32;
    renderdevice_desc.depth_bpp = 24;
    renderdevice_desc.stencil_bpp = 8;
    renderdevice_desc.refresh_rate = 60;
    renderdevice_desc.buffer_count = 1;
    renderdevice_desc.fullscreen = No;
    renderdevice_desc.device_type = KE_RENDERDEVICE_OGL3;
    
    if( !ke_create_window_and_device( &renderdevice_desc, &m_renderdevice ) )
        return false;
    
    /* Set default matrix values */
    Matrix4 world, view, projection;
    
    world = vmathM4MakeIdentity_V();
    view = vmathM4MakeIdentity_V();
    projection = vmathM4MakePerspective_V( M_PI_4, 4.0f/3.0f, 0.1f, 100.0f );
    
    m_renderdevice->set_world_matrix( &world );
    m_renderdevice->set_view_matrix( &view );
    m_renderdevice->set_projection_matrix( &projection );
    
    /* Load resources */
    create_programs();
    create_geometry();
    
    /* Start the physics engine */
    start_physics();
    
    /* Create us a new camera */
    //m_camera = ke_new_camera();
    
    return true;
}

void hiddendemo_t::uninitialize()
{
    /* Kill the camera */
    //delete m_camera;
    
    /* Stop the physics engine */
    stop_physics();
    
    /* Kill the resources */
    delete_geometry();
    delete_programs();
    
    /* Uninitialize the engine */
    ke_destroy_window_and_device( m_renderdevice );
    ke_uninitialize();
}

void hiddendemo_t::on_render()
{
    Matrix4 world, view;
    
    float clear_colour[] = { 0.0f, 0.3f, 0.0f, 1.0f };
    float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.2, 0.2f, 0.2f, 0.2f };
    float light_dir[] = { 1.0f, 1.0f, 1.0f, 0 };
    
    /* Clear the screen and depth buffer*/
    m_renderdevice->set_clear_colour_fv( clear_colour );
    m_renderdevice->set_clear_depth( 1.0f );
    m_renderdevice->clear_colour_buffer();
    m_renderdevice->clear_depth_buffer();
    
    /* Set render states */
    ke_state_t rs[4];
    rs[0].state = KE_RS_DEPTHTEST;
    rs[0].param1 = Yes;
    rs[1].state = KE_RS_POLYGONMODE;
    rs[1].param1 = KE_FRONT_AND_BACK;
    rs[1].param2 = KE_LINE;
    rs[2].state = KE_RS_CULLMODE;
    rs[2].param1 = Yes;
    rs[2].param2 = KE_CULL_CCW;
    rs[3].state = -1;
     
     m_renderdevice->set_render_states(rs);
    
    /* Set view matrix */
    view = m_camera.get_view_matrix(); /*vmathM4MakeLookAt_V( vmathP3MakeFromElems_V(-8.5f, 1.8f, 11.1f),
                               vmathP3MakeFromElems_V(0.6f, 0.0f, -0.75f),
                               vmathV3MakeFromElems_V( 0, 1, 0 ) );*/
    m_renderdevice->set_view_matrix( &view );
    
    /* Set default program */
    m_renderdevice->set_program( m_vertex_light_program );
    m_renderdevice->set_program_constant_4fv( "light_dir", 1, light_dir );
    m_renderdevice->set_program_constant_4fv( "ambient", 1, ambient );
    m_renderdevice->set_program_constant_4fv( "diffuse", 1, white );
    
    /* Render the sphere */
    m_renderdevice->set_geometry_buffer( m_sphere );
    m_renderdevice->draw_indexed_vertices( KE_TRIANGLES, m_sphere_indices );
    
    /* Swap the buffers */
    m_renderdevice->swap();
}

void hiddendemo_t::on_update()
{
    ke_button_t keys[256];
    
    /* Update the framerate counter */
    m_framerate.UpdateFPS();
    //std::cout << m_framerate.GetFPS() << std::endl;
    
    /* Get keyboard state */
    ke_get_key_state( keys );
    
    /* Update camera */
    m_camera.update();
    
    /* Screenshot */
    if( keys['c'].pressed && keys['c'].timestamp.frames <= 1 )
    {
        ke_save_screenshot_jpg( m_renderdevice );
    }
    
    /* Update physics simulator */
    m_physics->update_simulator();
}

/*void hiddendemo_t::on_keyboard( void* context, void* input_context )
{
#if 0
    hiddendemo_t* this_ptr = static_cast<hiddendemo_t*>( context );
    SDL_Event* input_event = static_cast<SDL_Event*>( input_context );
    
    if( input_event->key.keysym.sym == 'u' )
    {
        ke_rigid_body_t* rb = this_ptr->get_physics()->get_rigid_body( this_ptr->m_rigid_body_id[0] );
        neV3 v;
        v.Set( 0, 10.0f, 0 );
        
        rb->rigid_body->SetVelocity(v);
    }
    //this_ptr->get_camera()->on_key_press(input_context);
#endif
}

void hiddendemo_t::on_mouse( void* context, void* input_context )
{
    hiddendemo_t* this_ptr = static_cast<hiddendemo_t*>( context );
    SDL_Event* input_event = static_cast<SDL_Event*>( input_context );
    
    //this_ptr->get_camera()->on_mouse_move( input_event->motion.x, input_event->motion.y );
}

void hiddendemo_t::on_gamepad( void* context, void* input_context )
{
    hiddendemo_t* this_ptr = static_cast<hiddendemo_t*>( context );
}*/

int hiddendemo_t::main_loop()
{
    /* Process our mainloop */
    while( !ke_quit_requested() )
    {
        /* Process events */
        ke_process_events();
        
        /* Render and update the demo */
        on_render();
        on_update();
    }
    
    /* Uninitialize the demo */
    uninitialize();
    delete this;
    
    return 0;
}

bool hiddendemo_t::create_geometry()
{
    ke_mesh_t mesh;
    ke_vertexattribute_t vertex_attributes[] =
    {
        { KE_VA_POSITION, 3, KE_FLOAT, No, 8*sizeof(float), 0 },
        { KE_VA_NORMAL,   3, KE_FLOAT, No, 8*sizeof(float), 3*sizeof(float) },
        { KE_VA_TEXTURE0, 2, KE_FLOAT, No, 8*sizeof(float), 6*sizeof(float) },
        { -1, 0, 0, 0, 0, 0 },
    };
    
    /* Open the box mesh */
    if( ke_open_scene( "resource/mesh/sphere.obj" ) )
    {
        /* Read in the vertex data */
        if( ke_read_mesh_vertex_data( 0, &mesh ) )
        {
            /* Create a new geometry buffer */
            m_renderdevice->create_geometry_buffer( mesh.vertices, mesh.vertex_count*8*sizeof(float), mesh.indices, mesh.index_count*sizeof(uint32_t), KE_UNSIGNED_INT, 0, vertex_attributes, &m_sphere );
            m_sphere_indices = mesh.index_count;
            
            /* Free the vertex data */
            ke_free_mesh_vertex_data( &mesh );
        }
        
        /* Close the scene */
        ke_close_scene();
    }
    
    return true;
}

void hiddendemo_t::delete_geometry()
{
    /* Delete meshes */
    m_renderdevice->delete_geometry_buffer( m_sphere );
}

bool hiddendemo_t::create_programs()
{
    std::string str_vs = get_shader_string( "resource/shaders/default.vs" );
    std::string str_fs = get_shader_string( "resource/shaders/default.fs" );
    
    if( !m_renderdevice->create_program( str_vs.c_str(), str_fs.c_str(), NULL, NULL, &m_default_program ) )
        return false;
    
    str_vs = get_shader_string( "resource/shaders/pervertex_directional.vs" );
    str_fs = get_shader_string( "resource/shaders/pervertex_directional.fs" );
    
    if( !m_renderdevice->create_program( str_vs.c_str(), str_fs.c_str(), NULL, NULL, &m_vertex_light_program ) )
        return false;
    
    return true;
}

void hiddendemo_t::delete_programs()
{
    m_renderdevice->delete_program( m_vertex_light_program );
    m_renderdevice->delete_program( m_default_program );
}

bool hiddendemo_t::start_physics()
{
    neV3 gravity;
    
    /* Initialize physics engine */
    gravity.Set( 0, -10.0f, 0 );
    m_physics = new ke_physics_simulator_t( 5, 1, gravity );
    
    return true;
}

void hiddendemo_t::stop_physics()
{
    m_physics->remove_all_animated_bodies();
    m_physics->remove_all_rigid_bodies();
    
    delete m_physics;
}

std::string hiddendemo_t::get_shader_string( std::string filename )
{
    std::ifstream file( filename );
    std::ostringstream str;
    std::string line;
    
    if( file.is_open() )
    {
        while( getline( file, line ) )
        {
            str << line << std::endl;
        }
        
        file.close();
        
        return str.str();
    }
    
    return " ";
}