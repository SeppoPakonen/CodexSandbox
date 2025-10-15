/*
 *   GRAVITY STRIKE -- menu/moptions.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MOPTIONS_
#define _MOPTIONS_

enum M_OPTIONS_CONTROLS {
  M_OPTIONS_LANGUAGE,
  M_OPTIONS_PIXELS,
  M_OPTIONS_RESOLUTION,
  M_OPTIONS_VOLUME,
  M_OPTIONS_MVOLUME,
  M_OPTIONS_CONTROLS,
  M_OPTIONS_2PCONTROLS,
  M_OPTIONS_SPCONTROLS,
  M_OPTIONS_BACK,
  M_OPTIONS_CAPTION
};

class mOptions
{
  private:
    class mControl *button[12];
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    int   lang_changed;                            // language changed?

  public:
    // constr. / destr.
    mOptions();
    ~mOptions();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

