/*
 *   GRAVITY STRIKE -- menu/mnetgini.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MNETGAMEINI_
#define _MNETGAMEINI_

enum M_NETGAMEINI_CONTROLS {
  M_NETGAMEINI_CAPTION,
  M_NETGAMEINI_BACK,
  M_NETGAMEINI_SERVER,
  M_NETGAMEINI_CLIENT,
  M_NETGAMEINI_SERVERIP
};

class mNetGameInit
{
  private:
    class mControl *button[25];
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    int   moved_in;                                // temp var for not playing a kling sound

    int   check_client_connection;                 // client connection attempt?

    void  init_net();                              // init networking functionality

  public:
    // constr. / destr.
    mNetGameInit();
    ~mNetGameInit();

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

