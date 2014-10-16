//
//  rigidbodydemo.cpp
//  rigidbodies
//
//  Created by Shogun3D on 8/13/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "rigidbodydemo.h"


#define NET3_TO_MATRIX4X4f( tmat )          {                              \
{ tmat.rot[0][0], tmat.rot[1][0], tmat.rot[2][0], tmat.pos[0] },  \
{ tmat.rot[0][1], tmat.rot[1][1], tmat.rot[2][1], tmat.pos[1] },  \
{ tmat.rot[0][2], tmat.rot[1][2], tmat.rot[2][2], tmat.pos[2] },  \
{ 0.0f,          0.0f,          0.0f,          1.0f           } }

rigidbodydemo_t::rigidbodydemo_t(): m_renderdevice(NULL), m_audiodevice(NULL)
{
    /* Initialize the frame rate counter */
    m_framerate.StartFPSClock();
}

rigidbodydemo_t::~rigidbodydemo_t()
{
    
}

bool rigidbodydemo_t::initialize()
{
    /* Set the callback context and functions for input */
    ke_set_context_pointer( this );
    ke_set_keyboard_callback( rigidbodydemo_t::on_keyboard );
    ke_set_mouse_callback( rigidbodydemo_t::on_mouse );
    ke_set_gamepad_callback( rigidbodydemo_t::on_gamepad );
    
    /* Now, intialize the engine */
    bool ret = ke_initialize();
    if( !ret )
        return false;
    
    /* Create our rendering device */
    ke_renderdevice_desc_t renderdevice_desc;
#if defined(__MOBILE_OS__) && defined(__APPLE__) /* iOS */
    m_width = 320;
    m_height = 480;
    renderdevice_desc.width = 320;
    renderdevice_desc.height = 480;
    renderdevice_desc.colour_bpp = 32;
    renderdevice_desc.depth_bpp = 24;
    renderdevice_desc.stencil_bpp = 8;
    renderdevice_desc.refresh_rate = 60;
    renderdevice_desc.buffer_count = 1;
    renderdevice_desc.fullscreen = No;
    renderdevice_desc.device_type = KE_RENDERDEVICE_OGLES2;
#else
    m_width = 640;
    m_height = 480;
    renderdevice_desc.width = 640;
    renderdevice_desc.height = 480;
    renderdevice_desc.colour_bpp = 32;
    renderdevice_desc.depth_bpp = 24;
    renderdevice_desc.stencil_bpp = 8;
    renderdevice_desc.refresh_rate = 60;
    renderdevice_desc.buffer_count = 1;
    renderdevice_desc.fullscreen = No;
    renderdevice_desc.device_type = KE_RENDERDEVICE_OGL3;
#endif
    
    if( !ke_create_window_and_device( &renderdevice_desc, &m_renderdevice ) )
        return false;
    
    /* Set default matrix values */
    Matrix4 world, view, projection;
    Point3 eye = { 4, 3, 3 };
    Point3 look = { 0, 0, 0 };
    Vector3 up = { 0, 1, 0 };
    float aspect = float( m_width/m_height );
    
    world = vmathM4MakeIdentity_V();
    view = vmathM4MakeIdentity_V();
    projection = vmathM4MakePerspective_V( M_PI_4, aspect, 0.1f, 100.0f );
    
    m_renderdevice->set_world_matrix( &world );
    m_renderdevice->set_view_matrix( &view );
    m_renderdevice->set_projection_matrix( &projection );
    
    /* Load resources */
    create_programs();
    create_geometry();
    
    /* Start the physics engine */
    start_physics();
    
    /* Create us a new camera */
    m_camera = ke_new_camera();
    
    return true;
}

void rigidbodydemo_t::uninitialize()
{
    /* Kill the camera */
    delete m_camera;
    
    /* Stop the physics engine */
    stop_physics();
    
    /* Kill the resources */
    delete_geometry();
    delete_programs();
    
    /* Uninitialize the engine */
    ke_destroy_window_and_device( m_renderdevice );
    ke_uninitialize();
}

