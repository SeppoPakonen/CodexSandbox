/*
 *   GRAVITY STRIKE -- menu/moptions.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

/* DONE (#2#): Option für Musiklautstärke */


#include "moptions.h"
#include "controls/mtypes.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gssound.h"
#include "../gsscript.h"
#include "../gsglob.h"
#include "../gserror.h"
#include "gsmd.h"
#include "gsmenu.h"
#include "allegttf.h"

mOptions::mOptions()
{
  active_button = 0;
  button_count = 9;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  esc_pressed = 0;
  lang_changed = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
}

mOptions::~mOptions()
{
  globals->last_menu = MENU_OPTIONS;

  if (lang_changed) script->load_text();

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mOptions::init(int scatter)
{
  button[M_OPTIONS_LANGUAGE] = new mArrowOption();
  if (scatter) button[M_OPTIONS_LANGUAGE]->create(245, 125, 245, -100, 150, 55, A_LEFTRIGHT);
  else button[M_OPTIONS_LANGUAGE]->create(245, 125, 0, 0, 150, 55, A_LEFTRIGHT);

  button[M_OPTIONS_PIXELS] = new mArrowOption();
  if (scatter) button[M_OPTIONS_PIXELS]->create(165, 200, -150, 200, 150, 55, A_LEFTRIGHT);
  else button[M_OPTIONS_PIXELS]->create(165, 200, 0, 0, 150, 55, A_LEFTRIGHT);

  button[M_OPTIONS_RESOLUTION] = new mArrowOption();
  if (scatter) button[M_OPTIONS_RESOLUTION]->create(325, 200, 800, 200, 150, 55, A_LEFTRIGHT);
  else button[M_OPTIONS_RESOLUTION]->create(325, 200, 0, 0, 150, 55, A_LEFTRIGHT);

  button[M_OPTIONS_VOLUME] = new mArrowOption();
  if (scatter) button[M_OPTIONS_VOLUME]->create(165, 275, -150, 275, 150, 55, A_LEFTRIGHT);
  else button[M_OPTIONS_VOLUME]->create(165, 275, 0, 0, 150, 55, A_LEFTRIGHT);

  button[M_OPTIONS_MVOLUME] = new mArrowOption();
  if (scatter) button[M_OPTIONS_MVOLUME]->create(325, 275, 800, 275, 150, 55, A_LEFTRIGHT);
  else button[M_OPTIONS_MVOLUME]->create(325, 275, 0, 0, 150, 55, A_LEFTRIGHT);

  button[M_OPTIONS_BACK] = new mMainButton();
  if (scatter) button[M_OPTIONS_BACK]->create(245, 430, 245, 500, 150, 30);
  else button[M_OPTIONS_BACK]->create(245, 430, 0, 0, 150, 30);

  button[M_OPTIONS_CONTROLS] = new mMainButton();
  if (scatter) button[M_OPTIONS_CONTROLS]->create(80, 350, -150, 350, 150, 30);
  else button[M_OPTIONS_CONTROLS]->create(80, 360, 0, 0, 150, 30);

  button[M_OPTIONS_2PCONTROLS] = new mMainButton();
  if (scatter) button[M_OPTIONS_2PCONTROLS]->create(250, 350, 250, 500, 150, 30);
  else button[M_OPTIONS_2PCONTROLS]->create(250, 360, 0, 0, 150, 30);

  button[M_OPTIONS_SPCONTROLS] = new mMainButton();
  if (scatter) button[M_OPTIONS_SPCONTROLS]->create(420, 350, 640, 350, 150, 30);
  else button[M_OPTIONS_SPCONTROLS]->create(420, 350, 0, 0, 150, 30);

  button[M_OPTIONS_LANGUAGE]->set_text(language->get_string(T_MENU_LANGUAGE));
  button[M_OPTIONS_PIXELS]->set_text(language->get_string(T_MENU_PIXAMOUNT));
  button[M_OPTIONS_RESOLUTION]->set_text(language->get_string(T_MENU_RESOLUTION));
  button[M_OPTIONS_VOLUME]->set_text(language->get_string(T_MENU_VOLUME));
  button[M_OPTIONS_MVOLUME]->set_text(language->get_string(T_MENU_MVOLUME));
  button[M_OPTIONS_BACK]->set_text(language->get_string(T_MENU_BACK));
  button[M_OPTIONS_CONTROLS]->set_text(language->get_string(T_MENU_CONTROLS));
  button[M_OPTIONS_2PCONTROLS]->set_text(language->get_string(T_MENU_2PCONTROLS));
  button[M_OPTIONS_SPCONTROLS]->set_text(language->get_string(T_MENU_SPCONTROLS));

  button[M_OPTIONS_CAPTION] = new mNormalText();
  button[M_OPTIONS_CAPTION]->create(286, 64, 800, 64);
  button[M_OPTIONS_CAPTION]->set_int(FONT_BANK);
  button[M_OPTIONS_CAPTION]->set_text(language->get_string(T_MENU_CAP_OPTIONS));
  button[M_OPTIONS_CAPTION]->set_int(globals->col_yellow);
  button[M_OPTIONS_CAPTION]->set_flag(MC_INACTIVE);
}

int mOptions::check()
{
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

int mOptions::handle_controls()
{
  // language
  if (button[M_OPTIONS_LANGUAGE]->get_arrow1()->get_flags() & MC_PUSHED)
  {
    globals->language--;
    gsound->menu_changebar();
    mouse_b = 0;
    lang_changed = 1;
  }
  else if (button[M_OPTIONS_LANGUAGE]->get_arrow2()->get_flags() & MC_PUSHED)
  {
    globals->language++;
    gsound->menu_changebar();
    mouse_b = 0;
    lang_changed = 1;
  }
  if (globals->language > 2) globals->language = 0;
  else if (globals->language < 0) globals->language = 2;

  // resolution
  if (button[M_OPTIONS_RESOLUTION]->get_arrow1()->get_flags() & MC_PUSHED)
  {
    globals->use_resolution_nr--;
    gsound->menu_changebar();
    mouse_b = 0;
  }
  else if (button[M_OPTIONS_RESOLUTION]->get_arrow2()->get_flags() & MC_PUSHED)
  {
    globals->use_resolution_nr++;
    gsound->menu_changebar();
    mouse_b = 0;
  }
  if (globals->use_resolution_nr > 4) globals->use_resolution_nr = 0;
  else if (globals->use_resolution_nr < 0) globals->use_resolution_nr = 4;

  // pixels
  if (button[M_OPTIONS_PIXELS]->get_arrow1()->get_flags() & MC_PUSHED)
    if (globals->pixamount >= 2) { globals->pixamount-=2; gsound->menu_changebar(); }
  if (button[M_OPTIONS_PIXELS]->get_arrow2()->get_flags() & MC_PUSHED)
    if (globals->pixamount <= 198) { globals->pixamount+=2; gsound->menu_changebar(); }

  button[M_OPTIONS_PIXELS]->get_bar()->set_int(globals->pixamount/2);

  // volume
  if (button[M_OPTIONS_VOLUME]->get_arrow1()->get_flags() & MC_PUSHED)
    if (globals->soundvol > 0) { globals->soundvol--; gsound->menu_changebar(); }
  if (button[M_OPTIONS_VOLUME]->get_arrow2()->get_flags() & MC_PUSHED)
    if (globals->soundvol < 100) { globals->soundvol++; gsound->menu_changebar(); }

  button[M_OPTIONS_VOLUME]->get_bar()->set_int(globals->soundvol);

  // music volume
  if (button[M_OPTIONS_MVOLUME]->get_arrow1()->get_flags() & MC_PUSHED)
    if (globals->musicvol > 0) { globals->musicvol--; gsound->menu_changebar(); gsound->set_music_volume(globals->musicvol*1.5); }
  if (button[M_OPTIONS_MVOLUME]->get_arrow2()->get_flags() & MC_PUSHED)
    if (globals->musicvol < 100) { globals->musicvol++; gsound->menu_changebar(); gsound->set_music_volume(globals->musicvol*1.5); }

  button[M_OPTIONS_MVOLUME]->get_bar()->set_int(globals->musicvol);

  // controls
  if (button[M_OPTIONS_CONTROLS]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_CONTROLS;
    gsound->menu_select();
    button[M_OPTIONS_CONTROLS]->set_flag(MC_INVISIBLE);
    button[M_OPTIONS_CONTROLS]->do_explosion();
    move_out();
  }

  // 2p controls
  if (button[M_OPTIONS_2PCONTROLS]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_2PCONTROLS;
    gsound->menu_select();
    button[M_OPTIONS_2PCONTROLS]->set_flag(MC_INVISIBLE);
    button[M_OPTIONS_2PCONTROLS]->do_explosion();
    move_out();
  }

  // Special controls
  if (button[M_OPTIONS_SPCONTROLS]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_SPCONTROLS;
    gsound->menu_select();
    button[M_OPTIONS_SPCONTROLS]->set_flag(MC_INVISIBLE);
    button[M_OPTIONS_SPCONTROLS]->do_explosion();
    move_out();
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_OPTIONS_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    switch_to_menu = MENU_MAIN;
    gsound->menu_select();
    button[M_OPTIONS_BACK]->set_flag(MC_INVISIBLE);
    button[M_OPTIONS_BACK]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  return 0;
}

void mOptions::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }

  // language
  int x = (int)button[M_OPTIONS_LANGUAGE]->get_bar()->get_x();
  int y = (int)button[M_OPTIONS_LANGUAGE]->get_bar()->get_y();
  int w = (int)button[M_OPTIONS_LANGUAGE]->get_bar()->get_w();
  int h = (int)button[M_OPTIONS_LANGUAGE]->get_bar()->get_h();

  char *text;

  switch (globals->language)
  {
    case LANG_DE : text = language->get_string(T_MENU_LANG_DE);
                   break;
    case LANG_EN : text = language->get_string(T_MENU_LANG_EN);
                   break;
    case LANG_CR : text = language->get_string(T_MENU_LANG_CR);
                   break;
    default      : text = language->get_string(T_MENU_LANG_EN);
                   break;
  }
  FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  int tlen = text_length(f, text);

  aatextout(v, f, text, x+w/2-tlen/2, y+h/2-text_height(f)/2, button[M_OPTIONS_LANGUAGE]->get_color2());


  // resolution
  x = (int)button[M_OPTIONS_RESOLUTION]->get_bar()->get_x();
  y = (int)button[M_OPTIONS_RESOLUTION]->get_bar()->get_y();
  w = (int)button[M_OPTIONS_RESOLUTION]->get_bar()->get_w();
  h = (int)button[M_OPTIONS_RESOLUTION]->get_bar()->get_h();

  char rtext[20];
  sprintf(rtext, "%d x %d", globals->game_resolution[globals->use_resolution_nr]);
  f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  tlen = text_length(f, rtext);

  aatextout(v, f, rtext, x+w/2-tlen/2, y+h/2-text_height(f)/2, button[M_OPTIONS_RESOLUTION]->get_color2());

/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "pixamount", globals->pixamount), 10, 400, makecol8(255,255,255));
*/
}

int mOptions::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mOptions::move_out()
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_OPTIONS_CAPTION]->set_dest_x( button[M_OPTIONS_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

