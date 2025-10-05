/*
 *   GRAVITY STRIKE -- gslevel.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSLEVEL_
#define _GSLEVEL_

class gsLevel
{
  private:
    // wrong var
    char error_str[100];
    int  fading_in;                                // fade in in progress?
    int  exit_level;                               // exit level on end...
    int  replay_mode;
    
    // functions
    void init();                                   // initializes players & other things
    void flip();                                   // flips vscreen to screen
    void do_logic();                               // does game logic
    void draw_stuff();                             // draws graphics
    void clear_stuff();                            // clears graphics

  public:
    // constr. / destr.
    gsLevel();                                     // create the level
    ~gsLevel();                                    // ...and destroy it

    // get/set
    char *get_error_str()                          { return error_str; }

    // functions
    int  init_stuff();                             // init level
    void play();                                   // main function
};

#endif