void rigidbodydemo_t::on_render()
{
    Matrix4 world, view;

    float clear_colour[] = { 0.0f, 0.3f, 0.0f, 1.0f };
    float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambient[] = { 0.2, 0.2f, 0.2f, 0.2f };
    float light_dir[] = { 0, 1.0f, 1.0f, 0 };
    float colours[6][4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.5f, 0.0f, 1.0f },
        { 0.5f, 0.5f, 1.0f, 1.0f },
    };
    
    /* Clear the screen and depth buffer*/
    m_renderdevice->set_clear_colour_fv( clear_colour );
    m_renderdevice->set_clear_depth( 1.0f );
    m_renderdevice->clear_colour_buffer();
    m_renderdevice->clear_depth_buffer();
    
    /* Set render states */
    /*ke_state_t rs[3];
    rs[0].state = KE_RS_DEPTHTEST;
    rs[0].param1 = Yes;
    rs[1].state = KE_RS_POLYGONMODE;
    rs[1].param1 = KE_FRONT_AND_BACK;
    rs[1].param2 = KE_LINE;
    rs[2].state = -1;
    
    m_renderdevice->set_render_states(rs);*/
    
    /* Set view matrix */
    view = vmathM4MakeLookAt_V( vmathP3MakeFromElems_V(-8.5f, 1.8f, 11.1f),
                               vmathP3MakeFromElems_V(0.6f, 0.0f, -0.75f),
                               vmathV3MakeFromElems_V( 0, 1, 0 ) );
    m_renderdevice->set_view_matrix( &view );
    
    /* Update the camera matrix */
    //m_camera->get_world_matrix( &world );
    //m_renderdevice->set_world_matrix( &world );
    
    /* Set default program */
    m_renderdevice->set_program( m_vertex_light_program );
    m_renderdevice->set_program_constant_4fv( "light_dir", 1, light_dir );
    m_renderdevice->set_program_constant_4fv( "ambient", 1, ambient );
    
    /* Render each rigid body */
    for( int i = 0; i < 5; ++i )
    {
        /* Attempt to locate this rigid body from it's id */
        ke_rigid_body_t* rb = m_physics->get_rigid_body( m_rigid_body_id[i] );
        
        /* If we find it, attempt to render it */
        if( rb )
        {
            /* Get a compatible copy of the transform matrix, then set it as the current world matrix */
            Matrix4 transform;
            neT3 mtx = rb->rigid_body->GetTransform();
            memcpy( &transform, &mtx, sizeof(float)*16 );
            m_renderdevice->set_world_matrix( &transform );
            
#ifdef __MOBILE_OS__
            /*Matrix4 normal_matrix;
            vmathM4Mul( &normal_matrix, &view, &transform );
            vmathM4Inverse( &normal_matrix, &normal_matrix );
            vmathM4Transpose( &normal_matrix, &normal_matrix );
            
            m_renderdevice->set_program_constant_4fv( "normal_mtx", 4, &normal_matrix.col0.x );*/
#endif
            
            /* Render the box geometry buffer */
            m_renderdevice->set_geometry_buffer( m_box );
            m_renderdevice->set_program_constant_4fv( "diffuse", 1, colours[i] );
            m_renderdevice->draw_indexed_vertices( KE_TRIANGLES, sizeof( ke_mesh_vertex_t ), m_box_indices );
        }
    }
    
    /* Now render the animated body */
    ke_animated_body_t* ab = m_physics->get_animated_body( m_animated_body_id );
    if( ab )
    {
        /* Get a compatible copy of the transform matrix, then set it as the current world matrix */
        Matrix4 transform = NET3_TO_MATRIX4X4f(ab->animated_body->GetTransform());
        //neT3 mtx = ab->animated_body->GetTransform();
        //memcpy( &transform, &mtx, sizeof(float)*16 );
        
        Matrix4 scale = vmathM4MakeScale_V( vmathV3MakeFromElems_V(10.0f+(10.0f*0.6f), 0.8f+(0.8f*0.6f), 10.0f+(10.0f*0.6f)) );
        transform = vmathM4Mul_V( scale, transform );
        m_renderdevice->set_world_matrix( &transform );
        
#ifdef __MOBILE_OS__
        /*Matrix4 normal_matrix;
        vmathM4Mul( &normal_matrix, &view, &transform );
        vmathM4Inverse( &normal_matrix, &normal_matrix );
        vmathM4Transpose( &normal_matrix, &normal_matrix );
        
        m_renderdevice->set_program_constant_4fv( "normal_mtx", 4, &normal_matrix.col0.x );*/
#endif
        
        /* Render the geometry buffer */
        m_renderdevice->set_geometry_buffer( m_box );
        m_renderdevice->set_program_constant_4fv( "diffuse", 1, colours[5] );
        m_renderdevice->draw_indexed_vertices( KE_TRIANGLES, sizeof( ke_mesh_vertex_t ), m_box_indices );
    }
    
    /* Swap the buffers */
    m_renderdevice->swap();
}

void rigidbodydemo_t::on_update()
{
    ke_button_t keys[256];
    
    /* Update the framerate counter */
    m_framerate.UpdateFPS();
    //std::cout << m_framerate.GetFPS() << std::endl;
    
    /* Get keyboard state */
    ke_get_key_state( keys );
    
    /* If U was pressed, make the bottom box jump */
    if( keys['u'].pressed )
    {
        ke_rigid_body_t* rb = get_physics()->get_rigid_body( m_rigid_body_id[0] );
        neV3 v;
        v.Set( 0, 10.0f, 0 );
        
        rb->rigid_body->SetVelocity(v);
    }
    
    /* Screenshot */
    if( keys['c'].pressed && keys['c'].timestamp.frames <= 1 )
    {
        ke_save_screenshot_jpg( m_renderdevice );
    }
    
    /* Update physics simulator */
    m_physics->update_simulator();
}

