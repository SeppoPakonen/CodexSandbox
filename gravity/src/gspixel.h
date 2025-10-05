/*
 *   GRAVITY STRIKE -- gspixel.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _PIXCLASS_
#define _PIXCLASS_

enum PIXELRAIN_TYPES
{
  PIX_WHITE,
  PIX_YELLOW,
  PIX_GREEN,
  PIX_BLUE,
  PIX_BLUEWHITE,
  PIX_RED,
  PIX_ALL,
  PIX_DEBRIS1,
  PIX_DEBRIS2,
  PIX_DEBRIS3,
  PIX_DEBRIS4,
  PIX_DEBRIS5
};

class Pixel
{
 public:
  Pixel *prev, *next;
  BITMAP *image;                                   // bitmap of pixel (optional)
  float x;                                         // x position
  float y;                                         // y position
  float xspd;                                      // x speed
  float yspd;                                      // y speed
  float xspd2;                                     // secondary x speed
  float yspd2;                                     // secondary y speed
  int   startx;                                    // start point x of pixel
  int   starty;                                    // start point y of pixel
  int   color;                                     // color
  int   tcolor;                                    // ??
  int   lifetime;                                  // pixel life time
  int   max_xab;                                   // maximum travel in x direction
  int   max_yab;                                   // maximum travel in y direction
  int   *color_table;                              // appropriate color scheme
  int   dspd;                                      // fading speed
  int   bounce;                                    // bounce count
  float vdec;                                      // vdec variable

 public:
  Pixel(float x, float y,
        float xspd, float yspd, float xspd2, float yspd2,
        int startx, int starty,
        int max_xab, int max_yab, int *color_table,
        int maxtable=10, int lifetime=6000, int dspd=1, BITMAP *image=NULL, float vdec=0);
  ~Pixel();

  Pixel *get_prev()                                { return prev; }
  Pixel *get_next()                                { return next; }
  float get_x()                                    { return x; }
  float get_y()                                    { return y; }
  void set_x(float w)                              { x = w; }
  void set_y(float w)                              { y = w; }
  float get_xspd()                                 { return xspd; }
  float get_yspd()                                 { return yspd; }
  void set_xspd(float w)                           { xspd = w; }
  void set_yspd(float w)                           { yspd = w; }

  void draw(class gsMap *);
  void draw(BITMAP *);
  void move(float gravity);
};

class gsPixelRain
{
 private:
  int count;
  Pixel *first_pixel, *last_pixel;

 public:
  // constructor / destructor
  gsPixelRain();
  ~gsPixelRain();

  // variables

  // get / put
  Pixel *get_first()                               { return first_pixel; }
  Pixel *get_last()                                { return last_pixel; }
  int   get_count()                                { return count; }

  // functions
  void add(int anz, int startx, int starty, int xradius, int yradius,
           float xab, float yab, int spd,
           int *colortable, int maxtable, int lifetime=6000, int dspd=0,
           int max_xab=1500, int max_yab=1500, int img=0,
           DATAFILE *d=NULL, BITMAP *bit=NULL, float vdec=0);
  void add(int anz, int startx, int starty, int xradius, int yradius,
           float xab, float yab, int spd,
           int what, int maxtable=10, int lifetime=6000, int dspd=0,
           int max_xab=1500, int max_yab=1500, float vdec=0);
  void draw(class gsMap *);
  void draw(BITMAP *);
  void move(float gravity);
  void check_collisions(class gsMap *);
  void clear(class gsMap *);
  void clear(BITMAP *);

 protected:
  void remove(Pixel *temp);
};

#endif

