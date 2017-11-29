#include <ul.h>
/******************************************************************************
ulRescale.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.
       
Comments:


       
******************************************************************************/
/*
// scale fname fname w h
main(int argc, char * argv[])
{
int r1,c1;
unsigned char * image;
unsigned char * result;
int r2, c2;
int vs,ve,fs,fe;
int v,f;
char name[100];

  
    if (argc != 8)
    {
    printf("ispline prefix(.tga) w h  vs ve fs fe\n");
    exit(1);
    }
    printf("reading\n");
    
      
        
          r2 = atoi(argv[3]);
          c2 = atoi(argv[2]);
          
            
              vs = atoi(argv[4]);
              ve = atoi(argv[5]);
              
                fs = atoi(argv[6]);
                fe = atoi(argv[7]);
                
                  
                    for(v=vs; v<=ve; v++)
                    {
                    for(f=fs; f<=fe; f++)
                    {
                    sprintf(name,"%s_%02d_%04d.tga",argv[1],v,f);
                    
                      printf("reading %s\n", name);
                      image = read_tga(name, NULL, &r1, &c1);
                      
                        printf("Processing\n");
                        result = scale_image(image, r1, c1, NULL, r2, c2);
                        
                          
                            sprintf(name,"scale\\_%s_%02d_%04d.tga",argv[1],v,f);
                            printf("writing %s\n", name);
                            write_tga(name, result, r2, c2);
                            }
                            }
                            return 0;
                            }
                            
*/

void scale_array(real64 * in, int r1, int c1, real64 * result, int r2, int c2);

// real64 to pixel
unsigned char dtop(real64 v)
{
    if (v < 0.0)
        return 0;
    else if (v > 255.0)
        return 255;
    else
        return (unsigned char)(v + 0.5);
}

// if result == NULL buffer is allocated
//
// r g b cvt to real64 precision
// resampled cols
// resampled rowls
// cvt -> unsigned long
//
// pixel order a b g r

unsigned char * scale_image(unsigned char * in, 
                            int planes,
                            int r1, int c1, unsigned char * result, int r2, int c2)
{
    int i;
    unsigned char * data;
    
    
    real64 * r = 0;
    real64 * g = 0;
    real64 * b = 0;
    real64 * a = 0;
    
    real64 * red1 = 0;
    real64 * green1 = 0;
    real64 * blue1 = 0;
    real64 * alpha1 = 0;
    
    
    real64 * red2 = 0;
    real64 * green2 = 0;
    real64 * blue2 = 0;
    real64 * alpha2 = 0;
    
    
    
    //red1 = (real64 *) calloc(r1 * c1, sizeof(real64));
    //green1 = (real64 *) calloc(r1 * c1, sizeof(real64));
    //blue1 = (real64 *) calloc(r1 * c1, sizeof(real64));
    red1 = new real64[r1 * c1];
    green1 = new real64[r1 * c1];
    blue1 = new real64[r1 * c1];
    if (planes == 4)
        alpha1 = new real64[r1 * c1];

    for(i=0; i<r1 * c1; i++)
    {
        red1[i] = 0;
        green1[i] = 0;
        blue1[i] = 0;
        if (planes == 4)
            alpha1[i] = 0;
        
    }
    
    //red2 = (real64 *) calloc(r2 * c2, sizeof(real64));
    //green2 = (real64 *) calloc(r2 * c2, sizeof(real64));
    //blue2 = (real64 *) calloc(r2 * c2, sizeof(real64));
    red2 = new real64[r2 * c2];
    green2 = new real64[r2 * c2];
    blue2 = new real64[r2 * c2];
    if (planes == 4)
        alpha2 = new real64[r2 * c2];
    for(i=0; i<r2 * c2; i++)
    {
        red2[i] = 0;
        green2[i] = 0;
        blue2[i] = 0;
        if (planes == 4)
            alpha2[i] = 0;
    }
    
    if (result == 0)
    {
        //result = (unsigned char *)calloc(r2 * c2, 4);
        result = new unsigned char[r2 * c2 * planes];
    }
    
    data = in;
    r = red1;
    g = green1;
    b = blue1;
    a = alpha1;
    
    for(i=0; i<r1 * c1; i++)
    {
        
        *b++ = (real64)*data++;
        *g++ = (real64)*data++;
        *r++ = (real64)*data++;
        if (planes == 4)
            *a++ = (real64)*data++;
    }
    
    scale_array(red1, r1, c1, red2, r2, c2);
    scale_array(green1, r1, c1, green2, r2, c2);
    scale_array(blue1, r1, c1, blue2, r2, c2);
    if (planes == 4)
        scale_array(alpha1, r1, c1, alpha2, r2, c2);
    
    //printf("Final Result\n");
    
    r = red2;
    g = green2;
    b = blue2;
    a = alpha2;

    data = result;
    for (i=0; i<r2 * c2; i++)
    {
        *data++ = dtop(*b++);
        *data++ = dtop(*g++);
        *data++ = dtop(*r++);
        if (planes == 4)
            *data++ = dtop(*a++);
    }
    
    
    SAFE_ARRAY_DELETE(red1);
    SAFE_ARRAY_DELETE(green1);
    SAFE_ARRAY_DELETE(blue1);
    SAFE_ARRAY_DELETE(alpha1);
    
    SAFE_ARRAY_DELETE(red2);
    SAFE_ARRAY_DELETE(green2); 
    SAFE_ARRAY_DELETE(blue2);
    SAFE_ARRAY_DELETE(alpha2);
    
    return result;
}
