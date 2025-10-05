/*
 *   GRAVITY STRIKE -- menu/controls/maoption.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MARROWOPTION_
#define _MARROWOPTION_

#include "mcontrol.h"

#include <string.h>

enum ARROW_OPTION_DIRECTION {
  A_LEFTRIGHT,
  A_UPDOWN
};

#include "mbar.h"

class mArrowOption : public mControl
{
  private:
    int direction;                                 // direction of arrow
    char text[40];                                 // text to display

    // sub objects
    class mArrowButton *arrow1;                    // left / up arrow
    class mArrowButton *arrow2;                    // right / down arrow
    class mBar         *bar;                       // status bar

  public:
    // destr.
    ~mArrowOption();

    // get / set (overloaded)
    void set_text(char *n)                         { strncpy(text, n, 40); }
    void  set_dest_x(int n);
    void  set_dest_y(int n);
    class mArrowButton *get_arrow1()               { return arrow1; }
    class mArrowButton *get_arrow2()               { return arrow2; }
    class mBar         *get_bar()                  { return bar; }

    // functions
    int  update();                                 // overload move function
    void check();                                  // overload check function
    void create(int x, int y, int dx, int dy, int w, int h, int dir);// create arrow option button
    void draw(BITMAP *);                           // draw button
};

#endif

