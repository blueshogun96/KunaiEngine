//
//  ke_physics.cpp
//
//  Created by Shogun3D on 7/27/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KePhysics.h"
#include "KeSystem.h"



/*
 * Name: KePhysicsSimulator::
 * Desc: 
 */
KePhysicsSimulator::KePhysicsSimulator() : time_since_last_update(0), start_time(KeGetPerformanceCounter()), rigid_bodies(NULL), animated_bodies(NULL)
{
    /* Default gravity setting */
    neV3 gravity;
    gravity.Set( 0.0f, -10.0f, 0.0f );
    
    /* Initialize simulator with the default values */
    simulator = neSimulator::CreateSimulator( this->size_info, NULL, &gravity );
}

/*
 * Name: KePhysicsSimulator::
 * Desc:
 */
KePhysicsSimulator::KePhysicsSimulator( int max_rigid_bodies, int max_animated_bodies, neV3 gravity ) : time_since_last_update(0), start_time(KeGetPerformanceCounter()), rigid_bodies(NULL), animated_bodies(NULL)
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
 * Name: KePhysicsSimulator::~KePhysicsSimulator()
 * Desc:
 */
KePhysicsSimulator::~KePhysicsSimulator()
{
    /* Delete the simulator */
    neSimulator::DestroySimulator( simulator );
}

/*
 * Name: KePhysicsSimulator::SetGravity
 * Desc:
 */
void KePhysicsSimulator::SetGravity( neV3 gravity )
{
    simulator->Gravity( gravity );
}

/*
 * Name: KePhysicsSimulator::AddRigidBodyBox
 * Desc:
 */
uint32_t KePhysicsSimulator::AddRigidBodyBox( neV3 position, neV3 size, float mass )
{
    static uint32_t rb_id = 0;
    
    /* Add a new rigid body box to the simulator */
    KeRigidBody rigid_body;
    rigid_body.rigid_body = simulator->CreateRigidBody();
    
    /* Create box geometry */
    rigid_body.geometry = rigid_body.rigid_body->AddGeometry();
    rigid_body.geometry->SetBoxSize( size );
    rigid_body.rigid_body->UpdateBoundingInfo();
    
    /* Set mass and intertia tensor */
    rigid_body.rigid_body->SetInertiaTensor( neBoxInertiaTensor( size, mass ) );
    rigid_body.rigid_body->SetMass( mass );
    
    /* Set position */
    rigid_body.rigid_body->SetPos( position );
    
    /* Give this rigid body a unique ID number */
    rigid_body.rb_id = ++rb_id;
    
    /* Add this to the list of rigit bodies */
//    list_add_end<KeRigidBody*>( &rigid_bodies, rigid_body );
    rigid_bodies.push_back( rigid_body );
    
    return rb_id;
}

/*
 * Name: KePhysicsSimulator::AddRigidBodyBox
 * Desc:
 */
uint32_t KePhysicsSimulator::AddRigidBodySphere( neV3 position, float radius, float mass )
{
    static uint32_t rb_id = 0;
    
    /* Add a new rigid body box to the simulator */
    KeRigidBody rigid_body;
    rigid_body.rigid_body = simulator->CreateRigidBody();
    
    /* Create box geometry */
    rigid_body.geometry = rigid_body.rigid_body->AddGeometry();
    rigid_body.geometry->SetSphereDiameter( radius * 2 );
    rigid_body.rigid_body->UpdateBoundingInfo();
    
    /* Set mass and intertia tensor */
    rigid_body.rigid_body->SetInertiaTensor( neSphereInertiaTensor( radius * 2, mass ) );
    rigid_body.rigid_body->SetMass( mass );
    
    /* Set position */
    rigid_body.rigid_body->SetPos( position );
    
    /* Give this rigid body a unique ID number */
    rigid_body.rb_id = ++rb_id;
    
    /* Add this to the list of rigit bodies */
    rigid_bodies.push_back( rigid_body );
    
    return rb_id;
}

