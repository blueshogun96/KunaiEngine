//
//  process.hpp
//  luna
//
//  Created by Shogun3D on 1/18/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef luna_process_hpp
#define luna_process_hpp

/* For non-Windows compilers */
#ifndef PURE
#define PURE =0
#endif

#include <stdint.h>
#include "linkedlist.h"

/* Base process class */
class ke_base_process_t
{
public:
    ke_base_process_t() {}
    virtual ~ke_base_process_t() {}
    
public:
    virtual bool init() PURE;
    virtual void render() PURE;
    virtual void update() PURE;
    virtual void input( void* ) PURE;
    virtual void uninit() PURE;
    
    uint32_t id;
    bool active;
};

/* Process linked list node */
class ke_process_node_t : public node_t<ke_base_process_t*>
{
public:
    ke_process_node_t() {};
    virtual ~ke_process_node_t()
    {
        delete data;
    }
};

/* Process management class */
class ke_process_manager_t
{
public:
    ke_process_manager_t(): id_counter(0), input_context(NULL), process_list(NULL) {}
    virtual ~ke_process_manager_t() { process_remove_all(); };
    
public:
    int process_add_new( ke_base_process_t* process );
    int process_remove( uint32_t id );
    void process_remove_all();
    void process_on_render();
    void process_on_update();
    void process_on_input();
    int  process_activate( uint32_t id );
    int  process_deactivate( uint32_t id );
    void process_input_context( void* context );
    
protected:
    void* input_context;
    uint32_t id_counter;
    ke_process_node_t* process_list;
};

ke_process_manager_t* ke_create_process_manager();
void ke_destroy_process_manager( ke_process_manager_t* process_manager );

#endif
