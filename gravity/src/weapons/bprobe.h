/*
 *   GRAVITY STRIKE -- weapons/bprobe.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_PROBE_
#define _B_PROBE_

#include "../gsbullet.h"

class bulletProbe : public Bullet
{
  private:
    // variables
    long target;                                   // -1 if no target set yet (over 1000 if enemy)
    int   time_since_fire;                         // time (in frames) since weapon was fired
    float dxspd;                                   // xspeed to add
    float dyspd;                                   // yspeed to add
    float dest_xspd;                               // destination speed of bullet
    float dest_yspd;
    int   angle;                                   // angle to shoot bullets

    // functions
    int search_target(int x, int y, long sig);

  public:
    bulletProbe(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletProbe(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletProbe();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void hit(float n);
};

#endif

