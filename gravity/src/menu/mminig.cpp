/*
 *   GRAVITY STRIKE -- menu/mminig.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

/* DONE (#1#): Menü anpassen für Minigames */


#include "mminig.h"
#include "controls/mtypes.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsplayer.h"
#include "../gsfade.h"
#include "../gssound.h"
#include "../gsscript.h"
#include "../gsglob.h"
#include "../gserror.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>

mMiniGame::mMiniGame()
{
  globals->players = 1;
  globals->minigame = 1;
  globals->singleplayer = 0;

  active_button = 2;
  button_count = 21;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  minimap = NULL;
  moved_in = FALSE;
  esc_pressed = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;

  if (strlen(globals->pplayer[0].pship) <= 1) strcpy(globals->pplayer[0].pship, globals->def_ship);
  pl[0] = new gsPlayer(0, 0, 0, globals->pplayer[0].pship);

  if (strlen(globals->pplayer[1].pship) <= 1) strcpy(globals->pplayer[1].pship, globals->def_ship);
  pl[1] = new gsPlayer(1, 0, 0, globals->pplayer[1].pship);

  script->read_level_files();
  globals->selected_level = globals->get_level_num_from_filename(globals->last_minigame);
  
  infobox = 0;
}

mMiniGame::~mMiniGame()
{
  globals->last_menu = MENU_MINIGAME;
  strcpy(globals->pplayer[0].pname, button[M_MINIGAME_P1NAME]->get_text());
  strcpy(globals->pplayer[1].pname, button[M_MINIGAME_P2NAME]->get_text());

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);

  if (pl[0])    delete(pl[0]);
  if (pl[1])    delete(pl[1]);

  if (minimap)  destroy_bitmap(minimap);
}

