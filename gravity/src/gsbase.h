/*
 *   GRAVITY STRIKE -- gsbase.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSBASE_
#define _GSBASE_

#include <string.h>

enum BASE_TYPE {
  PLAYER_BASE,
  CARGO_BASE,
  ENEMY_S_BASE,
  ENEMY_L_BASE,
  RELOAD_BASE,
  ENEMY_BASE_LEFT,
  ENEMY_BASE_MIDDLE,
  ENEMY_BASE_RIGHT,
  CUSTOM_BASE
};

enum CARGO_TYPE {
  CARGO_NONE,
  CARGO_NORMAL,
  CARGO_OBJECT
};

class Base
{
  private:
    Base   *prev, *next;
    RLE_SPRITE *image;                             // bitmap of base
    BASE_TYPE type;                                // type of base
    CARGO_TYPE cargo_type;                         // type of cargo
    int   cargo_weight;                            // weight of cargo
    char  cargo_name[40];                          // name of cargo :)
    class Object *cargo_object;                    // the object of the cargo...
    int   nr;                                      // number of base
    float x;                                       // x position of base (top left)
    float y;                                       // y position of base (top left)
    float xspd;
    float yspd;
    int   w;                                       // width of image
    int   h;                                       // height of image
    float land_x;                                  // x position of landing pad
    float land_y;                                  // y position of landing pad
    int   land_w;                                  // width of landing pad
    long player_on_base;                  // player landed?
    int   curframe;                                // animation stuff
    int   curframet;
    int   maxframe;
    int   maxframet;
    int   refuel_percent;                          // refueling of ships?
    int   refuel_player;                           // player permitted a refuel

  public:
    // constr./destr.
    Base(int num, BASE_TYPE t, int x, int y, int user=0, int car=0, int caram=0);
    ~Base();
  
    // get/put
    float get_x()                                  { return x; }
    float get_y()                                  { return y; }
    float get_xspd()                               { return xspd; }
    float get_yspd()                               { return yspd; }
    void set_x(float n)                            { x = n; }
    void set_y(float n)                            { y = n; }
    void set_xspd(float n)                         { xspd = n; }
    void set_yspd(float n)                         { yspd = n; }
    int get_width()                                { return w; }
    int get_height()                               { return h; }
    Base *get_prev()                               { return prev; }
    Base *get_next()                               { return next; }
    void set_prev(Base *n)                         { prev = n; }
    void set_next(Base *n)                         { next = n; }
    float get_land_x()                             { return land_x; }
    float get_land_y()                             { return land_y; }
    int get_land_w()                               { return land_w; }
    void set_land_w(int w)                         { land_w = w; }
    void set_land_x(float w)                       { land_x = w; }
    void set_land_y(float w)                       { land_y = w; }
    void set_type(BASE_TYPE n)                     { type = n; }
    BASE_TYPE get_type()                           { return type; }
    void set_cargo_type(int n)                     { cargo_type = (CARGO_TYPE)n; }
    int  get_cargo_type()                          { return cargo_type; }
    void set_cargo_object(class Object *);
    void set_cargo_weight(int n)                   { cargo_weight = n; }
    int  get_cargo_weight()                        { return cargo_weight; }
    class Object *get_cargo_object()               { return cargo_object; }
    void set_cargo_object(int n);
    void set_cargo_name(char *n)                   { strncpy(cargo_name, n, 40); }
    char *get_cargo_name()                         { return cargo_name; }
    int  get_nr()                                  { return nr; }
    long get_player_on_base()                      { return player_on_base; }
    void set_player_on_base(long n)                { player_on_base = n; }
    void set_refuel_percent(int n, int pl=0)       { refuel_percent = n; refuel_player = pl; }
    int  get_refuel_percent()                      { return refuel_percent; }
    int  get_refuel_player()                       { return refuel_player; }
  
    // functions
    void draw(class gsMap *);
    void draw_cargo(class gsMap*);
    void kill_landscape(class gsMap *);
    void update();
    void hit(int);
};

class gsBase
{
  private:
    Base *first_base, *last_base;
    int count;

  public:
    // constr. / destr.
    gsBase();
    ~gsBase();

    // get / set
    Base *get_first_base()                         { return first_base; }
    Base *get_last_base()                          { return last_base; }
    int  get_count()                               { return count; }

    // functions
    Base *add(int num, /*BASE_TYPE*/int t, int x, int y, int user=0, int c=0);  // add a base
    void update();                                 // update base animation (?)
    void draw(class gsMap *);
    void draw_cargo(class gsMap *);
    void kill_landscape(class gsMap *);            // kill landscape over landing pad
    void check_if_landed(class gsPlayer *);        // check if player has landed
    Base *get_base(int);                           // return base with nr ...
};

extern gsBase *bases;

#endif
