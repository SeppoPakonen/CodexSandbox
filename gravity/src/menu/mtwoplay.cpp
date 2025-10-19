/*
 *   GRAVITY STRIKE -- menu/mtwoplay.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mtwoplay.h"
#include "controls/mtypes.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsplayer.h"
#include "../gsfade.h"
#include "../gssound.h"
#include "../gsscript.h"
#include "../gsglob.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>

mTwoPlayer::mTwoPlayer()
{
  globals->players = 2;
  globals->minigame = 0;
  globals->singleplayer = 0;
  
  active_button = 2;
  button_count = 32;
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

  if (strlen(globals->last_leveltype) == 0) 
    strcpy(globals->last_leveltype, "m");
    
  script->read_level_files(0, globals->last_leveltype);
  globals->selected_level = globals->get_level_num_from_filename(globals->last_level);
}

mTwoPlayer::~mTwoPlayer()
{
  globals->last_menu = MENU_SPLITSCREEN;
  strcpy(globals->pplayer[0].pname, button[M_TWOPLAYER_P1NAME]->get_text());
  strcpy(globals->pplayer[1].pname, button[M_TWOPLAYER_P2NAME]->get_text());

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);

  if (pl[0])    delete(pl[0]);
  if (pl[1])    delete(pl[1]);

  if (minimap)  destroy_bitmap(minimap);
}

void mTwoPlayer::init(int scatter)
{
  button[M_TWOPLAYER_CAPTION] = new mNormalText();
  button[M_TWOPLAYER_CAPTION]->create(286, 64, 800, 64);
  button[M_TWOPLAYER_CAPTION]->set_int(FONT_BANK);
  button[M_TWOPLAYER_CAPTION]->set_text(language->get_string(T_MENU_CAP_SPLITSCREEN));
  button[M_TWOPLAYER_CAPTION]->set_int(globals->col_yellow);
  button[M_TWOPLAYER_CAPTION]->set_flag(MC_INACTIVE);

  button[M_TWOPLAYER_BACK] = new mMainButton();
  if (scatter) button[M_TWOPLAYER_BACK]->create(30, 430, 30, 500, 150, 30);
  else button[M_TWOPLAYER_BACK]->create(30, 430, 0, 0, 150, 30);
  button[M_TWOPLAYER_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_TWOPLAYER_PLAY] = new mMainButton();
  if (scatter) button[M_TWOPLAYER_PLAY]->create(450, 430, 450, 500, 150, 30);
  else button[M_TWOPLAYER_PLAY]->create(450, 430, 0, 0, 150, 30);
  button[M_TWOPLAYER_PLAY]->set_text(language->get_string(T_MENU_PLAY));
  button[M_TWOPLAYER_PLAY]->set_color1(globals->col_yellow);
  if (globals->max_levels <= 0) button[M_TWOPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_TWOPLAYER_P1NAME] = new mTextField();
  button[M_TWOPLAYER_P1NAME]->create(30, 5+100, -300, 5+100, 250, 15, 100);
  button[M_TWOPLAYER_P1NAME]->set_int(20);
  button[M_TWOPLAYER_P1NAME]->set_text(language->get_string(T_MENU_NAME_P1));
  button[M_TWOPLAYER_P1NAME]->set_text(globals->pplayer[0].pname);

  button[M_TWOPLAYER_P1SHIP] = new mShipIcon2();
  button[M_TWOPLAYER_P1SHIP]->create(30, 5+125, -300, 5+125, 90, 51);
  button[M_TWOPLAYER_P1SHIP]->set_int(1);
  button[M_TWOPLAYER_P1SHIP]->set_player(pl[0]);

  button[M_TWOPLAYER_P1CHOOSE] = new mSmallButton();
  button[M_TWOPLAYER_P1CHOOSE]->create(130, 5+125, -300, 5+125, 150, 15);
  button[M_TWOPLAYER_P1CHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_TWOPLAYER_P1EDIT] = new mSmallButton();
  button[M_TWOPLAYER_P1EDIT]->create(130, 5+145, -300, 5+145, 150, 15);
  button[M_TWOPLAYER_P1EDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));
  char filename[20];
  strncpy(filename, globals->pplayer[0].pship, 20); strupr(filename);
  if (strstr(filename, "DEF1.SHP") || strstr(filename, "DEF2.SHP") || strstr(filename, "DEF3.SHP"))
    button[M_TWOPLAYER_P1EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_TWOPLAYER_P2NAME] = new mTextField();
  button[M_TWOPLAYER_P2NAME]->create(320+30, 5+100, 640, 5+100, 250, 15, 100);
  button[M_TWOPLAYER_P2NAME]->set_int(20);
  button[M_TWOPLAYER_P2NAME]->set_text(language->get_string(T_MENU_NAME_P2));
  button[M_TWOPLAYER_P2NAME]->set_text(globals->pplayer[1].pname);

  button[M_TWOPLAYER_P2SHIP] = new mShipIcon2();
  button[M_TWOPLAYER_P2SHIP]->create(320+30, 5+125, 640, 5+125, 90, 51);
  button[M_TWOPLAYER_P2SHIP]->set_int(1);
  button[M_TWOPLAYER_P2SHIP]->set_player(pl[1]);

  button[M_TWOPLAYER_P2CHOOSE] = new mSmallButton();
  button[M_TWOPLAYER_P2CHOOSE]->create(320+130, 5+125, 640, 5+125, 150, 15);
  button[M_TWOPLAYER_P2CHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_TWOPLAYER_P2EDIT] = new mSmallButton();
  button[M_TWOPLAYER_P2EDIT]->create(320+130, 5+145, 640, 5+145, 150, 15);
  button[M_TWOPLAYER_P2EDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));
  strncpy(filename, globals->pplayer[1].pship, 20); strupr(filename);
  if (strstr(filename, "DEF1.SHP") || strstr(filename, "DEF2.SHP") || strstr(filename, "DEF3.SHP"))
    button[M_TWOPLAYER_P2EDIT]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_TWOPLAYER_DISP_DOGFIGHTS] = new mSmallButton();
  button[M_TWOPLAYER_DISP_DOGFIGHTS]->create(30, 200, -300, 200, 82, 16);
  button[M_TWOPLAYER_DISP_DOGFIGHTS]->set_text(language->get_string(T_MENU_DISP_DOGFIGHTS));
  if (!strcmp(globals->last_leveltype, "m"))
    button[M_TWOPLAYER_DISP_DOGFIGHTS]->set_int(1);

  button[M_TWOPLAYER_DISP_RACES] = new mSmallButton();
  button[M_TWOPLAYER_DISP_RACES]->create(205-82, 200, -220, 200, 82, 16);
  button[M_TWOPLAYER_DISP_RACES]->set_text(language->get_string(T_MENU_DISP_RACES));
  if (!strcmp(globals->last_leveltype, "r"))
    button[M_TWOPLAYER_DISP_RACES]->set_int(1);

  button[M_TWOPLAYER_LEVSELECT] = new mLevelSelect();
  button[M_TWOPLAYER_LEVSELECT]->create(30, 220, -300, 200, 175, 108);
  button[M_TWOPLAYER_LEVSELECT]->set_int(globals->selected_level);

  button[M_TWOPLAYER_AUTHOR] = new mTextField();
  button[M_TWOPLAYER_AUTHOR]->create(212, 200, 640+100, 200, 265, 15, 80);
  button[M_TWOPLAYER_AUTHOR]->set_int(0);
  button[M_TWOPLAYER_AUTHOR]->set_text(language->get_string(T_MENU_CAP_AUTHOR));
  button[M_TWOPLAYER_AUTHOR]->set_flag(MC_INACTIVE);

  button[M_TWOPLAYER_LEVDESC] = new mBlank();
  button[M_TWOPLAYER_LEVDESC]->create(212, 220, 640, 220, 265, 70, 0);

  button[M_TWOPLAYER_LEVELEXTRA] = new mCheckBox();
  button[M_TWOPLAYER_LEVELEXTRA]->create(212, 296+20, -300-200, 297+20, 17, 12);
  button[M_TWOPLAYER_LEVELEXTRA]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_LEVELEXTRA]->set_text(language->get_string(T_MENU_LEVELEXTRA));
  if (globals->activate_extra)
    button[M_TWOPLAYER_LEVELEXTRA]->set_flag(MC_PUSHED);

  button[M_TWOPLAYER_CTF] = new mCheckBox();
  button[M_TWOPLAYER_CTF]->create(347, 278+20,  640, 278+20, 17, 12);
  button[M_TWOPLAYER_CTF]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_CTF]->set_text(language->get_string(T_MENU_CAPTURE_THE_FLAG));
  if (globals->capture_the_flag)
    button[M_TWOPLAYER_CTF]->set_flag(MC_PUSHED);

  button[M_TWOPLAYER_HH] = new mCheckBox();
  button[M_TWOPLAYER_HH]->create(212, 278+20, -300-200, 278+20, 17, 12);
  button[M_TWOPLAYER_HH]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_HH]->set_text(language->get_string(T_MENU_HUNTER_HUNTED));
  if (globals->hunter_hunted)
    button[M_TWOPLAYER_HH]->set_flag(MC_PUSHED);

  button[M_TWOPLAYER_RACE] = new mCheckBox();
  button[M_TWOPLAYER_RACE]->create(347, 296, -300-200, 296, 17, 12);
  button[M_TWOPLAYER_RACE]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_RACE]->set_text(language->get_string(T_STATS_MODE_RACE));
  button[M_TWOPLAYER_RACE]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_TWOPLAYER_LEVMINIMAP] = new mBlank();
  button[M_TWOPLAYER_LEVMINIMAP]->create(484, 200, 640+280, 200, 115, 115, 0);

  int v = 10;

  button[M_TWOPLAYER_OPT_GRAVITY] = new mTextSlider();
  button[M_TWOPLAYER_OPT_GRAVITY]->set_int(1);
  button[M_TWOPLAYER_OPT_GRAVITY]->create(30, 350-15+v, -400, 350-15, 220, 14, 90);
  button[M_TWOPLAYER_OPT_GRAVITY]->set_text(language->get_string(T_MENU_GRAVITY));
  button[M_TWOPLAYER_OPT_GRAVITY]->get_bar()->set_int((int(globals->gravity_percent * 100.0) + 100) / 3);

  button[M_TWOPLAYER_OPT_GRAVITY_TXT] = new mNormalText();
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->create(260, 350-15+v, -400-30, 350-15);
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->set_int(FONT_IMPACT10);
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->set_text("n/a");
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->set_int(globals->col_green);
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->set_flag(MC_INACTIVE);

  button[M_TWOPLAYER_OPT_MAXFRAGS] = new mTextSlider();
  button[M_TWOPLAYER_OPT_MAXFRAGS]->set_int(1);
  button[M_TWOPLAYER_OPT_MAXFRAGS]->create(330, 350-15+v, 640, 350-15, 220, 14, 90);
  button[M_TWOPLAYER_OPT_MAXFRAGS]->set_text(language->get_string(T_MENU_MAXFRAGS));
  button[M_TWOPLAYER_OPT_MAXFRAGS]->get_bar()->set_int(globals->max_frags*2);

  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT] = new mNormalText();
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->create(560, 350-15+v, 640, 350-15);
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_int(FONT_IMPACT10);
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_text("n/a");
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_int(globals->col_green);
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_flag(MC_INACTIVE);

  button[M_TWOPLAYER_OPT_EXTRAS] = new mTextSlider();
  button[M_TWOPLAYER_OPT_EXTRAS]->set_int(1);
  button[M_TWOPLAYER_OPT_EXTRAS]->create(30, 370-15+v, -400, 370-15, 220, 14, 90);
  button[M_TWOPLAYER_OPT_EXTRAS]->set_text(language->get_string(T_MENU_EXTRAS));
  button[M_TWOPLAYER_OPT_EXTRAS]->get_bar()->set_int(globals->extra_amount * 10);

  button[M_TWOPLAYER_OPT_EXTRAS_TXT] = new mNormalText();
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->create(260, 370-15+v, -400-30, 370-15);
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->set_int(FONT_IMPACT10);
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->set_text("n/a");
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->set_int(globals->col_green);
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->set_flag(MC_INACTIVE);

  button[M_TWOPLAYER_OPT_MAXTIME] = new mTextSlider();
  button[M_TWOPLAYER_OPT_MAXTIME]->set_int(1);
  button[M_TWOPLAYER_OPT_MAXTIME]->create(330, 370-15+v, 640, 370-15, 220, 14, 90);
  button[M_TWOPLAYER_OPT_MAXTIME]->set_text(language->get_string(T_MENU_MAXTIME));
  button[M_TWOPLAYER_OPT_MAXTIME]->get_bar()->set_int((globals->max_time)/30);

  button[M_TWOPLAYER_OPT_MAXTIME_TXT] = new mNormalText();
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->create(560, 370-15+v, 640, 370-15);
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_int(FONT_IMPACT10);
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_text("n/a");
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_int(globals->col_green);
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_flag(MC_INACTIVE);


  button[M_TWOPLAYER_COMPEN] = new mCheckBox();
  button[M_TWOPLAYER_COMPEN]->create(30, 380+v,  640, 385, 17, 12);
  button[M_TWOPLAYER_COMPEN]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_COMPEN]->set_text(language->get_string(T_MENU_COMPUTER_PLAYER));
  button[M_TWOPLAYER_COMPEN]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_TWOPLAYER_COMPEN_STRENGTH] = new mTextSlider();
  button[M_TWOPLAYER_COMPEN_STRENGTH]->set_int(1);
  button[M_TWOPLAYER_COMPEN_STRENGTH]->create(330, 380+v, 640, 385, 220, 14, 90);
  button[M_TWOPLAYER_COMPEN_STRENGTH]->set_text(language->get_string(T_MENU_COMPUTER_PLAYER_STRENGTH));
  button[M_TWOPLAYER_COMPEN_STRENGTH]->get_bar()->set_int(globals->computer_player_strength*10);
  button[M_TWOPLAYER_COMPEN_STRENGTH]->set_flag(MC_INVISIBLE | MC_INACTIVE);
  
  
  button[M_TWOPLAYER_HANDICAP] = new mCheckBox();
  button[M_TWOPLAYER_HANDICAP]->create(30, 398+v,  640, 385, 17, 12);
  button[M_TWOPLAYER_HANDICAP]->set_int(-1); // align box on left side
  button[M_TWOPLAYER_HANDICAP]->set_text(language->get_string(T_MENU_HANDICAP));
  
}

int mTwoPlayer::check()
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

int mTwoPlayer::handle_controls()
{
  int i;
  int changed = 0;

  // check if level changed
  i = button[M_TWOPLAYER_LEVSELECT]->get_int();
  if (i >= 0 && !no_input)
  {
    if (moved_in) gsound->menu_select();
      else moved_in = TRUE;

    globals->selected_level = i;
    strncpy(globals->last_level, globals->level_files[i].levelfile, 20);
    if (globals->level_files[i].activate_race)
      strcpy(globals->last_leveltype, "r");
    else
      strcpy(globals->last_leveltype, "m");

    globals->race = 0;
    globals->hunter_hunted = 0;
    globals->capture_the_flag = 0;
    globals->activate_extra = 0;

    if (strlen(globals->level_files[globals->selected_level].activate_extra_str) > 1)
    {
      button[M_TWOPLAYER_LEVELEXTRA]->del_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_LEVELEXTRA]->del_flag(MC_INACTIVE);
      button[M_TWOPLAYER_LEVELEXTRA]->set_text(globals->level_files[globals->selected_level].activate_extra_str);
    }
    else
    {
      button[M_TWOPLAYER_LEVELEXTRA]->set_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_LEVELEXTRA]->set_flag(MC_INACTIVE);
      globals->activate_extra = 0;
    }

    button[M_TWOPLAYER_AUTHOR]->set_text(globals->level_files[globals->selected_level].author);

    if (globals->level_files[globals->selected_level].activate_ctf == 1)
    {
      button[M_TWOPLAYER_CTF]->del_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_CTF]->del_flag(MC_INACTIVE);
      button[M_TWOPLAYER_CTF]->del_flag(MC_PUSHED);
      globals->capture_the_flag = 0;
    }
    else
    {
      button[M_TWOPLAYER_CTF]->set_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_CTF]->set_flag(MC_INACTIVE);
      button[M_TWOPLAYER_CTF]->del_flag(MC_PUSHED);
      globals->capture_the_flag = 0;
    }

    if (globals->level_files[globals->selected_level].activate_hh == 1)
    {
      button[M_TWOPLAYER_HH]->del_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_HH]->del_flag(MC_INACTIVE);
      button[M_TWOPLAYER_HH]->del_flag(MC_PUSHED);
      globals->hunter_hunted = 0;
    }
    else
    {
      button[M_TWOPLAYER_HH]->set_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_HH]->set_flag(MC_INACTIVE);
      globals->hunter_hunted = 0;
    }

    if (globals->level_files[globals->selected_level].activate_race == 1)
    {
      globals->race = 1;
      button[M_TWOPLAYER_OPT_EXTRAS]->set_text(language->get_string(T_MENU_LAPS));
      button[M_TWOPLAYER_OPT_MAXFRAGS]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_OPT_MAXTIME]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_flag(MC_INVISIBLE);
    }
    else
    {
      globals->race = 0;
      button[M_TWOPLAYER_OPT_EXTRAS]->set_text(language->get_string(T_MENU_EXTRAS));
      button[M_TWOPLAYER_OPT_MAXFRAGS]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_OPT_MAXTIME]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->del_flag(MC_INVISIBLE);
      button[M_TWOPLAYER_OPT_MAXTIME_TXT]->del_flag(MC_INVISIBLE);
    }

    // computer player available?
    changed = 1;

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
  if (button[M_TWOPLAYER_PLAY]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    fade->initiate(FADE_TO_BLACK, 1, 60);
    switch_to_menu = START_GAME;
    button[M_TWOPLAYER_PLAY]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_PLAY]->do_explosion();
    button[M_TWOPLAYER_PLAY]->del_flag(MC_PUSHED);
    move_out();
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_TWOPLAYER_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_MAIN;
    button[M_TWOPLAYER_BACK]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_BACK]->do_explosion();
    button[M_TWOPLAYER_BACK]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 choose ship
  if (button[M_TWOPLAYER_P1CHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 0;
    button[M_TWOPLAYER_P1CHOOSE]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_P1CHOOSE]->do_explosion();
    button[M_TWOPLAYER_P1CHOOSE]->del_flag(MC_PUSHED);
    move_out();
  }

  // p2 choose ship
  if (button[M_TWOPLAYER_P2CHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 1;
    button[M_TWOPLAYER_P2CHOOSE]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_P2CHOOSE]->do_explosion();
    button[M_TWOPLAYER_P2CHOOSE]->del_flag(MC_PUSHED);
    move_out();
  }

  // p1 edit ship
  if (button[M_TWOPLAYER_P1EDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 0;
    button[M_TWOPLAYER_P1EDIT]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_P1EDIT]->do_explosion();
    button[M_TWOPLAYER_P1EDIT]->del_flag(MC_PUSHED);
    move_out();
  }

  // p2 edit ship
  if (button[M_TWOPLAYER_P2EDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 1;
    button[M_TWOPLAYER_P2EDIT]->set_flag(MC_INVISIBLE);
    button[M_TWOPLAYER_P2EDIT]->do_explosion();
    button[M_TWOPLAYER_P2EDIT]->del_flag(MC_PUSHED);
    move_out();
  }
  
  // display dogfights
  if (button[M_TWOPLAYER_DISP_DOGFIGHTS]->get_flags() & MC_PUSHED)
  {
    button[M_TWOPLAYER_DISP_DOGFIGHTS]->del_flag(MC_PUSHED);
    if (button[M_TWOPLAYER_DISP_DOGFIGHTS]->get_int() == 0)
    {
      gsound->menu_select();
      button[M_TWOPLAYER_DISP_DOGFIGHTS]->set_int(1);
      button[M_TWOPLAYER_DISP_RACES]->set_int(0);
      script->read_level_files(0, "m");
      button[M_TWOPLAYER_LEVSELECT]->reinit();
      if (globals->max_levels <= 0) button[M_TWOPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);
      else button[M_TWOPLAYER_PLAY]->del_flag(MC_INACTIVE | MC_INVISIBLE);
    }
  }

  // display races
  if (button[M_TWOPLAYER_DISP_RACES]->get_flags() & MC_PUSHED)
  {
    button[M_TWOPLAYER_DISP_RACES]->del_flag(MC_PUSHED);
    if (button[M_TWOPLAYER_DISP_RACES]->get_int() == 0)
    {
      gsound->menu_select();
      button[M_TWOPLAYER_DISP_DOGFIGHTS]->set_int(0);
      button[M_TWOPLAYER_DISP_RACES]->set_int(1);
      script->read_level_files(0, "r");
      button[M_TWOPLAYER_LEVSELECT]->reinit();
      if (globals->max_levels <= 0) button[M_TWOPLAYER_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);
      else button[M_TWOPLAYER_PLAY]->del_flag(MC_INACTIVE | MC_INVISIBLE);
    }
  }

  // activate level extra?
  if (button[M_TWOPLAYER_LEVELEXTRA]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_LEVELEXTRA]->get_flags() & MC_PUSHED)
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

  // activate ctf?
  if (button[M_TWOPLAYER_CTF]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_CTF]->get_flags() & MC_PUSHED)
    {
      button[M_TWOPLAYER_HH]->del_flag(MC_PUSHED);
      button[M_TWOPLAYER_RACE]->del_flag(MC_PUSHED);
      globals->hunter_hunted = 0;
      globals->capture_the_flag = 1;
      globals->race = 0;
      mouse_b = 0;
      changed = 1;
    }
    else
    {
      globals->capture_the_flag = 0;
      mouse_b = 0;
      changed = 1;
    }
  }

  // activate hh?
  if (button[M_TWOPLAYER_HH]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_HH]->get_flags() & MC_PUSHED)
    {
      button[M_TWOPLAYER_CTF]->del_flag(MC_PUSHED);
      button[M_TWOPLAYER_RACE]->del_flag(MC_PUSHED);
      globals->capture_the_flag = 0;
      globals->hunter_hunted = 1;
      globals->race = 0;
      mouse_b = 0;
      changed = 1;
    }
    else
    {
      globals->hunter_hunted = 0;
      mouse_b = 0;
      changed = 1;
    }
  }

  // activate race?
  if (button[M_TWOPLAYER_RACE]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_RACE]->get_flags() & MC_PUSHED)
    {
      button[M_TWOPLAYER_CTF]->del_flag(MC_PUSHED);
      button[M_TWOPLAYER_HH]->del_flag(MC_PUSHED);
      globals->capture_the_flag = 0;
      globals->hunter_hunted = 0;
      globals->race = 1;
      mouse_b = 0;
    }
    else
    {
      globals->race = 0;
      mouse_b = 0;
    }
  }

  // activate computer player?
  if (button[M_TWOPLAYER_COMPEN]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_COMPEN]->get_flags() & MC_PUSHED)
    {
      globals->computer_player = 1;
      mouse_b = 0;
      changed = 1;
    }
    else
    {
      globals->computer_player = 0;
      mouse_b = 0;
      changed = 1;
    }
  }

  // activate handicap mode?
  if (button[M_TWOPLAYER_HANDICAP]->get_flags() & MC_ACTIVE)
  {
    if (button[M_TWOPLAYER_HANDICAP]->get_flags() & MC_PUSHED)
    {
      globals->handicap = 1;
      mouse_b = 0;
      changed = 1;
    }
    else
    {
      globals->handicap = 0;
      mouse_b = 0;
      changed = 1;
    }
  }

  if (changed) // complay, hh or ctf activated/deactivated
  {
    if (!globals->race && !globals->capture_the_flag)
    {
      button[M_TWOPLAYER_COMPEN]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      if (globals->computer_player)
      {
        button[M_TWOPLAYER_COMPEN]->set_flag(MC_PUSHED);
        button[M_TWOPLAYER_COMPEN_STRENGTH]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      }
      else
      {
        button[M_TWOPLAYER_COMPEN]->del_flag(MC_PUSHED);
        button[M_TWOPLAYER_COMPEN_STRENGTH]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      }
      
      button[M_TWOPLAYER_HANDICAP]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      if (globals->handicap)
        button[M_TWOPLAYER_HANDICAP]->set_flag(MC_PUSHED);
      else
        button[M_TWOPLAYER_HANDICAP]->del_flag(MC_PUSHED);
    }
    else
    {
      button[M_TWOPLAYER_COMPEN]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_COMPEN]->del_flag(MC_PUSHED);
      button[M_TWOPLAYER_COMPEN_STRENGTH]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      globals->computer_player = 0;

      button[M_TWOPLAYER_HANDICAP]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_TWOPLAYER_HANDICAP]->del_flag(MC_PUSHED);
      globals->handicap = 0;
    }
  }

  // gravity
  static float f = button[M_TWOPLAYER_OPT_GRAVITY]->get_bar()->get_int();
  if (button[M_TWOPLAYER_OPT_GRAVITY]->get_arrow1()->get_flags() & MC_PUSHED)
    if (f > 0)  { f-=0.2; globals->gravity_percent = (f*3 - 100)/100.0; gsound->menu_changebar(); }
  if (button[M_TWOPLAYER_OPT_GRAVITY]->get_arrow2()->get_flags() & MC_PUSHED)
    if (f < 100)  { f+=0.2; globals->gravity_percent = (f*3 - 100)/100.0; gsound->menu_changebar(); }
  button[M_TWOPLAYER_OPT_GRAVITY]->get_bar()->set_int(int(f));

  char tmps[10];
  sprintf(tmps, "%.0f%%", globals->gravity_percent*100.0);
  button[M_TWOPLAYER_OPT_GRAVITY_TXT]->set_text(tmps);

  // extras
  i = button[M_TWOPLAYER_OPT_EXTRAS]->get_bar()->get_int();
  if (button[M_TWOPLAYER_OPT_EXTRAS]->get_arrow1()->get_flags() & MC_PUSHED)
    if (i > 0)  { i-=10; globals->extra_amount = i / 10; gsound->menu_changebar(); mouse_b = 0; }
  if (button[M_TWOPLAYER_OPT_EXTRAS]->get_arrow2()->get_flags() & MC_PUSHED)
    if (i < 100)  { i+=10; globals->extra_amount = i / 10; gsound->menu_changebar(); mouse_b = 0; }
  button[M_TWOPLAYER_OPT_EXTRAS]->get_bar()->set_int(i);

  sprintf(tmps, "%d", globals->extra_amount);
  button[M_TWOPLAYER_OPT_EXTRAS_TXT]->set_text(tmps);

  // max frags
  i = button[M_TWOPLAYER_OPT_MAXFRAGS]->get_bar()->get_int();
  if (button[M_TWOPLAYER_OPT_MAXFRAGS]->get_arrow1()->get_flags() & MC_PUSHED)
    if (i > 0)  { i-=2; globals->max_frags = (i / 2)+1; gsound->menu_changebar(); mouse_b = 0; }
  if (button[M_TWOPLAYER_OPT_MAXFRAGS]->get_arrow2()->get_flags() & MC_PUSHED)
    if (i < 100)  { i+=2; globals->max_frags = (i / 2)+1; gsound->menu_changebar(); mouse_b = 0; }
  button[M_TWOPLAYER_OPT_MAXFRAGS]->get_bar()->set_int(i);

  sprintf(tmps, "%d", globals->max_frags);
  button[M_TWOPLAYER_OPT_MAXFRAGS_TXT]->set_text(tmps);

  // max time
  i = button[M_TWOPLAYER_OPT_MAXTIME]->get_bar()->get_int();
  if (button[M_TWOPLAYER_OPT_MAXTIME]->get_arrow1()->get_flags() & MC_PUSHED)
    if (i > 0)  { i-=1; globals->max_time = (i*30); gsound->menu_changebar(); mouse_b = 0; }
  if (button[M_TWOPLAYER_OPT_MAXTIME]->get_arrow2()->get_flags() & MC_PUSHED)
    if (i < 100)  { i+=1; globals->max_time = (i*30); gsound->menu_changebar(); mouse_b = 0; }
  button[M_TWOPLAYER_OPT_MAXTIME]->get_bar()->set_int(i);

  sprintf(tmps, "%.1f min", globals->max_time/60.0);
  button[M_TWOPLAYER_OPT_MAXTIME_TXT]->set_text(tmps);

  // computer player strength
  i = button[M_TWOPLAYER_COMPEN_STRENGTH]->get_bar()->get_int();
  if (button[M_TWOPLAYER_COMPEN_STRENGTH]->get_arrow1()->get_flags() & MC_PUSHED)
    if (i > 0)  { i-=10; globals->computer_player_strength = i / 10; gsound->menu_changebar(); mouse_b = 0; }
  if (button[M_TWOPLAYER_COMPEN_STRENGTH]->get_arrow2()->get_flags() & MC_PUSHED)
    if (i < 100)  { i+=10; globals->computer_player_strength = i / 10; gsound->menu_changebar(); mouse_b = 0; }
  button[M_TWOPLAYER_COMPEN_STRENGTH]->get_bar()->set_int(i);


  int move_ret = move();

  if (move_ret && switch_to_menu == START_GAME)
    if (gsound->get_music_volume() > 0) gsound->set_music_volume(gsound->get_music_volume()-4);

  if (move_ret == 0 && moving) return switch_to_menu;

  return 0;
}

void mTwoPlayer::draw(BITMAP *v)
{
  if (globals->max_levels > 0)
  {
    // map description
    mytextout_center(v, (FONT *)globals->fontdat[FONT_IMPACT10].dat,
                        globals->level_files[globals->selected_level].desc,
                        (int)button[M_TWOPLAYER_LEVDESC]->get_x() + 8,
                        (int)button[M_TWOPLAYER_LEVDESC]->get_x() + button[M_TWOPLAYER_LEVDESC]->get_w() - 8,
                        (int)button[M_TWOPLAYER_LEVDESC]->get_y()+5,
                        globals->col_yellow);
  
    // map picture
    if (minimap)
    {
      int mx = (int)button[M_TWOPLAYER_LEVMINIMAP]->get_x();
      int my = (int)button[M_TWOPLAYER_LEVMINIMAP]->get_y();
      int mw = (int)button[M_TWOPLAYER_LEVMINIMAP]->get_w();
      int mh = (int)button[M_TWOPLAYER_LEVMINIMAP]->get_h();
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

int mTwoPlayer::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mTwoPlayer::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_TWOPLAYER_CAPTION]->set_dest_x( button[M_TWOPLAYER_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

