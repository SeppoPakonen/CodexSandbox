/*
 *   GRAVITY STRIKE -- gsenemy.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSENEMY_
#define _GSENEMY_

#include "gsframes.h"
#include "gswaypnt.h"
#include "gsscript.h"

enum DIRECTION
{
  D_LEFT,
  D_RIGHT,
  D_UP,
  D_DOWN
};

enum ENEMY_TYPE
{
  E_NONE,
  E_USER,
  E_TOWER_SINGLE,
  E_TOWER_FLAK,
  E_TOWER_THREE,
  E_TOWER_PULSE,
  E_TANK_MOBILETOWER,
  E_TANK_FLYINGTOWER,
  E_SHIP_BLUE1,
  E_SHIP_BLUE2,
  E_SHIP_BIGRED,
  E_SHIP_PROBE1,
  E_SHIP_PROBE2,
  E_SHIP_RELASH,
  E_SHIP_BIGBOSS
};

class Enemy
{
  protected:
    Enemy *prev, *next;                            // previous / next enemy in list
    Enemy *connected_enemy;                        // with this enemy connected?
    Image *frames;                                 // frame handler

    int kill_frames;                               // kill frame handler when done? (not for user images)
    BITMAP *backbuf;                               // save bground of specific enemies
    int   type;                                    // type of enemy
    int   image;                                   // (first) image of enemy (position in datafile)
    int   nr;                                      // number of enemy
    long psig;                                     // to which player? (-1 = neutral)
    float x;                                       // x position of enemy (top left)
    float y;                                       // y position of enemy (top left)
    int   w;                                       // width of image
    int   h;                                       // height of image
    int   ow;                                      // original width
    int   oh;                                      // original height
    float xspd;                                    // speed in x dir
    float yspd;                                    // speed in y dir
    float dxspd;                                   // destination speed
    float dyspd;                                   // "
    int   bound_x;                                 // bounding box x start (for screen clear)
    int   bound_y;                                 // bounding box y
    int   bound_w;                                 // width of bounding box
    int   bound_h;                                 // height of bounding box
    int   xradius;                                 // x radius to activate
    int   yradius;                                 // y radius to activate
    int   onehit;                                  // object hit?
    int   hitby;                                   // hit by "origin"
    float hitby_sx;                                // last bullet's x speed
    float hitby_sy;                                // last bullet's y speed
    float hitpoints;                               // hitpoints
    int   maxhit;                                  // maximum hitpoints
    int   dispframe;                               // frame to display (normally equal to current frame)
    int   curframe;                                // current frame
    int   curframet;                               // time in current frame
    int   maxframe;                                // maximum frame
    int   maxframet;                               // maximum frametime
    int   startframe;                              // frame to start with
    int   physical_frames;                         // how many "real" frames?
    int   user;                                    // user field (strength etc.)
    float bullet_speed;                            // speed of bullet
    float bullet_speed_x;                          // x speed of bull (opt)
    float bullet_speed_y;                          // y speed of bull (opt)
    float bullet_strength;                         // strength of bullet
    float bullet_hitp_hull;                        // hitpoints for player's hull
    float bullet_hitp_shield;                      // hitpoints for player's shield
    int   bullet_freq;                             // frequency of bullets
    int   cur_bullet_freq;                         // current freq count
    int   bullet_type;                             // type of bullets
    int   active;                                  // object activated?
    int   teleport_time;                           // time since last teleporting
    int   invisible;                               // is enemy invisible?
    int   freezed;                                 // is enemy freezed?
    long int shrinked;                             // is enemy shrinked?
    float shrinklevel;                             // current shrinklevel
    float maxshrinklevel;                          // maximum shrinklevel
    int   dont_continue;                           // don't continue after hook function?
    int   do_hooks;                                // do hooks (this or globals->do_all_hooks)
    int   kill;                                    // kill enemy?
    int   dead_by_network;                         // killed by some player on network?
    int   user_int[20];                            // user vars (int)
    float user_float[20];                          // user vars (floats)

    float old_wp_x;
    float old_wp_y;
    void  wp_calculate_new_speeds(Waypoint *);
    void  do_damage_effect();

  public:
    // constr. / destr.
    Enemy();
    virtual ~Enemy();

    gsWaypoint  *wp;                               // waypoint handler

    // get / set
    Enemy       *get_prev()                        { return prev; }
    Enemy       *get_next()                        { return next; }
    Enemy       *get_connected()                   { return connected_enemy; }
    void        set_connected(Enemy *n)            { connected_enemy = n; }
    void        set_prev(Enemy *n)                 { prev = n; }
    void        set_next(Enemy *n)                 { next = n; }
    void        set_type(int n)                    { type = n; }
    int         get_type()                         { return type; }
    int         get_nr()                           { return nr; }
    void        set_nr(int n)                      { nr = n; }
    long        get_sig()                          { return psig; }
    void        set_sig(long n)                    { psig = n; }
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
    int         get_xradius()                      { return xradius; }
    void        set_xradius(int n)                 { xradius = n; }
    int         get_yradius()                      { return yradius; }
    void        set_yradius(int n)                 { yradius = n; }
    float       get_xspd()                         { return xspd; }
    float       get_yspd()                         { return yspd; }
    void        set_xspd(float n)                  { xspd = n; }
    void        set_yspd(float n)                  { yspd = n; }
    float       get_hit()                          { return hitpoints; }
    int         get_maxhit()                       { return maxhit; }
    void        set_hit(float n)                   { hitpoints = n; }
    void        set_maxhit(int n)                  { maxhit = n; }
    int         get_onehit()                       { return onehit; }
    int         get_hitby()                        { return hitby; }
    float       get_hitby_sx()                     { return hitby_sx; }
    float       get_hitby_sy()                     { return hitby_sy; }
    int         get_user()                         { return user; }
    void        set_user(int n)                    { user = n; }
    int         get_active()                       { return active; }
    void        set_active(int n)                  { active = n; }
    void        set_bullet_speed(float n)          { bullet_speed = n; }
    float       get_bullet_speed()                 { return bullet_speed; }
    void        set_bullet_speed_x(float n)        { bullet_speed_x = n; }
    float       get_bullet_speed_x()               { return bullet_speed_x; }
    void        set_bullet_speed_y(float n)        { bullet_speed_y = n; }
    float       get_bullet_speed_y()               { return bullet_speed_y; }
    void        set_bullet_freq(int n)             { bullet_freq = n; }
    float       get_bullet_freq()                  { return bullet_freq; }
    void        set_bullet_strength(float n)       { bullet_strength = n; }
    float       get_bullet_strength()              { return bullet_strength; }
    void        set_bullet_hull_hitp(float n)      { bullet_hitp_hull = n; }
    float       get_bullet_hull_hitp()             { return bullet_hitp_hull; }
    void        set_bullet_shield_hitp(float n)    { bullet_hitp_shield = n; }
    float       get_bullet_shield_hitp()           { return bullet_hitp_shield; }
    void        set_cur_bullet_freq(int n)         { cur_bullet_freq = n; }
    int         get_cur_bullet_freq()              { return cur_bullet_freq; }
    void        set_bullet_type(int n)             { bullet_type = n; }
    int         get_bullet_type()                  { return bullet_type; }
    int         get_teleport_time()                { return teleport_time; }
    void        set_teleport_time(int n)           { teleport_time = n; }
    void        set_invisible(int n)               { invisible = n; }
    int         get_invisible()                    { return invisible; }
    void        set_freezed(int n)                 { freezed = n; }
    int         get_freezed()                      { return freezed; }
    void        set_shrinked(int n)                { shrinked = n; }
    int         get_shrinked()                     { return shrinked; }
    void        set_maxshrink(float n)             { maxshrinklevel = n; }
    float       get_maxshrink()                    { return maxshrinklevel; }
    int         get_dont_continue()                { return dont_continue; }
    void        set_dont_continue(int n)           { dont_continue = n; }
    int         get_do_hooks()                     { return do_hooks; }
    void        set_do_hooks(int n)                { do_hooks = n; }
    RLE_SPRITE* get_current_sprite();
    void        remove()                           { kill = 1; }
    int         get_remove()                       { return kill; }
    void        set_dead_by_network()              { dead_by_network = 1; }
    int         get_dead_by_network()              { return dead_by_network; }
    void        set_user_int(int n, int d)         { if (n >=0 && n <= 19) user_int[n] = d; }
    void        set_user_float(int n, float d)     { if (n >=0 && n <= 19) user_float[n] = d; }
    int         get_user_int(int n)                { if (n >=0 && n <= 19) return user_int[n]; else return 0; }
    int         get_user_float(int n)              { if (n >=0 && n <= 19) return user_float[n]; else return 0; }
   
    void        find_path(int targetx, int targety); // finds a path to targetx/targety and creates waypoints

    // abstract functions
    virtual void init();
    virtual int  hit(long frompl, int fromen, float n, int x=0, int y=0, float fxspd=0, float fyspd=0, int weapon=-1);
    virtual void move() = 0;
    virtual void update() = 0;
    virtual void draw(class gsMap *);
    virtual void dead() = 0;
    virtual void shoot() = 0;
    virtual void check_collisions();
};


class gsEnemy
{
  private:
    Enemy *first_enemy, *last_enemy;
    int    count;

  public:
    gsEnemy();
    ~gsEnemy();

    int get_count()                                { return count; }
    Enemy *get_first_enemy()                       { return first_enemy; }
    Enemy *get_enemy(int n);

    Enemy *add(int num,/*ENEMY_TYPE*/int t, int px, int py, long psig=-1, float user1=0, float user2=0, float user3=0, float user4=0, float user5=0, float user6=0);
    void add_to_list(Enemy *);
    void remove(Enemy *);
    void remove_for_script(Enemy *);
    void update();
    void draw(class gsMap *);
};

extern gsEnemy *enemies;

#endif
