//
//  ke_physics.cpp
//
//  Created by Shogun3D on 7/27/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_physics.h"
#include "ke_system.h"



/*
 * Name: ke_physics_simulator_t::
 * Desc: 
 */
ke_physics_simulator_t::ke_physics_simulator_t() : time_since_last_update(0), start_time(ke_get_performance_counter()), rigid_bodies(NULL), animated_bodies(NULL)
{
    /* Default gravity setting */
    neV3 gravity;
    gravity.Set( 0.0f, -10.0f, 0.0f );
    
    /* Initialize simulator with the default values */
    simulator = neSimulator::CreateSimulator( this->size_info, NULL, &gravity );
}

/*
 * Name: ke_physics_simulator_t::
 * Desc:
 */
ke_physics_simulator_t::ke_physics_simulator_t( int max_rigid_bodies, int max_animated_bodies, neV3 gravity ) : time_since_last_update(0), start_time(ke_get_performance_counter()), rigid_bodies(NULL), animated_bodies(NULL)
{
    /* Manually calculate the simulator size requirements */
    size_info.rigidBodiesCount = max_rigid_bodies;
    size_info.animatedBodiesCount = max_animated_bodies;
    size_info.rigidParticleCount = 0;
    size_info.controllersCount = 0;
    size_info.overlappedPairsCount = ( max_rigid_bodies * max_animated_bodies ) + max_rigid_bodies * ( max_rigid_bodies - 1 ) /2;
    size_info.geometriesCount = max_rigid_bodies + max_animated_bodies;
    size_info.constraintsCount = 0;
    size_info.constraintSetsCount = 0;
    size_info.constraintBufferSize = 0;
    size_info.sensorsCount = 0;
    size_info.terrainNodesGrowByCount = 0;
    size_info.terrainNodesStartCount = 0;
    
    /* Create the actual simulator */
    simulator = neSimulator::CreateSimulator( size_info, NULL, &gravity );
}

/*
 * Name: ke_physics_simulator_t::~ke_physics_simulator_t()
 * Desc:
 */
ke_physics_simulator_t::~ke_physics_simulator_t()
{
    /* Delete the simulator */
    neSimulator::DestroySimulator( simulator );
}

/*
 * Name: ke_physics_simulator_t::set_gravity
 * Desc:
 */
void ke_physics_simulator_t::set_gravity( neV3 gravity )
{
    simulator->Gravity( gravity );
}

/*
 * Name: ke_physics_simulator_t::add_rigid_body_box
 * Desc:
 */
uint32_t ke_physics_simulator_t::add_rigid_body_box( neV3 position, neV3 size, float mass )
{
    static uint32_t rb_id = 0;
    
    /* Add a new rigid body box to the simulator */
    ke_rigid_body_t* rigid_body = new ke_rigid_body_t;
    rigid_body->rigid_body = simulator->CreateRigidBody();
    
    /* Create box geometry */
    rigid_body->geometry = rigid_body->rigid_body->AddGeometry();
    rigid_body->geometry->SetBoxSize( size );
    rigid_body->rigid_body->UpdateBoundingInfo();
    
    /* Set mass and intertia tensor */
    rigid_body->rigid_body->SetInertiaTensor( neBoxInertiaTensor( size, mass ) );
    rigid_body->rigid_body->SetMass( mass );
    
    /* Set position */
    rigid_body->rigid_body->SetPos( position );
    
    /* Give this rigid body a unique ID number */
    rigid_body->rb_id = ++rb_id;
    
    /* Add this to the list of rigit bodies */
    list_add_end<ke_rigid_body_t*>( &rigid_bodies, rigid_body );
    
    return rb_id;
}

/*
 * Name: ke_physics_simulator_t::add_animated_body_box
 * Desc:
 */
uint32_t ke_physics_simulator_t::add_animated_body_box( neV3 position, neV3 size )
{
    static uint32_t ab_id = 0;
    
    /* Create a new animated body box */
    ke_animated_body_t* animated_body = new ke_animated_body_t;
    animated_body->animated_body = simulator->CreateAnimatedBody();
    
    /* Create box geometry */
    animated_body->geometry = animated_body->animated_body->AddGeometry();
    animated_body->geometry->SetBoxSize( size );
    animated_body->animated_body->UpdateBoundingInfo();
    
    /* Set animated body position */
    animated_body->animated_body->SetPos( position );
    
    /* Give this animated body a unique ID */
    animated_body->ab_id = ++ab_id;
    
    /* Add this animated body to the list */
    list_add_end<ke_animated_body_t*>( &animated_bodies, animated_body );
    
    return ab_id;
}

/*
 * Name: ke_physics_simulator_t::remove_rigid_body
 * Desc:
 */
void ke_physics_simulator_t::remove_rigid_body( uint32_t rb_id )
{
    node_t<ke_rigid_body_t*>* n = rigid_bodies;
    
    /* Search for a rigid body with this id number */
    while( n != NULL )
    {
        ke_rigid_body_t* rb = n->data;
        n = n->next;
        
        if( rb != NULL )
        {
            /* Does this id match? */
            if( rb->rb_id == rb_id )
            {
                /* If so, delete the geometry and free the rigid body */
                rb->rigid_body->RemoveGeometry( rb->geometry );
                simulator->FreeRigidBody( rb->rigid_body );
                
                /* Now remove it from the list */
                list_delete<ke_rigid_body_t*>( &rigid_bodies, rb );
                break;
            }
        }
    }
}

