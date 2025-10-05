/*
 *   GRAVITY STRIKE -- menu/mmain.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mmain.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gssound.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsnetw.h"
#include "../gserror.h"
#include "gsmenu.h"

mMain::mMain()
{
  active_button = 0;
  switch_to_menu = -1;
  no_input = 0;
  button_count = 8;
  moving = 0;
  esc_pressed = 0;
  globals->network_game = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
    
//errors->log(2, "selected level", globals->last_splevel, globals->selected_level);
    
}

mMain::~mMain()
{
  globals->last_menu = MENU_MAIN;

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mMain::init(int scatter)
{
  int my = 0;

  button[M_MAIN_1PLAYER] = new mMainButton();
  if (scatter) button[M_MAIN_1PLAYER]->create(220, 115+0*40, -200, 115+0*40, 200, 40);
  else button[M_MAIN_1PLAYER]->create(220, 115+0*40, 0, 0, 200, 40);
  button[M_MAIN_1PLAYER]->set_text(language->get_string(T_MENU_SINGLEPLAYER));

  button[M_MAIN_2PLAYER] = new mMainButton();
  if (scatter) button[M_MAIN_2PLAYER]->create(220, 115+1*40,  640, 115+1*40, 200, 40);
  else button[M_MAIN_2PLAYER]->create(220, 115+1*40, 0, 0, 200, 40);
  button[M_MAIN_2PLAYER]->set_text(language->get_string(T_MENU_SPLITSCREEN));

  button[M_MAIN_NETGAME] = new mMainButton();
  if (scatter) button[M_MAIN_NETGAME]->create(220, 115+2*40, -200, 115+2*40, 200, 40);
  else button[M_MAIN_NETGAME]->create(220, 115+2*40, 0, 0, 200, 40);
  button[M_MAIN_NETGAME]->set_text(language->get_string(T_MENU_NETGAME));

//  if (!net->get_active())
  {
    button[M_MAIN_NETGAME]->set_flag(MC_INACTIVE | MC_INVISIBLE);
    my = 40;
  }

  button[M_MAIN_MINIGAME] = new mMainButton();
  if (scatter) button[M_MAIN_MINIGAME]->create(220, 115+3*40-my,  640, 115+3*40-my, 200, 40);
  else button[M_MAIN_MINIGAME]->create(220, 115+3*40-my, 0, 0, 200, 40);
  button[M_MAIN_MINIGAME]->set_text(language->get_string(T_MENU_MINIGAME));

  button[M_MAIN_SHIPBUILDER] = new mMainButton();
  if (scatter) button[M_MAIN_SHIPBUILDER]->create(220, 115+4*40-my, -200, 115+4*40-my, 200, 40);
  else button[M_MAIN_SHIPBUILDER]->create(220, 115+4*40-my, 0, 0, 200, 40);
  button[M_MAIN_SHIPBUILDER]->set_text(language->get_string(T_MENU_SHIPBUILDER));

  button[M_MAIN_OPTIONS] = new mMainButton();
  if (scatter) button[M_MAIN_OPTIONS]->create(220, 115+5*40-my, 640, 115+5*40-my, 200, 40);
  else button[M_MAIN_OPTIONS]->create(220, 115+5*40-my, 0, 0, 200, 40);
  button[M_MAIN_OPTIONS]->set_text(language->get_string(T_MENU_OPTIONS));

  button[M_MAIN_CREDITS] = new mMainButton();
  if (scatter) button[M_MAIN_CREDITS]->create(220, 115+6*40-my, 640, 115+6*40-my, 200, 40);
  else button[M_MAIN_CREDITS]->create(220, 115+6*40-my, 0, 0, 200, 40);
  button[M_MAIN_CREDITS]->set_text(language->get_string(T_MENU_CREDITS));
button[M_MAIN_CREDITS]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_MAIN_EXIT] = new mMainButton();
  if (scatter) button[M_MAIN_EXIT]->create(220, 115+6*40+30, 220, 700, 200, 40);
  else button[M_MAIN_EXIT]->create(220, 115+6*40+30, 0, 0, 200, 40);
  button[M_MAIN_EXIT]->set_text(language->get_string(T_MENU_QUIT));

//  button[M_MAIN_NETGAME]->set_flag(MC_INACTIVE);
}

int mMain::check()
{
  for (int n=0; n < button_count; n++)
  {
    if (!no_input) button[n]->check();
    if (button[n]->get_flags() & MC_ACTIVE)
      if (active_button != n)
      {
        active_button = n; gsound->menu_change();
      }
  }

  if (active_button >= 0)
    button[active_button]->set_flag(MC_ACTIVE);

  if (!no_input)
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
  } // if !no_input

  return(handle_controls());
}

int mMain::handle_controls()
{
  int quit = 0;

  // singleplayer match
  if (button[M_MAIN_1PLAYER]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_SINGLEPLAYER;
    move_out();
  }

  // twoplayer splitscreen
  if (button[M_MAIN_2PLAYER]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_SPLITSCREEN;
    move_out();
  }

  // metwork game
  if (button[M_MAIN_NETGAME]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_NETGAMEINI;
    move_out();
  }

  // mini game
  if (button[M_MAIN_MINIGAME]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_MINIGAME;
    move_out();
  }

  // options
  if (button[M_MAIN_OPTIONS]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_OPTIONS;
    move_out();
  }

  // choose ship (for ship builder)
  if (button[M_MAIN_SHIPBUILDER]->get_flags() & MC_PUSHED)
  {
    switch_to_menu = MENU_CHOOSESHIP;
    move_out();
  }

  // exit
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_MAIN_EXIT]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    move_out();
    quit = 1;
  }

  if (active_button >= 0 && !(button[active_button]->get_flags() & MC_INACTIVE))
    if (button[active_button]->get_flags() & MC_PUSHED)
    {
      gsound->menu_select();
      button[active_button]->del_flag(MC_PUSHED);
      button[active_button]->del_flag(MC_ACTIVE);
      button[active_button]->set_flag(MC_INVISIBLE);
      button[active_button]->set_flag(MC_INACTIVE);
      button[active_button]->do_explosion();
      active_button = -1;
    }

  if (move() == 0 && moving) return switch_to_menu;

  if (quit) return MENU_QUIT;

  return 0;
}

void mMain::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }

/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "mwheel", mouse_z), 10, 400, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "switch", switch_to_menu), 10, 410, makecol8(255,255,255));
*/
}

int mMain::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

//  if (r) moving = 1;

  return r;
}

void mMain::move_out()
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  moving = 1;
  no_input = 1;
}

