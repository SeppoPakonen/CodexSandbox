/*
 *   GRAVITY STRIKE -- menu/mctrl.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MENUCONTROLS_
#define _MENUCONTROLS_

enum M_CONTROLS_CONTROLS {
  M_CONTROL_LEFT,
  M_CONTROL_RIGHT,
  M_CONTROL_THRUST,
  M_CONTROL_SHOOT,
  M_CONTROL_PREVWEAP,
  M_CONTROL_NEXTWEAP,
  M_CONTROL_AFTERBURNER,
  M_CONTROL_W1,
  M_CONTROL_W2,
  M_CONTROL_W3,
  M_CONTROL_W4,
  M_CONTROL_W5,
  M_CONTROL_W6,
  M_CONTROL_EXTRA,
  M_CONTROL_BACK,
  M_CONTROL_CAPTION
};

class mPControls
{
  private:
    class mControl *button[20];
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
    mPControls();
    ~mPControls();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

