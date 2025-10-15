/*
 *   GRAVITY STRIKE -- enemies/euser.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _E_USER_
#define _E_USER_

#include "../gsenemy.h"

class enemyUser : public Enemy
{
  private:
    int f_add;                                    // frame add (update)
    int user_image;                               // number of user image (enemies->userframe)

  public:
    enemyUser(int x, int y, float user1=0, float user2=0, float user3=0, float user4=0, float user5=0, float user6=0);
    ~enemyUser();

    void move();
    void update();
    void dead();
    void shoot();
    int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
};

#endif

