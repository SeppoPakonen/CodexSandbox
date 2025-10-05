/*
 *   GRAVITY STRIKE -- gsplayer.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSPLAYER_
#define _GSPLAYER_

#include "gsbullet.h"
#include "gspanel.h"
#include <string.h>

enum predefined_ships {
  SHIP_PIRANHA,
  SHIP_RAZOR,
  SHIP_BOMBER,
  SHIP_CUSTOM
};

enum PLAYER_VALUES {
  PV_HULL,
  PV_SHIELD,
  PV_HULL_RE,
  PV_SHIELD_RE,
  PV_WALLSHIELD,
  PV_WEAPON_RE,
  PV_RADAR_ZOOM,
  PV_WEAPON_SLOTS,
  PV_TURN_SPEED,
  PV_MAX_SPEED,
  PV_ACCELERATION,
  PV_FUEL_CONS,
  PV_WEAPON_STORAGE
};

// values are for bitfield
enum EXTRA_ABILITIES {
  EXTRA_NONE         = 0,
  EXTRA_AFTERBURNER  = 1,
  EXTRA_CLOAK        = 2,
  EXTRA_DEFLECTOR    = 4
};

enum PLAYER_DAMAGE {
  DAMAGE_NONE,
  DAMAGE_THRUSTER,
  DAMAGE_STEERING,
  DAMAGE_FUEL,
  DAMAGE_WEAPONS,
  DAMAGE_SHIELD,
  DAMAGE_LANDING,
  DAMAGE_RADAR,
  DAMAGE_MAXDAMAGE
};

typedef struct weaponslot {
  int type;                                        // type of bullet
  int bullets;                                     // amount of bullets left
  int reload;                                      // reload rate (in frames)
  int cur_reload;                                  // current reload time
  int upgrade;                                     // upgrade x active
  int max_upgrades;                                // maximum upgrades
  int active;                                      // slot active...?
} weaponslot;
/*
typedef struct extraslot {
  int type;                                        // type of extra
  int energy;                                      // amount of energy left
  int reload;                                      // reload rate (in frames)
  int cur_reload;                                  // current reload time
} extraslot;
*/
typedef struct statistics {
  char pname[50];                                  // player name (for stats use)
  int frags;                                       // how much kills?
  int suicides;                                    // how much suicides?
  int bullets_shot;                                // how much bullets shot?
  int bullets_hit;                                 // how much bullets hit?
  int extras_taken;                                // how much extras taken?
  int max_time_alive;                              // maximum alive time
  int laptime[10];                                 // time for each of (max) 10 laps (race mode)
  long int bonus;                                  // bonus (for scripting)
  long int enemy_bonus;                            // enemy bonus (for single player)
  int mission_status;                              // status of mission (single player)
  int lostlifes;                                   // lost lifes (single player)
  int used_time;                                   // time (sec) needed for level
  long int plsig;                                  // sig of statistics player (for network game)
  bool set;                                        // stats are set? (for network game)
} statistics;

class gsPlayer
{
  private:
    // ship characteristics
    predefined_ships ship_type;                    // ship type
    int     ship_color;                            // color of predef. ship
    float   turn_speed;                            // turning speed
    float   max_speed;                             // maximum speed
    float   player_acc;                            // player acceleration
    float   fuel_dec;                              // dec. of fuel
    int     max_hull;                              // max hitpoints of hull
    int     max_shield;                            // max hitpoints of shields
    int     max_fuel;                              // max fuel to load
    int     max_wall_shield;                       // max hitpoints of wall shield
    int     max_extra_power;                       // max extra power to have
    float   shield_recharge;                       // recharge time of shields
    float   wall_shield_recharge;                  // recharge rate of wall shield
    float   hull_repair;                           // hull repair time
    char    ship_name[30];                         // name of ship
    char    ship_desc[150];                        // description of ship
    int     max_radar_zoom;                        // maximum radar zoom?
    float   weapon_storage;                        // how much weapons can this ship carry? (percent value: 0-1)
    float   rocket_turn_speed;                     // how fast is the rocket weapon changing direction?
    float   weapon_recharge;                       // recharge percent value of weapons (0-1)
    int     avail_weapon_slots;                    // weapon slots available (0-6)
    int     refuel_time;                           // ticks for refueling
    float   weight_factor;                         // factor for weight
    float   extra_recharge;                        // recharge rate of extra

