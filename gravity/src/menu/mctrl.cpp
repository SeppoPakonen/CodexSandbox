/*
 *   GRAVITY STRIKE -- menu/mctrl.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mctrl.h"
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

mPControls::mPControls()
{
  active_button = 0;
  button_count = 16;
  moving = 0;
  switch_to_menu = -1;
  wait_for_key = -1;
  esc_pressed = 0;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
}

mPControls::~mPControls()
{
  globals->last_menu = MENU_CONTROLS;

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mPControls::init(int scatter)
{
  int n;
  for (n=0; n < 7; n++)
  {
    button[M_CONTROL_LEFT+n] = new mKeyboardOption();
    if (scatter) button[M_CONTROL_LEFT+n]->create(50, 115+n*40, -200, 115+n*40, 240, 25);
    else button[M_CONTROL_LEFT+n]->create(50, 115+n*40, 0, 0, 240, 25);
  }
  for (n=7; n < 14; n++)
  {
    button[M_CONTROL_LEFT+n] = new mKeyboardOption();
    if (scatter) button[M_CONTROL_LEFT+n]->create(350, 115+(n-7)*40, 640, 115+(n-7)*40, 240, 25);
    else button[M_CONTROL_LEFT+n]->create(350, 115+(n-7)*40, 0, 0, 240, 25);
  }

  button[M_CONTROL_BACK] = new mMainButton();
  if (scatter) button[M_CONTROL_BACK]->create(245, 430, 245, 500, 150, 30);
  else button[M_CONTROL_BACK]->create(245, 430, 0, 0, 150, 30);

  button[M_CONTROL_LEFT]->set_text(language->get_string(T_CONTROL_LEFT));
  button[M_CONTROL_LEFT]->set_int(controls->get_key(LEFT));
  button[M_CONTROL_RIGHT]->set_text(language->get_string(T_CONTROL_RIGHT));
  button[M_CONTROL_RIGHT]->set_int(controls->get_key(RIGHT));
  button[M_CONTROL_THRUST]->set_text(language->get_string(T_CONTROL_THRUST));
  button[M_CONTROL_THRUST]->set_int(controls->get_key(THRUST));
  button[M_CONTROL_SHOOT]->set_text(language->get_string(T_CONTROL_SHOOT));
  button[M_CONTROL_SHOOT]->set_int(controls->get_key(SHOOT));
  button[M_CONTROL_EXTRA]->set_text(language->get_string(T_CONTROL_EXTRA2));
  button[M_CONTROL_EXTRA]->set_int(controls->get_key(SPECIAL));
  button[M_CONTROL_AFTERBURNER]->set_text(language->get_string(T_CONTROL_EXTRA_ACTION));
  button[M_CONTROL_AFTERBURNER]->set_int(controls->get_key(EXTRA_ACTION));
  button[M_CONTROL_NEXTWEAP]->set_text(language->get_string(T_CONTROL_NEXTWEAP));
  button[M_CONTROL_NEXTWEAP]->set_int(controls->get_key(NEXT_WEAPON));
  button[M_CONTROL_PREVWEAP]->set_text(language->get_string(T_CONTROL_PREVWEAP));
  button[M_CONTROL_PREVWEAP]->set_int(controls->get_key(PREV_WEAPON));
  button[M_CONTROL_W1]->set_text(language->get_string(T_CONTROL_W1));
  button[M_CONTROL_W1]->set_int(controls->get_key(WEAPON_1));
  button[M_CONTROL_W2]->set_text(language->get_string(T_CONTROL_W2));
  button[M_CONTROL_W2]->set_int(controls->get_key(WEAPON_2));
  button[M_CONTROL_W3]->set_text(language->get_string(T_CONTROL_W3));
  button[M_CONTROL_W3]->set_int(controls->get_key(WEAPON_3));
  button[M_CONTROL_W4]->set_text(language->get_string(T_CONTROL_W4));
  button[M_CONTROL_W4]->set_int(controls->get_key(WEAPON_4));
  button[M_CONTROL_W5]->set_text(language->get_string(T_CONTROL_W5));
  button[M_CONTROL_W5]->set_int(controls->get_key(WEAPON_5));
  button[M_CONTROL_W6]->set_text(language->get_string(T_CONTROL_W6));
  button[M_CONTROL_W6]->set_int(controls->get_key(WEAPON_6));

  button[M_CONTROL_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_CONTROL_CAPTION] = new mNormalText();
  button[M_CONTROL_CAPTION]->create(286, 64, 800, 64);
  button[M_CONTROL_CAPTION]->set_int(FONT_BANK);
  button[M_CONTROL_CAPTION]->set_text(language->get_string(T_MENU_CAP_CONTROLS));
  button[M_CONTROL_CAPTION]->set_int(globals->col_yellow);
  button[M_CONTROL_CAPTION]->set_flag(MC_INACTIVE);
}

int mPControls::check()
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

int mPControls::handle_controls()
{
  wait_for_key = -1;

  if (active_button < M_CONTROL_BACK &&
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
        case M_CONTROL_LEFT       : key_to_change = LEFT; break;
        case M_CONTROL_RIGHT      : key_to_change = RIGHT; break;
        case M_CONTROL_THRUST     : key_to_change = THRUST; break;
        case M_CONTROL_SHOOT      : key_to_change = SHOOT; break;
        case M_CONTROL_NEXTWEAP   : key_to_change = NEXT_WEAPON; break;
        case M_CONTROL_PREVWEAP   : key_to_change = PREV_WEAPON; break;
        case M_CONTROL_W1         : key_to_change = WEAPON_1; break;
        case M_CONTROL_W2         : key_to_change = WEAPON_2; break;
        case M_CONTROL_W3         : key_to_change = WEAPON_3; break;
        case M_CONTROL_W4         : key_to_change = WEAPON_4; break;
        case M_CONTROL_W5         : key_to_change = WEAPON_5; break;
        case M_CONTROL_W6         : key_to_change = WEAPON_6; break;
        case M_CONTROL_EXTRA      : key_to_change = SPECIAL; break;
        case M_CONTROL_AFTERBURNER: key_to_change = EXTRA_ACTION; break;
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
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_CONTROL_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_OPTIONS;
    button[M_CONTROL_BACK]->set_flag(MC_INVISIBLE);
    button[M_CONTROL_BACK]->del_flag(MC_PUSHED);
    button[M_CONTROL_BACK]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  clear_keybuf();

  return 0;
}

void mPControls::draw(BITMAP *v)
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

int mPControls::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mPControls::move_out()
{
  for (int n=0; n < button_count-1; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_CONTROL_CAPTION]->set_dest_x( button[M_CONTROL_CAPTION]->get_x() + 400 );

  moving = 1;
}

int mPControls::get_pressed_key()
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


