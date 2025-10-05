/*
 *   GRAVITY STRIKE -- menu/moneplay.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MONEPLAYER_
#define _MONEPLAYER_

enum M_ONEPLAYER_CONTROLS {
  M_ONEPLAYER_CAPTION,
  M_ONEPLAYER_BACK,
  M_ONEPLAYER_PLAY,
  M_ONEPLAYER_P1NAME,
  M_ONEPLAYER_P1SHIP,
  M_ONEPLAYER_P1CHOOSE,
  M_ONEPLAYER_P1EDIT,
  M_ONEPLAYER_DISP_MISSIONS,
  M_ONEPLAYER_DISP_RACES,
  M_ONEPLAYER_LEVSELECT,
  M_ONEPLAYER_AUTHOR,
  M_ONEPLAYER_LEVDESC,
  M_ONEPLAYER_LEVELEXTRA,
  M_ONEPLAYER_LEVMINIMAP
};

class mOnePlayer
{
  private:
    class mControl *button[22];
    class gsPlayer *pl;                            // player to handle
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    BITMAP *minimap;                               // minimap to display
    int   moved_in;                                // temp var for not playing a kling sound
    char  leveldesc[500];                          // level description (variable due to depends_on)
    int   leveldesc_col;                           // color of description
    char  resolution_text[100];                    // text for displaying resolution

  public:
    // constr. / destr.
    mOnePlayer();
    ~mOnePlayer();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
    void check_level_depends();
};

#endif