    BITMAP  *ship[4];                              // ship without thrusters
    BITMAP  *shield_frame[10];                     // maximum ten shield frames
    BITMAP  *debris[4];                            // make four specific debris pixels
    int     max_shield_frame;                      // shield frames used...?
    int     temp_thrust;                           // how long to show one thruster pic
    int     pthrust;                               // thrusters started (by set_thrust())
//    int     paftburn;                              // afterburners started (by set_afterburner())
//    int     pcloak;                                // cloaking mode started (by set_cloak())
    int     pextra_action;                         // extra activated
    int     width;                                 // width of playship (head=0)
    int     height;                                // height of playship (head=0)
    int     owidth;                                // original width (for shrink function)
    int     oheight;                               // original height
    int     back_width;                            // width of background to save
    int     back_height;                           // height of background to save
    int     shield_w;                              // shield width
    int     shield_h;                              // shield height

    // control
    float   xpos,ypos;                             // "real" coordinates of playship (upper left corner)
    float   head;                                  // direction of playship
    float   head_to;                               // desired head of ship (script?)
    float   xspd,yspd;                             // speed
    float   xacc,yacc;                             // normal acceleration
    float   xacc2,yacc2;                           // second acceleration
    int     weight;                                // weight of ship
    int     max_weight;                            // maximum weight of ship

    int     homebase;                              // homebase of player
    int     home_x;                                // home x position
    int     home_y;                                // home y position
    int     land;                                  // ship landed on base "land" (0 = flying)
    int     land_dur;                              // landing duration?
    int     land_base;                             // landed on base...

    int     thrust;                                // full throttle...?
    int     only_thrust_frame;                     // show thrust pic...? (for script)
    int     extra_action;                          // extra action (e.g. afterburner, cloaking mode)
    int     freezed;                               // is player freezed? (= frames to freeze)
    long int shrinked;                             // is player shrinked? (= frames to shrink)
    int     delirium;                              // is player in delirium? (= frames to be confused)
    float   shrinklevel;                           // current shrinklevel
    int     hitby;                                 // last hit by player x
    int     killedby;                              // killed by player x
    int     hitbyweap;                             // last hit by weapon x
    int     time_alive;                            // ticks alive...?
    int     bumped;                                // player bump counter :)
    int     damage;                                // currently damaged system
    int     damage_norepair;                       // dont repair damage when hull is good

    int     last_hit_bumper;                       // time of last bumper hit
    int     last_hit_gas;                          // time of last delirium bomb hit

    int     lifes;                                 // lifes left?
    float   fuel;                                  // fuel left?
    int     dead;                                  // player dead?
    bool    authorative_dead;                      // dead because net server told us (player collisions)
    float   hull;                                  // status of hull (0 -> dead!)
    float   shield;                                // status of shields
    float   wall_shield;                           // status of wall shields
    float   extra_power;                           // remaining power of ship extra
    
    int     invulnerability;                       // ship is invulnerable? (in ticks)
    int     indestructibility;                     // ship is only indestructible? (in ticks)

    char    name[31];                              // player name
    char    shipimage_file[51];                    // name of file with ship image
    char    shipfile_file[51];                     // name of shipfile
    int     percent_value[21];                     // percent values (in config) of ship character
    int     orig_percent_value[21];                // original percent values (before changes due to handicaps etc.)
    long    sig;                                   // player signature/number (for network)
    int     network;                               // is this a network player?
    int     no_fuel_counter;                       // temp var for no_fuel_landing

    BITMAP  *background;                           // temporary background of ship sprite

    int     active_weapon;                         // active weapon slot
//    int     active_extra;                          // active extra slot
    weaponslot weapon[10];                         // weapon slots (up to 10)
//    extraslot  extra;                              // slots for an extra
    int     extra;
    int     shot;                                  // playship fired?
    int     shot_time;                             // time since last shot (for bombs)
    int     teleport_time;                         // time since last teleporting
    int     locked_player;                         // number of player locked (for rockets, ...)
    statistics stats;                              // player statistics

    class   gsMap   *dmap;                         // on which map?
    class   gsPlayerOverlay *overlay;              // status overlay class
    class   gsPanel *ppanel;                       // panel with status?

