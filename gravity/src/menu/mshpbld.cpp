/*
 *   GRAVITY STRIKE -- menu/mshpbld.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mshpbld.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsglob.h"
#include "../gserror.h"
#include "../gsplayer.h"
#include "../gsclear.h"
#include "../gsbullet.h"
#include "../gssound.h"
#include "../gsnetw.h"
#include "mstars.h"
#include "gsmd.h"
#include "gsmenu.h"

extern gsMenu *menu;
mShipBuilder  *this_menu;

enum ACTIVE_MSGBOXES {
  ACTIVE_MSG_SHIPLOADERROR = 1,
  ACTIVE_MSG_REALLYDELETE
};

// Simplified edit procedure for Allegro 5
int my_edit_proc(int msg, void *d, int c)
{
  // In Allegro 5, we don't need this complex edit procedure
  // Just return a simple value
  return 0;
}


mShipBuilder::mShipBuilder()
{
  active_button = M_SHPBLD_SHIPIMAGE;
  switch_to_menu = -1;
  no_input = 0;
  button_count = 75;
  moving = 0;
  strcpy(ship_filename, "");
  newship = 0;
  pl = NULL;
  strcpy(loadimage_path, "");
  sum = max_sum = 0;
  msgbox_active = 0;
  weapon_active = 0;
  esc_pressed = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
}

mShipBuilder::~mShipBuilder()
{
  globals->last_menu = MENU_SHIPBUILDER;

  if (pl)   delete(pl);

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mShipBuilder::create_user_ship_filename()
{
  int num = 0;
  char text[40];

  do
  {
    num++;
    sprintf(text, "%suser%03d.shp", globals->ship_dir, num);
  } while (exists(text));

  sprintf(text, "user%03d.shp", num);

  strcpy(ship_filename, text);
}

void mShipBuilder::init(int scatter, char *filename, int return_to)
{
  int group0_x, group0_y, group1_x, group1_y, group2_x, group2_y, group3_x, group3_y;
  int col = makecol8(58, 64, 191);

  if (return_to == -1)
    return_to_menu = MENU_CHOOSESHIP;
  else
    return_to_menu = return_to;

  strncpy(ship_filename, filename, 30);
  if (strcmp(strupr(filename), "CREATENEWSHIP") == 0)
  {
    create_user_ship_filename();
    newship = 1;
  }

  // create player structure
  if (!newship)
    pl = new gsPlayer(0, SHIP_PIRANHA, 0, ship_filename);
  else
    pl = new gsPlayer(0, SHIP_PIRANHA, 0, "#defnew_shp");

  button[M_SHPBLD_MSGBOX] = new mMessageBox();
  button[M_SHPBLD_MSGBOX]->create(640+320-80, 240-40, 640+320-80, 240-40, 160, 80);

  // 0th group: save/back/delete buttons
  group0_x = 0; group0_y = 0;

  button[M_SHPBLD_SAVE] = new mMainButton();
  button[M_SHPBLD_SAVE]->create(group0_x+10, group0_y+430,  group0_x+10, group0_y+500, 120, 30);
  button[M_SHPBLD_SAVE]->set_text(language->get_string(T_MENU_SAVE));

  button[M_SHPBLD_SAVENEW] = new mMainButton();
  button[M_SHPBLD_SAVENEW]->create(group0_x+135, group0_y+430, group0_x+135, group0_y+500, 120, 30);
  button[M_SHPBLD_SAVENEW]->set_text(language->get_string(T_MENU_SAVENEW));
  if (return_to_menu != MENU_CHOOSESHIP)
  {
    button[M_SHPBLD_SAVENEW]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_SAVENEW]->set_flag(MC_INVISIBLE);
  }

  button[M_SHPBLD_DELETE] = new mMainButton();
  button[M_SHPBLD_DELETE]->create(group0_x+260, group0_y+430, group0_x+260, group0_y+500, 120, 30);
  button[M_SHPBLD_DELETE]->set_text(language->get_string(T_MENU_DELETE));
  if (return_to_menu != MENU_CHOOSESHIP)
  {
    button[M_SHPBLD_DELETE]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_DELETE]->set_flag(MC_INVISIBLE);
  }

  button[M_SHPBLD_BACK] = new mMainButton();
  button[M_SHPBLD_BACK]->create(group0_x+510, group0_y+430, group0_x+510, group0_y+500, 120, 30);
  button[M_SHPBLD_BACK]->set_text(language->get_string(T_MENU_BACK));

  if (newship)
  {
    button[M_SHPBLD_SAVE]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_SAVE]->set_flag(MC_INVISIBLE);
    button[M_SHPBLD_DELETE]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_DELETE]->set_flag(MC_INVISIBLE);
  }
  else
  if (strstr(filename, "DEF1_SHP") || strstr(filename, "DEF2_SHP") || strstr(filename, "DEF3_SHP"))
  {
    button[M_SHPBLD_SAVE]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_SAVE]->set_flag(MC_INVISIBLE);
    button[M_SHPBLD_DELETE]->set_flag(MC_INACTIVE);
    button[M_SHPBLD_DELETE]->set_flag(MC_INVISIBLE);
  }

  button[M_SHPBLD_CAPTION] = new mNormalText();
  button[M_SHPBLD_CAPTION]->create(286, 64, 800, 64);
  button[M_SHPBLD_CAPTION]->set_int(FONT_BANK);
  button[M_SHPBLD_CAPTION]->set_text(language->get_string(T_MENU_CAP_SHIPBUILDER));
  button[M_SHPBLD_CAPTION]->set_int(globals->col_yellow);
  button[M_SHPBLD_CAPTION]->set_flag(MC_INACTIVE);

  // 1st group: general ship data
  group1_x = 0; group1_y = 0;

  button[M_SHPBLD_SHIPIMAGE] = new mShipIcon2();
  button[M_SHPBLD_SHIPIMAGE]->create(group1_x+10+16, group1_y+95, group1_x-100+15, group1_y+95, 100-16*2, 30);
  button[M_SHPBLD_SHIPIMAGE]->set_player(pl);

  button[M_SHPBLD_SHIPIMAGE_LEFT] = new mArrowButton();
  button[M_SHPBLD_SHIPIMAGE_LEFT]->create(group1_x+10, group1_y+95, group1_x-100, group1_y+95, 14, 29, A_LEFT);

  button[M_SHPBLD_SHIPIMAGE_RIGHT] = new mArrowButton();
  button[M_SHPBLD_SHIPIMAGE_RIGHT]->create(group1_x+10+100-14, group1_y+95, group1_x-14, group1_y+95, 14, 29, A_RIGHT);

  button[M_SHPBLD_NAME] = new mTextField();
  button[M_SHPBLD_NAME]->create(group1_x+125, group1_y+100, group1_x+640, group1_y+100, 250, 15, 85);
  button[M_SHPBLD_NAME]->set_int(30);
  button[M_SHPBLD_NAME]->set_text(language->get_string(T_MENU_SHIP_NAME));
  if (newship)
    button[M_SHPBLD_NAME]->set_text(language->get_string(T_MENU_NEWSHIP));
  else
    button[M_SHPBLD_NAME]->set_text(pl->get_ship_name());

  button[M_SHPBLD_DESC] = new mTextField();
  button[M_SHPBLD_DESC]->create(group1_x+125, group1_y+127, group1_x+640, group1_y+127, 570-65, 15, 85);
  button[M_SHPBLD_DESC]->set_int(150);
  button[M_SHPBLD_DESC]->set_text(language->get_string(T_MENU_SHIP_DESC));
  button[M_SHPBLD_DESC]->set_text(pl->get_ship_desc());

  button[M_SHPBLD_LOADIMAGE] = new mSmallButton();
  button[M_SHPBLD_LOADIMAGE]->create(group1_x+10, group1_y+127, group1_x-100, group1_y+127, 100, 15);
  button[M_SHPBLD_LOADIMAGE]->set_text(language->get_string(T_MENU_SHIP_LOADIMAGE));

  // 2nd group: ship characteristics
  group2_x = 0; group2_y = 0;
  int vspace = 25;

  button[M_SHPBLD_HULL] = new mTextSlider();
  button[M_SHPBLD_HULL]->set_int(1);
  button[M_SHPBLD_HULL]->create(group2_x+10, group2_y+vspace*0+180, group2_x-340, group2_y+vspace*0+180, 300, 14, 130);
  button[M_SHPBLD_HULL]->set_text(language->get_string(T_MENU_SHIP_HULL));

  button[M_SHPBLD_TURNSPEED] = new mTextSlider();
  button[M_SHPBLD_TURNSPEED]->set_int(1);
  button[M_SHPBLD_TURNSPEED]->create(group2_x+330, group2_y+vspace*0+180, group2_x+640, group2_y+vspace*0+180, 300, 14, 130);
  button[M_SHPBLD_TURNSPEED]->set_text(language->get_string(T_MENU_SHIP_TURN_SPEED));

  button[M_SHPBLD_SHIELD] = new mTextSlider();
  button[M_SHPBLD_SHIELD]->set_int(1);
  button[M_SHPBLD_SHIELD]->create(group2_x+10, group2_y+vspace*1+180, group2_x-340, group2_y+vspace*1+180, 300, 14, 130);
  button[M_SHPBLD_SHIELD]->set_text(language->get_string(T_MENU_SHIP_SHIELD));

  button[M_SHPBLD_MAXSPEED] = new mTextSlider();
  button[M_SHPBLD_MAXSPEED]->set_int(1);
  button[M_SHPBLD_MAXSPEED]->create(group2_x+330, group2_y+vspace*1+180, group2_x+640, group2_y+vspace*1+180, 300, 14, 130);
  button[M_SHPBLD_MAXSPEED]->set_text(language->get_string(T_MENU_SHIP_MAX_SPEED));

  button[M_SHPBLD_HULLRE] = new mTextSlider();
  button[M_SHPBLD_HULLRE]->set_int(1);
  button[M_SHPBLD_HULLRE]->create(group2_x+10, group2_y+vspace*2+180, group2_x-340, group2_y+vspace*2+180, 300, 14, 130);
  button[M_SHPBLD_HULLRE]->set_text(language->get_string(T_MENU_SHIP_HULL_RE));

  button[M_SHPBLD_ACCELERATION] = new mTextSlider();
  button[M_SHPBLD_ACCELERATION]->set_int(1);
  button[M_SHPBLD_ACCELERATION]->create(group2_x+330, group2_y+vspace*2+180, group2_x+640, group2_y+vspace*2+180, 300, 14, 130);
  button[M_SHPBLD_ACCELERATION]->set_text(language->get_string(T_MENU_SHIP_ACCELERATION));

  button[M_SHPBLD_SHIELDRE] = new mTextSlider();
  button[M_SHPBLD_SHIELDRE]->set_int(1);
  button[M_SHPBLD_SHIELDRE]->create(group2_x+10, group2_y+vspace*3+180, group2_x-340, group2_y+vspace*3+180, 300, 14, 130);
  button[M_SHPBLD_SHIELDRE]->set_text(language->get_string(T_MENU_SHIP_SHIELD_RE));

  button[M_SHPBLD_FUELCONS] = new mTextSlider();
  button[M_SHPBLD_FUELCONS]->set_int(1);
  button[M_SHPBLD_FUELCONS]->create(group2_x+330, group2_y+vspace*3+180, group2_x+640, group2_y+vspace*3+180, 300, 14, 130);
  button[M_SHPBLD_FUELCONS]->set_text(language->get_string(T_MENU_SHIP_FUEL_CONSUMPTION));

  button[M_SHPBLD_WEAPONRE] = new mTextSlider();
  button[M_SHPBLD_WEAPONRE]->set_int(1);
  button[M_SHPBLD_WEAPONRE]->create(group2_x+10, group2_y+vspace*4+180, group2_x-340, group2_y+vspace*4+180, 300, 14, 130);
  button[M_SHPBLD_WEAPONRE]->set_text(language->get_string(T_MENU_SHIP_WEAPON_RE));

  button[M_SHPBLD_WEAPONSTOR] = new mTextSlider();
  button[M_SHPBLD_WEAPONSTOR]->set_int(1);
  button[M_SHPBLD_WEAPONSTOR]->create(group2_x+330, group2_y+vspace*4+180, group2_x+640, group2_y+vspace*4+180, 300, 14, 130);
  button[M_SHPBLD_WEAPONSTOR]->set_text(language->get_string(T_MENU_SHIP_WEAPON_STORAGE));

  button[M_SHPBLD_RADARZOOM] = new mTextSlider();
  button[M_SHPBLD_RADARZOOM]->set_int(1);
  button[M_SHPBLD_RADARZOOM]->create(group2_x+10, group2_y+vspace*5+180, group2_x-340, group2_y+vspace*5+180, 300, 14, 130);
  button[M_SHPBLD_RADARZOOM]->set_text(language->get_string(T_MENU_SHIP_RADAR_ZOOM));

  button[M_SHPBLD_WEAPONSLOTS] = new mTextSlider();
  button[M_SHPBLD_WEAPONSLOTS]->set_int(1);
  button[M_SHPBLD_WEAPONSLOTS]->create(group2_x+330, group2_y+vspace*5+180, group2_x+640, group2_y+vspace*5+180, 300, 14, 130);
  button[M_SHPBLD_WEAPONSLOTS]->set_text(language->get_string(T_MENU_SHIP_WEAPON_SLOTS));

  button[M_SHPBLD_WALLSHIELD] = new mTextSlider();
  button[M_SHPBLD_WALLSHIELD]->set_int(1);
  button[M_SHPBLD_WALLSHIELD]->create(group2_x+10, group2_y+vspace*6+180, group2_x-340, group2_y+vspace*6+180, 300, 14, 130);
  button[M_SHPBLD_WALLSHIELD]->set_text(language->get_string(T_MENU_SHIP_WALLSHIELD));

  button[M_SHPBLD_CAP_EXTRA] = new mNormalText();
  button[M_SHPBLD_CAP_EXTRA]->create(group2_x+10, group2_y+vspace*5+235, 0, 0);
  button[M_SHPBLD_CAP_EXTRA]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_EXTRA]->set_text(language->get_string(T_MENU_SHIP_EXTRA));
  button[M_SHPBLD_CAP_EXTRA]->set_int(col);
  button[M_SHPBLD_CAP_EXTRA]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_EXTRA1] = new mCheckBox();
  button[M_SHPBLD_EXTRA1]->create(group2_x+140, group2_y+vspace*5+235, group2_x+640, group2_y+vspace*5+235, 20, 15);
  button[M_SHPBLD_EXTRA1]->set_text(language->get_string(T_MENU_SHIP_AFTERBURNER));

  button[M_SHPBLD_EXTRA2] = new mCheckBox();
  button[M_SHPBLD_EXTRA2]->create(group2_x+260, group2_y+vspace*5+235, group2_x+640, group2_y+vspace*5+235, 20, 15);
  button[M_SHPBLD_EXTRA2]->set_text(language->get_string(T_MENU_SHIP_CLOAK));
//  button[M_SHPBLD_EXTRA2]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_SHPBLD_EXTRA3] = new mCheckBox();
  button[M_SHPBLD_EXTRA3]->create(group2_x+380, group2_y+vspace*5+235, group2_x+640, group2_y+vspace*5+235, 20, 15);
  button[M_SHPBLD_EXTRA3]->set_text(language->get_string(T_MENU_SHIP_DEFLECTOR));

  // set extra slot to current value

  int n, c;
/*
  for (n=0; n < 5; n++)
  {
    extraslot tmp_eslot = pl->get_extra_slot(n);

    if (tmp_eslot.type == EXTRA_AFTERBURNER) button[M_SHPBLD_EXTRA1]->set_flag(MC_PUSHED);
    else if (tmp_eslot.type == EXTRA_CLOAK) button[M_SHPBLD_EXTRA2]->set_flag(MC_PUSHED);
//    else if (tmp_eslot.type == EXTRA_CHASEBLOCKER) button[M_SHPBLD_EXTRA3]->set_flags(MC_PUSHED);
  }
  */

  if (pl->get_extra() == EXTRA_AFTERBURNER) button[M_SHPBLD_EXTRA1]->set_flag(MC_PUSHED);
  else if (pl->get_extra() == EXTRA_CLOAK) button[M_SHPBLD_EXTRA2]->set_flag(MC_PUSHED);
  else if (pl->get_extra() == EXTRA_DEFLECTOR) button[M_SHPBLD_EXTRA3]->set_flag(MC_PUSHED);

  button[M_SHPBLD_REMPOINTS] = new mTextSlider();
  button[M_SHPBLD_REMPOINTS]->set_int(0);  // create without arrows
  button[M_SHPBLD_REMPOINTS]->create(group2_x+10, group2_y+vspace*6+240, group2_x+640, group2_y+vspace*6+240, 620, 14, 130);
  button[M_SHPBLD_REMPOINTS]->set_text(language->get_string(T_MENU_SHIP_REMPOINTS));

  button[M_SHPBLD_EDITWEAPONS] = new mMainButton();
  button[M_SHPBLD_EDITWEAPONS]->create(group2_x+385, group2_y+430,  group2_x+385, group2_y+500, 120, 30);
  button[M_SHPBLD_EDITWEAPONS]->set_text(language->get_string(T_MENU_EDITWEAPONS));
  button[M_SHPBLD_EDITWEAPONS]->set_color1(globals->col_yellow);

  // 3rd group: ship weapons
  group3_x = 640; group3_y = -15;

  button[M_SHPBLD_EDITSHIP] = new mMainButton();
  button[M_SHPBLD_EDITSHIP]->create(group3_x+385, group3_y+430+15,  group3_x+385, group3_y+500, 120, 30);
  button[M_SHPBLD_EDITSHIP]->set_text(language->get_string(T_MENU_EDITSHIP));
  button[M_SHPBLD_EDITSHIP]->set_color1(globals->col_yellow);

  // available weapons
  int yp = 0, xp = 0; n = 0;
  int xw = 75;
  int yw = 30;

  for (c=0; c < WEAPON_COUNT; c++)
  {
    button[M_SHPBLD_WEAPON1+c] = new mWeaponIcon();
    
    button[M_SHPBLD_WEAPON1+c]->create(group3_x+150+xw*n+1, group3_y+60+120+1+yp*yw, 0, 0, 58, 19, W_SINGLE+c);
    button[M_SHPBLD_WEAPON1_OSLOT+c] = new mBlank();
    button[M_SHPBLD_WEAPON1_OSLOT+c]->create(group3_x+150+xw*n, group3_y+60+120+yp*yw, 0, 0, 58, 19, 0);
    
    if (++n > 5) { n = 0; yp++; }
  }

  // ship's weapon slots
  c = 0; xp = 0;
  for (n=0; n < 6; n++)
  {
    button[M_SHPBLD_WEAPON1_SLOT+n] = new mBlank();
    button[M_SHPBLD_WEAPON1_SLOT+n]->create(group3_x+151 + xp*160, group3_y+55+220+c*25, 0, 0, 50+59, 20, 50);
    button[M_SHPBLD_WEAPON1_SLOT+n]->set_text(language->get_string(TEXT_STRINGS(T_MENU_SHIP_SLOT1+n)));
    if (++c > 1) { c = 0; xp++; }
  }

  button[M_SHPBLD_WEAPON_DESC] = new mBlank();
  button[M_SHPBLD_WEAPON_DESC]->create(group3_x+151, group3_y+390, 0, 0, 433, 40, 0);
  button[M_SHPBLD_WEAPON_DESC]->set_flag(MC_INVISIBLE);

  // captions
  button[M_SHPBLD_CAP_WEAPAVAIL] = new mNormalText();
  button[M_SHPBLD_CAP_WEAPAVAIL]->create(group3_x+10, group3_y+179, 0, 0);
  button[M_SHPBLD_CAP_WEAPAVAIL]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_WEAPAVAIL]->set_text(language->get_string(T_MENU_SHIP_TEXT_WEAPAVAIL));
  button[M_SHPBLD_CAP_WEAPAVAIL]->set_int(col);
  button[M_SHPBLD_CAP_WEAPAVAIL]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_WEAPSLOTS] = new mNormalText();
  button[M_SHPBLD_CAP_WEAPSLOTS]->create(group3_x+10, group3_y+275, 0, 0);
  button[M_SHPBLD_CAP_WEAPSLOTS]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_WEAPSLOTS]->set_text(language->get_string(T_MENU_SHIP_TEXT_WEAPSLOTS));
  button[M_SHPBLD_CAP_WEAPSLOTS]->set_int(col);
  button[M_SHPBLD_CAP_WEAPSLOTS]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_WEAPON_NAME] = new mNormalText();
  button[M_SHPBLD_CAP_WEAPON_NAME]->create(group3_x+10, group3_y+330, 0, 0);
  button[M_SHPBLD_CAP_WEAPON_NAME]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_WEAPON_NAME]->set_text(language->get_string(T_MENU_SHIP_NAME));
  button[M_SHPBLD_CAP_WEAPON_NAME]->set_int(col);
  button[M_SHPBLD_CAP_WEAPON_NAME]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_DAMAGE] = new mNormalText();
  button[M_SHPBLD_CAP_DAMAGE]->create(group3_x+10, group3_y+360, 0, 0);
  button[M_SHPBLD_CAP_DAMAGE]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_DAMAGE]->set_text(language->get_string(T_MENU_SHIP_DAMAGE_MP));
  button[M_SHPBLD_CAP_DAMAGE]->set_int(col);
  button[M_SHPBLD_CAP_DAMAGE]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_DAMAGE_SHIELD] = new mNormalText();
  button[M_SHPBLD_CAP_DAMAGE_SHIELD]->create(group3_x+151, group3_y+360, 0, 0);
  button[M_SHPBLD_CAP_DAMAGE_SHIELD]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_DAMAGE_SHIELD]->set_text(language->get_string(T_MENU_SHIP_SHIELD));
  button[M_SHPBLD_CAP_DAMAGE_SHIELD]->set_int(col);
  button[M_SHPBLD_CAP_DAMAGE_SHIELD]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_DAMAGE_HULL] = new mNormalText();
  button[M_SHPBLD_CAP_DAMAGE_HULL]->create(group3_x+151+130, group3_y+360, 0, 0);
  button[M_SHPBLD_CAP_DAMAGE_HULL]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_DAMAGE_HULL]->set_text(language->get_string(T_MENU_SHIP_HULL));
  button[M_SHPBLD_CAP_DAMAGE_HULL]->set_int(col);
  button[M_SHPBLD_CAP_DAMAGE_HULL]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_DAMAGE_SP] = new mNormalText();
  button[M_SHPBLD_CAP_DAMAGE_SP]->create(group3_x+151+250, group3_y+360, 0, 0);
  button[M_SHPBLD_CAP_DAMAGE_SP]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_DAMAGE_SP]->set_text(language->get_string(T_MENU_SHIP_DAMAGE_SP));
  button[M_SHPBLD_CAP_DAMAGE_SP]->set_int(col);
  button[M_SHPBLD_CAP_DAMAGE_SP]->set_flag(MC_INACTIVE);

  button[M_SHPBLD_CAP_WEAPDESC] = new mNormalText();
  button[M_SHPBLD_CAP_WEAPDESC]->create(group3_x+10, group3_y+390, 0, 0);
  button[M_SHPBLD_CAP_WEAPDESC]->set_int(FONT_IMPACT10);
  button[M_SHPBLD_CAP_WEAPDESC]->set_text(language->get_string(T_MENU_SHIP_TEXT_WEAPDESC));
  button[M_SHPBLD_CAP_WEAPDESC]->set_int(col);
  button[M_SHPBLD_CAP_WEAPDESC]->set_flag(MC_INACTIVE);


  // set weaponslots with correct weaps
  int num = pl->get_percent_value(PV_WEAPON_SLOTS);
  for (n=0; n < num; n++)
  {
    weaponslot *tmp_slot = pl->get_weapon_slot(n);

    if (tmp_slot->type != W_NONE)
    {
      button[M_SHPBLD_WEAPON1 + tmp_slot->type]->set_x( button[M_SHPBLD_WEAPON1_SLOT+n]->get_x() + button[M_SHPBLD_WEAPON1_SLOT+n]->get_int() + 1);
      button[M_SHPBLD_WEAPON1 + tmp_slot->type]->set_y( button[M_SHPBLD_WEAPON1_SLOT+n]->get_y() + 1 );
      button[M_SHPBLD_WEAPON1 + tmp_slot->type]->set_ox( button[M_SHPBLD_WEAPON1_SLOT+n]->get_x() + button[M_SHPBLD_WEAPON1_SLOT+n]->get_int() + 1);
      button[M_SHPBLD_WEAPON1 + tmp_slot->type]->set_oy( button[M_SHPBLD_WEAPON1_SLOT+n]->get_y() + 1 );
      button[M_SHPBLD_WEAPON1 + tmp_slot->type]->set_int(n);
    }
  } // for weaponslots


  // count percents
  count_ship_sum();
  update_weaponslots();

