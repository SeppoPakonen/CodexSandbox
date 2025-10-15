/*
 *   GRAVITY STRIKE -- weapons/bshrink.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _B_SHRINKER_
#define _B_SHRINKER_

#include "../gsbullet.h"

class bulletShrinker : public Bullet
{
  private:
    // variables
//    class BulletSprite *image;
    float orig_turn_speed;                         // original turn_speed
    float dest_xspd;                               // destination speed of bullet
    float dest_yspd;
    float turn_speed;                              // turn speed of rocket

    int search_target(int x, int y, long sig);

  public:
    bulletShrinker(class gsPlayer *,  int puser, int pupgrade);
    bulletShrinker(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade);
    ~bulletShrinker();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