    class   gsMap *show_status_map;                // map to draw overlay on?
    int     show_status_flag;                      // show overlay with status info?
    int     show_status_mode;                      // mode (0=nothing, 1=distance, 2=always)

    int     handicap;                              // player handicap (0-10)
    int     do_recoil;                             // activate recoil effects?

    int     play_thrust_sound;                     // is thruster sound looping at the moment?
    int     dont_continue;                         // don't continue after hook function?
    int     do_hooks;                              // do hook functions for this object?

    int     user_int[20];                          // user vars (int)
    float   user_float[20];                        // user vars (floats)

    // functions
    void    player_dead();                         // player is dead...
    void    create_debris_pixels();                // debris pixels...
    void    check_landing();                       // does actions when player landed
    int     over_base();                           // is player over landing pad?
    int     do_player_collision(gsPlayer *);       // collision with other player

    void    apply_gravity(int st);                 // applies grav to weapons and players (extra)
    void    apply_gravity_to(int px, int py, float *pxs, float *pys, int st, int ptype);
    
  public:
    // constructor, destructor
    gsPlayer(long num, int ship, int col, char *shipfile = NULL, class gsMap * = NULL);
    ~gsPlayer();

    // variables
    class   gsControl *controls;                   // player controls
    class   gsPanel *get_ppanel()                  { return ppanel; }

    // get/set
    float   get_x()                                { return xpos; }
    void    set_x(float a)                         { xpos = a; }
    float   get_y()                                { return ypos; }
    void    set_y(float a)                         { ypos = a; }
    float   get_xspd()                             { return xspd; }
    float   get_yspd()                             { return yspd; }
    void    set_xspd(float a)                      { if (!land) xspd = a; }
    void    set_yspd(float a)                      { if (!land) yspd = a; }
    float   get_xacc()                             { return xacc; }
    float   get_yacc()                             { return yacc; }
    void    set_xacc(float a)                      { xacc = a; }
    void    set_yacc(float a)                      { yacc = a; }
    void    set_xacc2(float a)                     { xacc2 = a; }
    void    set_yacc2(float a)                     { yacc2 = a; }

    int     get_width()                            { return width; }
    int     get_height()                           { return height; }
    int     get_owidth()                           { return owidth; }
    int     get_oheight()                          { return oheight; }
    int     get_back_width()                       { return back_width; }
    int     get_back_height()                      { return back_height; }
    int     get_shield_w()                         { return shield_w; }
    int     get_shield_h()                         { return shield_h; }

    float   get_head()                             { return head; }
    void    set_head(float a)                      { head = a; head_to = a; }
    float   get_head_to()                          { return head_to; }
    void    set_head_to(float a)                   { head_to = a; }
    void    turn_to(int x, int y);
    int     get_thrust()                           { return thrust; }
    void    set_thrust()                           { pthrust = 1; }
    void    show_thrust_frame(int n)               { only_thrust_frame = n; }
    int     get_extra_action()                     { return extra_action; }
    void    set_extra_action(int n=1)              { pextra_action = n; }
    int     get_weight()                           { return weight; }
    int     get_max_weight()                       { return max_weight; }
    void    set_weight(int n)                      { weight = n; }
    float   get_weight_factor()                    { return weight_factor; }
    void    set_weight_factor(float n)             { weight_factor = n; }
    void    set_lifes(int n);
    int     get_lifes()                            { return lifes; }

    int     get_network()                          { return network; }
    void    set_network(int n)                     { network = n; }
    long    get_sig()                              { return sig; }
    void    set_sig(long a)                        { sig = a; }
    int     get_hitby()                            { return hitby; }
    void    set_hitby(int n)                       { hitby = n; }
    int     get_killedby()                         { return killedby; }
    void    set_killedby(int n)                    { killedby = n; }
    int     get_hitbyweap()                        { return hitbyweap; }
    void    set_hitbyweap(int n)                   { hitbyweap = n; }
    int     get_time_alive()                       { return time_alive; }
    
    void    set_invulnerability(int n)             { invulnerability = n; }
    int     get_invulnerability()                  { return invulnerability; }
    void    set_indestructibility(int n)           { indestructibility = n; }
    int     get_indestructibility()                { return indestructibility; }

