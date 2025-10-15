/*
 *   GRAVITY STRIKE -- menu/mctrlsp.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MENUCONTROLSSP_
#define _MENUCONTROLSSP_

enum M_CONTROLS_CONTROLS_SP {
  M_CONTROLSP_PAUSE,
  M_CONTROLSP_SCREENSHOT,
  M_CONTROLSP_PANEL,
  M_CONTROLSP_DEBUG,
  M_CONTROLSP_CHAT,
  M_CONTROLSP_BACK,
  M_CONTROLSP_CAPTION  
};

class mPControlsSP
{
  private:
    class mControl *button[11];
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   wait_for_key;                            // waiting for keypress?

    // get pressed key
    int   get_pressed_key();

  public:
    // constr. / destr.
    mPControlsSP();
    ~mPControlsSP();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