//  errors->log(2, "SUM", "of ship", sum);
}

int mShipBuilder::count_ship_sum()
{
  sum = 0;
  max_sum = 500;

  sum += pl->get_percent_value(PV_SHIELD) * 8;
  sum += pl->get_percent_value(PV_HULL) * 4;
  sum += pl->get_percent_value(PV_WEAPON_SLOTS) * 5;
  sum += pl->get_percent_value(PV_HULL_RE);
  sum += pl->get_percent_value(PV_SHIELD_RE);
  sum += pl->get_percent_value(PV_WALLSHIELD);
  sum += pl->get_percent_value(PV_WEAPON_RE);
  sum += pl->get_percent_value(PV_RADAR_ZOOM);
  sum += pl->get_percent_value(PV_TURN_SPEED);
  sum += pl->get_percent_value(PV_MAX_SPEED);
  sum += pl->get_percent_value(PV_ACCELERATION);
  sum += pl->get_percent_value(PV_FUEL_CONS);
  sum += pl->get_percent_value(PV_WEAPON_STORAGE);

  if (button[M_SHPBLD_EXTRA1]->get_flags() & MC_PUSHED) sum += 30;
  if (button[M_SHPBLD_EXTRA2]->get_flags() & MC_PUSHED) sum += 30;
  if (button[M_SHPBLD_EXTRA3]->get_flags() & MC_PUSHED) sum += 30;

  if (sum > max_sum) return 1;
  return 0;
}

