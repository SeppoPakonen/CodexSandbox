/*
 *   GRAVITY STRIKE -- menu/mctrlsp.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mctrlsp.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gscontrl.h"
#include "../gserror.h"
#include "../gslng.h"
#include "../gssound.h"
#include "gsmd.h"
#include "gsmenu.h"

extern gsControl *controls;

mPControlsSP::mPControlsSP()
{
  active_button = 0;
  button_count = 7;
  moving = 0;
  switch_to_menu = -1;
  wait_for_key = -1;
  esc_pressed = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
}

mPControlsSP::~mPControlsSP()
{
  globals->last_menu = MENU_CONTROLS;

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mPControlsSP::init(int scatter)
{
  int n;
  for (n=0; n < 5; n++)
  {
    button[M_CONTROLSP_PAUSE+n] = new mKeyboardOption();
    if (scatter) button[M_CONTROLSP_PAUSE+n]->create(200, 115+n*40, -200, 115+n*40, 240, 25);
    else button[M_CONTROLSP_PAUSE+n]->create(200, 115+n*40, 0, 0, 240, 25);
  }

  button[M_CONTROLSP_BACK] = new mMainButton();
  if (scatter) button[M_CONTROLSP_BACK]->create(245, 430, 245, 500, 150, 30);
  else button[M_CONTROLSP_BACK]->create(245, 430, 0, 0, 150, 30);

  button[M_CONTROLSP_PAUSE]->set_text(language->get_string(T_CONTROL_PAUSE));
  button[M_CONTROLSP_PAUSE]->set_int(controls->get_key(SP_PAUSE));
  button[M_CONTROLSP_SCREENSHOT]->set_text(language->get_string(T_CONTROL_SCREENSHOT));
  button[M_CONTROLSP_SCREENSHOT]->set_int(controls->get_key(SP_SCREENSHOT));
  button[M_CONTROLSP_PANEL]->set_text(language->get_string(T_CONTROL_PANEL));
  button[M_CONTROLSP_PANEL]->set_int(controls->get_key(SP_PANEL));
  button[M_CONTROLSP_DEBUG]->set_text(language->get_string(T_CONTROL_DEBUG));
  button[M_CONTROLSP_DEBUG]->set_int(controls->get_key(SP_DEBUG));
  button[M_CONTROLSP_CHAT]->set_text(language->get_string(T_CONTROL_CHAT));
  button[M_CONTROLSP_CHAT]->set_int(controls->get_key(SP_CHAT));

  button[M_CONTROLSP_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_CONTROLSP_CAPTION] = new mNormalText();
  button[M_CONTROLSP_CAPTION]->create(286, 64, 800, 64);
  button[M_CONTROLSP_CAPTION]->set_int(FONT_BANK);
  button[M_CONTROLSP_CAPTION]->set_text(language->get_string(T_MENU_CAP_SPCONTROLS));
  button[M_CONTROLSP_CAPTION]->set_int(globals->col_yellow);
  button[M_CONTROLSP_CAPTION]->set_flag(MC_INACTIVE);
}

int mPControlsSP::check()
{
/*
  if (active_button >= 0)
    if (button[active_button]->get_flags() & MC_PUSHED)
    {
      button[active_button]->del_flag(MC_PUSHED);
    }
*/

  for (int n=0; n < button_count-1; n++)
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

  if (!moving && wait_for_key < 0)
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
      {
        button[active_button]->set_flag(MC_PUSHED);
      }
      clear_key(KEY_ENTER);
    }
  }

  return(handle_controls());
}