void rigidbodydemo_t::on_keyboard( void* context, void* input_context )
{
#if 0
    rigidbodydemo_t* this_ptr = static_cast<rigidbodydemo_t*>( context );
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

void rigidbodydemo_t::on_mouse( void* context, void* input_context )
{
    rigidbodydemo_t* this_ptr = static_cast<rigidbodydemo_t*>( context );
    SDL_Event* input_event = static_cast<SDL_Event*>( input_context );
    
    //this_ptr->get_camera()->on_mouse_move( input_event->motion.x, input_event->motion.y );
}

void rigidbodydemo_t::on_gamepad( void* context, void* input_context )
{
    rigidbodydemo_t* this_ptr = static_cast<rigidbodydemo_t*>( context );
}

int rigidbodydemo_t::main_loop()
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

bool rigidbodydemo_t::create_geometry()
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
    if( ke_open_scene( "resource/mesh/cube.obj" ) )
    {
        /* Read in the vertex data */
        if( ke_read_mesh_vertex_data( 0, &mesh ) )
        {
            /* Create a new geometry buffer */
            m_renderdevice->create_geometry_buffer( mesh.vertices, mesh.vertex_count*8*sizeof(float), mesh.indices, mesh.index_count*sizeof(uint32_t), KE_UNSIGNED_INT, 0, vertex_attributes, &m_box );
            m_box_indices = mesh.index_count;
            
            /* Free the vertex data */
            ke_free_mesh_vertex_data( &mesh );
        }
        
        /* Close the scene */
        ke_close_scene();
    }
    
    return true;
}

void rigidbodydemo_t::delete_geometry()
{
    /* Delete geometry buffer */
    m_renderdevice->delete_geometry_buffer( m_box );
}

bool rigidbodydemo_t::create_programs()
{
    ke_vertexattribute_t vertex_attributes[] =
    {
        { KE_VA_POSITION, 3, KE_FLOAT, No, 8*sizeof(float), 0 },
        { KE_VA_NORMAL,   3, KE_FLOAT, No, 8*sizeof(float), 3*sizeof(float) },
        { KE_VA_TEXTURE0, 2, KE_FLOAT, No, 8*sizeof(float), 6*sizeof(float) },
        { -1, 0, 0, 0, 0, 0 },
    };
    
#ifndef __MOBILE_OS__
    std::string str_vs = get_shader_string( "resource/shaders/default.vs" );
    std::string str_fs = get_shader_string( "resource/shaders/default.fs" );
    
    if( !m_renderdevice->create_program( str_vs.c_str(), str_fs.c_str(), NULL, NULL, vertex_attributes, &m_default_program ) )
        return false;
    
    str_vs = get_shader_string( "resource/shaders/pervertex_directional.vs" );
    str_fs = get_shader_string( "resource/shaders/pervertex_directional.fs" );
    
    if( !m_renderdevice->create_program( str_vs.c_str(), str_fs.c_str(), NULL, NULL, vertex_attributes, &m_vertex_light_program ) )
        return false;
#else
    std::string str_vs = get_shader_string( "resource/shaders/pervertex_directional_es2.vs" );
    std::string str_fs = get_shader_string( "resource/shaders/pervertex_directional_es2.fs" );
    
    if( !m_renderdevice->create_program( str_vs.c_str(), str_fs.c_str(), NULL, NULL, vertex_attributes, &m_vertex_light_program ) )
        return false;
#endif
    
    return true;
}

void rigidbodydemo_t::delete_programs()
{
    m_renderdevice->delete_program( m_vertex_light_program );
    m_renderdevice->delete_program( m_default_program );
}

bool rigidbodydemo_t::start_physics()
{
    neV3 gravity;
    
    /* Initialize physics engine */
    gravity.Set( 0, -10.0f, 0 );
    m_physics = new ke_physics_simulator_t( 5, 1, gravity );
    
    /* Add some rigid bodies */
    for( int i = 0; i < 5; ++i )
    {
        neV3 size, position;
        
        size.Set( 0.6f, 0.6f, 0.6f );
        
        if( i == 0 )
            position.Set( 0, 0.9f, 0 );
        else
            position.Set( 0, (0.9f * (float)(i + 1) ), 0 );
                         
        m_rigid_body_id[i] = m_physics->add_rigid_body_box( position, size, 0.1f );
    }
    
    /* Add an animated body */
    neV3 position, size;
    
    position.Set( 0, 0, 0 );
    size.Set( 10.0f, 0.8f, 10.0f );
    m_animated_body_id = m_physics->add_animated_body_box( position, size );
    
    return true;
}

void rigidbodydemo_t::stop_physics()
{
    m_physics->remove_all_animated_bodies();
    m_physics->remove_all_rigid_bodies();

    delete m_physics;
}

std::string rigidbodydemo_t::get_shader_string( std::string filename )
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
