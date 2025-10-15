/*
 *   GRAVITY STRIKE -- menu/mstars.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MSTARS_
#define _MSTARS_

class mStar
{
  private:
    mStar   *next;
    mStar   *prev;
    float   x;
    float   y;
    float   xspd;
    float   yspd;
    int     col;

  public:
    // constr. / destr.
    mStar(int x, int y, float xspd, float yspd, int col);
    ~mStar();

    // get / set
    float  get_x()                                 { return x; }
    void   set_x(float n)                          { x = n; }
    float  get_y()                                 { return y; }
    void   set_y(float n)                          { y = n; }
    float  get_xspd()                              { return xspd; }
    void   set_xspd(float n)                       { xspd = n; }
    float  get_yspd()                              { return yspd; }
    void   set_yspd(float n)                       { yspd = n; }
    int    get_col()                               { return col; }
    void   set_col(int n)                          { col = n; }
    mStar  *get_next()                             { return next; }
    void   set_next(mStar *n)                      { next = n; }
    mStar  *get_prev()                             { return prev; }
    void   set_prev(mStar *n)                      { prev = n; }

    // functions
    void draw(BITMAP *);
    void update();
};

class mStars
{
  private:
    mStar  *first_star;
    mStar  *last_star;
    int    count;

  public:
    // constr. / destr.
    mStars();
    ~mStars();

    // get / set

    // functions
    void add(int n);
    void update();
    void draw(BITMAP *);
};

#endif
