/*
 *   GRAVITY STRIKE -- menu/mshpbld.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MSHPBUILD_
#define _MSHPBUILD_

enum M_SHIPBUILDER_CONTROLS {
  M_SHPBLD_CAPTION,
  M_SHPBLD_SAVE,
  M_SHPBLD_SAVENEW,
  M_SHPBLD_DELETE,
  M_SHPBLD_BACK,

  M_SHPBLD_SHIPIMAGE,
  M_SHPBLD_SHIPIMAGE_LEFT,
  M_SHPBLD_SHIPIMAGE_RIGHT,
  M_SHPBLD_NAME,
  M_SHPBLD_DESC,
  M_SHPBLD_LOADIMAGE,

  M_SHPBLD_HULL,
  M_SHPBLD_TURNSPEED,
  M_SHPBLD_SHIELD,
  M_SHPBLD_MAXSPEED,
  M_SHPBLD_HULLRE,
  M_SHPBLD_ACCELERATION,
  M_SHPBLD_SHIELDRE,
  M_SHPBLD_WALLSHIELD,
  M_SHPBLD_FUELCONS,
  M_SHPBLD_WEAPONRE,
  M_SHPBLD_WEAPONSTOR,
  M_SHPBLD_RADARZOOM,
  M_SHPBLD_WEAPONSLOTS,
  M_SHPBLD_CAP_EXTRA,
  M_SHPBLD_EXTRA1,
  M_SHPBLD_EXTRA2,
  M_SHPBLD_EXTRA3,
  M_SHPBLD_REMPOINTS,

  M_SHPBLD_EDITWEAPONS,
  M_SHPBLD_EDITSHIP,

  M_SHPBLD_WEAPON1,
  M_SHPBLD_WEAPON2,
  M_SHPBLD_WEAPON3,
  M_SHPBLD_WEAPON4,
  M_SHPBLD_WEAPON5,
  M_SHPBLD_WEAPON6,
  M_SHPBLD_WEAPON7,
  M_SHPBLD_WEAPON8,
  M_SHPBLD_WEAPON9,
  M_SHPBLD_WEAPON10,
  M_SHPBLD_WEAPON11,
  M_SHPBLD_WEAPON12,
  M_SHPBLD_WEAPON13,
  M_SHPBLD_WEAPON14,
  M_SHPBLD_WEAPON1_OSLOT,
  M_SHPBLD_WEAPON2_OSLOT,
  M_SHPBLD_WEAPON3_OSLOT,
  M_SHPBLD_WEAPON4_OSLOT,
  M_SHPBLD_WEAPON5_OSLOT,
  M_SHPBLD_WEAPON6_OSLOT,
  M_SHPBLD_WEAPON7_OSLOT,
  M_SHPBLD_WEAPON8_OSLOT,
  M_SHPBLD_WEAPON9_OSLOT,
  M_SHPBLD_WEAPON10_OSLOT,
  M_SHPBLD_WEAPON11_OSLOT,
  M_SHPBLD_WEAPON12_OSLOT,
  M_SHPBLD_WEAPON13_OSLOT,
  M_SHPBLD_WEAPON14_OSLOT,
  M_SHPBLD_WEAPON1_SLOT,
  M_SHPBLD_WEAPON2_SLOT,
  M_SHPBLD_WEAPON3_SLOT,
  M_SHPBLD_WEAPON4_SLOT,
  M_SHPBLD_WEAPON5_SLOT,
  M_SHPBLD_WEAPON6_SLOT,

  M_SHPBLD_WEAPON_DESC,
  M_SHPBLD_CAP_WEAPAVAIL,
  M_SHPBLD_CAP_WEAPSLOTS,
  M_SHPBLD_CAP_WEAPDESC,
  M_SHPBLD_CAP_WEAPON_NAME,
  M_SHPBLD_CAP_DAMAGE,
  M_SHPBLD_CAP_DAMAGE_SHIELD,
  M_SHPBLD_CAP_DAMAGE_HULL,
  M_SHPBLD_CAP_DAMAGE_SP,

  M_SHPBLD_MSGBOX

//  M_SHPBLD_EXTRAS,
//  M_SHPBLD_AFTERBURNER,
//  M_SHPBLD_AUTOBREAK
};

class mShipBuilder
{
  private:
    class mControl *button[106];
    class gsPlayer *pl;                            // player structure to edit
    int   msgbox_active;                           // active message box?
    int   weapon_active;                           // dragging weapon around?
    int   active_button;
    int   esc_pressed;
    int   switch_to_menu;                          // which menu to display?
    int   moving;                                  // menu is moving in/out
    int   button_count;                            // how much buttons?
    int   no_input;                                // no input flag (while scrolling out)
    int   sum;                                     // summary of energy points
    int   max_sum;
    int   return_to_menu;                          // return to what menu on save/back?

    char  ship_filename[30];                       // filename of ship to edit
    int   newship;                                 // create a new ship?
    char  loadimage_path[100];                     // path of image to load
    char  loaded_image_path[100];                  // full path of "good" image

    // functions
    int  count_ship_sum();                        // counts the sum of energy used
    void update_weaponslots();                    // updates visible/active slots by player->avail_weapon_slots
    void create_user_ship_filename();             // create a filename for new ship
    void load_new_ship_image(char *);             // trys to load a new image for the ship
    void load_next_predefined_ship(int);          // loads the next predefined ship (either next || prev)
    void load_next_ship_color();                  // sets the next color for predefined ships
    void delete_shipfile(int mode);               // deletes the ship
    void save_shipfile();                         // saves ship under current ship_filaname

  public:
    // constr. / destr.
    mShipBuilder();
    ~mShipBuilder();

    // functions
    void init(int, char *filename, int ret_to=-1); // initialize menu
    int  check();                                  // check menu buttons (1 on exit)
    int  handle_controls();                        // handles pushes etc.
    int  move();                                   // move menu
    void move_out();                               // move menu out
    void draw(BITMAP *);                           // draw menu
};

#endif

