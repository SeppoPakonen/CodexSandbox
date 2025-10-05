/*
 *   GRAVITY STRIKE -- gscrater.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

/*
     Class gsCrater

     Handling of craters in map
*/

#include "allegro.h"

#ifndef _GSCRATER_
#define _GSCRATER_

class Crater
{
  private:
    // variables
    Crater *prev, *next;                           // previous or next crater in linked list

    int in_wall;                                   // crater in wall (1) or free space (0)? -1 if undecided
    int x;                                         // x pos of crater
    int y;                                         // y pos of crater
    int size;                                      // size of crater in pixels

    bool drawn;                                    // crater is actually drawn on visible map

  public:
    // Constr. / Destructor
    Crater(int px, int py, int psize);
    ~Crater();

    // get / set
    int      get_in_wall()                         { return in_wall; }
    void     set_in_wall(int n)                    { in_wall = n; }
    int      get_x()                               { return x; }
    void     set_x(int n)                          { x = n; }
    int      get_y()                               { return y; }
    void     set_y(int n)                          { y = n; }
    int      get_size()                            { return size; }
    void     set_size(int n)                       { size = n; }
    Crater   *get_prev()                           { return prev; }
    void     set_prev(Crater *n)                   { prev = n; }
    Crater   *get_next()                           { return next; }
    void     set_next(Crater *n)                   { next = n; }

    void     set_drawn(bool d)                     { drawn = d; }
    bool     get_drawn()                           { return drawn; }

    // functions
    void draw(class gsMap *);
};

class gsCrater
{
  private:
    Crater *first_crater, *last_crater;
    int    count;
    bool   all_drawn;

  public:
    // Constr. / Destr.
    gsCrater();
    ~gsCrater();

    // get / set
    Crater *get_first()                            { return first_crater; }
    Crater *get_last()                             { return last_crater; }
    int  get_count()                               { return count; }

    // Functions
    void add(int x, int y, int size, int draw=1, int net_send=1);
    void remove(Crater *);
    void draw(class gsMap *);
    void reset(class gsMap *);
    void update();
};


#endif