void mMiniGame::init(int scatter)
{
  button[M_MINIGAME_CAPTION] = new mNormalText();
  button[M_MINIGAME_CAPTION]->create(286, 64, 800, 64);
  button[M_MINIGAME_CAPTION]->set_int(FONT_BANK);
  button[M_MINIGAME_CAPTION]->set_text(language->get_string(T_MENU_CAP_MINIGAME));
  button[M_MINIGAME_CAPTION]->set_int(globals->col_yellow);
  button[M_MINIGAME_CAPTION]->set_flag(MC_INACTIVE);

  button[M_MINIGAME_BACK] = new mMainButton();
  if (scatter) button[M_MINIGAME_BACK]->create(30, 430, 30, 500, 150, 30);
  else button[M_MINIGAME_BACK]->create(30, 430, 0, 0, 150, 30);
  button[M_MINIGAME_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_MINIGAME_LEVELINFO] = new mMainButton();
  if (scatter) button[M_MINIGAME_LEVELINFO]->create(320-75, 430, 30, 500, 150, 30);
  else button[M_MINIGAME_LEVELINFO]->create(320-75, 430, 0, 0, 150, 30);
  button[M_MINIGAME_LEVELINFO]->set_text(language->get_string(T_MENU_LEVELINFO));
  button[M_MINIGAME_LEVELINFO]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_MINIGAME_PLAY] = new mMainButton();
  if (scatter) button[M_MINIGAME_PLAY]->create(450, 430, 450, 500, 150, 30);
  else button[M_MINIGAME_PLAY]->create(450, 430, 0, 0, 150, 30);
  button[M_MINIGAME_PLAY]->set_text(language->get_string(T_MENU_PLAY_MINIGAME));
  button[M_MINIGAME_PLAY]->set_color1(globals->col_yellow);
  if (globals->max_levels <= 0) button[M_MINIGAME_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_MINIGAME_P1NAME] = new mTextField();
  button[M_MINIGAME_P1NAME]->create(30, 5+100, -300, 5+100, 250, 15, 100);
  button[M_MINIGAME_P1NAME]->set_int(20);
  button[M_MINIGAME_P1NAME]->set_text(language->get_string(T_MENU_NAME_P1));
  button[M_MINIGAME_P1NAME]->set_text(globals->pplayer[0].pname);

  button[M_MINIGAME_P1SHIP] = new mShipIcon2();
  button[M_MINIGAME_P1SHIP]->create(30, 5+125, -300, 5+125, 90, 51);
  button[M_MINIGAME_P1SHIP]->set_int(1);
  button[M_MINIGAME_P1SHIP]->set_player(pl[0]);

  button[M_MINIGAME_P1CHOOSE] = new mSmallButton();
  button[M_MINIGAME_P1CHOOSE]->create(130, 5+125, -300, 5+125, 150, 15);
  button[M_MINIGAME_P1CHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_MINIGAME_P1EDIT] = new mSmallButton();
  button[M_MINIGAME_P1EDIT]->create(130, 5+145, -300, 5+145, 150, 15);
  button[M_MINIGAME_P1EDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));
  char filename[20];
  strncpy(filename, globals->pplayer[0].pship, 20); strupr(filename);
  if (strstr(filename, "DEF1.SHP") || strstr(filename, "DEF2.SHP") || strstr(filename, "DEF3.SHP"))
    button[M_MINIGAME_P1EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_MINIGAME_P2NAME] = new mTextField();
  button[M_MINIGAME_P2NAME]->create(320+30, 5+100, 640, 5+100, 250, 15, 100);
  button[M_MINIGAME_P2NAME]->set_int(20);
  button[M_MINIGAME_P2NAME]->set_text(language->get_string(T_MENU_NAME_P2));
  button[M_MINIGAME_P2NAME]->set_text(globals->pplayer[1].pname);

  button[M_MINIGAME_P2SHIP] = new mShipIcon2();
  button[M_MINIGAME_P2SHIP]->create(320+30, 5+125, 640, 5+125, 90, 51);
  button[M_MINIGAME_P2SHIP]->set_int(1);
  button[M_MINIGAME_P2SHIP]->set_player(pl[1]);

  button[M_MINIGAME_P2CHOOSE] = new mSmallButton();
  button[M_MINIGAME_P2CHOOSE]->create(320+130, 5+125, 640, 5+125, 150, 15);
  button[M_MINIGAME_P2CHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_MINIGAME_P2EDIT] = new mSmallButton();
  button[M_MINIGAME_P2EDIT]->create(320+130, 5+145, 640, 5+145, 150, 15);
  button[M_MINIGAME_P2EDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));
  strncpy(filename, globals->pplayer[1].pship, 20); strupr(filename);
  if (strstr(filename, "DEF1.SHP") || strstr(filename, "DEF2.SHP") || strstr(filename, "DEF3.SHP"))
    button[M_MINIGAME_P2EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_MINIGAME_LEVSELECT] = new mLevelSelect();
  button[M_MINIGAME_LEVSELECT]->create(30, 200, -300, 200, 155, 200);
  button[M_MINIGAME_LEVSELECT]->set_int(globals->selected_level);

  button[M_MINIGAME_AUTHOR] = new mTextField();
  button[M_MINIGAME_AUTHOR]->create(202, 200, 640+100, 200, 265, 15, 80);
  button[M_MINIGAME_AUTHOR]->set_int(0);
  button[M_MINIGAME_AUTHOR]->set_text(language->get_string(T_MENU_CAP_AUTHOR));
  button[M_MINIGAME_AUTHOR]->set_flag(MC_INACTIVE);

  button[M_MINIGAME_LEVDESC] = new mBlank();
  button[M_MINIGAME_LEVDESC]->create(202, 220, 640, 220, 265, 95, 0);

  button[M_MINIGAME_LEVELEXTRA] = new mCheckBox();
  button[M_MINIGAME_LEVELEXTRA]->create(202, 320+20, -300-200, 320+20, 17, 12);
  button[M_MINIGAME_LEVELEXTRA]->set_int(-1); // align box on left side
  button[M_MINIGAME_LEVELEXTRA]->set_text(language->get_string(T_MENU_LEVELEXTRA));
//  if (globals->activate_extra)
//    button[M_MINIGAME_LEVELEXTRA]->set_flag(MC_PUSHED);

  button[M_MINIGAME_LEVMINIMAP] = new mBlank();
  button[M_MINIGAME_LEVMINIMAP]->create(484, 200, 640+280, 200, 115, 115, 0);

  button[M_MINIGAME_OPT_EXTRAS] = new mTextSlider();
  button[M_MINIGAME_OPT_EXTRAS]->set_int(1);
  button[M_MINIGAME_OPT_EXTRAS]->create(202, 380, -400, 380, 220, 14, 60);
  button[M_MINIGAME_OPT_EXTRAS]->set_align(1);

  button[M_MINIGAME_OPT_EXTRAS_TXT] = new mNormalText();
  button[M_MINIGAME_OPT_EXTRAS_TXT]->create(450, 380, 640+30, 380);
  button[M_MINIGAME_OPT_EXTRAS_TXT]->set_int(FONT_IMPACT10);
  button[M_MINIGAME_OPT_EXTRAS_TXT]->set_text("n/a");
  button[M_MINIGAME_OPT_EXTRAS_TXT]->set_int(globals->col_green);
  button[M_MINIGAME_OPT_EXTRAS_TXT]->set_flag(MC_INACTIVE);

  button[M_MINIGAME_PLAYER2] = new mCheckBox();
  button[M_MINIGAME_PLAYER2]->create(202, 340,  640, 340, 17, 12);
  button[M_MINIGAME_PLAYER2]->set_int(-1); // align box on left side
  button[M_MINIGAME_PLAYER2]->set_text(language->get_string(T_MENU_2NDPLAYER));
  if (globals->temp_minigame_2player)
  {
    show_hide_player2(1);   
    button[M_MINIGAME_PLAYER2]->set_flag(MC_PUSHED);
  }
  else
    show_hide_player2(0);


  button[M_MINIGAME_LEVELINFOBOX] = new mTextBox();
  button[M_MINIGAME_LEVELINFOBOX]->create(640+30, 95, 640*2, 95, 580, 320);
}

