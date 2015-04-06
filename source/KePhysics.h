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
struct KeRigidBody
{
/*public:
    KeRigidBody() {};
    KeRigidBody( neSimulator* parent_sim );
    virtual ~KeRigidBody();*/
    
    neRigidBody*    rigid_body;
    neGeometry*     geometry;
    neSimulator*    parent_simulator;
    uint32_t        rb_id;
};

/* Animated body */
struct KeAnimatedBody
{
/*public:
    KeAnimatedBody() {};
    KeAnimatedBody( neSimulator* parent_sim );
    virtual ~KeAnimatedBody();*/
    
    neAnimatedBody* animated_body;
    neGeometry*     geometry;
    neSimulator*    parent_simulator;
    uint32_t        ab_id;
};


/* Physics simulation class */
class KePhysicsSimulator
{
public:
    KePhysicsSimulator();
    KePhysicsSimulator( int max_rigid_bodies, int max_animated_bodies, neV3 gravity );
    virtual ~KePhysicsSimulator();
    
public:
    void SetGravity( neV3 gravity );
    uint32_t AddRigidBodyBox( neV3 position, neV3 size, float mass );
    uint32_t AddAnimatedBodyBox( neV3 position, neV3 size );
    void RemoveRigidBody( uint32_t rb_id );
    void RemoveAnimatedBody( uint32_t ab_id );
    bool SetAnimatedBodyPosition( uint32_t id, neV3 position );
    bool SetAnimatedBodyRotation( uint32_t id, neV3 rotation );
    
    KeRigidBody* GetRigidBody( uint32_t id );
    KeAnimatedBody* GetAnimatedBody( uint32_t id );
    
    void RemoveAllRigidBodies();
    void RemoveAllAnimatedBodies();
    
    void UpdateSimulator( float frame_interval = 0.01667f );
    
protected:
    neSimulator*                    simulator;          /* Tokamak physics simualtor */
    node_t<KeRigidBody*>*			rigid_bodies;       /* A list of rigid bodies */
    node_t<KeAnimatedBody*>*		animated_bodies;    /* A list of animated bodies */
    neSimulatorSizeInfo             size_info;          /* Information about this physics simulator */
    uint64_t                        start_time, end_time; /* Start and end time */
    float                           time_since_last_update;
};

#endif /* defined(__ke_physics__) */
