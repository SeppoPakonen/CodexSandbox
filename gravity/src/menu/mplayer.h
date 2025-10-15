/*
 *   GRAVITY STRIKE -- menu/mplayer.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MPLAYER_
#define _MPLAYER_

class mPlayer
{
  private:
    float   turn_speed;                            // how fast to turn?
    float   acceleration;                          // acceleration of ship

    int     sig;                                   // number identifying this ship
    float   x, y;                                  // "real" coordinates of playship (upper left corner)
    float   head;                                  // direction of playship
    float   xspd, yspd;                            // speed
    float   xacc, yacc;                            // acceleration
    int     frags;                                 // how much hitpoints

    float   move_to_x;                             // target x coordinate
    float   move_to_y;                             // target y coordinate
    float   head_to;                               // target heading (angle)
    float   to_dist;                               // distance to target (time of command)
    float   target_angle;                          // angle to turn ship when target reached

    BITMAP  *mship1;                               // ship without thrusters
    BITMAP  *mship2;                               // ship with thrusters
    BITMAP  *mship3;                               // ship with thrusters 2

  public:
    // constr. / destr.
    mPlayer(int num=0, int t=0, int x=0, int y=0);
    ~mPlayer();

    // get/set
    float get_x()                                  { return x; }
    float get_y()                                  { return y; }
    float get_head()                               { return head; }
    int   get_frags()                              { return frags; }
    void  inc_frags()                              { frags++; }
    int   get_sig()                                { return sig; }

    // functions
    void move_to(int x, int y, int ta=-1);         // move to coordinates and set spec. angle
    void turn_to(int x, int y);                    // turn head to coordinates
    void move();                                   // move player
    void shoot();                                  // shoot a bullet
    void draw(BITMAP *);                           // draw menu player
    void set_position(int x, int y);
};

#endif