void mShipBuilder::update_weaponslots()
{
  int num = pl->get_percent_value(PV_WEAPON_SLOTS);

  // deactivate all slots
  int n;
  for (n=0; n < 6; n++)
  {
    button[M_SHPBLD_WEAPON1_SLOT+n]->set_flag(MC_INVISIBLE);
    button[M_SHPBLD_WEAPON1_SLOT+n]->set_flag(MC_INACTIVE);
  }

  // activate first "num" slots
  for (n=0; n < num; n++)
  {
    button[M_SHPBLD_WEAPON1_SLOT+n]->del_flag(MC_INVISIBLE);
    button[M_SHPBLD_WEAPON1_SLOT+n]->del_flag(MC_INACTIVE);
  }

  // delete weapons in inactive slots
  for (n=num; n < 6; n++)
    pl->set_weapon(n, W_NONE);

  for (n=0; n < WEAPON_COUNT; n++)
  {
    int slot = button[M_SHPBLD_WEAPON1+n]->get_int();

    if (slot >= num)
    {
      button[M_SHPBLD_WEAPON1+n]->set_int(-1);

      button[M_SHPBLD_WEAPON1+n]->set_x( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_x() );
      button[M_SHPBLD_WEAPON1+n]->set_y( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_y() );
      button[M_SHPBLD_WEAPON1+n]->set_ox( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_x() );
      button[M_SHPBLD_WEAPON1+n]->set_oy( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_y() );
    }

  }

}

