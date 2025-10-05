/*
 *   GRAVITY STRIKE -- gsexplo.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSEXPLO_
#define _GSEXPLO_

enum EXPLOSION_TYPES
{
  EXP_SMALL_1,
  EXP_SMALL_2,
  EXP_SMALL_3,
  EXP_SMALL_4,
  EXP_MED_1,
  EXP_BIG_1,
  EXP_BIG_2
};

class Explosion
{
 private:
  Explosion *prev, *next;
  BITMAP *background;                              // background of explosion
  DATAFILE *dat;                                   // datafile to fetch bitmap data from
  long expl_player;                                // player associated with effect_shield
  int   x;                                         // x position
  int   y;                                         // y position
  int   w;                                         // width of explosion
  int   h;                                         // height of explosion
  int   bw;                                        // width of background
  int   bh;
  int   type;                                      // type of explosion (position of image in dataf)
  int   starttime;                                 // delay until explosion starts
  int   curframe;                                  // current frame
  int   maxframe;                                  // maximal frame
  int   curframet;                                 // visibility time of current frame
  int   maxframet;                                 // maximal visibility time of current frame
  long player_sig;                                 // sig of player for player effects (shield...)
                                                   // (-1 for stationary explosions)
  int   back_okay;                                 // background filled with actual background?

 public:
  Explosion(long pplayer_nr, int ptype, int pmaxframe, int pmaxframet, int pstarttime, int px, int py, DATAFILE *pdat);
  ~Explosion();

  Explosion *get_prev()                            { return prev; }
  Explosion *get_next()                            { return next; }
  void set_prev(Explosion *n)                      { prev = n; }
  void set_next(Explosion *n)                      { next = n; }
  int get_back_okay()                              { return back_okay; }
  int get_type()                                   { return type; }
  int get_x()                                      { return x; }
  int get_y()                                      { return x; }
  int get_width()                                  { return w; }
  int get_height()                                 { return h; }
  int get_starttime()                              { return starttime; }
  int get_curframe()                               { return curframe; }
  int get_maxframe()                               { return maxframe; }
  int get_maxframet()                              { return maxframet; }
  int get_player_sig()                             { return player_sig; }

  void draw(class gsMap *);                        // draws explosion frame
  void restore(class gsMap *);                     // restores background of explosion
  void save_background(gsMap *, gsMap *);          // saves background of explosion
  int update(class gsMap *, class gsMap *);        // updates explosion. returns 1 when explosion done.
};

class gsExplosion
{
 private:
  Explosion *exp, *first_exp, *last_exp;
  gsMap     *m, *m2;
  int count;

 public:
  // constructor / destructor
  gsExplosion(class gsMap *, class gsMap *);
  ~gsExplosion();

  // variables

  // get / put
  Explosion *get_first()                           { return first_exp; }
  Explosion *get_last()                            { return last_exp; }
  int get_count()                                  { return count; }

  // functions
  Explosion *add(long pplayer_sig, int ptype, int pmaxframe, int pmaxframet, int pstarttime, int px, int py, DATAFILE *dat);
  Explosion *add(int what, int speed, int pstart, int px, int py); // easyer for script
  void save_background();
  void draw(class gsMap *m);
  void draw_shield(class gsMap *m);
  void clear(class gsMap *m);
  void update();

 protected:
  void remove(Explosion *temp);
};

#endif

