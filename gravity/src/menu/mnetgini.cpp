/*
 *   GRAVITY STRIKE -- menu/mnetgini.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mnetgini.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsplayer.h"
#include "../gsfade.h"
#include "../gssound.h"
#include "../gserror.h"
#include "../gsnetw.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>

#define NETGINI_START_CONNECTING    1
#define NETGINI_CONNECTED           2
#define NETGINI_CONNECTION_FAILED   3

mNetGameInit::mNetGameInit()
{
  active_button = 2;
  button_count = 5;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  moved_in = FALSE;
  esc_pressed = 0;
  globals->network_game = 1;

  for (int n=0; n < button_count; n++)
    button[n] = NULL;

  globals->players = 1;
  net->reset();
  check_client_connection = 0;
}

mNetGameInit::~mNetGameInit()
{
  globals->last_menu = MENU_NETGAMEINI;
  net->chat.clear();

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);

  if (switch_to_menu == MENU_MAIN) net->reset();
}

void mNetGameInit::init_net()
{
}

void mNetGameInit::init(int scatter)
{
  init_net();

  button[M_NETGAMEINI_CAPTION] = new mNormalText();
  button[M_NETGAMEINI_CAPTION]->create(286, 64, 800, 64);
  button[M_NETGAMEINI_CAPTION]->set_int(FONT_BANK);
  button[M_NETGAMEINI_CAPTION]->set_text(language->get_string(T_MENU_CAP_NETGAME));
  button[M_NETGAMEINI_CAPTION]->set_int(globals->col_yellow);
  button[M_NETGAMEINI_CAPTION]->set_flag(MC_INACTIVE);

  button[M_NETGAMEINI_BACK] = new mMainButton();
  button[M_NETGAMEINI_BACK]->create(320-75, 430, 320-75, 500, 150, 30);
  button[M_NETGAMEINI_BACK]->set_text(language->get_string(T_MENU_BACK));

  button[M_NETGAMEINI_SERVER] = new mMainButton();
  button[M_NETGAMEINI_SERVER]->create(320-125, 200, 640, 100, 250, 30);
  button[M_NETGAMEINI_SERVER]->set_text(language->get_string(T_MENU_SERVER));

  button[M_NETGAMEINI_CLIENT] = new mMainButton();
  button[M_NETGAMEINI_CLIENT]->create(320-125, 270, 640, 200, 250, 30);
  button[M_NETGAMEINI_CLIENT]->set_text(language->get_string(T_MENU_CLIENT));
//  button[M_NETGAMEINI_CLIENT]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_NETGAMEINI_SERVERIP] = new mTextField();
  button[M_NETGAMEINI_SERVERIP]->create(240, 305, -300, 200, 150, 15, 60);
  button[M_NETGAMEINI_SERVERIP]->set_int(16);
  button[M_NETGAMEINI_SERVERIP]->set_text("Server IP");
  button[M_NETGAMEINI_SERVERIP]->set_text(globals->server_ip);

}

int mNetGameInit::check()
{
  // network messages
//  if (globals->game_time % 10 == 0)
  net->process_packets();

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

int mNetGameInit::handle_controls()
{
  // server
  if (!(button[M_NETGAMEINI_SERVER]->get_flags() & MC_INACTIVE) &&
      button[M_NETGAMEINI_SERVER]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();

    if (net->init(NET_SERVER) == 0)
    {
      button[M_NETGAMEINI_SERVER]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_NETGAMEINI_SERVER]->do_explosion();
      button[M_NETGAMEINI_SERVER]->del_flag(MC_PUSHED);

      switch_to_menu = MENU_NETGAME;
      move_out();
    }  

    button[M_NETGAMEINI_SERVER]->del_flag(MC_PUSHED);
  }

  // client
  if (button[M_NETGAMEINI_CLIENT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();

    // check ip
    /* 2bedone */
    strncpy(globals->server_ip, button[M_NETGAMEINI_SERVERIP]->get_text(), 16);

    if (net->init(NET_CLIENT) == 0)
    {
      button[M_NETGAMEINI_CLIENT]->set_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_NETGAMEINI_CLIENT]->do_explosion();
      button[M_NETGAMEINI_CLIENT]->del_flag(MC_PUSHED);
  
      check_client_connection = NETGINI_START_CONNECTING;
    }

    button[M_NETGAMEINI_CLIENT]->del_flag(MC_PUSHED);
  }

  if (check_client_connection == NETGINI_START_CONNECTING)
  {
    int cstatus = net->client_check_connection_attempt();
    
    if (cstatus == 0)
      check_client_connection = NETGINI_CONNECTION_FAILED;
    else if (cstatus == 1)
      check_client_connection = NETGINI_CONNECTED;
  }

  if (check_client_connection == NETGINI_CONNECTED)
  {
      switch_to_menu = MENU_NETGAME;
      move_out();
      check_client_connection = 0;
  }

  if (check_client_connection == NETGINI_CONNECTION_FAILED)
  {
      button[M_NETGAMEINI_CLIENT]->del_flag(MC_INVISIBLE | MC_INACTIVE);
      button[M_NETGAMEINI_CLIENT]->do_explosion();
      check_client_connection = 0;
  }


  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_NETGAMEINI_BACK]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_MAIN;
    button[M_NETGAMEINI_BACK]->set_flag(MC_INVISIBLE);
    button[M_NETGAMEINI_BACK]->do_explosion();
    button[M_NETGAMEINI_BACK]->del_flag(MC_PUSHED);
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  return 0;
}

void mNetGameInit::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
}

int mNetGameInit::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mNetGameInit::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_NETGAMEINI_CAPTION]->set_dest_x( button[M_NETGAMEINI_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