int mShipBuilder::check()
{
  // network messages
  net->process_packets();

  if (net->is_server() && globals->game_time % 120 == 0)
    net->menu_server_update_clients();

  if (net->is_client() && globals->game_time % 60 == 0)
    net->menu_client_update_server();

  int n;
  if (!msgbox_active)
  {
    for (n=0; n < button_count; n++)
    {
      if (!moving)
      {
        if (weapon_active <= 0 ||
            (n >= M_SHPBLD_WEAPON1_OSLOT && n <= M_SHPBLD_WEAPON6_SLOT) )
        {
          button[n]->check();
          if (button[n]->get_flags() & MC_ACTIVE)
            if (active_button != n)
            {
              active_button = n;
              if (n < M_SHPBLD_WEAPON1 || n > M_SHPBLD_WEAPON6_SLOT)
                gsound->menu_change();
            }
        }
      }
    }
  }
  else
    button[M_SHPBLD_MSGBOX]->check();

//  for (n=0; n < button_count; n++)
//    if (n != active_button && button[n]->get_flags() & MC_ACTIVE) button[n]->del_flag(MC_ACTIVE);

  if (active_button >= 0)
    button[active_button]->set_flag(MC_ACTIVE);

  if (!no_input && !msgbox_active && weapon_active < 0)
  {
    if (key[KEY_UP])
    {
      do
      {
        if (active_button <= 0) active_button = button_count-1;
        else if (active_button > 0) active_button--;
      } while (button[active_button]->get_flags() & MC_INACTIVE);
      gsound->menu_change();
      clear_key(KEY_UP);
    }
    else if (key[KEY_DOWN])
    {
      do
      {
        if (active_button >= button_count-1) active_button = 1;
        else if (active_button < button_count-1) active_button++;
      }
      while (button[active_button]->get_flags() & MC_INACTIVE);
      gsound->menu_change();
      clear_key(KEY_DOWN);
    }
    else if (key[KEY_ENTER])
    {
      if (active_button >= 0)
        button[active_button]->set_flag(MC_PUSHED);
      clear_key(KEY_ENTER);
    }
  } // if !no_input

  return(handle_controls());
}

