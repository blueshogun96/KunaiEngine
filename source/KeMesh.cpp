//
//  ke_mesh.cpp
//
//  Created by Shogun3D on 7/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeMesh.h"
#include "nvdebug.h"


/*
 * Globals
 */
const aiScene*      scene = NULL;
Assimp::Importer    importer;


bool KeOpenScene( char* meshfile )
{
    /* Import this mesh from disk */
    scene = importer.ReadFile( meshfile, aiProcessPreset_TargetRealtime_Quality );
    if( !scene )
        return false;
    
    return true;
}

void KeCloseScene()
{
    /* Close this mesh currently in memory */
    importer.FreeScene();
}

int KeGetMeshCount()
{
    /* Return the number of meshes */
    return scene->mNumMeshes;
}

bool KeReadMeshVertexData( int index, KeMesh* mesh_out )
{
    aiMesh* mesh;
    
    /* Did we already import a mesh file? */
    if( !scene )
        return false;
    
    mesh = scene->mMeshes[index];
    
    /* Save important mesh values */
    mesh_out->vertex_count = mesh->mNumVertices;
    mesh_out->index_count = mesh->mNumFaces * 3;
    mesh_out->face_count = mesh->mNumFaces;
    
    /* Allocate space for vertices */
    mesh_out->vertices = new KeMeshVertex[mesh_out->vertex_count];
    
    const aiVector3D zero( 0, 0, 0 );
    
    /* Save the relevant vertex data */
    /* TODO: Allow users to specify what vertex attributes they want */
    for( int i = 0; i < mesh->mNumVertices; i++ )
    {
        const aiVector3D* pos = &mesh->mVertices[i];
        const aiVector3D* nor = mesh->HasNormals() ? &mesh->mNormals[i] : &zero;
        const aiVector3D* tex0 = mesh->HasTextureCoords(0) ? &mesh->mTextureCoords[0][i] : &zero;
        
        aiVector2D tex( tex0->x, tex0->y );
        
        mesh_out->vertices[i].set( pos, nor, &tex );
    }
    
    /* Allocate space for indices */
    mesh_out->indices = new uint32_t[mesh_out->index_count];
    for( int i = 0; i < mesh->mNumFaces; i++ )
    {
        const aiFace &face = mesh->mFaces[i];
        mesh_out->indices[(i*3)+0] = face.mIndices[0];
        mesh_out->indices[(i*3)+1] = face.mIndices[1];
        mesh_out->indices[(i*3)+2] = face.mIndices[2];
    }
    
    return true;
}

void KeFreeMeshVertexData( KeMesh* mesh_out )
{
    delete [] mesh_out->vertices;
    delete [] mesh_out->indices;
}
