/*
 *   GRAVITY STRIKE -- menu/mnwstats.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mnwstats.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gssound.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsglob.h"
#include "../gsplayer.h"
#include "../gsnetw.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>

mNetStats::mNetStats()
{
  active_button = 1;
  switch_to_menu = -1;
  no_input = 0;
  button_count = 2;
  moving = 0;
  bonus_flag = 0;
  esc_pressed = 0;

  net->get_netplayer(0)->ready = false;
  net->get_netplayer(0)->in_game = false;
  if (net->is_server()) net->set_server_in_game(false);

  for (int n=0; n < 80; n++)
    button[n] = NULL;
}

mNetStats::~mNetStats()
{
  globals->last_menu = MENU_NETGAME;

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

long int mNetStats::calc_score(int pl)
{
  if (!net->get_netplayer(pl)) return -1;

  int score = 0;
  statistics stat = net->get_netplayer(pl)->player_stats;

  score += stat.frags * 50;
  score -= stat.suicides * 50;
  if (stat.bullets_shot == 0) stat.bullets_shot = 1;
  score += int(float((float)stat.bullets_hit / (float)stat.bullets_shot) * 100.0) * 10;
  score += stat.extras_taken * 50;

  if (stat.bonus != 0) bonus_flag = 1;
  score += stat.bonus;

  return score;
}

typedef struct t_field {
  int x;
  int y;
} t_field;

void mNetStats::init(int n)
{
  if (!globals->use_user_stats) init_normal(n);
  else init_user(n);
}

void mNetStats::init_normal(int scatter)
{
  char tmp[50];

  statistics stat;
  long int score[8];
  int pl;

  for (pl=0; pl < 6; pl++)
    score[pl] = calc_score(pl);

  // back
  button[M_NETSTATS_OKAY] = new mMainButton();
  if (scatter) button[M_NETSTATS_OKAY]->create(245, 430, 245, 500, 150, 30);
  else button[M_NETSTATS_OKAY]->create(245, 430, 0, 0, 150, 30);
  button[M_NETSTATS_OKAY]->set_text(language->get_string(T_MENU_OK));

  // caption
  button[M_NETSTATS_CAPTION] = new mNormalText();
  button[M_NETSTATS_CAPTION]->create(286, 64, 800, 64);
  button[M_NETSTATS_CAPTION]->set_int(FONT_BANK);
  button[M_NETSTATS_CAPTION]->set_text(language->get_string(T_STATS_CAP_NETWORK));
  button[M_NETSTATS_CAPTION]->set_int(globals->col_yellow);
  button[M_NETSTATS_CAPTION]->set_flag(MC_INACTIVE);

  int row[10], col[10];

  if (globals->game_mode == GAME_DOGFIGHT)
  {
    row[0] = 175;
    row[1] = 205;
    row[2] = 235;
    row[3] = 265;
    row[4] = 295;
    row[5] = 345;

    col[0] = 30;
    col[1] = 240;
    col[2] = 300;
    col[3] = 360;
    col[4] = 420;
    col[5] = 480;
    col[6] = 540;
    col[7] = 600;

    // left row (description of items)
    button[button_count] = new mNormalText();
    button[button_count]->create(col[0]+10, row[0], col[0]-640, row[0]);
    button[button_count]->set_int(FONT_IMPACT10);
    strcpy(tmp, language->get_string(T_STATS_FRAGS)); strcat(tmp, " (x50)");
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    button[button_count] = new mNormalText();
    button[button_count]->create(col[0], row[1], col[0]-640, row[1]);
    button[button_count]->set_int(FONT_IMPACT10);
    strcpy(tmp, "- "); strcat(tmp, language->get_string(T_STATS_SUICIDES)); strcat(tmp, " (x50)");
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    button[button_count] = new mNormalText();
    button[button_count]->create(col[0], row[2], col[0]-640, row[2]);
    button[button_count]->set_int(FONT_IMPACT10);
    strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_HITPERCENT)); strcat(tmp, " (x10)");
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    button[button_count] = new mNormalText();
    button[button_count]->create(col[0], row[3], col[0]-640, row[3]);
    button[button_count]->set_int(FONT_IMPACT10);
    strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_EXTRASTAKEN)); strcat(tmp, " (x50)");
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    if (bonus_flag)
    {
      button[button_count] = new mNormalText();
      button[button_count]->create(col[0], row[4], col[0]-640, row[4]);
      button[button_count]->set_int(FONT_IMPACT10);
      strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_BONUS));
      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_green);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;
    }

    button[button_count] = new mNormalText();
    button[button_count]->create(col[0]+17, row[5], col[0]-640, row[5]);
    button[button_count]->set_int(FONT_IMPACT10);
    strcpy(tmp, language->get_string(T_STATS_SCORE));
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_white);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    // main stats
    for (pl=0; pl < 6; pl++)
    {
      if (!net->get_netplayer(pl)) continue;

      stat = net->get_netplayer(pl)->player_stats;
      int datafont = FONT_IMPACT10;
      FONT *f = (FONT*)globals->fontdat[datafont].dat;
      int pname_size = strlen(stat.pname) > 0 ? text_length(f, stat.pname) : 0;
      int totop;

      if (pl % 2 == 0) totop = 50; else totop = 32;

      // player names on top
      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl]-pname_size/2, row[0]-totop, 640+col[1+pl], row[0]-totop);
      button[button_count]->set_int(FONT_IMPACT10);
      if (strlen(stat.pname) > 0)
        button[button_count]->set_text(stat.pname);
      else
        button[button_count]->set_text("--");
      button[button_count]->set_int(globals->col_green);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;

      // data fields
      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[0], 640+col[1+pl], row[0]);
      button[button_count]->set_int(datafont);
      sprintf(tmp, "%d", stat.frags);
      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_yellow);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;
  
      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[1], 640+col[1+pl], row[1]);
      button[button_count]->set_int(datafont);
      sprintf(tmp, "%d", stat.suicides);
      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_orange);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;

      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[2], 640+col[1+pl], row[2]);
      button[button_count]->set_int(datafont);
      if (stat.bullets_shot == 0) stat.bullets_shot = 1;
      sprintf(tmp, "%.1f%%", float((float)stat.bullets_hit / (float)stat.bullets_shot) * 100.0);
      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_yellow);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;

      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[3], 640+col[1+pl], row[3]);
      button[button_count]->set_int(datafont);
      sprintf(tmp, "%d", stat.extras_taken);
      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_yellow);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;

      if (bonus_flag)
      {
        button[button_count] = new mNormalText();
        button[button_count]->create(col[1+pl], row[4], 640+col[1+pl], row[4]);
        button[button_count]->set_int(datafont);
        sprintf(tmp, "%ld", stat.bonus);
        button[button_count]->set_text(tmp);
        button[button_count]->set_int(globals->col_yellow);
        button[button_count]->set_flag(MC_INACTIVE);
    
        button_count++;
      }

      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[5], 640+col[1+pl], row[5]);
      button[button_count]->set_int(datafont);
      sprintf(tmp, "%ld", score[pl]);

      button[button_count]->set_text(tmp);
      button[button_count]->set_int(globals->col_white);
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;
    } // for players

  } // dogfight

  else if (globals->game_mode == GAME_RACE)
  {
  } // race

}

void mNetStats::init_user(int scatter)
{
  statistics stat;
  int pl,n;

  // back
  button[M_NETSTATS_OKAY] = new mMainButton();
  if (scatter) button[M_NETSTATS_OKAY]->create(245, 430, 245, 500, 150, 30);
  else button[M_NETSTATS_OKAY]->create(245, 430, 0, 0, 150, 30);
  button[M_NETSTATS_OKAY]->set_text(language->get_string(T_MENU_OK));

  // caption
  button[M_NETSTATS_CAPTION] = new mNormalText();
  button[M_NETSTATS_CAPTION]->create(286, 64, 800, 64);
  button[M_NETSTATS_CAPTION]->set_int(FONT_BANK);
  button[M_NETSTATS_CAPTION]->set_text(language->get_string(T_STATS_CAP_NETWORK));
  button[M_NETSTATS_CAPTION]->set_int(globals->col_yellow);
  button[M_NETSTATS_CAPTION]->set_flag(MC_INACTIVE);

  int row[15], col[15];

  row[0] = 175+0*25;
  row[1] = 175+1*25;
  row[2] = 175+2*25;
  row[3] = 175+3*25;
  row[4] = 175+4*25;
  row[5] = 175+5*25;
  row[6] = 175+6*25;
  row[7] = 175+7*25;
  row[8] = 175+8*25;
  row[9] = 175+9*25;
  row[10] = 175+10*25;

  col[0] = 30;
  col[1] = 240;
  col[2] = 300;
  col[3] = 360;
  col[4] = 420;
  col[5] = 480;
  col[6] = 540;
  col[7] = 600;

  // left row (description of items)
  for (n=0; n < 10; n++)
  {
    button[button_count] = new mNormalText();
    button[button_count]->create(col[0]+globals->user_stats[n].get_plusx(), row[n], col[0]-640, row[n]);
    button[button_count]->set_int(FONT_IMPACT10);
    button[button_count]->set_text(globals->user_stats[n].get_text());
    button[button_count]->set_int(globals->user_stats[n].get_textcolor());
    button[button_count]->set_flag(MC_INACTIVE);
  
    button_count++;
  }

  // main stats
  for (pl=0; pl < 6; pl++)
  {
    if (!net->get_netplayer(pl)) continue;

    stat = net->get_netplayer(pl)->player_stats;
    int datafont = FONT_IMPACT10;
    FONT *f = (FONT*)globals->fontdat[datafont].dat;
    int pname_size = strlen(stat.pname) > 0 ? text_length(f, stat.pname) : 0;
    int totop;

    if (pl % 2 == 0) totop = 50; else totop = 32;

    // player names on top
    button[button_count] = new mNormalText();
    button[button_count]->create(col[1+pl]-pname_size/2, row[0]-totop, 640+col[1+pl], row[0]-totop);
    button[button_count]->set_int(FONT_IMPACT10);
    if (strlen(stat.pname) > 0)
      button[button_count]->set_text(stat.pname);
    else
      button[button_count]->set_text("--");
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;

    // data fields
    for (n=0; n < 10; n++)
    {
      button[button_count] = new mNormalText();
      button[button_count]->create(col[1+pl], row[n], 640+col[1+pl], row[n]);
      button[button_count]->set_int(datafont);
      button[button_count]->set_text(globals->user_stats[n].get_value(pl));
      button[button_count]->set_int(globals->user_stats[n].get_valuecolor(pl));
      button[button_count]->set_flag(MC_INACTIVE);
  
      button_count++;
    }
  } // for players

}

int mNetStats::check()
{
  // network
  net->process_packets();

  // updates
  if (net->is_server() && globals->game_time % 60 == 0)
    net->menu_server_update_clients();

  if (net->is_client() && globals->game_time % 30 == 0)
    net->menu_client_update_server();

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

int mNetStats::handle_controls()
{
  int quit = 0;

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_NETSTATS_OKAY]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = MENU_NETGAME;
    button[M_NETSTATS_OKAY]->set_flag(MC_INVISIBLE);
    button[M_NETSTATS_OKAY]->set_flag(MC_INACTIVE);
    button[M_NETSTATS_OKAY]->del_flag(MC_PUSHED);
    button[M_NETSTATS_OKAY]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  if (quit) return MENU_QUIT;

  return 0;
}

void mNetStats::draw(BITMAP *v)
{
  for (int n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }

/*
  char tmpstr[50];
  text_mode(0);
  textout(globals->vscreen, font, ftoa(tmpstr, "abutton", active_button), 10, 400, makecol8(255,255,255));
  textout(globals->vscreen, font, ftoa(tmpstr, "switch", switch_to_menu), 10, 410, makecol8(255,255,255));
*/
}

int mNetStats::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

//  if (r) moving = 1;

  return r;
}

void mNetStats::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_NETSTATS_CAPTION]->set_dest_x( button[M_NETSTATS_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