/*
 * Name: ke_physics_simulator_t::remove_animated_body
 * Desc:
 */
void ke_physics_simulator_t::remove_animated_body( uint32_t ab_id )
{
    node_t<ke_animated_body_t*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        ke_animated_body_t* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Does this id match? */
            if( ab->ab_id == ab_id )
            {
                /* If so, delete the geometry and free the animated body */
                ab->animated_body->RemoveGeometry( ab->geometry );
                simulator->FreeAnimatedBody( ab->animated_body );
                
                /* Now remove it from the list */
                list_delete<ke_animated_body_t*>( &animated_bodies, ab );
                break;
            }
        }
    }
}

/*
 * Name: ke_physics_simulator_t::
 * Desc:
 */
bool ke_physics_simulator_t::set_animated_body_position( uint32_t id, neV3 position )
{
    node_t<ke_animated_body_t*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        ke_animated_body_t* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Does this id match? */
            if( ab->ab_id == id )
            {
                /* Set the position and return success */
                ab->animated_body->SetPos( position );
                return true;
            }
        }
    }
    
    /* Animated body id not found */
    return false;
}

/*
 * Name: ke_physics_simulator_t::
 * Desc:
 */
bool ke_physics_simulator_t::set_animated_body_rotation( uint32_t id, neV3 rotation )
{
    node_t<ke_animated_body_t*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        ke_animated_body_t* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Does this id match? */
            if( ab->ab_id == id )
            {
                /* Set the rotation and return success */
                neT3 matrix;
                matrix.rot.RotateXYZ( rotation );
                ab->animated_body->SetRotation( matrix.rot );
                return true;
            }
        }
    }
    
    /* Animated body id not found */
    return false;
}

ke_rigid_body_t* ke_physics_simulator_t::get_rigid_body( uint32_t id )
{
    node_t<ke_rigid_body_t*>* n = rigid_bodies;
    
    /* Traverse though the list in search for a rigid body with a matching id number */
    while( n != NULL )
    {
        ke_rigid_body_t* rb = n->data;
        n = n->next;
        
        if( rb != NULL )
        {
            /* Does this id match? */
            if( rb->rb_id == id )
            {
                /* If so, return with this pointer as a match */
                return rb;
            }
        }
    }
    
    /* Did not find a match */
    return NULL;
}

ke_animated_body_t* ke_physics_simulator_t::get_animated_body( uint32_t id )
{
    node_t<ke_animated_body_t*>* n = animated_bodies;
    
    /* Traverse though the list in search for an animated body with a matching id number */
    while( n != NULL )
    {
        ke_animated_body_t* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Does this id match? */
            if( ab->ab_id == id )
            {
                /* If so, return with this pointer as a match */
                return ab;
            }
        }
    }
    
    /* Did not find a match */
    return NULL;
}

void ke_physics_simulator_t::remove_all_rigid_bodies()
{
    node_t<ke_rigid_body_t*>* n = rigid_bodies;
    
    while( n != NULL )
    {
        ke_rigid_body_t* rb = n->data;
        n = n->next;
        
        if( rb != NULL )
        {
            /* Delete the geometry and free the animated body */
            rb->rigid_body->RemoveGeometry( rb->geometry );
            simulator->FreeRigidBody( rb->rigid_body );
            
            /* Now remove it from the list */
            list_delete<ke_rigid_body_t*>( &rigid_bodies, rb );
        }
    }
}

void ke_physics_simulator_t::remove_all_animated_bodies()
{
    node_t<ke_animated_body_t*>* n = animated_bodies;
    
    while( n != NULL )
    {
        ke_animated_body_t* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Delete the geometry and free the animated body */
            ab->animated_body->RemoveGeometry( ab->geometry );
            simulator->FreeAnimatedBody( ab->animated_body );
            
            /* Now remove it from the list */
            list_delete<ke_animated_body_t*>( &animated_bodies, ab );
        }
    }
}

/*
 * Name: ke_physics_simulator_t::update_simulator
 * Desc:
 */
void ke_physics_simulator_t::update_simulator( float frame_interval )
{
    /* Get the end time */
    end_time = ke_get_performance_counter();
    
    /* Calculate the elapsed time since the physics engine was last updated */
    float frame_time = float( end_time - start_time ) / float(ke_get_performance_frequency());
    time_since_last_update += frame_time;
    
    /* If the time is greater than the desired frame interval (i.e. 1/60th of a second), update the physics. */
    /* By default, the time interval is 0.01667f.  This is calculated with the following formula: vblank/1000 where
       vblank = 1000/refreshrate. */
    if( time_since_last_update > frame_interval )
    {
        simulator->Advance( frame_interval, 1, 0 );
        time_since_last_update -= frame_interval;
    }
    
    /* Reset start time */
    start_time = ke_get_performance_counter();
}
