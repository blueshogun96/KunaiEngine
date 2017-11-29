/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulGraphicsFile.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/



#ifndef __NVIDIA_ULGRAPHICSFILE_H__
#define __NVIDIA_ULGRAPHICSFILE_H__


#include <ctype.h>
#include "ulCommon.h"



#ifdef MULTI_THREAD
	#pragma comment(lib, "utilityMT.lib")
	#pragma comment(lib, "libtiffMT.lib")
	#pragma comment(lib, "jpegMT.lib")
	#pragma comment(lib, "gbmMT.lib")

	#pragma comment(lib, "nvImageLibMT.lib")
	#pragma message( "Image Library - nvImageLibraryMT" )
#else
	#pragma comment(lib, "utility.lib")
	#pragma comment(lib, "libtiff.lib")
	#pragma comment(lib, "jpeg.lib")
	#pragma comment(lib, "gbm.lib")
	
	#pragma comment(lib, "nvImageLib.lib")
	#pragma message( "Image Library - nvImageLibrary" )

#endif







class ulGraphicsFile
{
public:
    ulGraphicsFile() { memset(&m, 0, sizeof(m));}
    virtual ~ulGraphicsFile() 
    { 
        SAFE_ARRAY_DELETE(m.grfile_data);
    }
    
    virtual DWORD * ReadFile(const char * filename, DWORD flags = 0);
    virtual int WriteFile(const char *filename, unsigned char *data, 
        const DWORD &width, const DWORD &height, 
        int src_format, int dest_depth, DWORD format);
    
    DWORD * GetDataPointer() {return (DWORD *)m.grfile_data;}
    //void SetDataPointer(DWORD * ptr) {m.data = ptr;}
    
    unsigned short GetWidth() {return (unsigned short)m.dwWidth;}
    unsigned short GetHeight() {return (unsigned short)m.dwHeight;}
    unsigned short GetDepth() {return (unsigned short)m.bytes_per_pixel;}
    
    //void SetWidth(int w) {m.dwWidth = w;}
    //void SetHeight(int h) {m.dwHeight = h;}
    
    void SetBytesPerPixel(int d)
    {
        m.bytes_per_pixel = d;
        m.bits_per_pixel = d << 3;
    }
    
    void SetBitsPerPixel(int d)
    {
        m.bits_per_pixel = d;
        switch(d)
        {
        case 8:
            m.bytes_per_pixel = 1;
            break;
        case 16:
            m.bytes_per_pixel = 2;
            break;
        case 24:
            m.bytes_per_pixel = 3;
            break;
        case 32:
            m.bytes_per_pixel = 4;
            break;
        }
    }
    
    typedef struct ulGraphicsFileData
    {
//        unsigned char *m_data;
        
        DWORD dwWidth;
        DWORD dwHeight;
        
        FILE *fp;
        
        unsigned char *grfile_data;
        int hxw;
        int size_in_bytes;
        int bits_per_pixel;
        int bytes_per_pixel;
        unsigned char * endptr;
    } ulGraphicsFileData;
    
    ulGraphicsFileData m;
};



class ulTiff : public ulGraphicsFile
{   
public:
    DWORD * ReadFile(char * filename, DWORD format = 0);
    
};


class ulBmp: public ulGraphicsFile
{   
public:
    //DWORD * ReadFile(char * filename, DWORD format = 0);
    
    //int WriteFile(char *filename, unsigned char *data, const DWORD &width, const DWORD &height, 
    //  int dest_depth, DWORD format = 0);
    
    //void put_scanline(int y, unsigned char *buffer, int stride);
};


typedef struct pal_entry
{
    unsigned char red, green, blue;
    unsigned short red_sq, green_sq, blue_sq;
} pal_entry_t;

