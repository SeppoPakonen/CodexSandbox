/*
 *   GRAVITY STRIKE -- gsframes.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSFRAMES_
#define _GSFRAMES_

#define GSFR_MAX_FRAMES   100

enum GSFR_SPRITE_DATA {
  DATA_RLE,
  DATA_BITMAP
};

class Frame
{
  private:
    void *data;
    int w;
    int h;
    int disptime;
    int kill;                                        // destroy frame when done?
    int type;                                        // image type of frame

  public:
    Frame(void *dat, int type, int kill=0, int w=0, int h=0, int time=0);
    ~Frame();

    int get_kill()                                   { return kill; }
    int get_w()                                      { return w; }
    int get_h()                                      { return h; }
    int get_disptime()                               { return disptime; }
    void set_disptime(int n)                         { disptime = n; }
    int get_type()                                   { return type; }
    void *get_data()                                 { return data; }
};

class Image
{
  private:
    Image *prev, *next;
    Frame *frame[GSFR_MAX_FRAMES];                   // max. 50 frames
    int framecount;                                  // how much frames loaded?
    int sig;                                         // image signature
    int w, h;                                        // width/height of one image

    void create_n_a_frame(int type);                 // create a "not available" frame

  public:
    Image(int nosig=0);
    ~Image();

    // get/set
    int get_framecount()                             { return framecount; }
    Image *get_prev()                                { return prev; }
    Image *get_next()                                { return next; }
    void  set_next(Image *n)                         { next = n; }
    void  set_prev(Image *n)                         { prev = n; }
    int   get_sig()                                  { return sig; }
    int   get_width()                                { return w; }
    int   get_height()                               { return h; }

    // functions
    void add_frames_from_datafile(DATAFILE *, int type, int pos, int num=1, int w=0, int h=0, int t=0); // loads position+num sprites from datafile
    void add_frames_from_imagefile(char *filename, int type, int t=0, int rot=0, int dont_touch_filename=0, DATAFILE *datafile=NULL, int datafile_pos=0); // loads frame sequence from a file
    void add_rle_frame(RLE_SPRITE *);                // adds a frame by pointer
    void delete_frame(int num);                      // deletes a frame in array

    RLE_SPRITE *get_rle_frame(int num);              // return frame with num as rle
    BITMAP *get_bitmap_frame(int num);               // return frame with num as bitmap
};

class gsImage
{
  private:
    int count;
    Image *first_image, *last_image;

  public:
    gsImage();
    ~gsImage();

    // get / put
    Image *get_first()                               { return first_image; }
    Image *get_last()                                { return last_image; }
    int   get_count()                                { return count; }

    // functions
    Image *add();
    int   load(char *, int rotate=0);
    void  remove(Image *);
    void  remove(int num);

    Image *get_image(int sig);
};

extern gsImage *images;

#endif
