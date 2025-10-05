/*
 *   GRAVITY STRIKE -- menu/mminig.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MMINIGAME_
#define _MMINIGAME_

enum M_MINIGAME_CONTROLS {
  M_MINIGAME_CAPTION,
  M_MINIGAME_BACK,
  M_MINIGAME_PLAY,
  M_MINIGAME_LEVELINFO,
  M_MINIGAME_P1NAME,
  M_MINIGAME_P1SHIP,
  M_MINIGAME_P1CHOOSE,
  M_MINIGAME_P1EDIT,
  M_MINIGAME_P2NAME,
  M_MINIGAME_P2SHIP,
  M_MINIGAME_P2CHOOSE,
  M_MINIGAME_P2EDIT,
  M_MINIGAME_LEVSELECT,
  M_MINIGAME_AUTHOR,
  M_MINIGAME_LEVDESC,
  M_MINIGAME_LEVELEXTRA,
  M_MINIGAME_LEVMINIMAP,
  M_MINIGAME_OPT_EXTRAS,
  M_MINIGAME_OPT_EXTRAS_TXT,
  M_MINIGAME_PLAYER2,
  M_MINIGAME_LEVELINFOBOX
};

class mMiniGame
{
  private:
    class mControl *button[28];
    class gsPlayer *pl[2];                         // players to handle
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    BITMAP *minimap;                               // minimap to display
    int   moved_in;                                // temp var for not playing a kling sound
    int   infobox;                                 // temp var for infobox
    int   extrabar_max;                            // temp var for max value of extrabar
    int   extrabar_steps;                          // temp var for steps of extra bar
    char  resolution_text[100];                    // text for displaying resolution

    void  show_hide_player2(int);                  // hide/show player 2 controls
    
  public:
    // constr. / destr.
    mMiniGame();
    ~mMiniGame();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

