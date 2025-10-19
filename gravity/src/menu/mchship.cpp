/*
 *   GRAVITY STRIKE -- menu/mchship.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mchship.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsplayer.h"
#include "../gserror.h"
#include "../gssound.h"
#include "../gsnetw.h"
#include "gsmd.h"
#include "gsmenu.h"

mChooseShip *thismenu = NULL;
int    temp_num = 0;                               // temporary var for check_shipfile

mChooseShip::mChooseShip()
{
  start_with = 0;
  init_all();
}

mChooseShip::~mChooseShip()
{
  globals->last_menu = MENU_CHOOSESHIP;
  kill_all();
}

void mChooseShip::init_all()
{
  active_button = 2;
  button_count = 0;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  esc_pressed = 0;
  ship_count = 0;

  for (int n=0; n < 45; n++)
    button[n] = NULL;
}

void mChooseShip::kill_all()
{
  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

int check_if_temp_ship(char *s)
{
  if (s[0] == 'T' && s[1] == 'M' && s[2] == 'P') return 1;
  return 0;
}

void check_shipfile(const char *fname, int attrib, int param)
{
  int x,y,sx,sy,w,h,xanz,yanz,xpos,ypos;
  xanz = 7; yanz = 5;
  w = 65; h = 50; // 74
  x = 44; y = 100; // starting position
  xpos = temp_num % xanz;
  ypos = temp_num / xanz;

  x += (w + 15)*xpos;
  y += (h + 10)*ypos;

  sx = fxrand() % 200 + 640;
  sy = fxrand() % 300 + 100;

//errors->log(2, "load ship icon", (char*)fname, param);

  char f[50];
  strncpy(f, get_filename(fname), 50);
  strupr(f);

  if (!strstr(f, "DEFNEW.SHP") && !strstr(f, "DEF1.SHP") && !strstr(f, "DEF2.SHP") && !strstr(f, "DEF3.SHP") &&
      !check_if_temp_ship(f) && (temp_num < xanz*yanz || param == 1))
  {
    thismenu->button[temp_num] = new mShipIcon();
    thismenu->button[temp_num]->create(x, y, sx, sy, w, h);
    
    if (param == 0)
      thismenu->button[temp_num]->set_text((char*)fname);
    else
      thismenu->button[temp_num]->set_text("createnewship");
  
    if (thismenu->button[temp_num]->get_int()) // error
    {
      delete(thismenu->button[temp_num]);
    }
    else // okay
    {
      temp_num++;
    }
  } // if temp_num < xanz*yanz-1
}

int tnum;

void read_ship(const char *fname, int attrib, int param)
{
  if (tnum < MAX_SHIP_FILES) strncpy(thismenu->ship_files[tnum].name, get_filename(fname), 30);
  tnum++;
}

int mChooseShip::read_ships_and_sort_them(char *path)
{
  tnum = 0;
  thismenu = this;

  // Use standard C++ file operations instead of Allegro functions
  // TODO: Implement file scanning using standard C++ functions
  errors->log(1, "menu", "read_ships_and_sort_them: file scanning not implemented");

  if (tnum >= MAX_SHIP_FILES) tnum = MAX_SHIP_FILES;

  // sort
  int minIndex,i,k;
  shipfile memo;

  for (i=0; i < tnum-1; i++)
  {
    minIndex = i;
    for (k=i; k < tnum; k++)
    {
      if (stricmp(ship_files[k].name, ship_files[minIndex].name) < 0)
        minIndex = k;
    }
    memo = ship_files[i];
    ship_files[i] = ship_files[minIndex];
    ship_files[minIndex] = memo;
  }

  return tnum;
}


void mChooseShip::init(int scatter)
{
  char path[50];
  strcpy(path, globals->ship_dir);
  strcat(path, "*.shp");

  ship_count = read_ships_and_sort_them(path);

  temp_num = 0;
  thismenu = this;

  show_newship = 0;
  cnt_def_ships = 0;
  
  // new ship button
  if (globals->last_menu != MENU_SPLITSCREEN &&
      globals->last_menu != MENU_SINGLEPLAYER &&
      globals->last_menu != MENU_MINIGAME &&
      globals->last_menu != MENU_NETGAME)
  {
    check_shipfile("", 0, 1);
    show_newship = 1;
  }

  if (start_with == 0)
  {
    check_shipfile("#def1_shp", 0, 0);
    check_shipfile("#def2_shp", 0, 0);
    check_shipfile("#def3_shp", 0, 0);
    cnt_def_ships = 3;
  }

  for (int tnum=start_with; tnum < ship_count; tnum++)
  {
    check_shipfile(ship_files[tnum].name, 0, 0);
  }

  button_count += temp_num;

  // back
  button[button_count] = new mMainButton();
  if (scatter) button[button_count]->create(245, 430, 245, 500, 150, 30);
  else button[button_count]->create(245, 430, 0, 0, 150, 30);
  button[button_count]->set_text(language->get_string(T_MENU_BACK));
  b_back = button_count;

  button_count++;

  // next page
  button[button_count] = new mMainButton();
  if (scatter) button[button_count]->create(445, 430, 445, 500, 150, 30);
  else button[button_count]->create(445, 430, 0, 0, 150, 30);
  button[button_count]->set_text(language->get_string(T_MENU_NEXTPAGE));
  b_nextpage = button_count;

  if (start_with+35 >= ship_count) button[button_count]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button_count++;

  // last page
  button[button_count] = new mMainButton();
  if (scatter) button[button_count]->create(45, 430, 45, 500, 150, 30);
  else button[button_count]->create(45, 430, 0, 0, 150, 30);
  button[button_count]->set_text(language->get_string(T_MENU_LASTPAGE));
  b_lastpage = button_count;

  if (start_with == 0) button[button_count]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  // caption
  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(286, 64, 800, 64);
  button[button_count]->set_int(FONT_BANK);
  button[button_count]->set_text(language->get_string(T_MENU_CAP_CHOOSESHIP));
  button[button_count]->set_int(globals->col_yellow);
  button[button_count]->set_flag(MC_INACTIVE);
  b_caption = button_count;

  button_count++;
}

int mChooseShip::check()
{
  // network messages
  net->process_packets();

  // network updates
  if (net->is_server() && globals->game_time % 120 == 0)
    net->menu_server_update_clients();

  if (net->is_client() && globals->game_time % 60 == 0)
    net->menu_client_update_server();


  if (active_button >= 0)
    if (button[active_button]->get_flags() & MC_PUSHED)
    {
      button[active_button]->del_flag(MC_PUSHED);
    }

  for (int n=0; n < button_count; n++)
  {
    if (!moving)
    {
      button[n]->check();
      if (button[n]->get_flags() & MC_ACTIVE)
        if (active_button != n)
        {
          active_button = n; gsound->menu_change();
        }
    }
  }

  if (active_button >= 0)
    button[active_button]->set_flag(MC_ACTIVE);

  if (!moving)
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
        if (active_button >= button_count-1) active_button = 0;
        else if (active_button < button_count-1) active_button++;
      } while (button[active_button]->get_flags() & MC_INACTIVE);
      gsound->menu_change();
      clear_key(KEY_DOWN);
    }
    else if (key[KEY_ENTER])
    {
      if (active_button >= 0)
        button[active_button]->set_flag(MC_PUSHED);
      clear_key(KEY_ENTER);
    }
  }

  return(handle_controls());
}

int mChooseShip::handle_controls()
{
  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[b_back]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    globals->ship_num = -1;
    switch_to_menu = MENU_MAIN;
//    switch_to_menu = globals->last_menu;
    if (globals->last_menu == MENU_SPLITSCREEN)
      switch_to_menu = MENU_SPLITSCREEN;
    else if (globals->last_menu == MENU_NETGAME)
      switch_to_menu = MENU_NETGAME;
    else if (globals->last_menu == MENU_SINGLEPLAYER)
      switch_to_menu = MENU_SINGLEPLAYER;
    else if (globals->last_menu == MENU_MINIGAME)
      switch_to_menu = MENU_MINIGAME;
      
    button[b_back]->set_flag(MC_INVISIBLE);
    button[b_back]->do_explosion();
    button[b_back]->del_flag(MC_PUSHED);
    mouse_b = 0;
    move_out();
  }

  // next page
  if (key[KEY_PGDN] || button[b_nextpage]->get_flags() & MC_PUSHED)
  {
    if (start_with + 35 - cnt_def_ships - show_newship < ship_count)
    {
      start_with += 35 - cnt_def_ships - show_newship;
      kill_all();
      init_all();
      init(0);
      clear_key(KEY_PGDN);
      mouse_b = 0;
    }
  }

  // last page
  if (key[KEY_PGUP] || button[b_lastpage]->get_flags() & MC_PUSHED)
  {
    if (start_with > 0)
    {
      start_with -= 35 - cnt_def_ships - show_newship;
      if (start_with < 0) start_with = 0;
      kill_all();
      init_all();
      init(0);
      clear_key(KEY_PGUP);
      mouse_b = 0;
    }
  }

  // check ship buttons
  for (int n=0; n < button_count-4; n++)
  {
    if (button[n]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      strncpy(chosen_ship_filename, button[n]->get_text(), 30);
      switch_to_menu = MENU_SHIPBUILDER;
//      switch_to_menu = globals->last_menu;
      if (globals->last_menu == MENU_SPLITSCREEN)
        switch_to_menu = MENU_SPLITSCREEN;
      else if (globals->last_menu == MENU_NETGAME)
        switch_to_menu = MENU_NETGAME;
      else if (globals->last_menu == MENU_SINGLEPLAYER)
        switch_to_menu = MENU_SINGLEPLAYER;
      else if (globals->last_menu == MENU_MINIGAME)
        switch_to_menu = MENU_MINIGAME;
      button[n]->set_flag(MC_INACTIVE);
      button[n]->do_explosion();
      move_out();
    }
  }

  if (move() == 0 && moving) return switch_to_menu;

  return 0;
}

void mChooseShip::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "start_with", start_with), 10, 400, makecol8(255,255,255));
*/
}

int mChooseShip::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mChooseShip::move_out()
{
  for (int n=0; n < button_count-1; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 500 );
  }
  button[b_caption]->set_dest_x( button[b_caption]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

