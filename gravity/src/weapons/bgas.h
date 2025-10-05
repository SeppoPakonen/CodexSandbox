/*
 *   GRAVITY STRIKE -- weapons/bgas.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_GAS_
#define _B_GAS_

#include "../gsbullet.h"

class bulletGas : public Bullet
{
  private:
    float dxspd;                                   // xspeed to add
    float dyspd;                                   // yspeed to add
    int   ttl;                                     // time to live
    int   timeleft;                                // time till explosion
    int   destruction_radius;                      // destruction radius
    int   max_ttl;

    void  explode();                               // do explosion effects
    void  check_mine_radius();

    void  apply_gravity_to(int px, int py, float *pxs, float *pys, int ptype, int str);
    void  apply_gravity(int s);
    
  public:
    bulletGas(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletGas(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletGas();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

