/*
 *   GRAVITY STRIKE -- menu/mctrl2.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mctrl2.h"
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

mPControls2P::mPControls2P()
{
  active_button = 0;
  button_count = 20;
  moving = 0;
  switch_to_menu = -1;
  wait_for_key = -1;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;
}

mPControls2P::~mPControls2P()
{
  globals->last_menu = MENU_2PCONTROLS;

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

void mPControls2P::init(int scatter)
{
  int n;
  for (n=0; n < 8; n++)
  {
    button[M_CONTROL2P_LEFT_1+n] = new mKeyboardOption();
    if (scatter) button[M_CONTROL2P_LEFT_1+n]->create(50, 135+n*35, -200, 135+n*35, 240, 25);
    else button[M_CONTROL2P_LEFT_1+n]->create(50, 135+n*35, 0, 0, 240, 25);
  }
  for (n=8; n < 16; n++)
  {
    button[M_CONTROL2P_LEFT_1+n] = new mKeyboardOption();
    if (scatter) button[M_CONTROL2P_LEFT_1+n]->create(350, 135+(n-8)*35, 640, 135+(n-8)*35, 240, 25);
    else button[M_CONTROL2P_LEFT_1+n]->create(350, 135+(n-8)*35, 0, 0, 240, 25);
  }

  button[M_CONTROL2P_BACK] = new mMainButton();
  if (scatter) button[M_CONTROL2P_BACK]->create(245, 430, 245, 500, 150, 30);
  else button[M_CONTROL2P_BACK]->create(245, 430, 0, 0, 150, 30);

  button[M_CONTROL2P_LEFT_1]->set_text(language->get_string(T_CONTROL_LEFT));
  button[M_CONTROL2P_LEFT_1]->set_int(controls->get_key(LEFT_L1));
  button[M_CONTROL2P_RIGHT_1]->set_text(language->get_string(T_CONTROL_RIGHT));
  button[M_CONTROL2P_RIGHT_1]->set_int(controls->get_key(RIGHT_L1));
  button[M_CONTROL2P_THRUST_1]->set_text(language->get_string(T_CONTROL_THRUST));
  button[M_CONTROL2P_THRUST_1]->set_int(controls->get_key(THRUST_L1));
  button[M_CONTROL2P_SHOOT_1]->set_text(language->get_string(T_CONTROL_SHOOT));
  button[M_CONTROL2P_SHOOT_1]->set_int(controls->get_key(SHOOT_L1));
  button[M_CONTROL2P_EXTRA_1]->set_text(language->get_string(T_CONTROL_EXTRA2));
  button[M_CONTROL2P_EXTRA_1]->set_int(controls->get_key(SPECIAL_L1));
  button[M_CONTROL2P_AFTERBURNER_1]->set_text(language->get_string(T_CONTROL_EXTRA_ACTION));
  button[M_CONTROL2P_AFTERBURNER_1]->set_int(controls->get_key(EXTRA_ACTION_L1));
  button[M_CONTROL2P_NEXTWEAP_1]->set_text(language->get_string(T_CONTROL_NEXTWEAP));
  button[M_CONTROL2P_NEXTWEAP_1]->set_int(controls->get_key(NEXT_WEAPON_L1));
  button[M_CONTROL2P_PREVWEAP_1]->set_text(language->get_string(T_CONTROL_PREVWEAP));
  button[M_CONTROL2P_PREVWEAP_1]->set_int(controls->get_key(PREV_WEAPON_L1));

  button[M_CONTROL2P_LEFT_2]->set_text(language->get_string(T_CONTROL_LEFT));
  button[M_CONTROL2P_LEFT_2]->set_int(controls->get_key(LEFT_L2));
  button[M_CONTROL2P_RIGHT_2]->set_text(language->get_string(T_CONTROL_RIGHT));
  button[M_CONTROL2P_RIGHT_2]->set_int(controls->get_key(RIGHT_L2));
  button[M_CONTROL2P_THRUST_2]->set_text(language->get_string(T_CONTROL_THRUST));
  button[M_CONTROL2P_THRUST_2]->set_int(controls->get_key(THRUST_L2));
  button[M_CONTROL2P_SHOOT_2]->set_text(language->get_string(T_CONTROL_SHOOT));
  button[M_CONTROL2P_SHOOT_2]->set_int(controls->get_key(SHOOT_L2));
  button[M_CONTROL2P_EXTRA_2]->set_text(language->get_string(T_CONTROL_EXTRA2));
  button[M_CONTROL2P_EXTRA_2]->set_int(controls->get_key(SPECIAL_L2));
  button[M_CONTROL2P_AFTERBURNER_2]->set_text(language->get_string(T_CONTROL_EXTRA_ACTION));
  button[M_CONTROL2P_AFTERBURNER_2]->set_int(controls->get_key(EXTRA_ACTION_L2));
  button[M_CONTROL2P_NEXTWEAP_2]->set_text(language->get_string(T_CONTROL_NEXTWEAP));
  button[M_CONTROL2P_NEXTWEAP_2]->set_int(controls->get_key(NEXT_WEAPON_L2));
  button[M_CONTROL2P_PREVWEAP_2]->set_text(language->get_string(T_CONTROL_PREVWEAP));
  button[M_CONTROL2P_PREVWEAP_2]->set_int(controls->get_key(PREV_WEAPON_L2));

  button[M_CONTROL2P_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_CONTROL2P_CAPTION] = new mNormalText();
  button[M_CONTROL2P_CAPTION]->create(286, 64, 800, 64);
  button[M_CONTROL2P_CAPTION]->set_int(FONT_BANK);
  button[M_CONTROL2P_CAPTION]->set_text(language->get_string(T_MENU_CAP_2PCONTROLS));
  button[M_CONTROL2P_CAPTION]->set_int(globals->col_yellow);
  button[M_CONTROL2P_CAPTION]->set_flag(MC_INACTIVE);

  button[M_CONTROL2P_PLAYER1] = new mNormalText();
  button[M_CONTROL2P_PLAYER1]->create(50, 110, 840, 110);
  button[M_CONTROL2P_PLAYER1]->set_int(FONT_IMPACT10);
  button[M_CONTROL2P_PLAYER1]->set_text(language->get_string(T_MENU_TEXT_PLAYER1));
  button[M_CONTROL2P_PLAYER1]->set_int(globals->col_grey);
  button[M_CONTROL2P_PLAYER1]->set_flag(MC_INACTIVE);

  button[M_CONTROL2P_PLAYER2] = new mNormalText();
  button[M_CONTROL2P_PLAYER2]->create(590-text_length((FONT*)globals->fontdat[FONT_IMPACT10].dat,language->get_string(T_MENU_TEXT_PLAYER2)), 110, -200, 110);
  button[M_CONTROL2P_PLAYER2]->set_int(FONT_IMPACT10);
  button[M_CONTROL2P_PLAYER2]->set_text(language->get_string(T_MENU_TEXT_PLAYER2));
  button[M_CONTROL2P_PLAYER2]->set_int(globals->col_grey);
  button[M_CONTROL2P_PLAYER2]->set_flag(MC_INACTIVE);
}

int mPControls2P::check()
{
/*
  if (active_button >= 0)
    if (button[active_button]->get_flags() & MC_PUSHED)
    {
      button[active_button]->del_flag(MC_PUSHED);
    }
*/

  for (int n=0; n < button_count-3; n++)
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
        button[active_button]->set_flag(MC_PUSHED);
      clear_key(KEY_ENTER);
    }
  }

  return(handle_controls());
}

