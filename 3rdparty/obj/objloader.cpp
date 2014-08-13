/************************************************************
 *Loads obj file - limited to vertices, faces, normals, texture maps
 *loads to object structure as defined in .h file
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>

#include "objloader.h"


int object_type::getvertices( float** vertices, float** normals, float** texcoords, bool swap_yz )
{
    int vertex_count = polygons_qty * 3;
    int v = 0, n = 0, t = 0;
    
    (*vertices) = new float[vertex_count*3*sizeof(float)];
    (*normals) = new float[vertex_count*3*sizeof(float)];
    (*texcoords) = new float[vertex_count*2*sizeof(float)];
    
    if( swap_yz )
    {
        for (int j=0;j<polygons_qty;j++)
        {
            //----------------- FIRST VERTEX -----------------
            //Normal coordinates of the first vertex
            (*normals)[n+0] = normcoord[ polygon[j].n[0] - 1 ].i;
            (*normals)[n+1] = normcoord[ polygon[j].n[0] - 1 ].k;
            (*normals)[n+2] = normcoord[ polygon[j].n[0] - 1 ].j;
            // Texture coordinates of the first vertex
            (*texcoords)[t+0] = mapcoord[ polygon[j].t[0] - 1 ].u,
            (*texcoords)[t+1] = 1.0f - mapcoord[ polygon[j].t[0] - 1 ].v;
            // Coordinates of the first vertex
            (*vertices)[v+0] = vertex[ polygon[j].v[0] - 1].x,
            (*vertices)[v+1] = vertex[ polygon[j].v[0] - 1].z,
            (*vertices)[v+2] = vertex[ polygon[j].v[0] - 1].y;
            v+=3, n+=3, t+=2;
            
            //----------------- SECOND VERTEX -----------------
            //Normal coordinates of the first vertex
            (*normals)[n+0] = normcoord[ polygon[j].n[1] - 1 ].i;
            (*normals)[n+1] = normcoord[ polygon[j].n[1] - 1 ].k;
            (*normals)[n+2] = normcoord[ polygon[j].n[1] - 1 ].j;
            // Texture coordinates of the first vertex
            (*texcoords)[t+0] = mapcoord[ polygon[j].t[1] - 1 ].u,
            (*texcoords)[t+1] = 1.0f - mapcoord[ polygon[j].t[1] - 1 ].v;
            // Coordinates of the first vertex
            (*vertices)[v+0] = vertex[ polygon[j].v[1] - 1].x,
            (*vertices)[v+1] = vertex[ polygon[j].v[1] - 1].z,
            (*vertices)[v+2] = vertex[ polygon[j].v[1] - 1].y;
            v+=3, n+=3, t+=2;
            
            //----------------- THIRD VERTEX -----------------
            //Normal coordinates of the first vertex
            (*normals)[n+0] = normcoord[ polygon[j].n[2] - 1 ].i;
            (*normals)[n+1] = normcoord[ polygon[j].n[2] - 1 ].k;
            (*normals)[n+2] = normcoord[ polygon[j].n[2] - 1 ].j;
            // Texture coordinates of the first vertex
            (*texcoords)[t+0] = mapcoord[ polygon[j].t[2] - 1 ].u,
            (*texcoords)[t+1] = 1.0f - mapcoord[ polygon[j].t[2] - 1 ].v;
            // Coordinates of the first vertex
            (*vertices)[v+0] = vertex[ polygon[j].v[2] - 1].x,
            (*vertices)[v+1] = vertex[ polygon[j].v[2] - 1].z,
            (*vertices)[v+2] = vertex[ polygon[j].v[2] - 1].y;
            v+=3, n+=3, t+=2;
        }
        
        return vertex_count;
    }
    
    
    for (int j=0;j<polygons_qty;j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        (*normals)[n+0] = normcoord[ polygon[j].n[0] - 1 ].i;
        (*normals)[n+1] = normcoord[ polygon[j].n[0] - 1 ].j;
        (*normals)[n+2] = normcoord[ polygon[j].n[0] - 1 ].k;
        // Texture coordinates of the first vertex
        (*texcoords)[t+0] = mapcoord[ polygon[j].t[0] - 1 ].u,
        (*texcoords)[t+1] =  mapcoord[ polygon[j].t[0] - 1 ].v;
        // Coordinates of the first vertex
        (*vertices)[v+0] = vertex[ polygon[j].v[0] - 1].x,
        (*vertices)[v+1] = vertex[ polygon[j].v[0] - 1].y,
        (*vertices)[v+2] = vertex[ polygon[j].v[0] - 1].z;
        v+=3, n+=3, t+=2;
        
        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the first vertex
        (*normals)[n+0] = normcoord[ polygon[j].n[1] - 1 ].i;
        (*normals)[n+1] = normcoord[ polygon[j].n[1] - 1 ].j;
        (*normals)[n+2] = normcoord[ polygon[j].n[1] - 1 ].k;
        // Texture coordinates of the first vertex
        (*texcoords)[t+0] = mapcoord[ polygon[j].t[1] - 1 ].u,
        (*texcoords)[t+1] =  mapcoord[ polygon[j].t[1] - 1 ].v;
        // Coordinates of the first vertex
        (*vertices)[v+0] = vertex[ polygon[j].v[1] - 1].x,
        (*vertices)[v+1] = vertex[ polygon[j].v[1] - 1].y,
        (*vertices)[v+2] = vertex[ polygon[j].v[1] - 1].z;
        v+=3, n+=3, t+=2;
        
        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the first vertex
        (*normals)[n+0] = normcoord[ polygon[j].n[2] - 1 ].i;
        (*normals)[n+1] = normcoord[ polygon[j].n[2] - 1 ].j;
        (*normals)[n+2] = normcoord[ polygon[j].n[2] - 1 ].k;
        // Texture coordinates of the first vertex
        (*texcoords)[t+0] = mapcoord[ polygon[j].t[2] - 1 ].u,
        (*texcoords)[t+1] =  mapcoord[ polygon[j].t[2] - 1 ].v;
        // Coordinates of the first vertex
        (*vertices)[v+0] = vertex[ polygon[j].v[2] - 1].x,
        (*vertices)[v+1] = vertex[ polygon[j].v[2] - 1].y,
        (*vertices)[v+2] = vertex[ polygon[j].v[2] - 1].z;
        v+=3, n+=3, t+=2;
    }
    
    return vertex_count;
}

void object_type::render()
{
    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    
    for (int j=0;j<polygons_qty;j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f( normcoord[ polygon[j].n[0] - 1 ].i,
                   normcoord[ polygon[j].n[0] - 1 ].j,
                   normcoord[ polygon[j].n[0] - 1 ].k);
        // Texture coordinates of the first vertex
        glTexCoord2f( mapcoord[ polygon[j].t[0] - 1 ].u,
                     mapcoord[ polygon[j].t[0] - 1 ].v);
        // Coordinates of the first vertex
        glVertex3f( vertex[ polygon[j].v[0] - 1].x,
                   vertex[ polygon[j].v[0] - 1].y,
                   vertex[ polygon[j].v[0] - 1].z);
        
        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f( normcoord[ polygon[j].n[1] - 1 ].i,
                   normcoord[ polygon[j].n[1] - 1 ].j,
                   normcoord[ polygon[j].n[1] - 1 ].k);
        // Texture coordinates of the first vertex
        glTexCoord2f( mapcoord[ polygon[j].t[1] - 1 ].u,
                     mapcoord[ polygon[j].t[1] - 1 ].v);
        // Coordinates of the first vertex
        glVertex3f( vertex[ polygon[j].v[1] - 1].x,
                   vertex[ polygon[j].v[1] - 1].y,
                   vertex[ polygon[j].v[1] - 1].z);
        
        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f( normcoord[ polygon[j].n[2] - 1 ].i,
                   normcoord[ polygon[j].n[2] - 1 ].j,
                   normcoord[ polygon[j].n[2] - 1 ].k);
        // Texture coordinates of the first vertex
        glTexCoord2f( mapcoord[ polygon[j].t[2] - 1 ].u,
                     mapcoord[ polygon[j].t[2] - 1 ].v);
        // Coordinates of the first vertex
        glVertex3f( vertex[ polygon[j].v[2] - 1].x,
                   vertex[ polygon[j].v[2] - 1].y,
                   vertex[ polygon[j].v[2] - 1].z);
    }
    glEnd();
}


/*
 vertex_type vertex[MAX_VERTICES];
 mapcoord_type mapcoord[MAX_VERTICES];
 normcoord_type normcoord[MAX_NORMALS];
 polygon_type polygon[MAX_POLYGONS];
 int id_texture
 */


