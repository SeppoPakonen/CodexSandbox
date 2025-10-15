/*
 *   GRAVITY STRIKE -- menu/controls/mtext.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MSTATICTEXT_
#define _MSTATICTEXT_

#include "mcontrol.h"
#include <string.h>

class mNormalText : public mControl
{
  private:
    char text[81];                                 // text do display
    int  font_num;                                 // font number in datafile
    int  color;                                    // text color

  public:
    // get/set
    void set_text(char *n)                         { strncpy(text, n, 80); }
    void set_int(int n)                            { if (strlen(text) == 0) font_num = n; else color = n; }

    // functions
    void create(int x, int y, int dx, int dy);     // create text button
    void draw(BITMAP *);                           // draw bar
};

#endif

