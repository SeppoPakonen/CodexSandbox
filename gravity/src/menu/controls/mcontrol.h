/*
 *   GRAVITY STRIKE -- menu/controls/mcontrol.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _MCONTROL_
#define _MCONTROL_

enum MCONTROL_STATUS_FLAGS {
  MC_ACTIVE       = 1,
  MC_PUSHED       = 2,
  MC_INVISIBLE    = 4,
  MC_INACTIVE     = 8
};

class mControl
{
  protected:
    float x;                                       // x position
    float y;                                       // y position
    float ox;                                      // original x pos
    float oy;                                      // original y pos
    float dest_x;                                  // destination x
    float dest_y;                                  // destination y
    float idist;                                   // initial distance to target
    float xacc;                                    // x acceleration
    float yacc;                                    // y acceleration
    float xspd;                                    // x speed
    float yspd;                                    // y speed
    float gspd;                                    // global speed
    int   w;                                       // width of control element
    int   h;                                       // height of control el.

    int   flags;                                   // status flag

    int   color1;                                  // color 1 (back)
    int   color2;                                  // color 2 (light_back)
    int   color3;                                  // color 3 (border)

  public:
    // constr. / destr.
    mControl();
    virtual ~mControl();

    // get/set
    void  set_color1(int n)                        { color1 = n; }
    void  set_color2(int n)                        { color2 = n; }
    void  set_color3(int n)                        { color3 = n; }
    int   get_color1()                             { return color1; }
    int   get_color2()                             { return color2; }
    int   get_color3()                             { return color3; }
    void  set_flag(int n)                          { flags |= n; }
    void  del_flag(int n)                          { flags &= 255-n; }
    int   get_flags()                              { return flags; }
    void  set_x(float n)                           { x = dest_x = n; }
    void  set_y(float n)                           { y = dest_y = n; }
    void  set_ox(float n)                          { ox = n; }
    void  set_oy(float n)                          { oy = n; }
    void  set_xacc(float n)                        { xacc = n; }
    void  set_yacc(float n)                        { yacc = n; }
    float get_xspd()                               { return xspd; }
    float get_yspd()                               { return yspd; }
    float get_x()                                  { return x; }
    float get_y()                                  { return y; }
    int   get_w()                                  { return w; }
    int   get_h()                                  { return h; }
    float get_ox()                                 { return ox; }
    float get_oy()                                 { return oy; }
    float get_dest_x()                             { return dest_x; }
    float get_dest_y()                             { return dest_y; }
    virtual void set_dest_x(int n);
    virtual void set_dest_y(int n);

    // functions
    virtual void create(int x, int y, int sx, int sy);// create standard control
    virtual int  update();                         // update control
    virtual void check();                          // check control for input (mouse)
    virtual void draw(BITMAP *) =0;                // draw control

    // virtual functions of derived classes
    virtual void set_text(char *);
    virtual char *get_text();
    virtual void set_player(class gsPlayer *);
    virtual void set_int(int n);
    virtual int  get_int();
    virtual void set_float(float n);
    virtual float get_float();
    virtual void set_align(int n);
    virtual int  get_align();
    virtual void create(int x, int y, int dx, int dy, int dir);
    virtual void create(int x, int y, int sx, int sy, int w, int h);
    virtual void create(int x, int y, int sx, int sy, int w, int h, int dir);
    virtual void do_explosion();
    virtual void reinit();
    virtual class mArrowButton *get_arrow1();
    virtual class mArrowButton *get_arrow2();
    virtual class mBar *get_bar();
    virtual class mSmallButton *get_sbutton(int n);
};

#endif

