/*
 *   GRAVITY STRIKE -- menu/controls/mlevsel.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MLEVELSELECT_
#define _MLEVELSELECT_

#include "mcontrol.h"

#include <string.h>

#include "mbar.h"

class mLevelSelect : public mControl
{
  private:
    int active_entry;                              // selected entry
    int old_active_entry;                          // old selected entry
    int max_lines;                                 // max lines we're able to display at once
    int disp_from;                                 // display from array pos...

    // sub objects
    class mArrowButton *arrow1;                    // up arrow
    class mArrowButton *arrow2;                    // down arrow
    class mBar         *bar;                       // status bar

    void  scroll_visible();                        // scroll list so that active entry is visible
    void  draw_bar_lines(BITMAP *v);               // draws the bar lines
    void  init();                                  // initialise some vars

  public:
    // destr.
    ~mLevelSelect();

    // get / set (overloaded)
    void  set_int(int n);
    int   get_int()                                { if (old_active_entry != active_entry) { old_active_entry = active_entry; return active_entry; } else return -1; }
    void  set_dest_x(int n);
    void  set_dest_y(int n);
    class mArrowButton *get_arrow1()               { return arrow1; }
    class mArrowButton *get_arrow2()               { return arrow2; }
    class mBar         *get_bar()                  { return bar; }

    // functions
    int  update();                                 // overload move function
    void check();                                  // overload check function
    void create(int x, int y, int dx, int dy, int w, int h);// create arrow option button
    void draw(BITMAP *);                           // draw button
    void reinit()                                  { init(); }
};

#endif