    void    set_damage(int n)                      { if (n < DAMAGE_MAXDAMAGE) { damage = n; if (n != DAMAGE_NONE) damage_norepair = 1; else damage_norepair = 0; } }
    int     get_damage()                           { return damage; }

    int     get_frags()                            { return stats.frags; }
    void    set_frags(int n);
    void    inc_frags();
    void    dec_frags();
    int     get_suicides()                         { return stats.suicides; }
    void    set_suicides(int n)                    { stats.suicides = n; }
    void    inc_suicides()                         { stats.suicides++; }
    void    inc_bullets_hit()                      { stats.bullets_hit++; }
    void    inc_bonus(int n=1)                     { stats.bonus += n; }
    void    dec_bonus(int n=1)                     { stats.bonus -= n; }
    void    inc_enemy_bonus(int n=1)               { stats.enemy_bonus += n; }
    void    dec_enemy_bonus(int n=1)               { stats.enemy_bonus -= n; }
    void    set_mission_status(int n)              { stats.mission_status = n; }
    int     get_mission_status()                   { return stats.mission_status; }
    long int get_bonus()                           { return stats.bonus; }
    void    inc_bullets_shot(int n)                { stats.bullets_shot += n; }
    int     get_bullets_shot()                     { return stats.bullets_shot; }
    void    inc_bullets_hit(int n)                 { stats.bullets_hit += n; }
    int     get_bullets_hit()                      { return stats.bullets_hit; }
    statistics *get_statistics()                   { return &stats; }
    statistics get_statistics_copy()               { return stats; }
    void    set_statistics(statistics *n)          { stats = *n; }

    int     get_homebase()                         { return homebase; }
    void    set_homebase(int);
    int     get_home_x()                           { return home_x; }
    int     get_home_y()                           { return home_y; }
    void    set_home_x(int a)                      { home_x = a; }
    void    set_home_y(int a)                      { home_y = a; }

    BITMAP  *get_ship(int n)                       { return ship[n]; }
    BITMAP  *get_shield_frame(int n)               { if (n >= 0 && n <= max_shield_frame) return shield_frame[n]; return NULL; }
    int     get_max_shield_frame()                 { return max_shield_frame; }

    char    *get_name()                            { return name; }
    void    set_name(const char *a);
    char    *get_ship_name()                       { return ship_name; }
    void    set_ship_name(const char *a)           { strncpy(ship_name, a, 30); }
    char    *get_ship_desc()                       { return ship_desc; }
    void    set_ship_desc(const char *a)           { strncpy(ship_desc, a, 150); }
    char    *get_shipimage_filename()              { return shipimage_file; }
    char    *get_shipfile_filename()               { return shipfile_file; }

    void    set_cur_percent_value(int n, int v)    { percent_value[n] = v; }
    int     get_cur_percent_value(int n)           { return percent_value[n]; }
    void    set_percent_value(int n, int v)        { percent_value[n] = orig_percent_value[n] = v; }
    int     get_percent_value(int n)               { return orig_percent_value[n]; }

    void    set_ship_value(int what, int value);
    int     get_ship_value(int what);

    void    set_panel(class gsPanel *n)            { ppanel = n; }

    class   gsMap *get_map()                       { return dmap; }
    void    set_map(class gsMap *m)                { dmap = m; }

    void    set_dead()                             { dead = 1; }
    int     get_dead()                             { return dead; }
    void    set_authorative_dead(bool n)           { authorative_dead = n; }
    int     get_authorative_dead()                 { return authorative_dead; }
    void    set_freezed(int n)                     { freezed = n; }
    int     get_freezed()                          { return freezed; }
    void    set_shrinked(int n)                    { shrinked = n; }
    int     get_shrinked()                         { return shrinked; }
    void    set_delirium(int n)                    { delirium = n; }
    int     get_delirium()                         { return delirium; }

    void    set_land(int n)                        { land = n; }
    int     get_land()                             { return land; }
    int     get_land_dur()                         { return land_dur; }
    void    set_land_base(int n)                   { land_base = n; }
    int     get_land_base()                        { return land_base; }

