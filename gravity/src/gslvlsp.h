/*
 *   GRAVITY STRIKE -- gslvlsp.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GS_SINGLE_LEVEL_
#define _GS_SINGLE_LEVEL_

class gsSingleLevel
{
  private:
    // wrong var
    char error_str[100];
    int  fading_in;                                // fade in in progress?
    int  exit_level;                               // exit level on end...
    int  replay_mode;
    int  mission_done_counter;                     // pause when mission ends
    int  old_max_time;
    int  time_to_player_start;                     // seconds until player starts (for needed_time calculation)

    // functions
    void init();                                   // initializes players & other things
    void flip();                                   // flips vscreen to screen
    void do_logic();                               // does game logic
    void draw_stuff();                             // draws graphics
    void clear_stuff();                            // clears graphics

  public:
    // constr. / destr.
    gsSingleLevel();                               // create the level
    ~gsSingleLevel();                              // ...and destroy it

    // get/set
    char *get_error_str()                          { return error_str; }

    // functions
    int  init_stuff();                             // init level
    void play();                                   // main function
};

#endif
