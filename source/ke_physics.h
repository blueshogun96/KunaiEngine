//
//  ke_physics.h
//
//  Created by Shogun3D on 7/27/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_physics__
#define __ke_physics__

#include <iostream>
#include <tokamak.h>
#include "linkedlist.h"


/* Rigid body */
struct ke_rigid_body_t
{
    neRigidBody*    rigid_body;
    neGeometry*     geometry;
    uint32_t        rb_id;
};

/* Animated body */
struct ke_animated_body_t
{
    neAnimatedBody* animated_body;
    neGeometry*     geometry;
    uint32_t        ab_id;
};


/* Physics simulation class */
class ke_physics_simulator_t
{
public:
    ke_physics_simulator_t();
    ke_physics_simulator_t( int max_rigid_bodies, int max_animated_bodies, neV3 gravity );
    virtual ~ke_physics_simulator_t();
    
public:
    void set_gravity( neV3 gravity );
    uint32_t add_rigid_body_box( neV3 position, neV3 size, float mass );
    uint32_t add_animated_body_box( neV3 position, neV3 size );
    void remove_rigid_body( uint32_t rb_id );
    void remove_animated_body( uint32_t ab_id );
    bool set_animated_body_position( uint32_t id, neV3 position );
    bool set_animated_body_rotation( uint32_t id, neV3 rotation );
    
    void remove_all_rigid_bodies();
    void remove_all_animated_bodies();
    
    void update_simulator( float frame_interval = 0.01667f );
    
protected:
    neSimulator*                    simulator;          /* Tokamak physics simualtor */
    node_t<ke_rigid_body_t*>*       rigid_bodies;       /* A list of rigid bodies */
    node_t<ke_animated_body_t*>*    animated_bodies;    /* A list of animated bodies */
    neSimulatorSizeInfo             size_info;          /* Information about this physics simulator */
    uint64_t                        start_time, end_time; /* Start and end time */
    float                           time_since_last_update;
};

#endif /* defined(__ke_physics__) */