class ulGif : public ulGraphicsFile
{
public:
/*
DWORD * ReadFile(char * filename, DWORD format = 0);
int wread(void);
int bread(void);

  
    HRESULT check_signature(void);
    HRESULT read_screen();
    void load_palette(void);
    unsigned long palette_delta(const pal_entry_t * p1, const pal_entry_t * p2);
    int find_farthest_palette_entry(int divider);
    int find_closest_fg(const pal_entry_t * p, int max_index);
    void sort_palette(void);
    void init_colors(unsigned char *map, int ncolors, char sorted);
    HRESULT read_image(void);
    void read_extension();
    short decoder(short linewidth);
    void map_palette(void);
    int out_line(unsigned char *line, int len);
    short get_next_code(void);
    
      
        
          int nsimulcolor;
          int palette[256][3];
          char global;                    // Is there a global color map?        
          char global_sort;               // Non-zero if global table is sorted. 
          int global_bits;                // Number of bits of global colors     
          unsigned char global_map[256 * 3];      // The global color map.               
          int backclr;                    // Background color (if global is set). 
          int colres;                     // Number of bits of color resolution. 
          int *interleavetable;
          
            pal_entry_t gif_palette[256];   // Max of 256 simultaneous colors.  
            int num_gif_palette_colors;     // Actual simultaneous colors.      
            uchar gif_to_colortable[256];   // Max of 256 simultaneous colors.  
            pal_entry_t *sorted_gif_palette[256];   // Colors in descending importance. 
            int bad_code_count;
            unsigned int row;
    */
};


class ulPPM: public ulGraphicsFile
{
public:
    //DWORD * ReadFile(char * filename, DWORD format = 0);
    //int wread(void);
    //int bread(void);
};



class ulJpeg: public ulGraphicsFile
{
public:
    DWORD * ReadFile(char * filename, DWORD format = 0);
    void put_scanline(int y,  unsigned char *buffer, int stride);
};



#define UL_GFX_SWAP_RGB     0x0001
#define UL_GFX_PAD          0x0004
#define UL_GFX_COMPRESSED   0x0008
#define UL_GFX_SIGNEDPAD    0x0010
#define UL_GFX_INVERT_VERT  0x0020
#define UL_GFX_INVERT_HORIZ 0x0040


#define UL_GFX_565          0x00020000

/*class ulTarga: public ulGraphicsFile
{   
public:
//DWORD * ReadFile(char * filename, DWORD format = 0);

  //int WriteFile(char *filename, unsigned char *data, const DWORD &width, const DWORD &height, 
  //  int dest_depth, DWORD format = 0);
  
    //void put_scanline(int y, unsigned char *buffer, int stride);
};*/




/****************************************************************
*                                                               *
*                        Targa                                  *
*                                                               *
****************************************************************/

#define UL_TGA_LR 0x10
#define UL_TGA_BT 0x20


class ulTarga : public ulGraphicsFile
{
public:
    DWORD * ReadFile(char * filename, DWORD flags = 0);
    
    
    
    int WriteFile(char *filename, unsigned char *data, const DWORD &width, const DWORD &height, 
        int src_format = 32, int dest_depth = 32, DWORD format = 0);
    
    
    void GetPixel(unsigned char * data, DWORD format, int depth,
        DWORD &a, DWORD &r, DWORD &g, DWORD &b);
    void WritePixel(int depth, DWORD a, DWORD r, DWORD g,
        DWORD b);
    
    int GetRLECount(unsigned char *data);
    int GetRawPixelCount(unsigned char *data);
    
    ulTarga();
    ~ulTarga();
    
    int wread(void);
    int bread(void);
    void bwrite(unsigned char data);
    void wwrite(unsigned short data);
    
    
    //DWORD * DataPointer() {return (DWORD *)m_data;}
    
    void Release();
    
    
    void GetPixel(int x, int y,
        real &a, real &r, real&g, real&b);
    
    void SetPixel(int x, int y,
        real &a, real &r, real&g, real&b);
    
    
    unsigned char GetDesc() { return m_desc;}
    void SetDesc(unsigned char desc) { m_desc = desc;}
    
private:
    unsigned char	m_id_length;        /* 0 */
    unsigned char	m_color_map_type;   /* 1 */
    unsigned char	m_type;				/* 2 */
    unsigned short	m_cm_index;         /* 3 4 */
    unsigned short	m_cm_length;        /* 5 6 */
    unsigned char	m_cm_entry_size;    /* 7 */
    unsigned short	m_x_org;			/* 8 9 */
    unsigned short	m_y_org;			/* 10 11 */
    //unsigned short m_width;			/* 12 13 */
    //unsigned short m_height;			/* 14 15 */
    //unsigned char m_depth;			/* 16 */
    unsigned char	m_desc;				/* 17 */
    
    unsigned char	m_id[256];
    unsigned char	m_color_palette[256 * 3];
    

    
};




#endif   // #ifndef __NVIDIA_ULGRAPHICSFILE_H__
