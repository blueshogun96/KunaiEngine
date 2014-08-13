//
//  process.cpp
//
//  Created by Shogun3D on 1/18/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_platform.h"
#include "ke_process.hpp"




/*
 * Name: ke_create_process_manager
 * Desc: Allocates a new process manager class.
 */
ke_process_manager_t* ke_create_process_manager()
{
    return new ke_process_manager_t();
}


/*
 * Name: ke_destroy_process_manager
 * Desc: Removes all processes (active and inactive) and deletes the class.
 */
void ke_destroy_process_manager( ke_process_manager_t* process_manager )
{
    if( process_manager )
    {
        process_manager->process_remove_all();
        delete process_manager;
    }
}


/*
 * Name: ke_process_manager_t::process_add_new
 * Desc: Adds a new process to the list in the order it was added.
 */
int ke_process_manager_t::process_add_new( ke_base_process_t* process )
{
    /* Sanity check */
    if( !process )
        return 0;
    
    /* Add the process to the linked list */
    process->id = ++id_counter;
    list_add_end<ke_base_process_t*>( (node_t<ke_base_process_t*>**) &process_list, process );
    
    /* Call the init function */
    if( !process->init() )
        return 0;
    
    return 1;
}


/*
 * Name: ke_process_manager_t::process_remove
 * Desc: Searches the list for a process with a matching id. If found, the process is
 *       deleted and returns 1 for success. If not, returns 0 for failure.
 */
int ke_process_manager_t::process_remove( uint32_t id )
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* Search the list of processes for a matching id. If we find one, call the uninit
     function (if it exists) and then remove it. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->id == id )
            {
                list_delete( (node_t<ke_base_process_t*>**) &process_list, p );
                return 1;
            }
        }
    }
    
    return 0;
}


/*
 * Name: ke_process_manager_t::process_remove_all
 * Desc: Removes all processes from the list.
 */
void ke_process_manager_t::process_remove_all()
{
    /* Kill all existing processes */
    list_clear( (node_t<ke_base_process_t*>**) &process_list );
}


/*
 * Name: ke_process_manager_t::process_on_render
 * Desc: Calls all active rendering processes.
 */
void ke_process_manager_t::process_on_render()
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* If there is a valid function pointer, call the rendering function for each process. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->active )
                p->render();
        }
    }
}


/*
 * Name: ke_process_manager_t::process_on_update
 * Desc: Calls all active updating processes.
 */
void ke_process_manager_t::process_on_update()
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* If there is a valid function pointer, call the updating function for each process. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->active )
                p->update();
        }
    }
}


/*
 * Name: ke_process_manager_t::process_on_input
 * Desc: Calls all active input processes.
 */
void ke_process_manager_t::process_on_input()
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* If there is a valid function pointer, call the input function for each process. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->active )
                p->input(input_context);
        }
    }
}


/*
 * Name: ke_process_manager_t::process_activate
 * Desc: Activates a process with a matching id.
 */
int  ke_process_manager_t::process_activate( uint32_t id )
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* Activate a process with a matching ID. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->id == id )
            {
                p->active = true;
                return 1;
            }
        }
    }
    
    /* Not found */
    return 0;
}


/*
 * Name: ke_process_manager_t::process_deactivate
 * Desc: Deactivates a process with a matching id.
 */
int  ke_process_manager_t::process_deactivate( uint32_t id )
{
    node_t<ke_base_process_t*>* n = process_list;
    
    /* Deactivate a process with a matching ID. */
    
    while( n != NULL )
    {
        ke_base_process_t* p = n->data;
        n = n->next;
        
        if( p != NULL )
        {
            if( p->id == id )
            {
                p->active = false;
                return 1;
            }
        }
    }
    
    /* Not found */
    return 0;
}


/*
 * Name: ke_process_manager_t::process_input_context
 * Desc: Sets the input context for input processes.
 */
void ke_process_manager_t::process_input_context( void* context )
{
    input_context = context;
}
