/*
 *   GRAVITY STRIKE -- gsclear.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSCLEAR_
#define _GSCLEAR_

class gsRectangle
{
  public:
    // Variables
    class     gsMap  *scr;
    BITMAP    *bscr;
    gsRectangle *next;
    int       x;
    int       y;
    int       w;
    int       h;

  public:
    // Functions
    gsRectangle(class gsMap *b, int x, int y, int w, int h);
    gsRectangle(BITMAP *b, int x, int y, int w, int h);
    void draw();
};

class gsClear
{
  private:
    // Variables
    gsRectangle *rect, *first_rect;
    int       count;

    // functions

  public:
    // constructor, destructor
    gsClear();
    ~gsClear();

    // variables

    // get/put
    gsRectangle *get_first()                         { return first_rect; }

    // functions
    void add(class gsMap *b, int x, int y, int w, int h);
    void add(BITMAP *v, int x, int y, int w, int h);

    void clear_screen();
    void clear_list();
};

extern gsClear *clearlist;

#endif

