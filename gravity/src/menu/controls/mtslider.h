/*
 *   GRAVITY STRIKE -- menu/controls/mtslider.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MTEXTSLIDER_
#define _MTEXTSLIDER_

#include "mcontrol.h"

#include <string.h>
#include "mbar.h"

class mTextSlider : public mControl
{
  private:
    int mode;                                      // 1 = with arrows, 0 = without
    char text[40];                                 // text to display
    int field_pos;                                 // beginning of bar
    int align;                                     // do align?

    // sub objects
    class mArrowButton *arrow1;                    // left / up arrow
    class mArrowButton *arrow2;                    // right / down arrow
    class mBar         *bar;                       // status bar

  public:
    // destr.
    ~mTextSlider();

    // get / set (overloaded)
    void set_text(char *n);
    void set_int(int n)                            { mode = n; }
    void set_align(int n)                          { align = n; }
    int  get_align()                               { return align; }
    void  set_dest_x(int n);
    void  set_dest_y(int n);
    class mArrowButton *get_arrow1()               { return arrow1; }
    class mArrowButton *get_arrow2()               { return arrow2; }
    class mBar         *get_bar()                  { return bar; }

    // functions
    int  update();                                 // overload move function
    void check();                                  // overload check function
    void create(int x, int y, int dx, int dy, int w, int h, int mode);// create arrow option button
    void draw(BITMAP *);                           // draw button
};

#endif