int object_type::objdatadisplay()
{
    int i;
    printf("VERTICES: %d\n",vertices_qty);
    for (i =0;i<vertices_qty;i++)
    {
        printf("%f %f %f\n",vertex[i].x,vertex[i].y,vertex[i].z);
    }
    
    printf("NORMALS: %d\n",normcoord_qty);
    for (i =0;i<normcoord_qty;i++)
    {
        printf("%f %f %f\n",normcoord[i].i,normcoord[i].j,normcoord[i].k);
    }
    printf("MAP COORDS: %d\n",mapcoord_qty);
    for (i =0;i<mapcoord_qty;i++)
    {
        printf("%f %f\n",mapcoord[i].u,mapcoord[i].v);
    }
    printf("POLYGONS: %d\n",polygons_qty);
    for (i=0;i<polygons_qty;i++) //for each vertex of polygon (triangle)
    {
        for (int j = 0;j<3;j++)
        {
            printf("%d::%d/%d/%d\n",i,polygon[i].v[j],polygon[i].t[j],polygon[i].n[j]);
        }
    }
    return 1;
}


int object_type::objloader(char *p_filename)
{
    int ivertex=0; //Index variable
    int inormal =0;
    int ipolygon=0;
    int imap=0;
    char string[256];
    FILE *l_file; //File pointer
    
    char l_char; //Char variable
    
    unsigned short l_face_flags; //Flag that stores some face information
    
    if ((l_file=fopen (p_filename, "rt"))== NULL) return 0; //Open the file
    
    
    while (!feof(l_file)) //Loop to scan the whole file
    {
        fscanf(l_file,"%c",&l_char);
        if(l_char=='\n')//read char if'/n' -skip to next and read
            fscanf(l_file,"%c",&l_char);
        switch (l_char) //parse
        {
            default: fgets(string,256,l_file);
                break;
            case 'v':   //a vertex or a normal or a text co-ord
                fscanf(l_file,"%c",&l_char);
                switch (l_char)
            {
                case ' ':   //a vertex -expect and so read 3 floats next
                    fscanf(l_file,"%f %f %f",&vertex[ivertex].x, &vertex[ivertex].y,&vertex[ivertex].z);
                    
                    
                    ivertex++;
                    break;
                case 'n': //a normal -expect and so read 3 floats next
                    fscanf(l_file,"%f %f %f",&normcoord[inormal].i, &normcoord[inormal].j,&normcoord[inormal].k);
                    inormal++;
                    break;
                case 't': //a texture map coord-expect and so read 2 floats next
                    fscanf(l_file,"%f %f",&mapcoord[imap].u, &mapcoord[imap].v);
                    imap++;
                    break;
            }  //end switch
                break;
            case 'f': //a face read next assume format is -> f 1/1/1 2/2/2 3/3/3
                for (int i=0;i<3;i++) //for each vertex of polygon (triangle)
                {
                    fscanf(l_file,"%c",&l_char); //read space char - ignore this
                    fscanf(l_file,"%d",&polygon[ipolygon].v[i]); //read vertex.
                    fscanf(l_file,"%c",&l_char); //read space char - ignore this
                    fscanf(l_file,"%d",&polygon[ipolygon].t[i]); //read text coord.
                    fscanf(l_file,"%c",&l_char); //read space char - ignore this
                    fscanf(l_file,"%d",&polygon[ipolygon].n[i]); //read normal.
                }
                ipolygon++;
                break;
                
        } //end switch
    }
    
    fclose (l_file); // Closes the file stream
    vertices_qty = ivertex;
    polygons_qty = ipolygon;
    mapcoord_qty = imap;
    normcoord_qty = inormal;
    
    return 1;  //if successful    
}