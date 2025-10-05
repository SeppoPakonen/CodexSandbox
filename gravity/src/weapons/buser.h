/*
 *   GRAVITY STRIKE -- weapons/buser.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_USER_
#define _B_USER_

#include "../gsbullet.h"

#include "../luaclass.h"

class bulletUser : public Bullet
{
  private:
    int mode;                                      // 0=single, 1=double
    long my_by_player;                    // for color
    Lua *bLua;                                     // associated script state

  public:
    bulletUser(class gsPlayer *,  int puser, int pupgrade);
    bulletUser(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade);
    ~bulletUser();

    void move();
    void move(int notrail);
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void draw(BITMAP *);
};

#endif

