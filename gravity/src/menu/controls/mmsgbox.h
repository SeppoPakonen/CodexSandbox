/*
 *   GRAVITY STRIKE -- menu/controls/mmsgbox.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MMSGBOX_
#define _MMSGBOX_

#include "mcontrol.h"

#include <string.h>

enum MESSAGEBOX_MODES {
  MSGBOX_OK,
  MSGBOX_YESNO
};

#include "mbar.h"

class mMessageBox : public mControl
{
  private:
    int mode;                                      // box mode
    char text[40];                                 // text to display

    // sub objects
    class mSmallButton  *msg_button[3];

  public:
    // destr.
    ~mMessageBox();

    // get / set (overloaded)
    void  set_text(char *n)                        { strncpy(text, n, 40); }
    void  set_int(int n);
    void  set_dest_x(int n);
    void  set_dest_y(int n);
    class mSmallButton *get_sbutton(int n)         { return msg_button[n]; }

    // functions
    int  update();                                 // overload move function
    void check();                                  // overload check function
    void create(int x, int y, int dx, int dy, int w, int h);// create message box
    void draw(BITMAP *);                           // draw box
};

#endif

