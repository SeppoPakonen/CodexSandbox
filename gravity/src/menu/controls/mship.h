/*
 *   GRAVITY STRIKE -- menu/controls/mship.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MSHIPICON_
#define _MSHIPICON_

#include "mcontrol.h"

class mShipIcon : public mControl
{
  private:
    class   gsPlayer  *pl;                         // player data
    BITMAP  *shipimage1;                           // bitmap of ship
    BITMAP  *shipimage2;                           // bitmap of ship (with thrusters on)
    int     stat_defense;                          // defensive capabilities of ship
    int     stat_attack;
    int     stat_agility;
    char    text[30];                              // name of ship
    char    filename[30];
    int     rot;                                   // rotation of ship
    int     dead;                                  // ship exploded?
    int     newship;                               // newship mode (no pic etc.)
    int     error;                                 // error in ship loading?

  public:
    // destr.
    ~mShipIcon();

    // get / set (overloaded)
    void  set_text(char *n);
    char  *get_text()                              { return filename; }
    int   get_int()                                { return error; }

    // functions
    int  update();                                 // overload move function
    void create(int x, int y, int dx, int dy, int w, int h);// create ship button
    void draw(BITMAP *);                           // draw button
    void do_explosion();                           // do explosion of ship
};

#endif