int mShipBuilder::handle_controls()
{
  if (weapon_active <= 0)
  {
    // back
    if ((key[KEY_ESC] && esc_pressed == 0) || button[M_SHPBLD_BACK]->get_flags() & MC_PUSHED)
    {
      esc_pressed = 1;
      gsound->menu_select();
      switch_to_menu = return_to_menu;
      button[M_SHPBLD_BACK]->set_flag(MC_INVISIBLE);
      button[M_SHPBLD_BACK]->set_flag(MC_INACTIVE);
      button[M_SHPBLD_BACK]->del_flag(MC_PUSHED);
      button[M_SHPBLD_BACK]->do_explosion();
      move_out();
    }
  
    // edit weapons
    if (button[M_SHPBLD_EDITWEAPONS]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_EDITWEAPONS]->del_flag(MC_PUSHED);
  
      for (int a=M_SHPBLD_HULL; a <= M_SHPBLD_CAP_DAMAGE_SP; a++)
      {
        button[a]->set_dest_x( button[a]->get_ox() - 640 );
      }
      button[M_SHPBLD_EDITWEAPONS]->set_dest_y( button[M_SHPBLD_EDITWEAPONS]->get_oy() + 100 );
      button[M_SHPBLD_EDITSHIP]->set_dest_y( button[M_SHPBLD_EDITSHIP]->get_oy() );
    }
  
    // edit ship
    if (button[M_SHPBLD_EDITSHIP]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_EDITSHIP]->del_flag(MC_PUSHED);
  
      for (int a=M_SHPBLD_HULL; a <= M_SHPBLD_CAP_DAMAGE_SP; a++)
      {
        button[a]->set_dest_x( button[a]->get_ox() );
      }
      button[M_SHPBLD_EDITWEAPONS]->set_dest_y( button[M_SHPBLD_EDITWEAPONS]->get_oy() );
      button[M_SHPBLD_EDITSHIP]->set_dest_y( button[M_SHPBLD_EDITSHIP]->get_oy() + 100);
    }

    // change image (left)
    if (button[M_SHPBLD_SHIPIMAGE_LEFT]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_SHIPIMAGE_LEFT]->del_flag(MC_PUSHED);
      load_next_predefined_ship(0);
      mouse_b = 0;
    }

    // change image (right)
    if (button[M_SHPBLD_SHIPIMAGE_RIGHT]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_SHIPIMAGE_RIGHT]->del_flag(MC_PUSHED);
      load_next_predefined_ship(1);
      mouse_b = 0;
    }

    // change ship color
    if (button[M_SHPBLD_SHIPIMAGE]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_SHIPIMAGE]->del_flag(MC_PUSHED);
      load_next_ship_color();
      mouse_b = 0;
    }

    // load image
    if (button[M_SHPBLD_LOADIMAGE]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      this_menu = this;
      int gtime = globals->game_time;
      // Use standard file dialog instead of Allegro GUI functions
      // For now, just log that this feature is not implemented
      errors->log(1, "shipbuilder", "file selection not implemented in Allegro 5");
      // You would implement a proper file dialog here using Allegro 5's file chooser functions
      globals->game_time = gtime;
      button[M_SHPBLD_LOADIMAGE]->del_flag(MC_PUSHED);
    }
    // check if there's a change in the path var
    if (strlen(loadimage_path) > 0)
    {
      load_new_ship_image(loadimage_path);
      strcpy(loadimage_path, "");
    }

    // message box: ship load error
    if (msgbox_active == ACTIVE_MSG_SHIPLOADERROR)
    {
      if (button[M_SHPBLD_MSGBOX]->get_sbutton(1)->get_flags() & MC_PUSHED)
      {
        gsound->menu_select();
        msgbox_active = 0;
        button[M_SHPBLD_MSGBOX]->get_sbutton(1)->del_flag(MC_PUSHED);
        button[M_SHPBLD_MSGBOX]->set_dest_x(button[M_SHPBLD_MSGBOX]->get_ox());
      }
    }

    // delete ship
    if (button[M_SHPBLD_DELETE]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[M_SHPBLD_DELETE]->del_flag(MC_PUSHED);
      delete_shipfile(0);
    }

    // message box: really delete?
    if (msgbox_active == ACTIVE_MSG_REALLYDELETE)
    {
      // if no -> do nothing
      if (button[M_SHPBLD_MSGBOX]->get_sbutton(2)->get_flags() & MC_PUSHED)
      {
        gsound->menu_select();
        msgbox_active = 0;
        button[M_SHPBLD_MSGBOX]->get_sbutton(2)->del_flag(MC_PUSHED);
        button[M_SHPBLD_MSGBOX]->set_dest_x(button[M_SHPBLD_MSGBOX]->get_ox());
      }
      // if yes -> delete ship, return to previous menu
      else if (button[M_SHPBLD_MSGBOX]->get_sbutton(0)->get_flags() & MC_PUSHED)
      {
        gsound->menu_select();
        msgbox_active = 0;
        button[M_SHPBLD_MSGBOX]->get_sbutton(0)->del_flag(MC_PUSHED);
//        button[M_SHPBLD_MSGBOX]->set_dest_x(button[M_SHPBLD_MSGBOX]->get_ox());

        delete_shipfile(1);

        // switch menu
        switch_to_menu = MENU_CHOOSESHIP;
        move_out();
      }
    }

    // save ship
    if (button[M_SHPBLD_SAVE]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      save_shipfile();

      switch_to_menu = return_to_menu;
      button[M_SHPBLD_SAVE]->set_flag(MC_INVISIBLE);
      button[M_SHPBLD_SAVE]->set_flag(MC_INACTIVE);
      button[M_SHPBLD_SAVE]->del_flag(MC_PUSHED);
      button[M_SHPBLD_SAVE]->do_explosion();
      move_out();
    }

    // save ship with new name
    if (button[M_SHPBLD_SAVENEW]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      create_user_ship_filename();
      save_shipfile();

      switch_to_menu = return_to_menu;
      button[M_SHPBLD_SAVENEW]->set_flag(MC_INVISIBLE);
      button[M_SHPBLD_SAVENEW]->set_flag(MC_INACTIVE);
      button[M_SHPBLD_SAVENEW]->del_flag(MC_PUSHED);
      button[M_SHPBLD_SAVENEW]->do_explosion();
      move_out();
    }
    
    // energy controls
    int i; int num;
    int ts = int(100 - ((float(sum) / float(max_sum))*100.0));

    if (ts == 0 && sum < max_sum) ts = 1;
    button[M_SHPBLD_REMPOINTS]->get_bar()->set_int(ts);
  
    num = PV_HULL;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_HULL]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-4 >= 0) { i--; sum -= 4; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    if (button[M_SHPBLD_HULL]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 20) { if (sum+4 <= max_sum) { i++; sum += 4; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    button[M_SHPBLD_HULL]->get_bar()->set_int(i*5);
  
    num = PV_SHIELD;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_SHIELD]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-8 >= 0) { i--; sum -= 8; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    if (button[M_SHPBLD_SHIELD]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 20) { if (sum+8 <= max_sum) { i++; sum += 8; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    button[M_SHPBLD_SHIELD]->get_bar()->set_int(i*5);
  
    num = PV_HULL_RE;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_HULLRE]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_HULLRE]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_HULLRE]->get_bar()->set_int(i);
  
    num = PV_SHIELD_RE;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_SHIELDRE]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_SHIELDRE]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_SHIELDRE]->get_bar()->set_int(i);

    num = PV_WALLSHIELD;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_WALLSHIELD]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_WALLSHIELD]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_WALLSHIELD]->get_bar()->set_int(i);
    
    num = PV_WEAPON_RE;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_WEAPONRE]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_WEAPONRE]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_WEAPONRE]->get_bar()->set_int(i);
  
    num = PV_RADAR_ZOOM;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_RADARZOOM]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum -= 1; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    if (button[M_SHPBLD_RADARZOOM]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 20) { if (sum+1 <= max_sum) { i++; sum += 1; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); } }
    button[M_SHPBLD_RADARZOOM]->get_bar()->set_int(i*5);

    num = PV_WEAPON_SLOTS;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_WEAPONSLOTS]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-5 >= 0) { i--; sum -= 5; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); update_weaponslots(); } }
    if (button[M_SHPBLD_WEAPONSLOTS]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 6) { if (sum+5 <= max_sum) { i++; sum += 5; pl->set_percent_value(num, i); mouse_b = 0; gsound->menu_changebar(); update_weaponslots(); } }
    button[M_SHPBLD_WEAPONSLOTS]->get_bar()->set_int(i*16.67);
    
    num = PV_TURN_SPEED;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_TURNSPEED]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_TURNSPEED]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_TURNSPEED]->get_bar()->set_int(i);
  
    num = PV_MAX_SPEED;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_MAXSPEED]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_MAXSPEED]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_MAXSPEED]->get_bar()->set_int(i);
  
    num = PV_ACCELERATION;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_ACCELERATION]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_ACCELERATION]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_ACCELERATION]->get_bar()->set_int(i);
  
    num = PV_FUEL_CONS;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_FUELCONS]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_FUELCONS]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_FUELCONS]->get_bar()->set_int(i);
  
    num = PV_WEAPON_STORAGE;
    i = pl->get_percent_value(num);
    if (button[M_SHPBLD_WEAPONSTOR]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { if (sum-1 >= 0) { i--; sum--; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    if (button[M_SHPBLD_WEAPONSTOR]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100) { if (sum+1 <= max_sum) { i++; sum++; pl->set_percent_value(num, i); gsound->menu_changebar(); } }
    button[M_SHPBLD_WEAPONSTOR]->get_bar()->set_int(i);

    // extras
    if (button[M_SHPBLD_EXTRA1]->get_flags() & MC_ACTIVE)
    {
      if (button[M_SHPBLD_EXTRA1]->get_flags() & MC_PUSHED)
      {
        if (pl->get_extra() != EXTRA_AFTERBURNER)
        {
          pl->set_extra(EXTRA_AFTERBURNER);
          button[M_SHPBLD_EXTRA2]->del_flag(MC_PUSHED);
          button[M_SHPBLD_EXTRA3]->del_flag(MC_PUSHED);
          if (count_ship_sum())
          {
            gsound->menu_nono();
            pl->set_extra(EXTRA_NONE);
            button[M_SHPBLD_EXTRA1]->del_flag(MC_PUSHED);
            count_ship_sum();
          }
          else
          {
            gsound->menu_select();
          }
        }
      }
      else
      {
        if (pl->get_extra() == EXTRA_AFTERBURNER)
        {
          gsound->menu_select();
          pl->set_extra(EXTRA_NONE);
          button[M_SHPBLD_EXTRA1]->del_flag(MC_PUSHED);
          count_ship_sum();
        }
      }
    }

    if (button[M_SHPBLD_EXTRA2]->get_flags() & MC_ACTIVE)
    {
      if (button[M_SHPBLD_EXTRA2]->get_flags() & MC_PUSHED)
      {
        if (pl->get_extra() != EXTRA_CLOAK)
        {
          pl->set_extra(EXTRA_CLOAK);
          button[M_SHPBLD_EXTRA1]->del_flag(MC_PUSHED);
          button[M_SHPBLD_EXTRA3]->del_flag(MC_PUSHED);
          if (count_ship_sum())
          {
            gsound->menu_nono();
            pl->set_extra(EXTRA_NONE);
            button[M_SHPBLD_EXTRA2]->del_flag(MC_PUSHED);
            count_ship_sum();
          }
          else
          {
            gsound->menu_select();
          }
        }
      }
      else
      {
        if (pl->get_extra() == EXTRA_CLOAK)
        {
          gsound->menu_select();
          pl->set_extra(EXTRA_NONE);
          button[M_SHPBLD_EXTRA2]->del_flag(MC_PUSHED);
          count_ship_sum();
        }
      }
    }

    if (button[M_SHPBLD_EXTRA3]->get_flags() & MC_ACTIVE)
    {
      if (button[M_SHPBLD_EXTRA3]->get_flags() & MC_PUSHED)
      {
        if (pl->get_extra() != EXTRA_DEFLECTOR)
        {
          pl->set_extra(EXTRA_DEFLECTOR);
          button[M_SHPBLD_EXTRA1]->del_flag(MC_PUSHED);
          button[M_SHPBLD_EXTRA2]->del_flag(MC_PUSHED);
          if (count_ship_sum())
          {
            gsound->menu_nono();
            pl->set_extra(EXTRA_NONE);
            button[M_SHPBLD_EXTRA3]->del_flag(MC_PUSHED);
            count_ship_sum();
          }
          else
          {
            gsound->menu_select();
          }
        }
      }
      else
      {
        if (pl->get_extra() == EXTRA_DEFLECTOR)
        {
          gsound->menu_select();
          pl->set_extra(EXTRA_NONE);
          button[M_SHPBLD_EXTRA3]->del_flag(MC_PUSHED);
          count_ship_sum();
        }
      }
    }

  } // ship controls

  // weapon controls
  for (int n=0; n < WEAPON_COUNT; n++)
  {
    if ((weapon_active <= 0 || weapon_active == M_SHPBLD_WEAPON1+n) &&
         button[M_SHPBLD_WEAPON1+n]->get_flags() & MC_PUSHED)
    {
      if (mouse_b & 1)
      {
        button[M_SHPBLD_WEAPON1+n]->set_x( mouse_x );
        button[M_SHPBLD_WEAPON1+n]->set_y( mouse_y );
        weapon_active = M_SHPBLD_WEAPON1+n;
      }
      else
      {
        button[M_SHPBLD_WEAPON1+n]->del_flag(MC_PUSHED);
        button[M_SHPBLD_WEAPON1+n]->set_dest_x( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_x() );
        button[M_SHPBLD_WEAPON1+n]->set_dest_y( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_y() );
        button[M_SHPBLD_WEAPON1+n]->set_ox( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_x() + 640);
        button[M_SHPBLD_WEAPON1+n]->set_oy( button[M_SHPBLD_WEAPON1_OSLOT+n]->get_y() + 0);
        int slot = button[M_SHPBLD_WEAPON1+n]->get_int();
        if (slot >= 0) pl->set_weapon(slot, W_NONE);
        button[M_SHPBLD_WEAPON1+n]->set_int(-1);

        for (int sn=0; sn < 6; sn++)
        {
          weaponslot *tmp_slot = pl->get_weapon_slot(sn);

          if (tmp_slot->type == W_NONE &&
              button[M_SHPBLD_WEAPON1_SLOT+sn]->get_flags() & MC_ACTIVE)
          {
            gsound->menu_select();
            button[M_SHPBLD_WEAPON1_SLOT+sn]->del_flag(MC_PUSHED);
            button[M_SHPBLD_WEAPON1+n]->set_dest_x( button[M_SHPBLD_WEAPON1_SLOT+sn]->get_x() + button[M_SHPBLD_WEAPON1_SLOT+sn]->get_int() + 1);
            button[M_SHPBLD_WEAPON1+n]->set_dest_y( button[M_SHPBLD_WEAPON1_SLOT+sn]->get_y() + 1);
            button[M_SHPBLD_WEAPON1+n]->set_ox( button[M_SHPBLD_WEAPON1_SLOT+sn]->get_x() + button[M_SHPBLD_WEAPON1_SLOT+sn]->get_int() + 640 + 1);
            button[M_SHPBLD_WEAPON1+n]->set_oy( button[M_SHPBLD_WEAPON1_SLOT+sn]->get_y() + 0);
            pl->set_weapon(sn, WEAPON_TYPE(W_SINGLE+n));
            button[M_SHPBLD_WEAPON1+n]->set_int(sn);
          }
        } // for slots

        weapon_active = -1;
      }
    }
  } // for weapons

  if (move() == 0 && moving) return switch_to_menu;

  return 0;
}

void mShipBuilder::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }

