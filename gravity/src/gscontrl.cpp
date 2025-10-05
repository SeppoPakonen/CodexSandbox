/*
 *   GRAVITY STRIKE -- gscontrl.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gscontrl.h"
#include "gsglob.h"
#include "gserror.h"
#include "gsnetw.h"
#include <stdio.h>


Keys::Keys()
{
  first_key = last_key = NULL;
  count = 0;
}

Keys::~Keys()
{
  clear();
}

void Keys::clear()
{
  PressedKey *temp = first_key, *next_key;

  while (temp != NULL)
  {
    next_key = temp->get_next();
    delete temp;
    temp = next_key;
  }

  first_key = last_key = NULL;
  count = 0;
}

PressedKey *Keys::find(int k)
{
  PressedKey *temp = first_key;

  while (temp != NULL)
  {
    if (temp->key == k) return temp;
    temp = temp->get_next();
  }

  return NULL;
}

PressedKey::PressedKey(unsigned char k, bool r)
{
  key = k;
  reset = r;

  prev = next = NULL;
}

PressedKey::~PressedKey()
{
}

PressedKey *Keys::add(unsigned char key, bool reset)
{
  PressedKey *mykey = new PressedKey(key, reset);
  add_to_list(mykey);
  return mykey;
}

void Keys::add_to_list(PressedKey *mykey)
{
  if (mykey != NULL)
  {
    if (first_key == NULL)
    {
      last_key = first_key = mykey;
      first_key->set_next(NULL); first_key->set_prev(NULL);
      mykey->set_next(NULL); mykey->set_prev(NULL);
    }
    else
    {
      last_key->set_next(mykey);
      mykey->set_prev(last_key);
      last_key = mykey;
      mykey->set_next(NULL);
    }

    count++;
  }
}

void Keys::remove(PressedKey *temp)
{
  PressedKey *next_key, *prev_key;

  if (temp != NULL)
  {
    prev_key = temp->get_prev();
    next_key = temp->get_next();

    if (prev_key==NULL) first_key = next_key;
    else prev_key->set_next(next_key);

    if (next_key==NULL) last_key = prev_key;
    else next_key->set_prev(prev_key);

    delete temp;
    temp = NULL;

    count--;
  }
}


gsControl::gsControl(gsPlayer *pl)
{
  our_player = pl;
  lastkey = 0;
  keycount = 23;
  mode = 0;
  disabled = 0;

  for (int n=0; n < 80; n++) key_layout[n] = 0;

  load_standard_layout();

  j0up = j0down = j0left = j0right = j0b1 = j0b2 = j0b3 = j0b4 = j0b5 = j0b6 = FALSE;
  j1up = j1down = j1left = j1right = j1b1 = j1b2 = j1b3 = j1b4 = j0b5 = j0b6 = FALSE;

  pressed_keys = new Keys();
}

gsControl::gsControl()
{
  lastkey = 0;
  keycount = 14;
  mode = 1;
  disabled = 0;

  for (int n=0; n < 60; n++) key_layout[n] = 0;

  load_standard_layout();

  pressed_keys = new Keys();
}

gsControl::~gsControl()
{
  delete pressed_keys;
}

void gsControl::set_type(enum input_types inp)
{
  input_type = inp;
//  load_key_layout(input_type);
}

void gsControl::load_standard_layout()
{
//  if (inp == LOCAL_SINGLE)
//  {
    key_layout[LEFT]        = KEY_LEFT;
    key_layout[RIGHT]       = KEY_RIGHT;
    key_layout[SHOOT]       = KEY_UP;
    key_layout[THRUST]      = KEY_SPACE;
    key_layout[SPECIAL]     = KEY_TAB;
    key_layout[EXTRA_ACTION]= KEY_LSHIFT;
    key_layout[NEXT_WEAPON] = KEY_8;
    key_layout[PREV_WEAPON] = KEY_7;
    key_layout[WEAPON_1]    = KEY_1;
    key_layout[WEAPON_2]    = KEY_2;
    key_layout[WEAPON_3]    = KEY_3;
    key_layout[WEAPON_4]    = KEY_4;
    key_layout[WEAPON_5]    = KEY_5;
    key_layout[WEAPON_6]    = KEY_6;

    key_layout[SP_PAUSE]    = KEY_HOME;
    key_layout[SP_SCREENSHOT] = KEY_F12;
    key_layout[SP_PANEL]    = KEY_SCRLOCK,
    key_layout[SP_DEBUG]    = KEY_0;
    key_layout[SP_CHAT]     = KEY_F1;
//  }
//  else
//  if (inp == LOCAL1)
//  {
    key_layout[LEFT_L1]        = KEY_A;
    key_layout[RIGHT_L1]       = KEY_D;
    key_layout[SHOOT_L1]       = KEY_W;
    key_layout[THRUST_L1]      = KEY_LCONTROL;
    key_layout[NEXT_WEAPON_L1] = KEY_8;
    key_layout[PREV_WEAPON_L1] = KEY_7;
    key_layout[SPECIAL_L1]     = KEY_TAB;
    key_layout[EXTRA_ACTION_L1]= KEY_LSHIFT;
    key_layout[WEAPON_1_L1]    = KEY_1;
    key_layout[WEAPON_2_L1]    = KEY_2;
    key_layout[WEAPON_3_L1]    = KEY_3;
    key_layout[WEAPON_4_L1]    = KEY_4;
    key_layout[WEAPON_5_L1]    = KEY_5;
    key_layout[WEAPON_6_L1]    = KEY_6;
//  }
//  else
//  if (inp == LOCAL2)
//  {
    key_layout[LEFT_L2]        = KEY_LEFT;
    key_layout[RIGHT_L2]       = KEY_RIGHT;
    key_layout[SHOOT_L2]       = KEY_UP;
    key_layout[THRUST_L2]      = KEY_RCONTROL;
    key_layout[NEXT_WEAPON_L2] = KEY_8_PAD;
    key_layout[PREV_WEAPON_L2] = KEY_7_PAD;
    key_layout[SPECIAL_L2]     = KEY_ENTER;
    key_layout[EXTRA_ACTION_L2]= KEY_RSHIFT;
    key_layout[WEAPON_1_L2]    = KEY_1_PAD;
    key_layout[WEAPON_2_L2]    = KEY_2_PAD;
    key_layout[WEAPON_3_L2]    = KEY_3_PAD;
    key_layout[WEAPON_4_L2]    = KEY_4_PAD;
    key_layout[WEAPON_5_L2]    = KEY_5_PAD;
    key_layout[WEAPON_6_L2]    = KEY_6_PAD;
//  }
}

void gsControl::update()
{
//  if (keyboard_needs_poll()) poll_keyboard(); ausgelagert nach gslevel.cpp
//  if (globals->use_joystick) poll_joystick();
  if (input_type != NETWORK_INPUT) if (keypressed()) lastkey = readkey() >> 8;
}

int gsControl::get_pressed_key()
{
  return lastkey;
}

int gsControl::get_joystick_state(int c, int reset)
{
  if (globals->use_joystick)
  {
    // released Joystick buttons?
    if (j0up && !joy[0].stick[0].axis[1].d1) j0up = FALSE;
    if (j0down && !joy[0].stick[0].axis[1].d2) j0down = FALSE;
    if (j0left && !joy[0].stick[0].axis[0].d1) j0left = FALSE;
    if (j0right && !joy[0].stick[0].axis[0].d2) j0right = FALSE;
    if (j0b1 && !joy[0].button[0].b) j0b1 = FALSE;
    if (j0b2 && !joy[0].button[1].b) j0b2 = FALSE;
    if (j0b3 && !joy[0].button[2].b) j0b3 = FALSE;
    if (j0b4 && !joy[0].button[3].b) j0b4 = FALSE;
  
    if (j1up && !joy[1].stick[0].axis[1].d1) j1up = FALSE;
    if (j1down && !joy[1].stick[0].axis[1].d2) j1down = FALSE;
    if (j1left && !joy[1].stick[0].axis[0].d1) j1left = FALSE;
    if (j1right && !joy[1].stick[0].axis[0].d2) j1right = FALSE;
    if (j1b1 && !joy[1].button[0].b) j1b1 = FALSE;
    if (j1b2 && !joy[1].button[1].b) j1b2 = FALSE;
    if (j1b3 && !joy[1].button[2].b) j1b3 = FALSE;
    if (j1b4 && !joy[1].button[3].b) j1b4 = FALSE;
  
    switch(c)
    {
      case JOY0_UP      : if (joy[0].stick[0].axis[1].d1) { if (reset) { if (!j0up) { j0up = TRUE; return 1; } } else return 1; } break;
      case JOY0_DOWN    : if (joy[0].stick[0].axis[1].d2) { if (reset) { if (!j0down) { j0down = TRUE; return 1; } } else return 1; } break;
      case JOY0_LEFT    : if (joy[0].stick[0].axis[0].d1) { if (reset) { if (!j0left) { j0left = TRUE; return 1; } } else return 1; } break;
      case JOY0_RIGHT   : if (joy[0].stick[0].axis[0].d2) { if (reset) { if (!j0right) { j0right = TRUE; return 1; } } else return 1; } break;
      case JOY0_B1      : if (joy[0].button[0].b) { if (reset) { if (!j0b1) { j0b1 = TRUE; return 1; } } else return 1; } break;
      case JOY0_B2      : if (joy[0].button[1].b) { if (reset) { if (!j0b2) { j0b2 = TRUE; return 1; } } else return 1; } break;
      case JOY0_B3      : if (joy[0].button[2].b) { if (reset) { if (!j0b3) { j0b3 = TRUE; return 1; } } else return 1; } break;
      case JOY0_B4      : if (joy[0].button[3].b) { if (reset)  { if (!j0b4) { j0b4 = TRUE; return 1; } } else return 1; } break;
      case JOY0_B5      : if (joy[0].button[4].b) { if (reset) { if (!j0b5) { j0b5 = TRUE; return 1; } } else return 1; } break;
      case JOY0_B6      : if (joy[0].button[5].b) { if (reset)  { if (!j0b6) { j0b6 = TRUE; return 1; } } else return 1; } break;
  
      case JOY1_UP      : if (joy[1].stick[0].axis[1].d1) { if (reset) { if (!j1up) { j1up = TRUE; return 1; } } else return 1; } break;
      case JOY1_DOWN    : if (joy[1].stick[0].axis[1].d2) { if (reset) { if (!j1down) { j1down = TRUE; return 1; } } else return 1; } break;
      case JOY1_LEFT    : if (joy[1].stick[0].axis[0].d1) { if (reset) { if (!j1left) { j1left = TRUE; return 1; } } else return 1; } break;
      case JOY1_RIGHT   : if (joy[1].stick[0].axis[0].d2) { if (reset) { if (!j1right) { j1right = TRUE; return 1; } } else return 1; } break;
      case JOY1_B1      : if (joy[1].button[0].b) { if (reset) { if (!j1b1) { j1b1 = TRUE; return 1; } } else return 1; } break;
      case JOY1_B2      : if (joy[1].button[1].b) { if (reset) { if (!j1b2) { j1b2 = TRUE; return 1; } } else return 1; } break;
      case JOY1_B3      : if (joy[1].button[2].b) { if (reset) { if (!j1b3) { j1b3 = TRUE; return 1; } } else return 1; } break;
      case JOY1_B4      : if (joy[1].button[3].b) { if (reset)  { if (!j1b4) { j1b4 = TRUE; return 1; } } else return 1; } break;
      case JOY1_B5      : if (joy[1].button[4].b) { if (reset) { if (!j1b5) { j1b5 = TRUE; return 1; } } else return 1; } break;
      case JOY1_B6      : if (joy[1].button[5].b) { if (reset)  { if (!j1b6) { j1b6 = TRUE; return 1; } } else return 1; } break;
    }
  }

  return 0;
}

void gsControl::set_input(int c, int state)
{
  int layout = input_type;
  int k = key_layout[c+layout*keycount];
  key[k] = state;
}  

int gsControl::get_input(int c, bool reset, int layout)
{
  if (input_type != NETWORK_INPUT)
  {
    if (layout==-1) layout = input_type;
    int k = key_layout[c+layout*keycount];

    if (k < 1000)
    {
      if (key[k])
      {
        if (reset) key[k] = 0;
        if (globals->network_game) 
        {
          if (add_to_pressed_keys(c, reset))
            net->game_send_player_input(c, true, reset);
        }
        return 1;
      }
    }
    else
    {
      if (get_joystick_state(k-1000, reset))
      {
        if (globals->network_game) 
        {
          if (add_to_pressed_keys(c, reset))
            net->game_send_player_input(c, true, reset);
        }
        return 1;
      }
    }
  }
  else // input from network function net->game_process_player_input
  {
    int k = key_layout[c+LOCAL_SINGLE*keycount];
  
    if (k < 1000)
    {
      PressedKey *mykey;

      if (mykey = pressed_keys->find(c))
      {
        if (reset) pressed_keys->remove(mykey);
//        errors->log(2, "net control", "pressed key found", c);
        return 1;
      }
    }
  }
  
  return 0;
}

int gsControl::left()
{
  if (get_input(LEFT, 0)) return 1;
  return 0;
}

int gsControl::right()
{
  if (get_input(RIGHT, 0)) return 1;
  return 0;
}

int gsControl::shoot()
{
  if (get_input(SHOOT, 0)) return 1;
  return 0;
}

int gsControl::thrust()
{
  if (get_input(THRUST, 0)) return 1;
  return 0;
}

int gsControl::special()
{
  if (get_input(SPECIAL, 1)) return 1;
  return 0;
}

int gsControl::extra_action()
{
  if (get_input(EXTRA_ACTION, 0)) return 1;
  return 0;
}

int gsControl::nw()
{
  if (get_input(NEXT_WEAPON, 1))  return 1;
  return 0;
}

int gsControl::pw()
{
  if (get_input(PREV_WEAPON, 1)) return 1;
  return 0;
}

int gsControl::w1()
{
  if (get_input(WEAPON_1, 1)) return 1;
  return 0;
}

int gsControl::w2()
{
  if (get_input(WEAPON_2, 1)) return 1;
  return 0;
}

int gsControl::w3()
{
  if (get_input(WEAPON_3, 1)) return 1;
  return 0;
}

int gsControl::w4()
{
  if (get_input(WEAPON_4, 1)) return 1;
  return 0;
}

int gsControl::w5()
{
  if (get_input(WEAPON_5, 1)) return 1;
  return 0;
}

int gsControl::w6()
{
  if (get_input(WEAPON_6, 1)) return 1;
  return 0;
}

void gsControl::load_key_cfg()
{
  char f[255];
  strcpy(f, globals->data_dir);
  strcat(f, globals->keyboard_file_name);
  FILE *keyf = fopen(f, "rb");

  if (keyf)
  {
    fread(&key_layout, sizeof(key_layout), 1, keyf);
    fclose(keyf);
  }
  else
  {
    errors->log(0, "key init", "error opening keyboard config, using std layout...");
  }

  // any joystick "key"?
  globals->use_joystick = 0;
  for (int n=0; n < 80; n++)
    if (key_layout[n] >= 1000)
      globals->use_joystick = 1;
}

void gsControl::save_key_cfg()
{
  char f[255];
  strcpy(f, globals->data_dir);
  strcat(f, globals->keyboard_file_name);
  FILE *keyf = fopen(f, "wb");

  if (keyf)
  {
    fwrite(&key_layout, sizeof(key_layout), 1, keyf);
    fclose(keyf);
  }
  else
  {
    errors->log(0, "key init", "error writing keyboard config!");
  }
}

void gsControl::set_key(player_controls k, int n)
{
  int double_key = 0;
  int start = 0, end = 0;

  if (k < keycount) { start = 0; end = keycount; }
  else { start = keycount; end = 80; }

  for (int a=start; a < end; a++)
  {
    if (a != k)
      if (key_layout[a] == n) double_key = 1;
  }

  if (!double_key) key_layout[k] = n;
}


bool gsControl::add_to_pressed_keys(int c, bool reset)
{
  if (!pressed_keys->find(c))
  {
    pressed_keys->add(c, reset);
    return true;
  }

  return false;
}

void gsControl::delete_from_pressed_keys(int k)
{
  PressedKey *mykey = pressed_keys->find(k);
  if (mykey) pressed_keys->remove(mykey);
}

/*
void gsControl::check_pressed_keys()
{
  LinkedList<PressedKey> templist = pressed_keys;
  PressedKey tk;

  pressed_keys.beginning();

  // check which keys are still pressed and delete them if not
  templist.beginning();
  while (templist.size() > 0)
  {
    tk = templist.peek();
    pressed_keys.find(tk); 

    if (!tk.reset && !key[key_layout[tk.key]])
    {
      net->game_send_player_input(tk.key, false, false);
      pressed_keys.del();
    }
    else if (tk.reset)
    {
      pressed_keys.del();
    }

    templist.pop();
  }

}
*/

void gsControl::check_pressed_keys()
{
  PressedKey *mykey = pressed_keys->get_first(), *next_key;

  // check which keys are still pressed and delete them if not
  while (mykey)
  {
    next_key = mykey->get_next();

    if (!mykey->reset && !key[key_layout[mykey->key]])
    {
      net->game_send_player_input(mykey->key, false, false);
      pressed_keys->remove(mykey);
    }
    else if (mykey->reset)
    {
      pressed_keys->remove(mykey);
    }

    mykey = next_key;
  }

}
