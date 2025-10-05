/*
 *   GRAVITY STRIKE -- menu/mmain.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MMAIN_
#define _MMAIN_

enum M_MAIN_CONTROLS {
  M_MAIN_1PLAYER,
  M_MAIN_2PLAYER,
  M_MAIN_NETGAME,
  M_MAIN_MINIGAME,
  M_MAIN_SHIPBUILDER,
  M_MAIN_OPTIONS,
  M_MAIN_CREDITS,
  M_MAIN_EXIT
};

class mMain
{
  private:
    class mControl *button[10];
    int   active_button;
    int   esc_pressed;
    int   switch_to_menu;                          // which menu to display?
    int   moving;                                  // menu is moving in/out
    int   button_count;                            // how much buttons?
    int   no_input;                                // no input flag (while scrolling out)

  public:
    // constr. / destr.
    mMain();
    ~mMain();

    // functions
    void init(int = 0);                            // initialize menu (0 = no inmoving)
    int  check();                                  // check menu buttons (1 on exit)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move menu out
    void draw(BITMAP *);                           // draw menu
};

#endif