int mPControlsSP::handle_controls()
{
  wait_for_key = -1;

  if (active_button < M_CONTROLSP_BACK &&
      button[active_button]->get_flags() & MC_PUSHED)
  {
    wait_for_key = active_button;
    int k = get_pressed_key();
    if (k)
    {
      gsound->menu_select();
      wait_for_key = -1;
      int key_to_change = 0;

      switch (active_button)
      {
        case M_CONTROLSP_PAUSE       : key_to_change = SP_PAUSE; break;
        case M_CONTROLSP_SCREENSHOT  : key_to_change = SP_SCREENSHOT; break;
        case M_CONTROLSP_PANEL       : key_to_change = SP_PANEL; break;
        case M_CONTROLSP_DEBUG       : key_to_change = SP_DEBUG; break;
        case M_CONTROLSP_CHAT        : key_to_change = SP_CHAT; break;
      }

      controls->set_key((player_controls)key_to_change, k);
      button[active_button]->set_int(controls->get_key((player_controls)key_to_change));
      button[active_button]->del_flag(MC_PUSHED);
    }
  }

  for (int n=0; n < button_count-2; n++)
  {
    if (n != wait_for_key && button[n]->get_flags() & MC_PUSHED)
      button[n]->del_flag(MC_PUSHED);
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_CONTROLSP_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_OPTIONS;
    button[M_CONTROLSP_BACK]->set_flag(MC_INVISIBLE);
    button[M_CONTROLSP_BACK]->del_flag(MC_PUSHED);
    button[M_CONTROLSP_BACK]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  clear_keybuf();

  return 0;
}

void mPControlsSP::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "waitkey", wait_for_key), 10, 400, makecol8(255,255,255));
*/
}

int mPControlsSP::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mPControlsSP::move_out()
{
  for (int n=0; n < button_count-1; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_CONTROLSP_CAPTION]->set_dest_x( button[M_CONTROLSP_CAPTION]->get_x() + 400 );

  moving = 1;
}

int mPControlsSP::get_pressed_key()
{
  int k = 0;

  if (wait_for_key >= 0)
  {
    if (key[KEY_LSHIFT]) return KEY_LSHIFT;
    if (key[KEY_RSHIFT]) return KEY_RSHIFT;
    if (key[KEY_ALT]) return KEY_ALT;
    if (key[KEY_ALTGR]) return KEY_ALTGR;
    if (key[KEY_LCONTROL]) return KEY_LCONTROL;
    if (key[KEY_RCONTROL]) return KEY_RCONTROL;
    if (key[KEY_LWIN]) return KEY_LWIN;
    if (key[KEY_RWIN]) return KEY_RWIN;
    if (key[KEY_MENU]) return KEY_MENU;
    if (key[KEY_SCRLOCK]) return KEY_SCRLOCK;
    if (key[KEY_NUMLOCK]) return KEY_NUMLOCK;
    if (key[KEY_CAPSLOCK]) return KEY_CAPSLOCK;

    if (globals->joystick_installed)
    {
      poll_joystick();

      if (joy[0].stick[0].axis[0].d1) return JOY0_LEFT+1000;
      if (joy[0].stick[0].axis[0].d2) return JOY0_RIGHT+1000;
      if (joy[0].stick[0].axis[1].d1) return JOY0_UP+1000;
      if (joy[0].stick[0].axis[1].d2) return JOY0_DOWN+1000;
      if (joy[0].button[0].b) return JOY0_B1+1000;
      if (joy[0].button[1].b) return JOY0_B2+1000;
      if (joy[0].button[2].b) return JOY0_B3+1000;
      if (joy[0].button[3].b) return JOY0_B4+1000;
      if (joy[0].button[4].b) return JOY0_B5+1000;
      if (joy[0].button[5].b) return JOY0_B6+1000;
      if (joy[1].stick[0].axis[0].d1) return JOY1_LEFT+1000;
      if (joy[1].stick[0].axis[0].d2) return JOY1_RIGHT+1000;
      if (joy[1].stick[0].axis[1].d1) return JOY1_UP+1000;
      if (joy[1].stick[0].axis[1].d2) return JOY1_DOWN+1000;
      if (joy[1].button[0].b) return JOY1_B1+1000;
      if (joy[1].button[1].b) return JOY1_B2+1000;
      if (joy[1].button[2].b) return JOY1_B3+1000;
      if (joy[1].button[3].b) return JOY1_B4+1000;
      if (joy[1].button[4].b) return JOY1_B5+1000;
      if (joy[1].button[5].b) return JOY1_B6+1000;
    }

    if (keypressed())
    {
      k = readkey() >> 8;
      if (k == 0)
      {
        k = readkey();
      }
    }

  }
  return k;
}