/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "sum", sum), 10, 400, makecol8(255,255,255));
//  textout(globals->vscreen, font, ftoa(tmpstr, "switch", switch_to_menu), 10, 410, makecol8(255,255,255));
*/
}

int mShipBuilder::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mShipBuilder::move_out()
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_SHPBLD_CAPTION]->set_dest_y( button[M_SHPBLD_CAPTION]->get_y() );
  button[M_SHPBLD_CAPTION]->set_dest_x( button[M_SHPBLD_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

void mShipBuilder::load_new_ship_image(char *f)
{
  int error = 0;

  if (strlen(get_filename(f)) > 0)
    error = pl->load_ship_image(f, 0);

  if (error)
  {
    // draw "there was an error" box
    errors->log(0, "error importing ship", f);

    button[M_SHPBLD_MSGBOX]->set_text(language->get_string(T_MENU_MSG_SHIPLOADERROR));
    button[M_SHPBLD_MSGBOX]->set_int(MSGBOX_OK);
    button[M_SHPBLD_MSGBOX]->set_dest_x( button[M_SHPBLD_MSGBOX]->get_ox() - 640 );

    msgbox_active = ACTIVE_MSG_SHIPLOADERROR;
  }
  else
  {
    errors->log(2, "ship image imported", f);
    strcpy(loaded_image_path, f);
  }
}

void mShipBuilder::load_next_predefined_ship(int dir)
{
  int cur_ship = pl->get_ship_type();
  int cur_col = pl->get_ship_color();

  if (dir == 0) cur_ship--; else cur_ship++;

  if (cur_ship < 0) cur_ship = 2;
  else if (cur_ship > 2) cur_ship = 0;

  if (cur_col < 0) cur_col = 0;

  pl->load_ship_image((predefined_ships)cur_ship, cur_col, 0);
}

void mShipBuilder::load_next_ship_color()
{
  int ship_type = pl->get_ship_type();

  if (ship_type != SHIP_CUSTOM)
  {
    int cur_color = pl->get_ship_color();

    if (++cur_color > 4) cur_color = 0;

    pl->load_ship_image((predefined_ships)ship_type, cur_color, 0);
  }
}

void mShipBuilder::delete_shipfile(int mode)
{
  if (mode == 0)
  {
    button[M_SHPBLD_MSGBOX]->set_text(language->get_string(T_MENU_MSG_REALLYDELETE));
    button[M_SHPBLD_MSGBOX]->set_int(MSGBOX_YESNO);
    button[M_SHPBLD_MSGBOX]->set_dest_x( button[M_SHPBLD_MSGBOX]->get_ox() - 640 );

    msgbox_active = ACTIVE_MSG_REALLYDELETE;
  }
  else
  {
    errors->log(2, "deleting ship", ship_filename);
    char p[50];
    strcpy(p, globals->ship_dir);
    strcat(p, ship_filename);
    // Use standard C++ file deletion instead of Allegro function
    remove(p);
  }
}

void mShipBuilder::save_shipfile()
{
  FILE *shp;
  char path[50];
  errors->log(2, "saving ship", ship_filename);

  // if custom ship and image not in ships-folder -> copy it
  strcpy(path, globals->ship_dir);
  strcat(path, pl->get_shipimage_filename());

  if (pl->get_ship_type() == SHIP_CUSTOM && !exists(path))
  {
    if (filecopy(loaded_image_path, path))
      errors->log(0, "copy ship image", "ERROR!", loaded_image_path);
    else
      errors->log(2, "copy ship image", "okay", loaded_image_path);
  }

  strcpy(path, globals->ship_dir);
  strcat(path, ship_filename);

  shp = fopen(path, "wt");
  if (!shp)
  {
    errors->log(0, "ERROR saving ship", path);
    return;
  }

  fprintf(shp, "-- GRAVITY STRIKE v%s: Ship Definition\n\n", VERSION);

  // type, name, description
  switch (pl->get_ship_type())
  {
    case SHIP_CUSTOM    :  fprintf(shp, "%-20s = CUSTOM\n%-20s = \"%s\"\n", "type", "image", pl->get_shipimage_filename()); break;
    case SHIP_PIRANHA   :  fprintf(shp, "%-20s = PIRANHA\n", "type"); break;
    case SHIP_RAZOR     :  fprintf(shp, "%-20s = RAZOR\n", "type"); break;
    case SHIP_BOMBER    :  fprintf(shp, "%-20s = BOMBER\n", "type"); break;
  }

  if (pl->get_ship_type() != SHIP_CUSTOM)
    fprintf(shp, "%-20s = %d\n", "color", pl->get_ship_color());

  fprintf(shp, "%-20s = \"%s\"\n", "name", button[M_SHPBLD_NAME]->get_text());
  fprintf(shp, "%-20s = \"%s\"\n\n", "description", button[M_SHPBLD_DESC]->get_text());

  // non percent values
  fprintf(shp, "%-20s = %d\n", "hull", pl->get_percent_value(PV_HULL));
  fprintf(shp, "%-20s = %d\n", "shield", pl->get_percent_value(PV_SHIELD));
  fprintf(shp, "%-20s = %d\n", "radar_zoom", pl->get_percent_value(PV_RADAR_ZOOM));
  fprintf(shp, "%-20s = %d\n\n", "weapon_slots", pl->get_percent_value(PV_WEAPON_SLOTS));

  // percent values
  fprintf(shp, "%-20s = %d\n", "weapon_storage", pl->get_percent_value(PV_WEAPON_STORAGE));
  fprintf(shp, "%-20s = %d\n\n", "weapon_recharge", pl->get_percent_value(PV_WEAPON_RE));

  fprintf(shp, "%-20s = %d\n", "shield_recharge", pl->get_percent_value(PV_SHIELD_RE));
  fprintf(shp, "%-20s = %d\n", "hull_repair", pl->get_percent_value(PV_HULL_RE));
  fprintf(shp, "%-20s = %d\n\n", "wall_shield", pl->get_percent_value(PV_WALLSHIELD));

  fprintf(shp, "%-20s = %d\n", "turn_speed", pl->get_percent_value(PV_TURN_SPEED));
  fprintf(shp, "%-20s = %d\n", "maximum_speed", pl->get_percent_value(PV_MAX_SPEED));
  fprintf(shp, "%-20s = %d\n\n", "acceleration", pl->get_percent_value(PV_ACCELERATION));

  fprintf(shp, "%-20s = %d\n\n", "fuel_consumption", pl->get_percent_value(PV_FUEL_CONS));

  char extra_str[100];
  
  int n;
  switch (pl->get_extra())
  {
    case EXTRA_NONE        : strcpy(extra_str, "E_NONE");        break;
    case EXTRA_AFTERBURNER : strcpy(extra_str, "E_AFTERBURNER"); break;
    case EXTRA_CLOAK       : strcpy(extra_str, "E_CLOAK");       break;
    case EXTRA_DEFLECTOR   : strcpy(extra_str, "E_DEFLECTOR");   break;
  }

/*
  for (n=0; n < 5; n++)
  {
    extraslot eslot = pl->get_extra_slot(n);
    if (eslot.type == EXTRA_AFTERBURNER)
    {
      davor = 1;
      strcpy(extra_str, "E_AFTERBURNER");
    }
    else if (eslot.type == EXTRA_AUTOBREAK)
    {
      if (davor) strcat(extra_str, " + E_AUTOBREAK");
      else strcpy(extra_str, "E_AUTOBREAK");
      davor = 1;
    }
    else if (eslot.type == EXTRA_CHASEBLOCKER)
    {
      if (davor) strcat(extra_str, " + E_CHASEBLOCKER");
      else strcpy(extra_str, "E_CHASEBLOCKER");
      davor = 1;
    }
  }
*/
  fprintf(shp, "%-20s = %s\n\n", "extras", extra_str);

  // weapons
  for (n=1; n <= 6; n++)
  {
    char weapslot[20];
    char weapname[20];
    strcpy(weapname, "");
    sprintf(weapslot, "weapon_slot_%d", n);

    weaponslot *tmp_slot = pl->get_weapon_slot(n-1);
    switch (tmp_slot->type)
    {
      case W_SINGLE   :  strcpy(weapname, "W_SINGLE");   break;
      case W_DOUBLE   :  strcpy(weapname, "W_DOUBLE");   break;
      case W_BOMB     :  strcpy(weapname, "W_BOMB");     break;
      case W_BIGBOMB  :  strcpy(weapname, "W_BIGBOMB") ; break;
      case W_ROCKET   :  strcpy(weapname, "W_ROCKET");   break;
      case W_MINE     :  strcpy(weapname, "W_MINE");     break;
      case W_FREEZER  :  strcpy(weapname, "W_FREEZER");  break;
      case W_SHRINKER :  strcpy(weapname, "W_SHRINKER"); break;
      case W_PROBE    :  strcpy(weapname, "W_PROBE");    break;
      case W_GAS      :  strcpy(weapname, "W_GAS");      break;
      case W_LASER    :  strcpy(weapname, "W_LASER");    break;
      case W_BUMPER   :  strcpy(weapname, "W_BUMPER");   break;
      case W_SHOTGUN  :  strcpy(weapname, "W_SHOTGUN");   break;
      case W_ACID     :  strcpy(weapname, "W_ACID");     break;
      default         :  strcpy(weapname, "W_NONE");     break;
    }

    fprintf(shp, "%-20s = %s\n", weapslot, weapname);
  }


  fclose(shp);
}

