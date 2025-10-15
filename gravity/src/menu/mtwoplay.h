/*
 *   GRAVITY STRIKE -- menu/mtwoplay.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MTWOPLAYER_
#define _MTWOPLAYER_

enum M_TWOPLAYER_CONTROLS {
  M_TWOPLAYER_CAPTION,
  M_TWOPLAYER_BACK,
  M_TWOPLAYER_PLAY,
  M_TWOPLAYER_P1NAME,
  M_TWOPLAYER_P1SHIP,
  M_TWOPLAYER_P1CHOOSE,
  M_TWOPLAYER_P1EDIT,
  M_TWOPLAYER_P2NAME,
  M_TWOPLAYER_P2SHIP,
  M_TWOPLAYER_P2CHOOSE,
  M_TWOPLAYER_P2EDIT,
  M_TWOPLAYER_DISP_DOGFIGHTS,
  M_TWOPLAYER_DISP_RACES,
  M_TWOPLAYER_LEVSELECT,
  M_TWOPLAYER_AUTHOR,
  M_TWOPLAYER_LEVDESC,
  M_TWOPLAYER_LEVELEXTRA,
  M_TWOPLAYER_CTF,
  M_TWOPLAYER_HH,
  M_TWOPLAYER_RACE,
  M_TWOPLAYER_LEVMINIMAP,
  M_TWOPLAYER_OPT_GRAVITY,
  M_TWOPLAYER_OPT_GRAVITY_TXT,
  M_TWOPLAYER_OPT_EXTRAS,
  M_TWOPLAYER_OPT_EXTRAS_TXT,
  M_TWOPLAYER_OPT_MAXFRAGS,
  M_TWOPLAYER_OPT_MAXFRAGS_TXT,
  M_TWOPLAYER_OPT_MAXTIME,
  M_TWOPLAYER_OPT_MAXTIME_TXT,
  M_TWOPLAYER_COMPEN,
  M_TWOPLAYER_COMPEN_STRENGTH,
  M_TWOPLAYER_HANDICAP
};

class mTwoPlayer
{
  private:
    class mControl *button[32];
    class gsPlayer *pl[2];                         // players to handle
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    BITMAP *minimap;                               // minimap to display
    int   moved_in;                                // temp var for not playing a kling sound
    char  resolution_text[100];                    // text for displaying resolution

  public:
    // constr. / destr.
    mTwoPlayer();
    ~mTwoPlayer();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