/*
 * Name: KePhysicsSimulator::AddAnimatedBodyBox
 * Desc:
 */
uint32_t KePhysicsSimulator::AddAnimatedBodyBox( neV3 position, neV3 size )
{
    static uint32_t ab_id = 0;
    
    /* Create a new animated body box */
    KeAnimatedBody animated_body;
    animated_body.animated_body = simulator->CreateAnimatedBody();
    
    /* Create box geometry */
    animated_body.geometry = animated_body.animated_body->AddGeometry();
    animated_body.geometry->SetBoxSize( size );
    animated_body.animated_body->UpdateBoundingInfo();
    
    /* Set animated body position */
    animated_body.animated_body->SetPos( position );
    
    /* Give this animated body a unique ID */
    animated_body.ab_id = ++ab_id;
    
    /* Add this animated body to the list */
//    list_add_end<KeAnimatedBody*>( &animated_bodies, animated_body );
    animated_bodies.push_back( animated_body );
    
    return ab_id;
}

/*
 * Name: KePhysicsSimulator::RemoveRigidBody
 * Desc:
 */
void KePhysicsSimulator::RemoveRigidBody( uint32_t rb_id )
{
#if 0
    node_t<KeRigidBody*>* n = rigid_bodies;
    
    /* Search for a rigid body with this id number */
    while( n != NULL )
    {
        KeRigidBody* rb = n->data;
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
                list_delete<KeRigidBody*>( &rigid_bodies, rb );
                break;
            }
        }
    }
#else
    std::vector<KeRigidBody>::iterator i = rigid_bodies.begin();
    
    while( i != rigid_bodies.end() )
    {
        /* Does this id match? */
        if( i->rb_id == rb_id )
        {
            /* If so, delete the geometry and free the rigid body */
            i->rigid_body->RemoveGeometry( i->geometry );
            simulator->FreeRigidBody( i->rigid_body );
            
            /* Now remove it from the list */
            rigid_bodies.erase(i);
            break;
        }
        
        ++i;
    }
#endif
}

/*
 * Name: KePhysicsSimulator::RemoveAnimatedBody
 * Desc:
 */
void KePhysicsSimulator::RemoveAnimatedBody( uint32_t ab_id )
{
#if 0
    node_t<KeAnimatedBody*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        KeAnimatedBody* ab = n->data;
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
                list_delete<KeAnimatedBody*>( &animated_bodies, ab );
                break;
            }
        }
    }
#else
    std::vector<KeAnimatedBody>::iterator i = animated_bodies.begin();
    
    while( i != animated_bodies.end() )
    {
        /* Does this id match? */
        if( i->ab_id == ab_id )
        {
            /* If so, delete the geometry and free the rigid body */
            i->animated_body->RemoveGeometry( i->geometry );
            simulator->FreeAnimatedBody( i->animated_body );
            
            /* Now remove it from the list */
            animated_bodies.erase(i);
            break;
        }
        
        ++i;
    }
#endif
}

/*
 * Name: KePhysicsSimulator::
 * Desc:
 */
bool KePhysicsSimulator::SetAnimatedBodyPosition( uint32_t id, neV3 position )
{
#if 0
    node_t<KeAnimatedBody*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        KeAnimatedBody* ab = n->data;
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
#else
    std::vector<KeAnimatedBody>::iterator i = animated_bodies.begin();
    
    while( i != animated_bodies.end() )
    {
        /* Does this id match? */
        if( i->ab_id == id )
        {
            /* Set the position and return success */
            i->animated_body->SetPos( position );
            return true;
        }
        
        ++i;
    }
#endif
    
    /* Animated body id not found */
    return false;
}

/*
 * Name: KePhysicsSimulator::
 * Desc:
 */
