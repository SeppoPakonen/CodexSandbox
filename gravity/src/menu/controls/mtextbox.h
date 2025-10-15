/*
 *   GRAVITY STRIKE -- menu/controls/mtextbox.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MTEXTBOX_
#define _MTEXTBOX_

#include "mcontrol.h"

#include <string.h>

#include "mbar.h"

class mTextBox : public mControl
{
  private:
    int max_lines;                                 // max lines we're able to display at once
    int disp_from;                                 // display from/to array pos...
    char filename[255];                            // text file to load
    char *text[1000];                              // loaded text file
    int textlines;
    int lang_begin, lang_end;                      // language parts

    // sub objects
    class mArrowButton *arrow1;                    // up arrow
    class mArrowButton *arrow2;                    // down arrow
    class mBar         *bar;                       // status bar

    void  draw_bar_lines(BITMAP *v);               // draws the bar lines

  public:
    // destr.
    ~mTextBox();

    // get / set (overloaded)
    void  set_text(char *n);

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
};

#endif