    int     get_refuel_time()                      { return refuel_time; }
    void    set_refuel_time(int n)                 { refuel_time = n; }
    float   get_hull()                             { return hull; }
    void    set_hull(float n)                      { if (n <= max_hull && n >= 0) { hull = n; } if (ppanel) ppanel->update_shields(); }
    float   get_shield()                           { return shield; }
    void    set_shield(float n)                    { if (n <= max_shield && n >= 0) { shield = n; if (ppanel) ppanel->update_shields(); } }
    float   get_wall_shield()                      { return wall_shield; }
    void    set_wall_shield(float n)               { if (n <= max_wall_shield && n >= 0) { wall_shield = n;  if (ppanel) ppanel->update_shields(); } }
    float   get_fuel()                             { return fuel; }
    void    set_fuel(float n);
    float   get_max_shield(int h=0)                { if (!h) return max_shield; else return max_shield  * (1-0.08*handicap); }
    void    set_max_shield(int n)                  { max_shield = n; }
    float   get_max_wall_shield()                  { return max_wall_shield; }
    void    set_max_wall_shield(int n)             { max_wall_shield = n; }
    float   get_max_hull(int h=0)                  { if (!h) return max_hull; else return max_hull * (1-0.08*handicap); }
    void    set_max_hull(int n)                    { max_hull = n; }
    float   get_max_fuel()                         { return max_fuel; }
    float   get_hull_repair(int h=0)               { if (!h) return hull_repair; else return hull_repair  * (1-0.08*handicap); }
    void    set_hull_repair(float n)               { hull_repair = n; }
    float   get_shield_recharge(int h=0)           { if (!h) return shield_recharge; else return shield_recharge  * (1-0.08*handicap); }
    void    set_shield_recharge(float n)           { shield_recharge = n; }
    float   get_wall_shield_recharge(int h=0)      { if (!h) return wall_shield_recharge; else  return wall_shield_recharge * (1-0.08*handicap); }
    void    set_wall_shield_recharge(float n)      { wall_shield_recharge = n; }
    float   get_fuel_dec(int h=0)                  { if (!h) return fuel_dec; else return fuel_dec * (1+0.08*handicap); }
    void    set_fuel_dec(float n)                  { fuel_dec = n; }
    float   get_max_speed(int h=0)                 { if (!h) return max_speed; else return max_speed  * (1-0.08*handicap); }
    void    set_max_speed(float n)                 { max_speed = n; }
    float   get_player_acc(int h=0)                { if (!h) return player_acc; else return player_acc  * (1-0.08*handicap); }
    void    set_player_acc(float n)                { player_acc = n; }
    float   get_turn_speed(int h=0)                { if (!h) return turn_speed; else return turn_speed  * (1-0.08*handicap); }
    void    set_turn_speed(float n)                { turn_speed = n; }
    void    set_ship_type(predefined_ships n)      { ship_type = n; }
    int     get_ship_type()                        { return ship_type; }
    int     get_ship_color()                       { return ship_color; }
    void    set_max_radar_zoom(int n)              { max_radar_zoom = n; }
    int     get_max_radar_zoom()                   { return max_radar_zoom; }
    void    set_weapon_storage(float n)            { weapon_storage = n; }
    float   get_weapon_storage()                   { return weapon_storage; }
    int     get_locked_player()                    { return locked_player; }
    float   get_rocket_turn_speed()                { return rocket_turn_speed; }
    void    set_rocket_turn_speed(float n)         { rocket_turn_speed = n; }
    float   get_weapon_recharge()                  { return weapon_recharge; }
    void    set_weapon_recharge(float n)           { weapon_recharge = n; }
    float   get_extra_power()                      { return extra_power; }
    void    set_extra_power(int n);
    float   get_extra_recharge(int h=0)            { if (!h) return extra_recharge; else return extra_recharge  * (1-0.08*handicap); }
    void    set_extra_recharge(float n)            { extra_recharge = n; }
    int     get_max_extra_power()                  { return max_extra_power; }

