/*
 *   GRAVITY STRIKE -- menu/mnwstats.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MNETSTATS_
#define _MNETSTATS_

enum M_NETSTATS_CONTROLS {
  M_NETSTATS_CAPTION,
  M_NETSTATS_OKAY
};

class mNetStats
{
  private:
    class mControl *button[80];
    int   active_button;
    int   esc_pressed;
    int   switch_to_menu;                          // which menu to display?
    int   moving;                                  // menu is moving in/out
    int   button_count;                            // how much buttons?
    int   no_input;                                // no input flag (while scrolling out)
    int   bonus_flag;                              // 1, if bonus points should be displayed

    long int calc_score(int p);                    // calculate score of player p

  public:
    // constr. / destr.
    mNetStats();
    ~mNetStats();

    // functions
    void init(int = 0);                            // initialize menu (0 = no inmoving)
    void init_normal(int=0);
    void init_user(int=0);
    int  check();                                  // check menu buttons (1 on exit)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move menu out
    void draw(BITMAP *);                           // draw menu
};

#endif

