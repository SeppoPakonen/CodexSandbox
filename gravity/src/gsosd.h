/*
 *   GRAVITY STRIKE -- gsosd.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSOSD_
#define _GSOSD_

#include "gsframes.h"

enum OSDBIT_MODES {
  OSDBIT_SCREEN,
  OSDBIT_MAP,
  OSDBIT_PANEL
};

enum osd_colors {
  COL_WHITE,
  COL_RED,
  COL_YELLOW,
  COL_BLUE,
  COL_BLACK
};

typedef struct osd_attributes {
  int color;
  int font;
  int underlined : 1;
  int center : 1;
} osd_attributes;

typedef struct typewriter_text {
  int active;
  char origtext[4001];
  char *origtextptr;
  char text[4001];
  int y;
  int cur_x;
  int cur_y;
  int speed;
  osd_attributes attr;

  int curpos;
  int curnextcharwait;
  int curline_length;
  int pause;
  int remove_when_player_moves;
} typewriter_text;

class UserImageAnim
{
  private:
    class OsdBitmap *pbitmap;
    UserImageAnim *prev, *next;
    Image *frames;                                 // frame handler for user images

    int loop;                                      // identifier for looping animations (0 = no loop)
    int x;
    int y;
    int curframe;
    int maxframe;
    int startframe;
    int curframet;
    int maxframet;
    int imgnum;

  public:
    UserImageAnim(class OsdBitmap *, int num, int px, int py, int pmaxframet=5, int loop=1);
    ~UserImageAnim();

    int update();
    void draw();

    UserImageAnim *get_prev()                      { return prev; }
    void   set_prev(UserImageAnim *n)              { prev = n; }
    UserImageAnim *get_next()                      { return next; }
    void   set_next(UserImageAnim *n)              { next = n; }

    int    get_x()                                 { return x; }
    int    get_y()                                 { return y; }
    int    get_loop()                              { return loop; }
    int    get_curframe()                          { return curframe; }
    int    get_startframe()                        { return startframe; }
    int    get_curframet()                         { return curframet; }
    int    get_maxframe()                          { return maxframe; }
    int    get_maxframet()                         { return maxframet; }
    int    get_image()                             { return imgnum; }
    void   set_curframe(int n)                     { curframe = n; }
    void   set_startframe(int n)                   { startframe = n; }
    void   set_maxframe(int n)                     { maxframe = n; }
    void   set_curframet(int n)                    { curframet = n; }
    void   set_maxframet(int n)                    { maxframet = n; }
    void   set_image(int n);
};

class OsdBitmap
{
  private:
    OsdBitmap *prev, *next;
    UserImageAnim *first_anim, *last_anim;         // user iamge animations
    BITMAP *bitmap;                                // bitmap to draw on
    BITMAP *back;                                  // saved background
    BITMAP *to_bitmap;                             // no map? draw to to_bitmap... (Panel-OSD)
    class gsMap *scr;                              // pointer to map
    float  x;                                      // x position
    float  y;                                      // y position
    int    w;                                      // width
    int    h;                                      // height
    int    mode;                                   // absolute / screen
    int    sig;                                    // signature
    int    remtime;                                // remaining time
    int    active;                                 // is this osd active?
    int    visible;                                // is this osd visible?
    int    dont_save_back;                         // don't save background? (Panel-OSD)
    int    last_clear_color;                       // last color that was cleared to

    typewriter_text typewriter;                    // typewriter object
    void   update_typewriter_text();
    void   update_images();
    void   draw_images();

  public:
    OsdBitmap(class gsMap *scr, int sig, int x, int y, int w, int h, int mode);
    OsdBitmap(BITMAP *scr, int sig, int x, int y, int w, int h, int mode);
    ~OsdBitmap();

    // get/set
    OsdBitmap *get_prev()                          { return prev; }
    OsdBitmap *get_next()                          { return next; }
    float  get_x()                                 { return x; }
    float  get_y()                                 { return y; }
    void   set_x(float n)                          { x = n; }
    void   set_y(float n)                          { y = n; }
    int    get_width()                             { return w; }
    int    get_height()                            { return h; }
    int    get_mode()                              { return mode; }
    int    get_sig()                               { return sig; }
    BITMAP *get_bitmap()                           { return bitmap; }
    int    get_active()                            { return active; }
    int    get_timeout()                           { return remtime; }
    int    get_dont_save_back()                    { return dont_save_back; }
    void   set_dont_save_back(int n)               { dont_save_back = 1; }
    int    get_last_clear_color()                  { return last_clear_color; }

    void   set_prev(OsdBitmap *n)                  { prev = n; }
    void   set_next(OsdBitmap *n)                  { next = n; }
    void   set_mode(int n)                         { mode = n; }
    void   set_active(int n)                       { active = n; if (n==0) { typewriter.active = 0; remtime = 1; } }
    void   set_timeout(int n)                      { active = 1; remtime = n; }
    void   set_visible(int n)                      { visible = n; }
    int    get_visible()                           { return visible; }

    // general functions
    void draw();
    void save_background();
    void restore_background();
    int  update();

    // drawing functions
    void clear(int col=0);
    void draw_pixel(int col, int x, int y);
    void draw_rectangle(int col=0, int x1=0, int y1=0, int x2=0, int y2=0);
    void draw_filled_rectangle(int col, int x1, int y1, int x2, int y2);
    void draw_line(int col, int x1, int y1, int x2, int y2);
    void draw_circle(int col, int x, int y, int r);
    void draw_filled_circle(int col, int x, int y, int r);
    void draw_ellipse(int col, int x, int y, int rx, int ry);
    void draw_filled_ellipse(int col, int x, int y, int rx, int ry);
    void fill_area(int col, int x, int y);
    void draw_text(int f, char *t, int col=0, int x=0, int y=0, int mode=0);
    void draw_colored_text(int f, char *t, int col=0, int x=0, int y=0, int mode=0);
    void draw_typewriter_text(char *t, int starty=10, int speed=3, int rect=1, int remove=0);
    UserImageAnim *add_image(int num, int px, int py, int maxframet=5, int loop=1);
    void remove_image(UserImageAnim *);
};

class gsOsd
{
  private:
    class gsMap *mymap;                            // map to draw OSD on
    BITMAP *to_bitmap;                             // bitmap to draw OSD on (for Panel)
    OsdBitmap   *first_osdbit, *last_osdbit;       // pointers to osd bitmaps
    int   count;

  public:
    gsOsd(class gsMap *);
    gsOsd(BITMAP *);
    ~gsOsd();

    // get/set
    OsdBitmap *get_first_osdbit()                  { return first_osdbit; }
    OsdBitmap *get_last_osdbit()                   { return last_osdbit; }

    // functions
    OsdBitmap *add(int sig, int x, int y, int w, int h, int mode);
    OsdBitmap *add_label(int sig, int x, int y, int mode, int fnum, char *t, int col=0, int rect=0);
    void remove(OsdBitmap *);
    void draw();
    void save_background();
    void restore_background();
    void update();
    OsdBitmap *get_osdbit(int sig);
};

#endif
