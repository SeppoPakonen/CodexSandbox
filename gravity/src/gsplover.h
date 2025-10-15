/*
 *   GRAVITY STRIKE -- gsplover.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSPLAYEROVERLAY_
#define _GSPLAYEROVERLAY_

class gsPlayerOverlay
{
  private:
    int     x;                                     // x position
    int     y;                                     // y position
    int     w;                                     // width of overlay pic
    int     h;                                     // height of overlay pic
    char    pl_name[20];                           // name of player
    BITMAP  *back;                                 // overlay image
    class gsPlayer *opl;                           // player associated with overlay
    class gsMap    *omap;                          // map to draw overlay on

  public:
    gsPlayerOverlay(class gsPlayer *);
    ~gsPlayerOverlay();

    int     get_x()                                { return x; }
    int     get_y()                                { return y; }
    int     get_width()                            { return w; }
    int     get_height()                           { return h; }

    void    set_x(int n)                           { x = n; }
    void    set_y(int n)                           { y = n; }

    // functions
    void    set_map(class gsMap *);
    void    draw();
    void    save();
    void    restore();
    void    update_player_name();
};

#endif
