/*
 *   GRAVITY STRIKE -- gswaypnt.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSWAYPNT_
#define _GSWAYPNT_

class Waypoint {
  private:
    int x;
    int y;
    float speed;
    int pause;
    int curpause;
    Waypoint *next;

  public:
    Waypoint(int x, int y, float speed=1, int pause=0);
    ~Waypoint();

    void set_next(Waypoint *n)                   { next = n; }
    Waypoint *get_next()                         { return next; }

    int get_x()                                  { return x; }
    int get_y()                                  { return y; }
    float get_speed()                            { return speed; }
    int get_pause()                              { return pause; }
    int get_curpause()                           { return curpause; }
    void set_x(int n)                            { x = n; }
    void set_y(int n)                            { y = n; }
    void set_speed(float n)                      { speed = n; }
    void set_pause(int n)                        { pause = n; }
    void set_curpause(int n)                     { curpause = n; }
};

class gsWaypoint
{
  private:
    Waypoint *first;
    int count;                                   // how much waypoints?
    int current;                                 // current waypoint
    int direction;                               // for mode 1: +1 or -1
    int mode;                                    // mode (0: 0-1-2-0 or 1: 0-1-2-1-0, 2: 0-1-2-stop)

  public:
    gsWaypoint();
    ~gsWaypoint();

    int get_count()                              { return count; }
    int get_mode()                               { return mode; }
    void set_mode(int n)                         { mode = n; }
    int get_current()                            { return current; }
    void set_current(int n)                      { current = n; }
    void set_next();

    Waypoint *add(int x, int y, float speed=1, int pause=0);
    void remove(Waypoint *);
    void remove_all();
    Waypoint *get(int n);

//    int get_x(int n);
//    int get_y(int n);
};

#endif