    int     get_active_weapon()                    { return active_weapon; }
    void    set_active_weapon(int n);
//    int     get_active_extra()                     { return active_extra; }
    weaponslot *get_weapon_slot(int n)             { return &weapon[n]; }
    weaponslot get_weapon_slot_copy(int n)         { return weapon[n]; }
    void    set_weapon_slot(int n, weaponslot *w)   { weapon[n] = *w; if (ppanel) ppanel->update_weapons(); }
//    extraslot  get_extra_slot()/*(int n)*/         { return extra/*[n]*/; }
    int     get_weapon_bullets(int n)              { return weapon[n].bullets; }
    void    set_weapon_bullets(int n, int b)       { if (n >= 0 && n <= 6) { weapon[n].bullets = b; if (ppanel) ppanel->update_weapons(); } }
    int     get_extra()                            { return extra; }
    void    set_extra(int n)                       { extra = n; }
    int     get_avail_weapon_slots()               { return avail_weapon_slots; }
    void    set_avail_weapon_slots(int n)          { if (n >= 0 && n <= 6) avail_weapon_slots = n; }
    void    activate_weapon_slot(int n)            { weapon[n].active = 1; if (ppanel) ppanel->update_weapons(); }
    void    deactivate_weapon_slot(int n)          { weapon[n].active = 0; if (ppanel) ppanel->update_weapons(); }

    void    set_status_mode(int n)                 { show_status_mode = n; }
    void    show_status_set_map(class gsMap *m);
    void    show_status()                          { show_status_flag = 1; }
    void    hide_status()                          { show_status_flag = 0; }

    int     get_handicap()                         { return handicap; }
    void    set_handicap(int n);

    int     get_recoil()                           { return do_recoil; }
    void    set_recoil(int n)                      { do_recoil = n; }

    int     get_dont_continue()                    { return dont_continue; }
    void    set_dont_continue(int n)               { dont_continue = n; }
    int     get_do_hooks()                         { return do_hooks; }
    void    set_do_hooks(int n)                    { do_hooks = n; }

    void    set_user_int(int n, int d)             { if (n >=0 && n <= 19) user_int[n] = d; }
    void    set_user_float(int n, float d)         { if (n >=0 && n <= 19) user_float[n] = d; }
    int     get_user_int(int n)                    { if (n >=0 && n <= 19) return user_int[n]; else return 0; }
    int     get_user_float(int n)                  { if (n >=0 && n <= 19) return user_float[n]; else return 0; }

    // functions
    void init();                                   // init player
    void reinit();                                 // reinit player after death
    void set_standard_bullets();                   // sets all weapons with standard amount of bullets
    void apply_weapon_pack(int type, int locrand); // apply weapon pack
    void put_extra();                              // lays out an extra
    void convert_percents_to_shipvalues();         // converts percent values to the correct values
    int  load_ship_image(char *, int);                  // load ship image from given file
    void load_ship_image(predefined_ships, int col, int); // load ship image from normal data
    void return_home();                            // set player to his home
    void set_weapon(int n, /*WEAPON_TYPE*/int m, int bullets=0, int recharge=-1);  // set weapon slot n to weapon m
//    void set_extra(/*int n, EXTRA_ABILITIES*/int m, int energy=0, int recharge=-1);  // set extra slot to extra m
//    int  check_for_extra(/*enum EXTRA_ABILITIES*/int m);  // checks if extra is available
    void update();                                 // update player variables
    void move();                                   // move player
    void save_background();                        // save background of ship image
    void display(class gsMap *);                   // display playship
    void display_exhaust_pixels(gsMap *, gsMap * = NULL); // display exhaust pixels
    void clear(class gsMap *);                     // wipe away ship (overwrite with background)
    void check_map_collisions();                   // check collisions with walls
    void check_stuff();                            // check other stuff
    void check_player_collisions();                // check collisions with other players
    void check_object_collisions();                // check colls with objects
    void check_enemy_collisions();                 // check colls with enemies
    void shoot();                                  // player shoots active_weapon
    void do_damage(int dmg);                       // player is damaged, print info etc.
    void hit(int fromplayer, int fromenemy, float hullhit, float shieldhit, int weap=0, int fx=0, int fy=0, int nostat=0);   // hit by weapon
    void choose_next_available_weapon(int prevweap);  // sets the next weapon when bullets run out
    void choose_next_weapon(int dir);              // next/prev weapon

    void save_overlay_back();                      // save background before drawing overlay
    void draw_overlay();                           // draw overlay (if needed)
    void restore_overlay();                        // restore overlay
    int  load_ship(char *);                        // loads a new ship
    int  load_image(char *);                       // loads a new ship image

};

int get_player_nr_by_sig(long);                     // get array number by signature
gsPlayer *get_player_by_sig(long);                  // get player struct by signature
extern gsPlayer *player[];

#endif