void mMiniGame::show_hide_player2(int show)
{
  if (show == 0) // hide player
  {
    button[M_MINIGAME_P2NAME]->set_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2SHIP]->set_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2CHOOSE]->set_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);
    //button[M_MINIGAME_PLAYER2]->del_flag(MC_PUSHED);
  }
  else // show player
  {
    button[M_MINIGAME_P2NAME]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2SHIP]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2CHOOSE]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    button[M_MINIGAME_P2EDIT]->del_flag(MC_INVISIBLE | MC_INACTIVE);
  }
}

int mMiniGame::check()
{
/*  if (active_button >= 0)
    if (button[active_button]->get_flags() & MC_PUSHED)
    {
      button[active_button]->del_flag(MC_PUSHED);
    }
*/
  for (int n=0; n < button_count; n++)
  {
    if (!moving)
    {
      button[n]->check();
      if (button[n]->get_flags() & MC_ACTIVE)
        if (active_button != n && active_button != M_MINIGAME_LEVELINFOBOX)
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

int mMiniGame::handle_controls()
{
  int i;
  int changed = 0;

  // check if level changed
  i = button[M_MINIGAME_LEVSELECT]->get_int();
  if (i >= 0 && !no_input)
  {
    if (moved_in) gsound->menu_select();
      else moved_in = TRUE;

    globals->selected_level = i;
    strncpy(globals->last_minigame, globals->level_files[i].levelfile, 20);
    
    globals->race = 0;
    globals->hunter_hunted = 0;
    globals->capture_the_flag = 0;
    globals->activate_extra = 0;

    if (strlen(globals->level_files[globals->selected_level].activate_extra_str) > 1)
    {
      button[M_MINIGAME_LEVELEXTRA]->del_flag(MC_INVISIBLE);
      button[M_MINIGAME_LEVELEXTRA]->del_flag(MC_INACTIVE);
      button[M_MINIGAME_LEVELEXTRA]->set_text(globals->level_files[globals->selected_level].activate_extra_str);
    }
    else
    {
      button[M_MINIGAME_LEVELEXTRA]->set_flag(MC_INVISIBLE);
      button[M_MINIGAME_LEVELEXTRA]->set_flag(MC_INACTIVE);
      globals->activate_extra = 0;
    }

    button[M_MINIGAME_AUTHOR]->set_text(globals->level_files[globals->selected_level].author);

    if (strlen(globals->level_files[globals->selected_level].activate_extra_bar_str) > 1)
    {
      char tmpbarstr[50], barstr[50];
      extrabar_max = 10; extrabar_steps = 1;
      strncpy(tmpbarstr, globals->level_files[globals->selected_level].activate_extra_bar_str, 40);
      sscanf(tmpbarstr, "%d %d %[^\n]", &extrabar_max, &extrabar_steps, barstr);
      if (extrabar_steps <= 0) extrabar_steps = 1;
      if (extrabar_steps > extrabar_max) extrabar_steps = extrabar_max/2;
      button[M_MINIGAME_OPT_EXTRAS]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_MINIGAME_OPT_EXTRAS_TXT]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_MINIGAME_OPT_EXTRAS]->set_text(barstr);
      button[M_MINIGAME_OPT_EXTRAS]->get_bar()->set_float(50);
      globals->activate_extra_bar = int(50.0 / (100.0/extrabar_max));
      changed = 1;
    }
    else
    {
      button[M_MINIGAME_OPT_EXTRAS]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_MINIGAME_OPT_EXTRAS_TXT]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      globals->activate_extra_bar = 0;
      changed = 1;
    }


    if (globals->level_files[globals->selected_level].max_players > 1)
    {
      button[M_MINIGAME_PLAYER2]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    }
    else
    {
      button[M_MINIGAME_PLAYER2]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_MINIGAME_PLAYER2]->del_flag(MC_PUSHED);
      show_hide_player2(0);
      globals->players = 1;
    }

    if (minimap) destroy_bitmap(minimap); minimap = NULL;

    char path[150];
    char npath[150];
    sprintf(path, "%s%s/%s", globals->level_dir,
                             globals->level_files[globals->selected_level].leveldir,
                             globals->level_files[globals->selected_level].mapfile);
    replace_extension(npath, path, "tga", 150);

    if (exists(npath))
      minimap = load_tga(npath, NULL);
      
    // levelinfo?
    sprintf(path, "%s%s/%s", globals->level_dir,
                             globals->level_files[globals->selected_level].leveldir,
                             globals->level_files[globals->selected_level].levelfile);
    replace_extension(npath, path, "txt", 150);

    if (exists(npath))
    {
      button[M_MINIGAME_LEVELINFOBOX]->set_text(npath);
      button[M_MINIGAME_LEVELINFO]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    }
    else
    {
      button[M_MINIGAME_LEVELINFOBOX]->set_text("");
      button[M_MINIGAME_LEVELINFO]->set_flag(MC_INVISIBLE | MC_INACTIVE);
    }
    
    // resolution info
    int reqres = globals->level_files[globals->selected_level].require_resolution_nr;

    char var_text[30];
    if (globals->language == LANG_DE)
      strcpy(var_text, "VARIABEL");
    else
      strcpy(var_text, "VARIABLE");
      
    char res_text[40];
    strncpy(res_text, language->get_string(T_MENU_RESOLUTION), 39);
    strupr(res_text);

    if (reqres == -1)
      sprintf(resolution_text, "%s: %s",
              res_text,
              var_text);
    else
      sprintf(resolution_text, "%s: %d X %d", 
              res_text,
              globals->game_resolution[reqres].x,
              globals->game_resolution[reqres].y);
  }

  // play!
  if (button[M_MINIGAME_PLAY]->get_flags() & MC_PUSHED)
  {
    if (strlen(globals->level_files[globals->selected_level].initscriptfile) > 0)
    {
      gsound->menu_select();
      fade->initiate(FADE_TO_BLACK, 1, 60);
      switch_to_menu = START_GAME;
      button[M_MINIGAME_PLAY]->set_flag(MC_INVISIBLE);
      button[M_MINIGAME_PLAY]->do_explosion();
      button[M_MINIGAME_PLAY]->del_flag(MC_PUSHED);
      move_out();
    }
  }

  // levelinfo
  if (button[M_MINIGAME_LEVELINFO]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    button[M_MINIGAME_LEVELINFO]->del_flag(MC_PUSHED);
    mouse_b = 0;
    if (!infobox)
    {
      button[M_MINIGAME_LEVELINFOBOX]->set_dest_x(30);
      infobox = 1;
    }
    else
    {
      button[M_MINIGAME_LEVELINFOBOX]->set_dest_x(640);
      infobox = 0;
    }    
  }
  

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_MINIGAME_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_MAIN;
    button[M_MINIGAME_BACK]->set_flag(MC_INVISIBLE);
    button[M_MINIGAME_BACK]->do_explosion();
    button[M_MINIGAME_BACK]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 choose ship
  if (button[M_MINIGAME_P1CHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 0;
    button[M_MINIGAME_P1CHOOSE]->set_flag(MC_INVISIBLE);
    button[M_MINIGAME_P1CHOOSE]->do_explosion();
    button[M_MINIGAME_P1CHOOSE]->del_flag(MC_PUSHED);
    move_out();
  }

  // p2 choose ship
  if (button[M_MINIGAME_P2CHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 1;
    button[M_MINIGAME_P2CHOOSE]->set_flag(MC_INVISIBLE);
    button[M_MINIGAME_P2CHOOSE]->do_explosion();
    button[M_MINIGAME_P2CHOOSE]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 edit ship
  if (button[M_MINIGAME_P1EDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 0;
    button[M_MINIGAME_P1EDIT]->set_flag(MC_INVISIBLE);
    button[M_MINIGAME_P1EDIT]->do_explosion();
    button[M_MINIGAME_P1EDIT]->del_flag(MC_PUSHED);
    move_out();
  }

  // p2 edit ship
  if (button[M_MINIGAME_P2EDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 1;
    button[M_MINIGAME_P2EDIT]->set_flag(MC_INVISIBLE);
    button[M_MINIGAME_P2EDIT]->do_explosion();
    button[M_MINIGAME_P2EDIT]->del_flag(MC_PUSHED);
    move_out();
  }

  // activate level extra?
  if (button[M_MINIGAME_LEVELEXTRA]->get_flags() & MC_ACTIVE)
  {
    if (button[M_MINIGAME_LEVELEXTRA]->get_flags() & MC_PUSHED)
    {
      globals->activate_extra = 1;
      mouse_b = 0;
    }
    else
    {
      globals->activate_extra = 0;
      mouse_b = 0;
    }
  }

  // activate second player?
  if (changed || button[M_MINIGAME_PLAYER2]->get_flags() & MC_ACTIVE)
  {
    if ((globals->level_files[globals->selected_level].max_players >= 2 &&
        globals->level_files[globals->selected_level].activate_singleplayer == 0) ||
        button[M_MINIGAME_PLAYER2]->get_flags() & MC_PUSHED)
    {
      globals->players = 2;
      show_hide_player2(1);
      mouse_b = 0;
      globals->temp_minigame_2player = 1;
      button[M_MINIGAME_PLAYER2]->set_flag(MC_PUSHED);
    }
    else
    {
      globals->players = 1;
      show_hide_player2(0);
      mouse_b = 0;
      globals->temp_minigame_2player = 0;
    }
  }

  // extras
  float f = button[M_MINIGAME_OPT_EXTRAS]->get_bar()->get_float();
  if (button[M_MINIGAME_OPT_EXTRAS]->get_arrow1()->get_flags() & MC_PUSHED)
    if (f > 0)  { f-=(100.0/extrabar_max)*extrabar_steps; globals->activate_extra_bar = int(f / (100.0/extrabar_max)); gsound->menu_changebar(); mouse_b = 0; changed = 1; }
  if (button[M_MINIGAME_OPT_EXTRAS]->get_arrow2()->get_flags() & MC_PUSHED)
    if (f < 100)  { f+=(100.0/extrabar_max)*extrabar_steps; globals->activate_extra_bar = int(f / (100.0/extrabar_max)); gsound->menu_changebar(); mouse_b = 0; changed = 1; }

  if (changed)  
  {
    button[M_MINIGAME_OPT_EXTRAS]->get_bar()->set_float(f);
    
    char tmps[20];
    sprintf(tmps, "%d", globals->activate_extra_bar);
    button[M_MINIGAME_OPT_EXTRAS_TXT]->set_text(tmps);
    button[M_MINIGAME_OPT_EXTRAS_TXT]->set_dest_x(button[M_MINIGAME_OPT_EXTRAS]->get_dest_x()+
                                                  button[M_MINIGAME_OPT_EXTRAS]->get_w()+5);
  }


  int move_ret = move();

  if (move_ret && switch_to_menu == START_GAME)
    if (gsound->get_music_volume() > 0) gsound->set_music_volume(gsound->get_music_volume()-4);

  if (move_ret == 0 && moving) return switch_to_menu;

  return 0;
}

void mMiniGame::draw(BITMAP *v)
{
  // map description
  if (globals->max_levels > 0)
  {
    mytextout_center(v, (FONT *)globals->fontdat[FONT_IMPACT10].dat,
                        globals->level_files[globals->selected_level].desc,
                        (int)button[M_MINIGAME_LEVDESC]->get_x() + 5,
                        (int)button[M_MINIGAME_LEVDESC]->get_x() + button[M_MINIGAME_LEVDESC]->get_w() - 5,
                        (int)button[M_MINIGAME_LEVDESC]->get_y()+5,
                        globals->col_yellow);
  
    // map picture
    if (minimap)
    {
      int mx = (int)button[M_MINIGAME_LEVMINIMAP]->get_x();
      int my = (int)button[M_MINIGAME_LEVMINIMAP]->get_y();
      int mw = (int)button[M_MINIGAME_LEVMINIMAP]->get_w();
      int mh = (int)button[M_MINIGAME_LEVMINIMAP]->get_h();
  //    int mmw = minimap->w;
  //    int mmh = minimap->h;
  
  //    masked_stretch_blit(minimap, v, 0, 0, mmw, mmh, mx, my, mw, mh);
      masked_blit(minimap, v, 0, 0, mx, my, mw, mh);

      // resolution desc
      mytextout_left(v, (FONT *)globals->fontdat[FONT_MINIFONT].dat,
                        resolution_text,
                        mx+1,
                        mx+mw,
                        my+mh+5,
                        globals->col_yellow);      
    }
  }
  
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, mitoa(tmpstr, globals->activate_extra), 10, 400, makecol8(255,255,255));
*/
}

int mMiniGame::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mMiniGame::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_MINIGAME_CAPTION]->set_dest_x( button[M_MINIGAME_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

