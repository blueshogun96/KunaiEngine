//
//  ke_physics.h
//
//  Created by Shogun3D on 7/27/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_physics__
#define __ke_physics__

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <tokamak.h>
//#include "linkedlist.h"


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
    uint32_t AddRigidBodySphere( neV3 position, float radius, float mass );
    uint32_t AddAnimatedBodyBox( neV3 position, neV3 size );
    void RemoveRigidBody( uint32_t rb_id );
    void RemoveAnimatedBody( uint32_t ab_id );
    bool SetAnimatedBodyPosition( uint32_t id, neV3 position );
    bool SetAnimatedBodyRotation( uint32_t id, neV3 rotation );
    
    bool GetRigidBody( uint32_t id, KeRigidBody* rigid_body );
    bool GetAnimatedBody( uint32_t id, KeAnimatedBody* animated_body );
    
    void RemoveAllRigidBodies();
    void RemoveAllAnimatedBodies();
    
    void UpdateSimulator( float frame_interval = 0.01667f );
    
protected:
    neSimulator*                    simulator;          /* Tokamak physics simualtor */
    std::vector<KeRigidBody>		rigid_bodies;       /* A list of rigid bodies */
    std::vector<KeAnimatedBody>		animated_bodies;    /* A list of animated bodies */
//    std::unordered_map<uint32_t, std::unique_ptr<KeRigidBody>> rb_map;
//    std::unordered_map<uint32_t, std::unique_ptr<KeAnimatedBody>> ab_map;
    neSimulatorSizeInfo             size_info;          /* Information about this physics simulator */
    uint64_t                        start_time, end_time; /* Start and end time */
    float                           time_since_last_update;
};

#endif /* defined(__ke_physics__) */
