/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulJpeg.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

//#include <ul.h>
#include "ulGraphicsFile.h"


#include <setjmp.h>

#include "jpeglib.h"
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;



METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


void ulJpeg::put_scanline(int y, unsigned char *buffer, int stride)
{
  DWORD x;
  unsigned char *data;
  data = &m.grfile_data[y * m.dwWidth * 4];

  switch(m.bytes_per_pixel)
  {
  case 3:
    for(x=0;x<m.dwWidth;x++)
    {
      *data++ = *buffer++;
      *data++ = *buffer++;
      *data++ = *buffer++;
      *data++ = 0;
    }       
    break;
  case 4:
    for(x=0;x<m.dwWidth;x++)
    {
      *data++ = *buffer++;
      *data++ = *buffer++;
      *data++ = *buffer++;
      *data++ = *buffer++;
    }       
    break;
  }


}

DWORD * ulJpeg::ReadFile(char * filename, DWORD flags)
{
  
    SAFE_ARRAY_DELETE(m.grfile_data);



    /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct my_error_mgr jerr;
  /* More stuff */
  //FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
   * so that the setjmp() error recovery below can assume the file is open.
   * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
   * requires it in order to read binary files.
   */

  if ((m.fp = fopen(filename, "rb")) == NULL) 
    return 0;

  /* Step 1: allocate and initialize JPEG decompression object */

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) 
  {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    fclose(m.fp);
    return 0;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_stdio_src(&cinfo, m.fp);

  /* Step 3: read file parameters with jpeg_read_header() */

  jpeg_read_header(&cinfo, TRUE);
  /* We can ignore the return value from jpeg_read_header since
   *   (a) suspension is not possible with the stdio data source, and
   *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
   * See libjpeg.doc for more info.
   */

  /* Step 4: set parameters for decompression */

  /* In this example, we don't need to change any of the defaults set by
   * jpeg_read_header(), so we do nothing here.
   */

  /* Step 5: Start decompressor */

  jpeg_start_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* We may need to do some setup of our own at this point before reading
   * the data.  After jpeg_start_decompress() we have the correct scaled
   * output image dimensions available, as well as the output colormap
   * if we asked for color quantization.
   * In this example, we need to make an output work buffer of the right size.
   */ 
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;


  m.dwWidth = cinfo.output_width;
  m.dwHeight = cinfo.output_height;
  SetBytesPerPixel(cinfo.num_components);

  m.grfile_data = (unsigned char *)new LONG[m.dwWidth * m.dwHeight];
  if (m.grfile_data == 0)
    return 0;



  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   */
  int y = 0;
  while (cinfo.output_scanline < cinfo.output_height) 
  {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    jpeg_read_scanlines(&cinfo, buffer, 1);
    /* Assume put_scanline_someplace wants a pointer and sample count. */
    put_scanline(y, buffer[0], row_stride);
    y++;
  }

  /* Step 7: Finish decompression */

  jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
   * with the stdio data source.
   */

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
   * Here we postpone it until after no more JPEG errors are possible,
   * so as to simplify the setjmp error logic above.  (Actually, I don't
   * think that jpeg_destroy can do an error exit, but why assume anything...)
   */
  fclose(m.fp);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  /* And we're done! */
  
  unsigned long * swap = (unsigned long *)m.grfile_data;
  if (flags & UL_GFX_SWAP_RGB)
  {
    DWORD i;
    int a, r,b,g,c;
    for(i=0; i<m.dwHeight * m.dwWidth; i++)
    {
      c = *swap;
      a = (c >> 24) & 0xFF;
      r = (c >> 16) & 0xFF;
      g = (c >> 8) & 0xFF;
      b = (c >> 0) & 0xFF;
      
      *swap++ = (a << 24) | (b << 16) | (g << 8) | r;
    }
  }

  return (unsigned long *)m.grfile_data;

}
