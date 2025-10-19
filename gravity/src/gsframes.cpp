/*
 *   GRAVITY STRIKE -- gsframes.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsframes.h"
#include "gsglob.h"
#include "gshelper.h"
#include "gserror.h"

#include <stdio.h>

Frame::Frame(void *pdata, int ptype, int pkill, int pw, int ph, int ptime)
{
  data = NULL;
  w = h = disptime = kill = type = 0;

  switch (ptype)
  {
    case DATA_RLE    : data = (RLE_SPRITE*)pdata; break;
    case DATA_BITMAP : data = (BITMAP*)pdata; break;
  }

  type = ptype;
  kill = pkill;
  w = pw;
  h = ph;
  disptime = ptime;

  if (type == DATA_RLE)
  {
    if (w == 0) w = ((RLE_SPRITE*)data)->w;
    if (h == 0) h = ((RLE_SPRITE*)data)->h;
  }
  else
  {
    if (w == 0) w = ((BITMAP*)data)->w;
    if (h == 0) h = ((BITMAP*)data)->h;
  }
}

Frame::~Frame()
{
  if (kill)
  {
    switch (type)
    {
      case DATA_RLE    : destroy_rle_sprite((RLE_SPRITE*)data); break;
      case DATA_BITMAP : destroy_bitmap((BITMAP*)data); break;
    }
  }
}

Image::Image(int nosig)
{
  for (int n=0; n < GSFR_MAX_FRAMES; n++)
    frame[n] = NULL;

  framecount = 0;
  sig = -1;

  if (!nosig)
  {
    while (sig == -1 || images->get_image(sig))
      sig = 10000 + gsrand()%10000;
  }
  else sig = -1000;
}

Image::~Image()
{
  for (int n=0; n < framecount; n++)
    delete frame[n];
}

void Image::add_frames_from_datafile(DATAFILE *data, int type, int pos, int num, int w, int h, int t)
{
  for (int n=pos; n < pos+num; n++)
  {
    frame[framecount] = new Frame(data[n].dat, type, 0, w, h, t);
    framecount++;
  }
}

void Image::add_frames_from_imagefile(char *filename, int type, int t, int rot, int dont_touch_filename, DATAFILE *datafile, int datafile_pos)
{
  BITMAP *user_bit;
  RLE_SPRITE *user_rle;
  BITMAP *mybitmap = NULL;
  char mypath[255];

  if (strlen(get_filename(filename)) == 0)
  {
    sig = 0;
    return;
  }

  if (datafile)
  {
    mybitmap = (BITMAP*)(datafile[datafile_pos].dat);
//    errors->log(1, "bitmap bla", mybitmap->h);
  }
  else if (dont_touch_filename)
  {
    strncpy(mypath, filename, 254);
    mybitmap = load_bitmap(mypath, NULL);
  }  
  else
  {
    sprintf(mypath, "%s%s/%s", globals->level_dir,
                               globals->level_files[globals->selected_level].leveldir,
                               filename);
    mybitmap = load_bitmap(mypath, NULL);
  }

  if (!mybitmap)
  {
    errors->log(1, "user image not found or no image file!", mypath);
    create_n_a_frame(type);
    return;
  }

  // ---- retrieve frames ----
  int n;

  // get border color
  int border_color = getpixel(mybitmap, 0, 0);

  // get width of frame sequence
  for (n=0; n < mybitmap->w; n++)
    if (getpixel(mybitmap, n, 0) != border_color) break;
  int sequence_width = n-1;

  // get height of frames
  for (n=1; n < mybitmap->h; n++)
    if (getpixel(mybitmap, 1, n) == border_color) break;
  int frame_height = n-1;

  // get width of first frame
  for (n=1; n < sequence_width; n++)
    if (getpixel(mybitmap, n, 1) == border_color) break;
  int frame_width = n-1;

  // check if sequence consists of equal sized frames
  if (sequence_width % (frame_width+1) != 0)
  {
    errors->log(1, "error: frames doesn't have same sizes!", sequence_width, frame_width);
    create_n_a_frame(type);
    return;
  }

  // if so, then crop frames
  int frames = sequence_width / (frame_width+1);
  int minx=0, miny=0, maxx=0, maxy=0;
  for (n=0; n < frames; n++)
  {
    user_bit = create_bitmap(frame_width, frame_height);
    blit(mybitmap, user_bit, 1+n*(frame_width+1), 1, 0, 0, frame_width, frame_height);

    if (rot)
    {
      BITMAP *tmpbit = create_bitmap(user_bit->w*3, user_bit->h*3);
      clear(tmpbit);
      rotate_sprite(tmpbit, user_bit, 10, 10, ftofix(rot/1.40625));

      // find corners
      if (n == 0)
      {
        int tmpx, tmpy;
        minx=tmpbit->w; miny=tmpbit->h;
        maxx=0; maxy=0;
        for (tmpx=0; tmpx < tmpbit->w; tmpx++)
          for (tmpy=0; tmpy < tmpbit->h; tmpy++)
          {
            if (getpixel(tmpbit, tmpx, tmpy))
            {
              if (maxx < tmpx) maxx = tmpx;
              if (maxy < tmpy) maxy = tmpy;
              if (minx > tmpx) minx = tmpx;
              if (miny > tmpy) miny = tmpy;
            }
          }
      }

      // copy to mybitmap
      destroy_bitmap(user_bit);
      user_bit = create_bitmap(maxx-minx+2, maxy-miny+2);
      blit(tmpbit, user_bit, minx-1, miny-1, 0, 0, maxx-minx+2, maxy-miny+2);
      destroy_bitmap(tmpbit);
    }

    if (type == DATA_RLE)
    {
      user_rle = get_rle_sprite(user_bit);
      frame[framecount] = new Frame(user_rle, type, 1, 0, 0, t);
      destroy_bitmap(user_bit);
    }
    else
    {
      frame[framecount] = new Frame(user_bit, type, 1, 0, 0, t);
    }
    
    framecount++;
  }

  w = frame[0]->get_w();
  h = frame[0]->get_h();

  if (!datafile)
  {
    errors->log(2, "loaded user image", mypath, frames);
    destroy_bitmap(mybitmap);
  }
}

void Image::create_n_a_frame(int type)
{
  BITMAP *user_bit = create_bitmap(50, 25);
  clear(user_bit);
  rect(user_bit, 0, 0, 49, 24, globals->col_white);
  textout_centre(user_bit, (ALLEGRO_FONT*)NULL, "n/a", 25, 10, globals->col_yellow);

  RLE_SPRITE *user_rle = get_rle_sprite(user_bit);
  frame[framecount] = new Frame(user_rle, type, 1, 0, 0, 0);
  framecount++;

  destroy_bitmap(user_bit);
}

void Image::add_rle_frame(RLE_SPRITE *data)
{
}

void Image::delete_frame(int num)
{
}

RLE_SPRITE *Image::get_rle_frame(int num)
{
  if (num >= 0 && num <= framecount)
    return (RLE_SPRITE*)frame[num]->get_data();

  return NULL;
}

BITMAP *Image::get_bitmap_frame(int num)
{
  if (num >= 0 && num <= framecount)
    return (BITMAP*)frame[num]->get_data();

  return NULL;
}

gsImage::gsImage()
{
  count = 0;
  first_image = last_image = NULL;
}

gsImage::~gsImage()
{
  Image *temp, *next_image;
  temp = first_image;

  while (temp != NULL)
  {
    next_image = temp->get_next();
    delete temp;
    temp = next_image;
  }
}

Image *gsImage::add()
{
  Image *temp_image = NULL;

  temp_image = new Image();

  if (temp_image != NULL)
  {
    if (first_image==NULL)
    {
      last_image = first_image = temp_image;
      first_image->set_next(NULL); first_image->set_prev(NULL);
      temp_image->set_next(NULL); temp_image->set_prev(NULL);
    }
    else
    {
      last_image->set_next(temp_image);
      temp_image->set_prev(last_image);
      last_image = temp_image;
      temp_image->set_next(NULL);
    }
  } // temp_image != NULL

  count++;
  return temp_image;
}

void gsImage::remove(Image *temp)
{
  Image *next_image, *prev_image;

  if (temp != NULL)
  {
    prev_image = temp->get_prev();
    next_image = temp->get_next();

    if (prev_image==NULL) first_image = next_image;
    else prev_image->set_next(next_image);

    if (next_image==NULL) last_image = prev_image;
    else next_image->set_prev(prev_image);

    delete temp;
    temp = NULL;

    count--;
  }
}

int gsImage::load(char *filename, int rotate)
{
  Image *temp = add();

  temp->add_frames_from_imagefile(filename, DATA_RLE, 0, rotate);

  return temp->get_sig();
}

void gsImage::remove(int sig)
{
  Image *tmpi = get_image(sig);
  if (tmpi) remove(tmpi);
}

Image *gsImage::get_image(int sig)
{
  Image *temp;
  temp = first_image;

  while (temp != NULL)
  {
    if (temp->get_sig() == sig) return temp;
    temp = temp->get_next();
  }

  return NULL;
}

