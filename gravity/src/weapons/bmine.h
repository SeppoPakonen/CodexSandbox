/*
 *   GRAVITY STRIKE -- weapons/bmine.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _B_MINE_
#define _B_MINE_

#include "../gsbullet.h"

class bulletMine : public Bullet
{
  private:
    float dxspd;                                   // xspeed to add
    float dyspd;                                   // yspeed to add
    int   activation_radius;                       // activation radius
    int   destruction_radius;                      // destruction radius
    int   visibility_radius;                       // visibility radius
    
    void  explode();                               // do explosion effects
    void  check_mine_radius();
    void  apply_gravity();
    void  apply_gravity_to(int px, int py, float *pxs, float *pys, int ptype, int str);

  public:
    bulletMine(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletMine(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletMine();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void hit(float n);
};

#endif

