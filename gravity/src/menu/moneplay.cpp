/*
 *   GRAVITY STRIKE -- menu/moneplay.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "moneplay.h"
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

mOnePlayer::mOnePlayer()
{
  globals->players = 1;
  globals->singleplayer = 1;
  globals->minigame = 0;

  active_button = 2;
  button_count = 14;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  minimap = NULL;
  moved_in = FALSE;
  esc_pressed = 0;
  strcpy(leveldesc, "");
  leveldesc_col = globals->col_yellow;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;

  if (strlen(globals->pplayer[0].pship) <= 1) strcpy(globals->pplayer[0].pship, globals->def_ship);
  pl = new gsPlayer(0, 0, 0, globals->pplayer[0].pship);

  if (strlen(globals->last_leveltype) == 0) 
    strcpy(globals->last_leveltype, "m");
    
  script->read_level_files(0, globals->last_leveltype);
  globals->selected_level = globals->get_level_num_from_filename(globals->last_splevel);

//errors->log(2, globals->last_splevel, globals->selected_level, globals->max_levels);
}

mOnePlayer::~mOnePlayer()
{
  globals->last_menu = MENU_SINGLEPLAYER;
  strcpy(globals->pplayer[0].pname, button[M_ONEPLAYER_P1NAME]->get_text());

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);

  if (pl)    delete(pl);

  if (minimap)  destroy_bitmap(minimap);
}

void mOnePlayer::init(int scatter)
{
  button[M_ONEPLAYER_CAPTION] = new mNormalText();
  button[M_ONEPLAYER_CAPTION]->create(286, 64, 800, 64);
  button[M_ONEPLAYER_CAPTION]->set_int(FONT_BANK);
  button[M_ONEPLAYER_CAPTION]->set_text(language->get_string(T_MENU_CAP_SINGLEPLAYER));
  button[M_ONEPLAYER_CAPTION]->set_int(globals->col_yellow);
  button[M_ONEPLAYER_CAPTION]->set_flag(MC_INACTIVE);

  button[M_ONEPLAYER_BACK] = new mMainButton();
  if (scatter) button[M_ONEPLAYER_BACK]->create(30, 430, 30, 500, 150, 30);
  else button[M_ONEPLAYER_BACK]->create(30, 430, 0, 0, 150, 30);
  button[M_ONEPLAYER_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_ONEPLAYER_PLAY] = new mMainButton();
  if (scatter) button[M_ONEPLAYER_PLAY]->create(450, 430, 450, 500, 150, 30);
  else button[M_ONEPLAYER_PLAY]->create(450, 430, 0, 0, 150, 30);
  button[M_ONEPLAYER_PLAY]->set_text(language->get_string(T_MENU_PLAY));
  button[M_ONEPLAYER_PLAY]->set_color1(globals->col_yellow);
  if (globals->max_levels <= 0) button[M_ONEPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_ONEPLAYER_P1NAME] = new mTextField();
  button[M_ONEPLAYER_P1NAME]->create(30, 5+100, -300, 5+100, 250, 15, 100);
  button[M_ONEPLAYER_P1NAME]->set_int(20);
  button[M_ONEPLAYER_P1NAME]->set_text(language->get_string(T_MENU_NAME_P1));
  button[M_ONEPLAYER_P1NAME]->set_text(globals->pplayer[0].pname);

  button[M_ONEPLAYER_P1SHIP] = new mShipIcon2();
  button[M_ONEPLAYER_P1SHIP]->create(30, 5+125, -300, 5+125, 90, 51);
  button[M_ONEPLAYER_P1SHIP]->set_int(1);
  button[M_ONEPLAYER_P1SHIP]->set_player(pl);

  button[M_ONEPLAYER_P1CHOOSE] = new mSmallButton();
  button[M_ONEPLAYER_P1CHOOSE]->create(130, 5+125, -300, 5+125, 150, 15);
  button[M_ONEPLAYER_P1CHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_ONEPLAYER_P1EDIT] = new mSmallButton();
  button[M_ONEPLAYER_P1EDIT]->create(130, 5+145, -300, 5+145, 150, 15);
  button[M_ONEPLAYER_P1EDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));
  char filename[20];
  strncpy(filename, globals->pplayer[0].pship, 20); strupr(filename);
  if (strstr(filename, "DEF1.SHP") || strstr(filename, "DEF2.SHP") || strstr(filename, "DEF3.SHP"))
    button[M_ONEPLAYER_P1EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_ONEPLAYER_DISP_MISSIONS] = new mSmallButton();
  button[M_ONEPLAYER_DISP_MISSIONS]->create(30, 200, -300, 200, 88, 16);
  button[M_ONEPLAYER_DISP_MISSIONS]->set_text(language->get_string(T_MENU_DISP_MISSIONS));
  if (!strcmp(globals->last_leveltype, "m"))
    button[M_ONEPLAYER_DISP_MISSIONS]->set_int(1);

  button[M_ONEPLAYER_DISP_RACES] = new mSmallButton();
  button[M_ONEPLAYER_DISP_RACES]->create(211-88, 200, -220, 200, 88, 16);
  button[M_ONEPLAYER_DISP_RACES]->set_text(language->get_string(T_MENU_DISP_RACES));
  if (!strcmp(globals->last_leveltype, "r"))
    button[M_ONEPLAYER_DISP_RACES]->set_int(1);

  button[M_ONEPLAYER_LEVSELECT] = new mLevelSelect();
  button[M_ONEPLAYER_LEVSELECT]->create(30, 220, -300, 200, 181, 219-22);
  button[M_ONEPLAYER_LEVSELECT]->set_int(globals->selected_level);

  button[M_ONEPLAYER_AUTHOR] = new mTextField();
  button[M_ONEPLAYER_AUTHOR]->create(215, 200, 640+100, 200, 265, 15, 80);
  button[M_ONEPLAYER_AUTHOR]->set_int(0);
  button[M_ONEPLAYER_AUTHOR]->set_text(language->get_string(T_MENU_CAP_AUTHOR));
  button[M_ONEPLAYER_AUTHOR]->set_flag(MC_INACTIVE);

  button[M_ONEPLAYER_LEVDESC] = new mBlank();
  button[M_ONEPLAYER_LEVDESC]->create(215, 220, 640, 220, 265, 95, 0);

  button[M_ONEPLAYER_LEVELEXTRA] = new mCheckBox();
  button[M_ONEPLAYER_LEVELEXTRA]->create(215, 296+25, -300-200, 297+25, 17, 12);
  button[M_ONEPLAYER_LEVELEXTRA]->set_int(-1); // align box on left side
  button[M_ONEPLAYER_LEVELEXTRA]->set_text(language->get_string(T_MENU_LEVELEXTRA));
  if (globals->activate_extra)
    button[M_ONEPLAYER_LEVELEXTRA]->set_flag(MC_PUSHED);
/*
  button[M_ONEPLAYER_LEVEL_LOCKED] = new mNormalText();
  button[M_ONEPLAYER_LEVEL_LOCKED]->create(215, 350, -500, 350);
  button[M_ONEPLAYER_LEVEL_LOCKED]->set_int(FONT_IMPACT10);
  button[M_ONEPLAYER_LEVEL_LOCKED]->set_text(language->get_string(T_MENU_LEVEL_LOCKED));
  button[M_ONEPLAYER_LEVEL_LOCKED]->set_int(globals->col_yellow);
  button[M_ONEPLAYER_LEVEL_LOCKED]->set_flag(MC_INACTIVE);

  button[M_ONEPLAYER_LEVEL_LOCKED_TXT] = new mNormalText();
  button[M_ONEPLAYER_LEVEL_LOCKED_TXT]->create(215, 370, -500, 370);
  button[M_ONEPLAYER_LEVEL_LOCKED_TXT]->set_int(FONT_IMPACT10);
  button[M_ONEPLAYER_LEVEL_LOCKED_TXT]->set_int(globals->col_yellow);
  button[M_ONEPLAYER_LEVEL_LOCKED_TXT]->set_flag(MC_INACTIVE);
  button[M_ONEPLAYER_LEVEL_LOCKED_TXT]->set_text("");
*/
  button[M_ONEPLAYER_LEVMINIMAP] = new mBlank();
  button[M_ONEPLAYER_LEVMINIMAP]->create(484, 200, 640+280, 200, 115, 115, 0);
}

