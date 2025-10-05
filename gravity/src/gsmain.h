/*
 *   GRAVITY STRIKE -- gsmain.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <stdio.h>
//#include <string.h>
#include "allegro.h"

#ifndef _GSMAIN_
#define _GSMAIN_

class gsMain
{
  private:
    // variables
    class gsSingleLevel *splevel;                  // single player level to play
    class gsLevel *level;                          // level to create & play
    class gsNetLevel *nwlevel;                     // net level to create & play
    class gsMenu  *menu;                           // menu system

    // functions
    void exit();

  public:
    // constr. / destrc.
    gsMain();
    ~gsMain();

    // functions
    int  init();                                   // global init stuff
    void play();
    void do_sp_level();
    void do_level();
    void do_network_level();
    void create_map_image(int levnum);
    void create_map_images();                      // creates minimaps for menu
    void read_level_files();                       // puts level info into level_files
    void sort_level_files();                       // sorts files in array
};

#endif
