/*
 *   GRAVITY STRIKE -- menu/mctrl2.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MENUCONTROLS2P_
#define _MENUCONTROLS2P_

enum M_CONTROLS_2PCONTROLS {
  M_CONTROL2P_LEFT_1,
  M_CONTROL2P_RIGHT_1,
  M_CONTROL2P_THRUST_1,
  M_CONTROL2P_SHOOT_1,
  M_CONTROL2P_PREVWEAP_1,
  M_CONTROL2P_NEXTWEAP_1,
  M_CONTROL2P_AFTERBURNER_1,
  M_CONTROL2P_EXTRA_1,

  M_CONTROL2P_LEFT_2,
  M_CONTROL2P_RIGHT_2,
  M_CONTROL2P_THRUST_2,
  M_CONTROL2P_SHOOT_2,
  M_CONTROL2P_PREVWEAP_2,
  M_CONTROL2P_NEXTWEAP_2,
  M_CONTROL2P_AFTERBURNER_2,
  M_CONTROL2P_EXTRA_2,

  M_CONTROL2P_BACK,
  M_CONTROL2P_CAPTION,
  M_CONTROL2P_PLAYER1,
  M_CONTROL2P_PLAYER2
};

class mPControls2P
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
    mPControls2P();
    ~mPControls2P();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

