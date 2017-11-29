/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\CommonSrc\Utility
File:  ulGif.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/

//#include <ul.h>

#include "ulGraphicsFile.h"
#include <limits.h>
     
#if 0



/*
 * Various error codes used by decoder and my own routines...   It's okay for
 * you to define whatever you want, as long as it's negative...  It will be
 * returned intact up the various subroutine levels...
 */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR EOF
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4


#if 1
#define get_byte() fgetc(m.fp)
#else
/****************************************************************************
Used globally to read from the file/data stream.
March 13, 1991
****************************************************************************/
int 
get_byte(void)
{
  int ch = fgetc(m.fp);
  
  return ch == EOF ? READ_ERROR : ch;
}
#endif



int ulGif::bread()
{
  return fgetc(m.fp);
}


DWORD * ulGif::ReadFile(char * filename, DWORD format)
{
 
    SAFE_ARRAY_DELETE(m.data);
    

  bad_code_count = 0;
  int quit;
  char ch;
  
  

  m.fp = fopen(filename, "rb");
  if (m.fp == 0)
    return 0;
  
  if (check_signature())
    return 0;
  if (read_screen())
    return 0;
  
  m.hxw = m.dwWidth * m.dwHeight;

  m.data = (unsigned char *) new unsigned long[m.hxw];
  if (m.data == 0)
    return 0;



  quit = 0;
  do
  {
    ch = get_byte();
    switch (ch)
    {
    case '\0':
      break;          /* this kludge for non-standard files */
    case ',':
      if (read_image())
      {
        SAFE_ARRAY_DELETE(m.data);
        return 0;
      }
      break;
    case ';':
      quit = 1;
      break;
    case '!':
      read_extension();
      break;
    default:
      break;
    }
    //flush();
  } while (!quit);
  
  fclose(m.fp);
  

  unsigned long * swap = (unsigned long *)m.data;
  if (flags & UL_GFX_SWAP_RGB)
  {
    int i;
    int r,b,g,c;
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


  return (unsigned long *)m.data;


}





HRESULT ulGif::check_signature(void)
{
  char buf[6];
  
  fread(buf, sizeof(buf[0]), 6, m.fp);
  if (strncmp(buf, "GIF", 3))
    return 1; //fatal("file is not a GIF file");
  
  if (strncmp(&buf[3], "87a", 3))
    return 1;
  return 0;//fatal("Only GIF 87a supported");
}

HRESULT ulGif::read_screen()
{
  unsigned char buf[7];
  
  fread(buf, sizeof(buf[0]), 7, m.fp);
  
  if (buf[6])
    return 1; //fatal("screen descriptor is invalid");
  
  m.dwWidth = buf[0] + ((unsigned short) buf[1] << 8);
  m.dwHeight = buf[2] + ((unsigned short) buf[3] << 8);
  global = buf[4] & 0x80;
  colres = ((buf[4] & 0x70) >> 4) + 1;
  global_bits = (buf[4] & 0x07) + 1;
  global_sort = (buf[4] & 0x08) != 0;
  backclr = buf[5];
  
  if (global)
    fread((char *) global_map, 1, 3 * (1 << global_bits), m.fp);
  return 0;
}




/****************************************************************************
Select and initialize the graphics mode appropriate for the image size and
colors.  Return non-zero if successful.

FG should have a get_first_mode(), get_next_mode() that lets the
application query for best mode fit... someday but not today.

This overrides the init_all() in the FG library allowing the reduction of
size of the executable by not pulling in certain drivers that are not useful.

  NOTE: At start up time the global "(*init) ()" is pointing to this
        function.  Calling init will call this function on the first call
        and which will reassign init to the init for the proper display
        graphics adapter.
  RETURNS:  Display type if successful.  NULL if error occured.

March 13, 1991
****************************************************************************/

/****************************************************************************
Set up the hardware for proper color redition.
March 13, 1991
****************************************************************************/
void ulGif::load_palette(void)
{
  pal_entry_t *p;
  int i = nsimulcolor;
  
  if (i > num_gif_palette_colors)
    i = num_gif_palette_colors;
  
  do
  {
    p = sorted_gif_palette[--i];
    palette[i][0] = p->red;
    palette[i][1] = p->green;
    palette[i][2] = p->blue;
    //setpalette(i, p->red, p->green, p->blue);
  } while (i > 0);
}


/****************************************************************************
Return the distance between two palette entries.
March 13, 1991
****************************************************************************/
unsigned long ulGif::palette_delta(const pal_entry_t * p1, const pal_entry_t * p2)
{
  long red, green, blue;
  
  red = p1->red;
  red -= p2->red;
  
  green = p1->green;
  green -= p2->green;
  
  blue = p1->blue;
  blue -= p2->blue;
  
  return red * red + green * green + blue * blue;
}

/****************************************************************************
Below and include 'divider' in sorted_gif_palette[] is the list of palette
entries that are maximally far apart.  Above 'divider' up to
num_gif_palette_colors is a list of entries from which one entry should be
selected that is farthest away from all the entries below 'divider'.

March 13, 1991
****************************************************************************/
int ulGif::find_farthest_palette_entry(int divider)
{
  int ret_val = divider + 1, i_lower, i_upper;
  unsigned long min_metric = ULONG_MAX;
  
  for (i_upper = divider + 1; i_upper < num_gif_palette_colors; i_upper++)
  {
    unsigned long metric = ULONG_MAX;
    pal_entry_t *p = sorted_gif_palette[i_upper];
    
    for (i_lower = 0; i_lower <= divider; i_lower++)
    {
      unsigned long temp = palette_delta(p, sorted_gif_palette[i_lower]);
      if (temp == 0)
      {
        metric = ULONG_MAX;
        break;
      }
      else
        metric += (ULONG_MAX / 256) / temp;
    }
    
    if (metric < min_metric)
    {
      ret_val = i_upper;
      min_metric = metric;
    }
  }
  return ret_val;
}

/****************************************************************************
Find the closest entry to *p in the sorted gif palette prior to this index.
March 13, 1991
****************************************************************************/
int ulGif::find_closest_fg(const pal_entry_t * p, int max_index)
{
  unsigned long min_distance = ULONG_MAX;
  int ret_val, i = max_index - 1;
  
  do
  {
    unsigned long distance = palette_delta(p, sorted_gif_palette[i]);
    
    if (distance < min_distance)
    {
      min_distance = distance;
      ret_val = i;
    }
  } while (--i >= 0);
  
  return ret_val;
}

/****************************************************************************
Sort the gif palette according to the colors that are the greatest distance
apart from each other.  This isn't optimum (optimum would require relative
frequency of each color) but should create a mapping that is better than
random.

If I were to define optimum it would be as follows:

Sum of pal_delta for each pixel kept to a minimum.

pal_delta equals:
  (red - gif_red)^2 + (green - gif_green)^2 + (blue - gif_blue)^2

A crude outline of how create the optimum (using this metric) would be to go
through the image and kept a count of how many occurances of a particular
palette entry occured.  Then the metric could be easily computed as:

  metric_val = 0;
  for(i = 0; i < num_gif_palette_colors; i++)
    metric_val += pal_freq[i] * pal_delta[i];

Then the tough part comes.  Changing the values of red, green, blue
such that the metric is minimized.  A first approximation could be to take
the most frequently occuring palette entrys (largest entries in pal_freq[])
and chose red, green, and blue such that those deltas are minimized.
This won't always work of course...

March 13, 1991
****************************************************************************/
void ulGif::sort_palette(void)
{
  if (num_gif_palette_colors > nsimulcolor)
  {
    pal_entry_t pal;
    int done_index, i;
    
    memset(&pal, 0, sizeof(pal));   /* An entry of BLACK -- a starting
    * point. */
    
    i = find_closest_fg(&pal, num_gif_palette_colors);
    done_index = 0;
    
    do
    {
      pal_entry_t *pal_p;
      
      pal_p = sorted_gif_palette[done_index];
      sorted_gif_palette[done_index] = sorted_gif_palette[i];
      sorted_gif_palette[i] = pal_p;
      
      i = find_farthest_palette_entry(done_index);
    } while (++done_index < nsimulcolor);
  }
}

/****************************************************************************
Map all the GIF color numbers into a FG color number.  Put FG color number
in gif_to_colortable[] at index 'gif_color_number'.  sorted_gif_palette[]
must already be sorted according to decreasing importance.  FG must be
initialized.
March 13, 1991
****************************************************************************/
void ulGif::map_palette(void)
{
  int i;
  
  for (i = 0; i < nsimulcolor; i++)
  {
    gif_to_colortable[i] = i;
  }
  
  while (i < num_gif_palette_colors)
  {
    gif_to_colortable[i] =
      find_closest_fg(sorted_gif_palette[i], nsimulcolor);
    i++;
  }
}

/****************************************************************************
Sort the color palette (if necessary).  This is to decide which colors to use.
Create the table that maps the GIF palette into the available palette.
March 13, 1991
****************************************************************************/
void ulGif::init_colors(unsigned char *map, int ncolors, char sorted)
{
  pal_entry_t **sort_p = &sorted_gif_palette[0];
  pal_entry_t *p = &gif_palette[0];
  
  num_gif_palette_colors = ncolors;
  
  do
  {
    *sort_p++ = p;
    p->red = *map++;
    p->green = *map++;
    p->blue = *map++;
    p++;
  } while (--ncolors > 0);
  
  if (!sorted)
    sort_palette();
  map_palette();
}



// row
int ulGif::out_line(unsigned char *line, int len)
{
  unsigned char *data;
  uchar *p = (uchar *) line;
  int y = interleavetable[row];
  int x;
  
  
  data = &m.data[y * m.dwWidth * 4];
  for (x = 0; x <len; x++)
  {
    *data++ = palette[*line][0];
    *data++ = palette[*line][1];
    *data++ = palette[*line][2];
    *data++ = 0;
    line++;
  }
  row++;
  return 0;
}


HRESULT ulGif::read_image(void)
{
  int color_bits;
  //char mess_buf[80];
  char local, interleaved, sorted;
  unsigned char buf[9], localmap[256 * 3], *map_p;
  //int line_spacing;
  //int mess_y;
  int ymax;
  
  fread(buf, sizeof(buf[0]), 9, m.fp);
  
  //m.dwHeight = (buf[2] + (((int) buf[3]) << 8));
  m.dwHeight = (buf[6] + (((int) buf[7]) << 8));
  //image_box[X1] = buf[0] + (((int) buf[1]) << 8);
  
  m.dwWidth = buf[4] + (((int) buf[5]) << 8);
  //image_box[X2] = image_width - 1;
  
  interleaved = (buf[8] & 0x40) != 0;
  sorted = (buf[8] & 0x20) != 0;
  
  //printf("width %d h %d\n",image_width, image_height);
  //exit(1);
  nsimulcolor = 256;
  
  if (global)
  {
  }
  
  local = buf[8] & 0x80;
  if (local)
  {
    int localbits = (buf[8] & 0x7) + 1;
    color_bits = localbits;
    
    fread(localmap, 1, 3 * (1 << localbits), m.fp);
    map_p = localmap;
  }
  else if (global)
  {
    map_p = global_map;
    color_bits = global_bits;
    sorted = global_sort;
  }
  else
    return 1; //fatal("no colormap present for image");
  
  
  init_colors(map_p, 1 << color_bits, sorted);
  
  load_palette();             /* Changes color definitions. */
  
  interleavetable = (int *) malloc(m.dwHeight * sizeof(int));
  if (!interleavetable)
    return 1;//fatal("not enough memory for interleave table");
  
  ymax = m.dwHeight - 1;
  if (interleaved)
  {
    int y;
    
    row = 0;
    
    for (y = 0; y <= ymax; y += 8)
      interleavetable[row++] = ymax - y;
    
    for (y = 4; y <= ymax; y += 8)
      interleavetable[row++] = ymax - y;
    
    for (y = 2; y <= ymax; y += 4)
      interleavetable[row++] = ymax - y;
    
    for (y = 1; y <= ymax; y += 2)
      interleavetable[row++] = ymax - y;
  }
  else
  {
    int y;
    
    for (y = 0; y <= ymax; y++)
      interleavetable[y] = ymax - y;
  }
  
  row = 0;
  switch (decoder(m.dwWidth))
  {
  case (OUT_OF_MEMORY):
    return 1;//fatal("Out of memory");
    break;
  case (BAD_CODE_SIZE):
    return 1;//fatal("Bad Code Size");
    break;
  case (READ_ERROR):
    return 1;//fatal("Read Error");
    break;
  case (WRITE_ERROR):
    return 1;//fatal("Write Error");
    break;
  case (OPEN_ERROR):
    return 1;//fatal("Open Error");
    break;
  case (CREATE_ERROR):
    return 1;//fatal("Create Error");
    break;
  }
  
  if (interleaved)
    free(interleavetable);
  
  return 0;
}


void ulGif::read_extension()
{
  unsigned char code, count;
  char buf[255];
  
  code = get_byte();
  while ((count = get_byte()) != 0)
    fread(buf, sizeof(buf[0]), count, m.fp);
}
/*************************
Remaining code is copyright by Steven A. Bennett
I have made numerous changes however it is still fundamentally his code.
*************************/

/*
 * DECODE.C - An LZW decoder for GIF Copyright (C) 1987, by Steven A. Bennett
 * 
 * Permission is given by the author to freely redistribute and include this
 * code in any program as long as this credit is given where due.
 * 
 * In accordance with the above, I want to credit Steve Wilhite who wrote the
 * code which this is heavily inspired by...
 * 
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of Compuserve,
 * Incorporated, an H&R Block Company.
 * 
 * Release Notes: This file contains a decoder routine for GIF images which is
 * similar, structurally, to the original routine by Steve Wilhite. It is,
 * however, somewhat noticably faster in most cases.
 * 
 */

/*
 * extern int get_byte()
 * 
 * - This external (machine specific) function is expected to return either the
 * next byte from the GIF file, or a negative number, as defined in ERRS.H.
 */

/*
 * extern int out_line(unsigned char *pixels, int linelen)
 * 
 * - This function takes a full line of pixels (one byte per pixel) and displays
 * them (or does whatever your program wants with them...).  It should return
 * zero, or negative if an error or some other event occurs which would
 * require aborting the decode process...  Note that the length passed will
 * almost always be equal to the line length passed to the decoder function,
 * with the sole exception occurring when an ending code occurs in an odd
 * place in the GIF file...  In any case, linelen will be equal to the number
 * of pixels passed...
 */
//extern int out_line(unsigned char *pixels, int linelen);

/*
 * extern int bad_code_count;
 * 
 * This value is the only other global required by the using program, and is
 * incremented each time an out of range code is read by the decoder. When
 * this value is non-zero after a decode, your GIF file is probably corrupt
 * in some way...
 */
//extern int bad_code_count;

#define MAX_CODES   4095

/* Static variables */
static short curr_size;         /* The current code size */
static short clear;             /* Value for a clear code */
static short ending;            /* Value for a ending code */
static short newcodes;          /* First available code */
static short top_slot;          /* Highest code for current size */
static short slot;              /* Last read code */

/*
* The following static variables are used for seperating out codes
*/
static short navail_bytes = 0;  /* # bytes left in block */
static short nbits_left = 0;    /* # bits left in current byte */
static unsigned char b1;        /* Current byte */
static unsigned char byte_buff[257];    /* Current block */
static unsigned char *pbytes;   /* Pointer to next byte in block */

static long code_mask[13] =
{
  0,
    0x0001, 0x0003,
    0x0007, 0x000F,
    0x001F, 0x003F,
    0x007F, 0x00FF,
    0x01FF, 0x03FF,
    0x07FF, 0x0FFF
};


/*
* This function initializes the decoder for reading a new image.
*/
static short 
init_exp(short size)
{
  curr_size = size + 1;
  top_slot = 1 << curr_size;
  clear = 1 << size;
  ending = clear + 1;
  slot = newcodes = ending + 1;
  navail_bytes = nbits_left = 0;
  return (0);
}

/*
* get_next_code() - gets the next code from the GIF file.  Returns the code,
* or else a negative number in case of file errors...
*/
short ulGif::get_next_code(void)
{
  short i, x;
  unsigned long ret;
  
  if (nbits_left == 0)
  {
    if (navail_bytes <= 0)
    {
      
    /*
    * Out of bytes in current block, so read next block
      */
      pbytes = byte_buff;
      if ((navail_bytes = get_byte()) < 0)
        return (navail_bytes);
      else if (navail_bytes)
      {
        for (i = 0; i < navail_bytes; ++i)
        {
          if ((x = get_byte()) < 0)
            return (x);
          byte_buff[i] = x;
        }
      }
    }
    b1 = *pbytes++;
    nbits_left = 8;
    --navail_bytes;
  }
  
  ret = b1 >> (8 - nbits_left);
  while (curr_size > nbits_left)
  {
    if (navail_bytes <= 0)
    {
    /*
    * Out of bytes in current block, so read next block
      */
      pbytes = byte_buff;
      if ((navail_bytes = get_byte()) < 0)
        return (navail_bytes);
      else if (navail_bytes)
      {
        for (i = 0; i < navail_bytes; ++i)
        {
          if ((x = get_byte()) < 0)
            return (x);
          byte_buff[i] = x;
        }
      }
    }
    b1 = *pbytes++;
    ret |= b1 << nbits_left;
    nbits_left += 8;
    --navail_bytes;
  }
  nbits_left -= curr_size;
  ret &= code_mask[curr_size];
  return ((short) (ret));
}


/*
 * The reason we have these seperated like this instead of using a structure
 * like the original Wilhite code did, is because this stuff generally
 * produces significantly faster code when compiled... This code is full of
 * similar speedups...  (For a good book on writing C for speed or for space
 * optomisation, see Efficient C by Tom Plum, published by Plum-Hall
 * Associates...)
 */
static unsigned char stack[MAX_CODES + 1];      /* Stack for storing pixels */
static unsigned char suffix[MAX_CODES + 1];     /* Suffix table */
static unsigned short prefix[MAX_CODES + 1];    /* Prefix linked list */

/*
 * short decoder(linewidth) short linewidth;               * Pixels per line
 * of image *
 * 
 * - This function decodes an LZW image, according to the method used in the GIF
 * spec.  Every *linewidth* "characters" (ie. pixels) decoded will generate a
 * call to out_line(), which is a user specific function to display a line of
 * pixels.  The function gets it's codes from get_next_code() which is
 * responsible for reading blocks of data and seperating them into the proper
 * size codes.  Finally, get_byte() is the global routine to read the next
 * byte from the GIF file.
 * 
 * It is generally a good idea to have linewidth correspond to the actual width
 * of a line (as specified in the Image header) to make your own code a bit
 * simpler, but it isn't absolutely necessary.
 * 
 * Returns: 0 if successful, else negative.
 * 
 */

short ulGif::decoder(short linewidth)
{
  unsigned char *sp, *bufptr;
  unsigned char *buf;
  short code, fc, oc, bufcnt;
  short c, size, ret;
  
  /*
  * Initialize for decoding a new image...
  */
  if ((size = get_byte()) < 0)
    return (size);
  if (size < 2 || 9 < size)
    return (BAD_CODE_SIZE);
  init_exp(size);
  
  /*
  * Initialize in case they forgot to put in a clear code. (This shouldn't
  * happen, but we'll try and decode it anyway...)
  */
  oc = fc = 0;
  
  /*
  * Allocate space for the decode buffer
  */
  
  buf = (unsigned char *) malloc(linewidth + 1);
  if (!buf)
    return (OUT_OF_MEMORY);
  
    /*
    * Set up the stack pointer and decode buffer pointer
  */
  sp = stack;
  bufptr = buf;
  bufcnt = linewidth;
  
  /*
  * This is the main loop.  For each code we get we pass through the
  * linked list of prefix codes, pushing the corresponding "character" for
  * each code onto the stack.  When the list reaches a single "character"
  * we push that on the stack too, and then start unstacking each
  * character for output in the correct order.  Special handling is
  * included for the clear code, and the whole thing ends when we get an
  * ending code.
  */
  while ((c = get_next_code()) != ending)
  {
  /*
  * If we had a file error, return without completing the decode
    */
    if (c < 0)
    {
      free(buf);
      return (0);
    }
    
    /*
    * If the code is a clear code, reinitialize all necessary items.
    */
    if (c == clear)
    {
      curr_size = size + 1;
      slot = newcodes;
      top_slot = 1 << curr_size;
      
      /*
      * Continue reading codes until we get a non-clear code (Another
      * unlikely, but possible case...)
      */
      while ((c = get_next_code()) == clear)
        ;
      
        /*
        * If we get an ending code immediately after a clear code (Yet
        * another unlikely case), then break out of the loop.
      */
      if (c == ending)
        break;
      
        /*
        * Finally, if the code is beyond the range of already set codes,
        * (This one had better NOT happen...  I have no idea what will
        * result from this, but I doubt it will look good...) then set
        * it to color zero.
      */
      if (c >= slot)
        c = 0;
      
      oc = fc = c;
      
      /*
      * And let us not forget to put the char into the buffer... And
      * if, on the off chance, we were exactly one pixel from the end
      * of the line, we have to send the buffer to the out_line()
      * routine...
      */
      *bufptr++ = c;
      if (--bufcnt == 0)
      {
        if ((ret = out_line(buf, linewidth)) < 0)
        {
          free(buf);
          return (ret);
        }
        bufptr = buf;
        bufcnt = linewidth;
      }
    }
    else
    {
    /*
    * In this case, it's not a clear code or an ending code, so it
    * must be a code code...  So we can now decode the code into a
    * stack of character codes. (Clear as mud, right?)
      */
      code = c;
      
      /*
      * Here we go again with one of those off chances...  If, on the
      * off chance, the code we got is beyond the range of those
      * already set up (Another thing which had better NOT happen...)
      * we trick the decoder into thinking it actually got the last
      * code read. (Hmmn... I'm not sure why this works...  But it
      * does...)
      */
      if (code >= slot)
      {
        if (code > slot)
          ++bad_code_count;
        code = oc;
        *sp++ = fc;
      }
      
      /*
      * Here we scan back along the linked list of prefixes, pushing
      * helpless characters (ie. suffixes) onto the stack as we do so.
      */
      while (code >= newcodes)
      {
        *sp++ = suffix[code];
        code = prefix[code];
      }
      
      /*
      * Push the last character on the stack, and set up the new
      * prefix and suffix, and if the required slot number is greater
      * than that allowed by the current bit size, increase the bit
      * size.  (NOTE - If we are all full, we *don't* save the new
      * suffix and prefix...  I'm not certain if this is correct... it
      * might be more proper to overwrite the last code...
      */
      *sp++ = code;
      if (slot < top_slot)
      {
        suffix[slot] = fc = code;
        prefix[slot++] = oc;
        oc = c;
      }
      if (slot >= top_slot)
      {
        if (curr_size < 12)
        {
          top_slot <<= 1;
          ++curr_size;
        }
      }
      /*
      * Now that we've pushed the decoded string (in reverse order)
      * onto the stack, lets pop it off and put it into our decode
      * buffer...  And when the decode buffer is full, write another
      * line...
      */
      while (sp > stack)
      {
        *bufptr++ = *(--sp);
        if (--bufcnt == 0)
        {
          if ((ret = out_line(buf, linewidth)) < 0)
          {
            free(buf);
            return (ret);
          }
          bufptr = buf;
          bufcnt = linewidth;
        }
      }
    }
    }
    ret = 0;
    if (bufcnt != linewidth)
      ret = out_line(buf, (linewidth - bufcnt));
    free(buf);
    return (ret);
}

#endif