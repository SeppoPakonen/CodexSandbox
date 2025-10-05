/*
 *   GRAVITY STRIKE -- menu/mnetgame.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MNETGAME_
#define _MNETGAME_

enum M_NETGAME_CONTROLS {
  M_NETGAME_CAPTION,
  M_NETGAME_BACK,
  M_NETGAME_MYSHIP,
  M_NETGAME_MYCHOOSE,
  M_NETGAME_MYEDIT,
  M_NETGAME_MYNAME,
  M_NETGAME_P2SHIP,
  M_NETGAME_P2CAP,
  M_NETGAME_P2NAME,
  M_NETGAME_P2READY,
  M_NETGAME_P3SHIP,
  M_NETGAME_P3CAP,
  M_NETGAME_P3NAME,
  M_NETGAME_P3READY,
  M_NETGAME_P4SHIP,
  M_NETGAME_P4CAP,
  M_NETGAME_P4NAME,
  M_NETGAME_P4READY,
  M_NETGAME_P5SHIP,
  M_NETGAME_P5CAP,
  M_NETGAME_P5NAME,
  M_NETGAME_P5READY,
  M_NETGAME_P6SHIP,
  M_NETGAME_P6CAP,
  M_NETGAME_P6NAME,
  M_NETGAME_P6READY,

  M_NETGAME_LEVCAP,
  M_NETGAME_LEVSELECT,
  M_NETGAME_LEVELNAME,
  M_NETGAME_LEVELEXTRA,
  M_NETGAME_CTF,
  M_NETGAME_HH,

  M_NETGAME_OPT_GRAVITY,
  M_NETGAME_OPT_GRAVITY_TXT,
  M_NETGAME_OPT_EXTRAS,
  M_NETGAME_OPT_EXTRAS_TXT,
  M_NETGAME_OPT_MAXFRAGS,
  M_NETGAME_OPT_MAXFRAGS_TXT,
  M_NETGAME_OPT_MAXTIME,
  M_NETGAME_OPT_MAXTIME_TXT,

  M_NETGAME_CHAT_INPUT,

  M_NETGAME_READY,
  M_NETGAME_PLAY
};

class mNetGame
{
  private:
    class mControl *button[57];
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    BITMAP *minimap;                               // minimap to display
    int   moved_in;                                // temp var for not playing a kling sound
    int   last_level;

  public:
    // constr. / destr.
    mNetGame(int=0);
    ~mNetGame();

    // functions
    void init(int=0, int=0);                       // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