int mPControls2P::handle_controls()
{
  wait_for_key = -1;

  if (active_button < M_CONTROL2P_BACK &&
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
        case M_CONTROL2P_LEFT_1       : key_to_change = LEFT_L1; break;
        case M_CONTROL2P_RIGHT_1      : key_to_change = RIGHT_L1; break;
        case M_CONTROL2P_THRUST_1     : key_to_change = THRUST_L1; break;
        case M_CONTROL2P_SHOOT_1      : key_to_change = SHOOT_L1; break;
        case M_CONTROL2P_NEXTWEAP_1   : key_to_change = NEXT_WEAPON_L1; break;
        case M_CONTROL2P_PREVWEAP_1   : key_to_change = PREV_WEAPON_L1; break;
        case M_CONTROL2P_EXTRA_1      : key_to_change = SPECIAL_L1; break;
        case M_CONTROL2P_AFTERBURNER_1: key_to_change = EXTRA_ACTION_L1; break;

        case M_CONTROL2P_LEFT_2       : key_to_change = LEFT_L2; break;
        case M_CONTROL2P_RIGHT_2      : key_to_change = RIGHT_L2; break;
        case M_CONTROL2P_THRUST_2     : key_to_change = THRUST_L2; break;
        case M_CONTROL2P_SHOOT_2      : key_to_change = SHOOT_L2; break;
        case M_CONTROL2P_NEXTWEAP_2   : key_to_change = NEXT_WEAPON_L2; break;
        case M_CONTROL2P_PREVWEAP_2   : key_to_change = PREV_WEAPON_L2; break;
        case M_CONTROL2P_EXTRA_2      : key_to_change = SPECIAL_L2; break;
        case M_CONTROL2P_AFTERBURNER_2: key_to_change = EXTRA_ACTION_L2; break;
      }

      controls->set_key((player_controls)key_to_change, k);
      button[active_button]->set_int(controls->get_key((player_controls)key_to_change));
      button[active_button]->del_flag(MC_PUSHED);
    }
  }

  for (int n=0; n < button_count-4; n++)
  {
    if (n != wait_for_key && button[n]->get_flags() & MC_PUSHED)
      button[n]->del_flag(MC_PUSHED);
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_CONTROL2P_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_OPTIONS;
    button[M_CONTROL2P_BACK]->set_flag(MC_INVISIBLE);
    button[M_CONTROL2P_BACK]->del_flag(MC_PUSHED);
    button[M_CONTROL2P_BACK]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  clear_keybuf();

  return 0;
}

void mPControls2P::draw(BITMAP *v)
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

int mPControls2P::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mPControls2P::move_out()
{
  for (int n=0; n < button_count-3; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_CONTROL2P_CAPTION]->set_dest_x( button[M_CONTROL2P_CAPTION]->get_x() + 400 );
  button[M_CONTROL2P_PLAYER1]->set_dest_x( button[M_CONTROL2P_PLAYER1]->get_x() - 200 );
  button[M_CONTROL2P_PLAYER2]->set_dest_x( button[M_CONTROL2P_PLAYER2]->get_x() + 200 );

  moving = 1;
}

int mPControls2P::get_pressed_key()
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

