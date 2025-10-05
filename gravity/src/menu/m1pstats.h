/*
 *   GRAVITY STRIKE -- menu/m1pstats.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _M1PSTATS_
#define _M1PSTATS_

enum M_1PSTATS_CONTROLS {
  M_1PSTATS_CAPTION,
  M_1PSTATS_OKAY,
  M_1PSTATS_LASTHI
};

class m1PStats
{
  private:
    class mControl *button[50];
    int   active_button;
    int   esc_pressed;
    int   switch_to_menu;                          // which menu to display?
    int   moving;                                  // menu is moving in/out
    int   button_count;                            // how much buttons?
    int   no_input;                                // no input flag (while scrolling out)
    int   bonus_flag;                              // 1, if bonus points should be displayed

    long int calc_score(int p);                    // calculate score of player p
    void init_hiscore_notification(int mode, char *saveret);

  public:
    // constr. / destr.
    m1PStats();
    ~m1PStats();

    // functions
    void init(int = 0);                            // initialize menu (0 = no inmoving)
    void init_normal(int n=0);
    void init_user(int n=0);
    int  check();                                  // check menu buttons (1 on exit)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move menu out
    void draw(BITMAP *);                           // draw menu
};

#endif

