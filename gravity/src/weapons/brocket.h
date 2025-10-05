/*
 *   GRAVITY STRIKE -- weapons/brocket.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_ROCKET_
#define _B_ROCKET_

#include "../gsbullet.h"

class bulletRocket : public Bullet
{
  private:
    // variables
    int   time_since_fire;                         // time (in frames) since weapon was fired
    float orig_turn_speed;                         // original turn_speed
    float dest_xspd;                               // destination speed of bullet
    float dest_yspd;
    float turn_speed;                              // turn speed of rocket
    int   shoot_angle;                             // for upgrade 1

    // functions
    int search_target(int x, int y, long sig);

  public:
    bulletRocket(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletRocket(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletRocket();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void hit(float n);
};

#endif

