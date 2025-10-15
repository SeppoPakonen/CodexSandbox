/*
 *   GRAVITY STRIKE -- gsobject.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSOBJECT_
#define _GSOBJECT_

#include "gsframes.h"
#include "gswaypnt.h"

enum OBJECT_TYPE {
  OBJ_USER,
  OBJ_PILE,
  OBJ_TUNNEL1,
  OBJ_BUILDING1,
  OBJ_BUILDING2,
  OBJ_BUILDING3,
  OBJ_BUILDING4,
  OBJ_BUILDING4_2,
  OBJ_BUILDING_GRAV,
  OBJ_POOL1,
  OBJ_POOL2,
  OBJ_PPILE1,
  OBJ_PPILE2,
  OBJ_MAN1,
  OBJ_DOORH,
  OBJ_DOORV,
  OBJ_DOORH2,
  OBJ_DOORV2,
  OBJ_SWITCHR,
  OBJ_SWITCHL,
  OBJ_SWITCHU,
  OBJ_SWITCHD,
  OBJ_RACEPOLEL,
  OBJ_RACEPOLER,
  OBJ_RACEPOLEU,
  OBJ_RACEPOLED,
  OBJ_TREE,
  OBJ_TREE2,
  OBJ_STONE_B1,
  OBJ_STONE_S1,
  OBJ_STONE_S2,
  OBJ_BLOCK_1,
  OBJ_BLOCK_2
};

enum SPOBJECT_TYPE {
  // specials
  SPOBJ_USER = 500,
  SPOBJ_BUMPL,
  SPOBJ_BUMPR,
  SPOBJ_BUMPU,
  SPOBJ_BUMPD,
  SPOBJ_EXTRA,
  SPOBJ_TELEPORT1,
  SPOBJ_TELEPORT2,
  SPOBJ_GRAVTRAP,
  SPOBJ_FLAG,
  SPOBJ_BACKTILE1,
  SPOBJ_GWALL_L,
  SPOBJ_GWALL_R,
  SPOBJ_GWALL_U,
  SPOBJ_GWALL_D
};

class Object
{
  private:
    Object *prev, *next;
    Image *frames;                                 // frame handler
    int kill_frames;                               // kill frame handler when done? (not for user images)
    BITMAP *backbuf;                               // save bground of specific objects
    int   type;                                    // type of object
    int   special;                                 // is this a special ob?
    int   image;                                   // (first) image of object (position in datafile)
    int   nr;                                      // number of object
    float x;                                       // x position of object (top left)
    float y;                                       // y position of object (top left)
    int   w;                                       // width of image
    int   h;                                       // height of image
    float xspd;                                    // speed in x dir
    float yspd;                                    // speed in y dir
    float dxspd;                                   // destination speed
    float dyspd;                                   // "
    int   bound_x;                                 // bounding box x start (for screen clear)
    int   bound_y;                                 // bounding box y
    int   bound_w;                                 // width of bounding box
    int   bound_h;                                 // height of bounding box
    int   onehit;                                  // object hit?
    int   hitby;                                   // hit by player num
    float hitby_sx;                                // last bullet's x speed
    float hitby_sy;                                // last bullet's y speed
    float hit;                                     // hitpoints
    int   maxhit;                                  // maximum hitpoints
    int   dispframe;                               // frame to display (normally equal to current frame)
    int   curframe;                                // current frame
    int   curframet;                               // time in current frame
    int   maxframe;                                // maximum frame
    int   maxframet;                               // maximum frametime
    int   startframe;                              // frame to start with
    int   physical_frames;                         // how many "real" frames?
    int   user;                                    // user field (strength etc.)
    int   user2;                                   // 2nd user field (additional internal settings)
    int   active;                                  // object activated? (special ob)
    int   teleport_time;                           // time since last teleporting
    int   invisible;                               // is object invisible?
    class gsMap *restrict_to_map;                  // is object restricted to map x?
    int   dont_continue;                           // don't continue after hook function?
    int   do_hooks;                                // do hook functions for this object?
    int   kill;                                    // kill object when it's safe to do?
    int   dont_update;                             // draw object only once
    int   dead_by_network;                         // object's dead was requested over network
    int   user_int[20];                            // user vars (int)
    float user_float[20];                          // user vars (floats)

    void  apply_gravity();                         // apply gravity to players & bullets
    void  apply_lin_gravity();                     // apply linear gravity to players & bullets
    void  apply_gravity_to(int x, int y, float *xs, float *ys, int type); // applies gravity to x/y coord generating xs/ys speeds
    void  apply_lin_gravity_to(int px, int py, float *pxs, float *pys, float factor); // applies linear gravity

  public:
    // constr./destr.
    Object(int c, OBJECT_TYPE t, int x, int y, int puser=0, int pmf=0);
    Object(int c, SPOBJECT_TYPE t, int x, int y, int puser=0, int pmf=0);
    Object(int c, OBJECT_TYPE t, Image *fr, int x, int y, int puser=0, int pmf=0);
    Object(int c, SPOBJECT_TYPE t, Image *fr, int x, int y, int puser=0, int pmf=0);
    ~Object();

    gsWaypoint  *wp;                               // waypoint handler
    
    // get/put
    Object      *get_prev()                        { return prev; }
    Object      *get_next()                        { return next; }
    void        set_prev(Object *n)                { prev = n; }
    void        set_next(Object *n)                { next = n; }
    void        set_type(int n)                    { type = n; }
    int         get_type()                         { return type; }
    int         get_nr()                           { return nr; }
    int         is_special()                       { return special; }
    void        set_maxframe(int n)                { maxframe = n; }
    int         get_maxframe()                     { return maxframe; }
    void        set_maxframet(int n)               { maxframet = n; }
    int         get_maxframet()                    { return maxframet; }
    void        set_dispframe(int n)               { dispframe = n; }
    int         get_dispframe()                    { return dispframe; }
    void        set_curframe(int n)                { curframe = n; }
    int         get_curframe()                     { return curframe; }
    void        set_curframet(int n)               { curframet = n; }
    int         get_curframet()                    { return curframet; }
    void        set_startframe(int n)              { startframe = n; }
    int         get_startframe()                   { return startframe; }
    float       get_x()                            { return x; }
    float       get_y()                            { return y; }
    void        set_x(int n)                       { x = n; }
    void        set_y(int n)                       { y = n; }
    int         get_width()                        { return w; }
    int         get_height()                       { return h; }
    void        set_bound_x(int n)                 { bound_x = n; }
    void        set_bound_y(int n)                 { bound_y = n; }
    void        set_bound_w(int n)                 { bound_w = n; }
    void        set_bound_h(int n)                 { bound_h = n; }
    float       get_xspd()                         { return xspd; }
    float       get_yspd()                         { return yspd; }
    void        set_xspd(float n)                  { xspd = n; }
    void        set_yspd(float n)                  { yspd = n; }
    float       get_hit()                          { return hit; }
    int         get_maxhit()                       { return maxhit; }
    void        set_hit(float n)                   { hit = n; }
    void        set_maxhit(int n)                  { maxhit = n; }
    int         get_onehit()                       { return onehit; }
    int         get_hitby()                        { return hitby; }
    float       get_hitby_sx()                     { return hitby_sx; }
    float       get_hitby_sy()                     { return hitby_sy; }
    int         get_user()                         { return user; }
    void        set_user(int n)                    { user = n; }
    int         get_user2()                        { return user2; }
    void        set_user2(int n)                   { user2 = n; }
    int         get_active()                       { return active; }
    void        set_active(int n)                  { active = n; }
    int         get_teleport_time()                { return teleport_time; }
    void        set_teleport_time(int n)           { teleport_time = n; }
    int         add_hitpoints(int from, float n, int weap=0, int fx=0, int fy=0, float fxspd=0, float fyspd=0);
    void        set_invisible(int n)               { invisible = n; }
    int         get_invisible()                    { return invisible; }
    int         get_dont_continue()                { return dont_continue; }
    void        set_dont_continue(int n)           { dont_continue = n; }
    int         get_do_hooks()                     { return do_hooks; }
    void        set_do_hooks(int n)                { do_hooks = n; }
    void        set_dont_update(int n)             { dont_update = n; }
    int         get_dont_update()                  { return dont_update; }
    class gsMap* get_restrict_to_map()             { return restrict_to_map; }
    void        set_restrict_to_map(class gsMap *n){ restrict_to_map = n; }
    RLE_SPRITE* get_current_sprite();
    void        remove()                           { kill = 1; }
    int         get_remove()                       { return kill; }
    void        set_dead_by_network()              { dead_by_network = 1; }
    void        set_user_int(int n, int d)         { if (n >=0 && n <= 19) user_int[n] = d; }
    void        set_user_float(int n, float d)     { if (n >=0 && n <= 19) user_float[n] = d; }
    int         get_user_int(int n)                { if (n >=0 && n <= 19) return user_int[n]; else return 0; }
    int         get_user_float(int n)              { if (n >=0 && n <= 19) return user_float[n]; else return 0; }

    // functions
    void draw(class gsMap *);
    void script_redraw();
    void save_background(class gsMap *);
    void restore_background(class gsMap *);
    int update();
    void move();
    void check_special_collisions();
    void check_collisions(int special);
};

class gsObject
{
  private:
    Object *first_object, *last_object;
    Object *first_spobject, *last_spobject;
    int count;

  public:
    // constr. / destr.
    gsObject();
    ~gsObject();

    // get / set
    Object *get_first_object()                     { return first_object; }
    Object *get_last_object()                      { return last_object; }
    Object *get_first_spobject()                   { return first_spobject; }
    Object *get_last_spobject()                    { return last_spobject; }
    int  get_count()                               { return count; }

    // functions
    Object *add(int num, /*OBJECT_TYPE*/int t, int x, int y, int puser=0, int pf=0);
    Object *add_special(int num, /*SPOBJECT_TYPE*/int t, int x, int y, int puser=0, int pf=0);
    void remove(Object *);
    void remove_special(Object *);
    void remove_for_script(Object *);
    void draw(class gsMap *);
    void draw_special(class gsMap *);
    void restore_background(class gsMap *);
    void save_background(class gsMap *);
    void update();
    Object *get_object(int);                       // return object with nr ...
    Object *get_spobject(int);                     // return special object with nr ...
};

extern gsObject *objects;

#endif
