/*
 *   GRAVITY STRIKE -- menu/mchship.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MCHOOSESHIP_
#define _MCHOOSESHIP_

#define MAX_SHIP_FILES 400

enum M_CHOOSESHIP_CONTROLS {
//  M_CHOOSESHIP_CAPTION,
//  M_CHOOSESHIP_BACK
};

typedef struct shipfile {
  char name[30];
} shipfile;

class mChooseShip
{
  private:
    int   active_button;
    int   esc_pressed;
    int   moving;                                  // menu is moving in/out
    int   switch_to_menu;                          // which menu to display?
    int   button_count;                            // how much buttons?
    int   no_input;
    int   b_back;                                  // back button
    int   b_nextpage;                              // next page button
    int   b_lastpage;                              // last page button
    int   b_caption;                               // caption
    int   b_new;                                   // new ship button
    int   start_with;                              // start with ship num...
    int   ship_count;                              // how much ships?
    int   cnt_def_ships;
    int   show_newship;

    char  chosen_ship_filename[30];                // filename of chosen ship
    int   read_ships_and_sort_them(char *);        // reads ship filenames in array

    void  kill_all();
    void  init_all();

  public:
    class mControl *button[45];

    // constr. / destr.
    mChooseShip();
    ~mChooseShip();

    shipfile ship_files[MAX_SHIP_FILES];                      // array with ship files
    char *get_chosen_ship()                        { return chosen_ship_filename; }

    // functions
    void init(int=0);                              // initialize menu
    int  check();                                  // check menu buttons (1 on exit/back)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move out menu
    void draw(BITMAP *);                           // draw menu
};

#endif

