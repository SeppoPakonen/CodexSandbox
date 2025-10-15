/*
 *   GRAVITY STRIKE -- menu/controls/mrship.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MSHIPICON2_
#define _MSHIPICON2_

#include "mcontrol.h"

class mShipIcon2 : public mControl
{
  private:
    class   gsPlayer  *pl;                         // player data
    char    filename[30];
    int     rot;                                   // rotation of ship
    int     dead;                                  // ship exploded?
    int     newship;                               // newship mode (no pic etc.)
    int     error;                                 // error in ship loading?
    int     mode;                                  // mode of icon (with text?)

  public:
    // destr.
    ~mShipIcon2();

    // get / set (overloaded)
    void  set_player(class gsPlayer *);
    int   get_int()                                { return error; }
    void  set_int(int n)                           { mode = n; }

    // functions
    int  update();                                 // overload move function
    void create(int x, int y, int dx, int dy, int w, int h);// create ship button
    void draw(BITMAP *);                           // draw button
    void do_explosion();                           // do explosion of ship
};

#endif

