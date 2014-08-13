//
//  ke_mesh.h
//
//  Created by Shogun3D on 7/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#ifndef __ke_mesh__
#define __ke_mesh__

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


/*
 * Mesh vertex
 */
struct ke_mesh_vertex_t
{
#ifdef __cplusplus
    void set( const aiVector3D* pos, const aiVector3D* normal, const aiVector2D* tex )
    {
        memmove( this->pos, pos, sizeof(float)*3 );
        memmove( this->normal, normal, sizeof(float)*3 );
        memmove( this->tex, tex, sizeof(float)*2 );
    }
#endif
    
    float pos[3];
    float normal[3];
    float tex[2];
};


/*
 * Mesh structure
 */
struct ke_mesh_t
{
    int face_count;
    int vertex_count;
    int index_count;
    
    ke_mesh_vertex_t* vertices;
    uint32_t* indices;
};


bool ke_open_scene( char* meshfile );
void ke_close_scene();
int ke_get_mesh_count();
bool ke_read_mesh_vertex_data( int index, ke_mesh_t* mesh_out );
void ke_free_mesh_vertex_data( ke_mesh_t* mesh_out );

#endif /* defined(__ke_mesh__) */
