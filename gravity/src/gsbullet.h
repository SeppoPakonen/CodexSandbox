/*
 *   GRAVITY STRIKE -- gsbullet.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSBULLET_
#define _GSBULLET_

#include "luaclass.h"
#include "gsframes.h"

#define WEAPON_COUNT  14
#define REAL_WEAPON_COUNT 17

enum WEAPON_TYPE
{
  W_SINGLE,
  W_DOUBLE,
  W_BOMB,
  W_BIGBOMB,
  W_ROCKET,
  W_MINE,
  W_FREEZER,
  W_SHRINKER,
  W_PROBE,
  W_GAS,
  W_LASER,
  W_BUMPER,
  W_SHOTGUN,
  W_ACID,
  W_USER,
  W_FAKE,
  W_NONE
};

class Bullet
{
  protected:
    Bullet *prev, *next;                           // previous / next bullet in list
    Image  *image;                                 // pointer to weapon frames
    BITMAP *backbuf;                               // for saving background of some bullets

    float  x;                                      // coordinates / speeds
    float  y;
    float  xspd;
    float  yspd;

    int    min_x;                                  // range of bullet
    int    max_x;
    int    min_y;
    int    max_y;

    int    w;                                      // width/height of bullet
    int    h;

    int    use_backbuf;                            // use background buffer instead of clear list
    
    int    curframe;                               // for animations
    int    curframet;
    int    maxframet;

    int    type;                                   // type of bullet
    long by_player;                                // sig of player shooting this bullet
    int    by_enemy;                               // object which shot this bullet
    int    net_dont_kill;                          // bullet created by network "add_bullet"

    int    ttl;                                    // time to live (frames)
    int    orig_ttl;                               // original time to live

    float  strength;                               // strength of bullet (hitpoints) (deprec.)

    float  hitp_hull;                              // hitpoints for hull hits
    float  hitp_shield;                            // hitpoints for shield hits
    
    float  hitp;                                   // current hitpoints
    int    maxhitp;                                // max hitpoints for destruction

    float  user;                                   // user var
    int    target;                                 // -1 if no target set yet
    int    teleport_time;                          // time since last teleporting
    int    upgrade;                                // upgrade status
    int    invisible;                              // projectile invisible?

    int    dont_continue;                          // don't continue after hook function?
    int    do_hooks;                               // do hooks (this or globals->do_all_hooks)
    
    int    kill;                                   // delete this bullet?
    int    spawn;                                  // multi-bullet-weapons: first bullet (1) or one of the next (0)?
    int    just_coll;                              // collision in last frame?

    int    user_int[20];                           // user vars (int)
    float  user_float[20];                         // user vars (floats)
    
  public:
    // constr. / destr.
    Bullet();
    virtual ~Bullet();

    // get / set
    float  get_x()                                 { return x; }
    float  get_y()                                 { return y; }
    float  get_xspd()                              { return xspd; }
    float  get_yspd()                              { return yspd; }
    Bullet *get_prev()                             { return prev; }
    Bullet *get_next()                             { return next; }
    void   set_x(float n)                          { x = n; }
    void   set_y(float n)                          { y = n; }
    void   set_xspd(float n)                       { xspd = n; }
    void   set_yspd(float n)                       { yspd = n; }
    void   set_prev(Bullet *n)                     { prev = n; }
    void   set_next(Bullet *n)                     { next = n; }
    int    get_width()                             { return w; }
    void   set_width(int n)                        { w = n; }
    int    get_height()                            { return h; }
    void   set_height(int n)                       { h = n; }
    int    get_min_x()                             { return min_x; }
    int    get_min_y()                             { return min_y; }
    int    get_max_x()                             { return max_x; }
    int    get_max_y()                             { return max_y; }
    void   set_min_x(int n)                        { min_x = n; }
    void   set_min_y(int n)                        { min_y = n; }
    void   set_max_x(int n)                        { max_x = n; }
    void   set_max_y(int n)                        { max_y = n; }
    long   get_by_player()                         { return by_player; }
    void   set_by_player(long n)                   { by_player = n; }
    int    get_by_enemy()                          { return by_enemy; }
    void   set_by_enemy(int n)                     { by_enemy = n; }
    void   set_net_dont_kill(int n)                { net_dont_kill = n; }
    float  get_strength()                          { return strength; }
    void   set_strength(float n)                   { strength = n; }
    float  get_hull_hitp()                         { return hitp_hull; }
    void   set_hull_hitp(float n)                  { hitp_hull = n; }
    float  get_shield_hitp()                       { return hitp_shield; }
    void   set_shield_hitp(float n)                { hitp_shield = n; }
    void   set_type(int n)                         { type = n; }
    int    get_type()                              { return type; }
    float  get_user()                              { return user; }
    void   set_user(float n)                       { user = n; }
    int    get_target()                            { return target; }
    void   set_target(int n)                       { target = n; }
    int    get_teleport_time()                     { return teleport_time; }
    void   dec_teleport_time()                     { if (teleport_time > 0) teleport_time--; }
    void   set_upgrade(int n)                      { upgrade = n; }
    int    get_upgrade()                           { return upgrade; }
    void   set_ttl(int n)                          { ttl = n; }
    int    get_ttl()                               { return ttl; }
    void   set_invisible(int n)                    { invisible = n; }
    int    get_invisible()                         { return invisible; }
    void   set_hit(float n)                        { hitp = n; }
    void   set_maxhit(int n)                       { maxhitp = n; }
    float  get_hit()                               { return hitp; }
    int    get_maxhit()                            { return maxhitp; }
    int    get_dont_continue()                     { return dont_continue; }
    void   set_dont_continue(int n)                { dont_continue = n; }
    int    get_do_hooks()                          { return do_hooks; }
    void   set_do_hooks(int n)                     { do_hooks = n; }
    void   remove()                                { kill = 1; }
    int    get_remove()                            { return kill; }

    void   set_user_int(int n, int d)              { if (n >=0 && n <= 19) user_int[n] = d; }
    void   set_user_float(int n, float d)          { if (n >=0 && n <= 19) user_float[n] = d; }
    int    get_user_int(int n)                     { if (n >=0 && n <= 19) return user_int[n]; else return 0; }
    int    get_user_float(int n)                   { if (n >=0 && n <= 19) return user_float[n]; else return 0; }


    // abstract functions
    virtual void move() = 0;
    virtual void move(int);
    virtual int  check_collisions() = 0;
    virtual int  check_map_collisions(class gsMap *) = 0;
    virtual void draw(class gsMap *) = 0;
    virtual void draw(BITMAP *);
    virtual void save_background(class gsMap *);
    virtual void restore_background(class gsMap *);
    virtual void hit(float n);
    void reset_min_max();
};


class gsBullet
{
  private:
    Bullet *first_bullet, *last_bullet;
    int    count;

  public:
    Image  *bullet_frames[REAL_WEAPON_COUNT][8];         // define weapon images for 8 players
    Lua    *bulletLua;

    gsBullet();
    ~gsBullet();

    int get_count()                                { return count; }
    Bullet *get_first()                            { return first_bullet; }

    void create_bullet_sprites();
    Bullet *add(/*WEAPON_TYPE*/int t, class gsPlayer *, int upgrade=0, int pspawn=1);
    Bullet *add(/*WEAPON_TYPE*/int t, long psig, int enemy_nr, int px, int py, float pxspd, float pyspd, float pstrength, float puser, int pupgrade=0, int net_dont_kill=0);
    Bullet *add(/*WEAPON_TYPE*/int t, long psig, int enemy_nr, int px, int py, float pxspd, float pyspd, float pstrength, float pshield, float phull, float puser, int pupgrade=0, int net_dont_kill=0, int pspawn=1 /* if multi-bullet-weapon: 1 for first shot, 0 for further bullets */);
    void add_to_list(Bullet *);
    void remove(Bullet *);
    void remove_for_script(Bullet *);
    void move();
    void check_collisions(class gsMap *, class gsMap * = NULL);
    void draw(class gsMap *);
    void draw(BITMAP *);
    void save_background(class gsMap *);
    void restore_background(class gsMap *);
};

#endif
