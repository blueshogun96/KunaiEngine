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
struct KeMeshVertex
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
struct KeMesh
{
    int face_count;
    int vertex_count;
    int index_count;
    
    KeMeshVertex* vertices;
    uint32_t* indices;
};


bool KeOpenScene( char* meshfile );
void KeCloseScene();
int KeGetMeshCount();
bool KeReadMeshVertexData( int index, KeMesh* mesh_out );
void KeFreeMeshVertexData( KeMesh* mesh_out );

#endif /* defined(__ke_mesh__) */
