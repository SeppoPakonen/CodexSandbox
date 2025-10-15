/*
 *   GRAVITY STRIKE -- gscontrl.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#include "network/LinkedList.h"

using namespace BasicDataStructures;

#ifndef _GSCONTROL_
#define _GSCONTROL_

enum input_types {
  LOCAL_SINGLE,
  LOCAL1,
  LOCAL2,
  NETWORK_INPUT
};

enum joystick_states {
  JOY0_UP,
  JOY0_DOWN,
  JOY0_LEFT,
  JOY0_RIGHT,
  JOY0_B1,
  JOY0_B2,
  JOY0_B3,
  JOY0_B4,
  JOY0_B5,
  JOY0_B6,
  JOY1_UP,
  JOY1_DOWN,
  JOY1_LEFT,
  JOY1_RIGHT,
  JOY1_B1,
  JOY1_B2,
  JOY1_B3,
  JOY1_B4,
  JOY1_B5,
  JOY1_B6
};

enum player_controls {
  LEFT,
  RIGHT,
  SHOOT,
  THRUST,
  NEXT_WEAPON,
  PREV_WEAPON,
  WEAPON_1,
  WEAPON_2,
  WEAPON_3,
  WEAPON_4,
  WEAPON_5,
  WEAPON_6,
  EXTRA_ACTION,
  SPECIAL,
  TEMP1,
  TEMP2,
  TEMP3,
  SP_PAUSE,
  SP_SCREENSHOT,
  SP_PANEL,
  SP_DEBUG,
  SP_CHAT,
  TEMP9,
  // 2 player match: player 1
  LEFT_L1,
  RIGHT_L1,
  SHOOT_L1,
  THRUST_L1,
  NEXT_WEAPON_L1,
  PREV_WEAPON_L1,
  WEAPON_1_L1,
  WEAPON_2_L1,
  WEAPON_3_L1,
  WEAPON_4_L1,
  WEAPON_5_L1,
  WEAPON_6_L1,
  EXTRA_ACTION_L1,
  SPECIAL_L1,
  TEMP1_L1,
  TEMP2_L1,
  TEMP3_L1,
  TEMP4_L1,
  TEMP5_L1,
  TEMP6_L1,
  TEMP7_L1,
  TEMP8_L1,
  TEMP9_L1,
  // 2 player match: player 2
  LEFT_L2,
  RIGHT_L2,
  SHOOT_L2,
  THRUST_L2,
  NEXT_WEAPON_L2,
  PREV_WEAPON_L2,
  WEAPON_1_L2,
  WEAPON_2_L2,
  WEAPON_3_L2,
  WEAPON_4_L2,
  WEAPON_5_L2,
  WEAPON_6_L2,
  EXTRA_ACTION_L2,
  SPECIAL_L2,
  TEMP1_L2,
  TEMP2_L2,
  TEMP3_L2,
  TEMP4_L2,
  TEMP5_L2,
  TEMP6_L2,
  TEMP7_L2,
  TEMP8_L2,
  TEMP9_L2
};

class PressedKey 
{
  private:
    PressedKey *prev, *next;

  public:
    unsigned char key;
    bool reset;

    PressedKey(unsigned char key, bool reset);
    ~PressedKey();

    void set_prev(PressedKey *p)                   { prev = p; }
    void set_next(PressedKey *p)                   { next = p; }
    PressedKey *get_prev()                         { return prev; }
    PressedKey *get_next()                         { return next; }
};

class Keys 
{
  private:
    PressedKey *first_key, *last_key;
    int count;

  public:
    Keys();
    ~Keys();

    int get_count()                                { return count; }
    PressedKey *get_first()                        { return first_key; }

    PressedKey *add(unsigned char key, bool reset);
    void       add_to_list(PressedKey *);
    void       remove(PressedKey *);
    void       clear();
    PressedKey *find(int k);
};

class gsControl
{
  private:
    // variables
    int              lastkey;
    enum input_types input_type;
    int              key_layout[80];
    //LinkedList<PressedKey>  pressed_keys;          // currently pressed keys (for network game)
    Keys             *pressed_keys;
    class gsPlayer   *our_player;
    int              keycount;                     // how much keys per input type
    int              mode;                         // 0 = player ctrl, 1 = setup
    int              disabled;                     // are controls activated?

    int              j0up, j0down, j0left, j0right, j0b1, j0b2, j0b3, j0b4, j0b5, j0b6;
    int              j1up, j1down, j1left, j1right, j1b1, j1b2, j1b3, j1b4, j1b5, j1b6;

    // functions
    void load_standard_layout();

  public:
    // constructor, destructor
    gsControl(class gsPlayer *);                   // player mode
    gsControl();                                   // setup mode
    ~gsControl();

    // get/set
    void set_key(player_controls k, int n);
    int  get_key(player_controls k)                { return key_layout[k]; }
    int  get_joystick_state(int c, int reset);
    int  get_input(int c, bool reset, int layout=-1);
    void set_input(int c, int state);
    int  get_lastkey()                             { return lastkey; }
    void set_lastkey(int k)                        { lastkey = k; }
    int  get_net_pressed_keys()                    { return pressed_keys->get_count(); }
    void check_pressed_keys();                     // checks if pressed keys are still pressed

    void enable()                                  { disabled = 0; }
    void disable()                                 { disabled = 1; }
    int  get_disabled()                            { return disabled; }

    // functions
    void  load_key_cfg();                          // load key configuration
    void  save_key_cfg();                          // save key configuration
    void  set_type(enum input_types);
    void  update();
    int   get_pressed_key();
    bool  add_to_pressed_keys(int k, bool reset);  // adds a key to pressed keys array if not already there
    void  delete_from_pressed_keys(int k);         // deletes k from pressed_keys array
    void  clear_pressed_keys()                     { pressed_keys->clear(); }

    int   left();
    int   right();
    int   thrust();
    int   shoot();
    int   special();
    int   extra_action();

    int   nw();
    int   pw();

    int   w1();
    int   w2();
    int   w3();
    int   w4();
    int   w5();
    int   w6();
};

#endif