bool KePhysicsSimulator::SetAnimatedBodyRotation( uint32_t id, neV3 rotation )
{
#if 0
    node_t<KeAnimatedBody*>* n = animated_bodies;
    
    /* Search for a animated body with this id number */
    while( n != NULL )
    {
        KeAnimatedBody* ab = n->data;
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
#else
    std::vector<KeAnimatedBody>::iterator i = animated_bodies.begin();
    
    while( i != animated_bodies.end() )
    {
        /* Does this id match? */
        if( i->ab_id == id )
        {
            /* Set the rotation and return success */
            neT3 matrix;
            matrix.rot.RotateXYZ( rotation );
            i->animated_body->SetRotation( matrix.rot );
            return true;
        }
        
        ++i;
    }
#endif
    
    /* Animated body id not found */
    return false;
}

bool KePhysicsSimulator::GetRigidBody( uint32_t id, KeRigidBody** rigid_body )
{
    std::vector<KeRigidBody>::iterator i = rigid_bodies.begin();
    
    while( i != rigid_bodies.end() )
    {
        /* Does this id match? */
        if( i->rb_id == id )
        {
            *rigid_body = &(*i);
            return true;
        }
        
        ++i;
    }
    
    return false;
}

bool KePhysicsSimulator::GetAnimatedBody( uint32_t id, KeAnimatedBody** animated_body )
{
#if 0
    node_t<KeAnimatedBody*>* n = animated_bodies;
    
    /* Traverse though the list in search for an animated body with a matching id number */
    while( n != NULL )
    {
        KeAnimatedBody* ab = n->data;
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
#else
    std::vector<KeAnimatedBody>::iterator i = animated_bodies.begin();
    
    while( i != animated_bodies.end() )
    {
        /* Does this id match? */
        if( i->ab_id == id )
        {
            *animated_body = &(*i);
            return true;
        }
        
        ++i;
    }

	return false;
#endif
}

void KePhysicsSimulator::RemoveAllRigidBodies()
{
#if 0
    node_t<KeRigidBody*>* n = rigid_bodies;
    
    while( n != NULL )
    {
        KeRigidBody* rb = n->data;
        n = n->next;
        
        if( rb != NULL )
        {
            /* Delete the geometry and free the animated body */
            rb->rigid_body->RemoveGeometry( rb->geometry );
            simulator->FreeRigidBody( rb->rigid_body );
            
            /* Now remove it from the list */
            list_delete<KeRigidBody*>( &rigid_bodies, rb );
        }
    }
#else
	std::vector<KeRigidBody>::iterator i = rigid_bodies.begin();

	while( i != rigid_bodies.end() )
	{
		/* Delete the geometry and free the animated body */
		i->rigid_body->RemoveGeometry( i->geometry );
		simulator->FreeRigidBody( i->rigid_body );

		++i;
	}

	animated_bodies.clear();
#endif
}

void KePhysicsSimulator::RemoveAllAnimatedBodies()
{
#if 0
    node_t<KeAnimatedBody*>* n = animated_bodies;
    
    while( n != NULL )
    {
        KeAnimatedBody* ab = n->data;
        n = n->next;
        
        if( ab != NULL )
        {
            /* Delete the geometry and free the animated body */
            ab->animated_body->RemoveGeometry( ab->geometry );
            simulator->FreeAnimatedBody( ab->animated_body );
            
            /* Now remove it from the list */
            list_delete<KeAnimatedBody*>( &animated_bodies, ab );
        }
    }
#else
	std::vector<KeAnimatedBody>::iterator i = animated_bodies.begin();

	while (i != animated_bodies.end())
	{
		/* Delete the geometry and free the animated body */
		i->animated_body->RemoveGeometry( i->geometry );
		simulator->FreeAnimatedBody( i->animated_body );
		
		++i;
	}

	animated_bodies.clear();
#endif
}

/*
 * Name: KePhysicsSimulator::UpdateSimulator
 * Desc:
 */
void KePhysicsSimulator::UpdateSimulator( float frame_interval )
{
    /* Get the end time */
    end_time = KeGetPerformanceCounter();
    
    /* Calculate the elapsed time since the physics engine was last updated */
    float frame_time = float( end_time - start_time ) / float(KeGetPerformanceFrequency());
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
    start_time = KeGetPerformanceCounter();
}