int mOnePlayer::check()
{
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

void mOnePlayer::check_level_depends()
{
  if (strlen(globals->level_files[globals->selected_level].depends_on) > 0)
  {
    if (!globals->check_level_done_file(globals->level_files[globals->selected_level].depends_on, 
                                        button[M_ONEPLAYER_P1NAME]->get_text()))
    {
      sprintf(leveldesc, "%s", globals->level_files[globals->selected_level].depends_on_txt);
      button[M_ONEPLAYER_PLAY]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      leveldesc_col = globals->col_orange;
    }
  }
}

int mOnePlayer::handle_controls()
{
  int i;

  // check if level changed
  i = button[M_ONEPLAYER_LEVSELECT]->get_int();
  if (i >= 0 && !no_input)
  {
    if (moved_in) gsound->menu_select();
      else moved_in = TRUE;

    globals->selected_level = i;
    strncpy(globals->last_splevel, globals->level_files[i].levelfile, 20);
//errors->log(2, "selected level", globals->last_splevel, globals->selected_level);
    if (globals->level_files[i].activate_race)
      strcpy(globals->last_leveltype, "r");
    else
      strcpy(globals->last_leveltype, "m");

    // default text is level description
    strcpy(leveldesc, globals->level_files[globals->selected_level].desc);
    button[M_ONEPLAYER_PLAY]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    leveldesc_col = globals->col_yellow;

    // check if level depends on another
    check_level_depends();

    if (strlen(globals->level_files[globals->selected_level].activate_extra_str) > 1)
    {
      button[M_ONEPLAYER_LEVELEXTRA]->del_flag(MC_INVISIBLE);
      button[M_ONEPLAYER_LEVELEXTRA]->del_flag(MC_INACTIVE);
      button[M_ONEPLAYER_LEVELEXTRA]->set_text(globals->level_files[globals->selected_level].activate_extra_str);
    }
    else
    {
      button[M_ONEPLAYER_LEVELEXTRA]->set_flag(MC_INVISIBLE);
      button[M_ONEPLAYER_LEVELEXTRA]->set_flag(MC_INACTIVE);
      globals->activate_extra = 0;
    }

    button[M_ONEPLAYER_AUTHOR]->set_text(globals->level_files[globals->selected_level].author);

    if (globals->level_files[globals->selected_level].activate_race == 1)
    {
      globals->race = 1;
    }
    else
    {
      globals->race = 0;
    }

    if (minimap) destroy_bitmap(minimap); 
    minimap = NULL;

    char path[150];
    char npath[150];
    sprintf(path, "%s%s/%s", globals->level_dir,
                             globals->level_files[globals->selected_level].leveldir,
                             globals->level_files[globals->selected_level].mapfile);
    replace_extension(npath, path, "tga", 150);

    if (exists(npath)) {
      // Use Allegro 5 function instead
      ALLEGRO_BITMAP *al_minimap = al_load_bitmap(npath);
      if (al_minimap) {
        // Create a new BITMAP wrapper for the ALLEGRO_BITMAP
        minimap = create_bitmap(al_get_bitmap_width(al_minimap), al_get_bitmap_height(al_minimap));
        if (minimap) {
          minimap->bitmap = al_minimap;
        } else {
          al_destroy_bitmap(al_minimap);
        }
      }
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
  if (button[M_ONEPLAYER_PLAY]->get_flags() & MC_PUSHED)
  {
    check_level_depends();
    button[M_ONEPLAYER_PLAY]->del_flag(MC_PUSHED);
    if (!(button[M_ONEPLAYER_PLAY]->get_flags() & MC_INACTIVE))
    {
      gsound->menu_select();
      fade->initiate(FADE_TO_BLACK, 1, 60);
      switch_to_menu = START_GAME;
      button[M_ONEPLAYER_PLAY]->set_flag(MC_INVISIBLE);
      button[M_ONEPLAYER_PLAY]->do_explosion();
      move_out();
    }
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_ONEPLAYER_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    globals->singleplayer = 0;
    gsound->menu_select();
    switch_to_menu = MENU_MAIN;
    button[M_ONEPLAYER_BACK]->set_flag(MC_INVISIBLE);
    button[M_ONEPLAYER_BACK]->do_explosion();
    button[M_ONEPLAYER_BACK]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 choose ship
  if (button[M_ONEPLAYER_P1CHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 0;
    button[M_ONEPLAYER_P1CHOOSE]->set_flag(MC_INVISIBLE);
    button[M_ONEPLAYER_P1CHOOSE]->do_explosion();
    button[M_ONEPLAYER_P1CHOOSE]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 edit ship
  if (button[M_ONEPLAYER_P1EDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 0;
    button[M_ONEPLAYER_P1EDIT]->set_flag(MC_INVISIBLE);
    button[M_ONEPLAYER_P1EDIT]->do_explosion();
    button[M_ONEPLAYER_P1EDIT]->del_flag(MC_PUSHED);
    move_out();
  }

  // display missions
  if (button[M_ONEPLAYER_DISP_MISSIONS]->get_flags() & MC_PUSHED)
  {
    button[M_ONEPLAYER_DISP_MISSIONS]->del_flag(MC_PUSHED);
    if (button[M_ONEPLAYER_DISP_MISSIONS]->get_int() == 0)
    {
      gsound->menu_select();
      button[M_ONEPLAYER_DISP_MISSIONS]->set_int(1);
      button[M_ONEPLAYER_DISP_RACES]->set_int(0);
      script->read_level_files(0, "m");
      button[M_ONEPLAYER_LEVSELECT]->reinit();
      if (globals->max_levels <= 0) button[M_ONEPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);
      else button[M_ONEPLAYER_PLAY]->del_flag(MC_INACTIVE | MC_INVISIBLE);
    }
  }

  // display races
  if (button[M_ONEPLAYER_DISP_RACES]->get_flags() & MC_PUSHED)
  {
    button[M_ONEPLAYER_DISP_RACES]->del_flag(MC_PUSHED);
    if (button[M_ONEPLAYER_DISP_RACES]->get_int() == 0)
    {
      gsound->menu_select();
      button[M_ONEPLAYER_DISP_MISSIONS]->set_int(0);
      button[M_ONEPLAYER_DISP_RACES]->set_int(1);
      script->read_level_files(0, "r");
      button[M_ONEPLAYER_LEVSELECT]->reinit();
      if (globals->max_levels <= 0) button[M_ONEPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);
      else button[M_ONEPLAYER_PLAY]->del_flag(MC_INACTIVE | MC_INVISIBLE);
    }
  }

  // activate level extra?
  if (button[M_ONEPLAYER_LEVELEXTRA]->get_flags() & MC_ACTIVE)
  {
    if (button[M_ONEPLAYER_LEVELEXTRA]->get_flags() & MC_PUSHED)
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

  int move_ret = move();

  if (move_ret && switch_to_menu == START_GAME)
    if (gsound->get_music_volume() > 0) gsound->set_music_volume(gsound->get_music_volume()-4);

  if (move_ret == 0 && moving) return switch_to_menu;

  return 0;
}

void mOnePlayer::draw(BITMAP *v)
{
  if (globals->max_levels > 0)
  {
    // map description
    mytextout_center(v, (FONT *)globals->fontdat[FONT_IMPACT10].dat,
                        leveldesc,
                        (int)button[M_ONEPLAYER_LEVDESC]->get_x() + 5,
                        (int)button[M_ONEPLAYER_LEVDESC]->get_x() + button[M_ONEPLAYER_LEVDESC]->get_w(),
                        (int)button[M_ONEPLAYER_LEVDESC]->get_y()+5,
                        leveldesc_col);
  
    // map picture
    if (minimap)
    {
      int mx = (int)button[M_ONEPLAYER_LEVMINIMAP]->get_x();
      int my = (int)button[M_ONEPLAYER_LEVMINIMAP]->get_y();
      int mw = (int)button[M_ONEPLAYER_LEVMINIMAP]->get_w();
      int mh = (int)button[M_ONEPLAYER_LEVMINIMAP]->get_h();
  
      masked_blit(minimap, v, 0, 0, mx, my, mw, mh);

      // resolution desc
      mytextout_left(v, (FONT *)globals->fontdat[FONT_MINIFONT].dat,
                        resolution_text,
                        mx+1,
                        mx+mw,
                        my+mh+5,
                        leveldesc_col);      
    }
  }
  
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
}

int mOnePlayer::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mOnePlayer::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_ONEPLAYER_CAPTION]->set_dest_x( button[M_ONEPLAYER_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

