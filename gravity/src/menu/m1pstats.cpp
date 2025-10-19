/*
 *   GRAVITY STRIKE -- menu/m1pstats.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "m1pstats.h"
#include "controls/mtypes.h"
#include "../gsglob.h"
#include "../gssound.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsglob.h"
#include "../gsplayer.h"
#include "../gserror.h"
#include "../gshiscor.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>
#include "../contrib/md5class.h"
#include <time.h>

m1PStats::m1PStats()
{
  active_button = 1;
  switch_to_menu = -1;
  no_input = 0;
  button_count = 3;
  moving = 0;
  bonus_flag = 0;
  esc_pressed = 0;

  for (int n=0; n < 50; n++)
    button[n] = NULL;
}

m1PStats::~m1PStats()
{
  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);
}

long int m1PStats::calc_score(int pl)
{
  int score = 0;
  statistics stat = globals->player_stats[pl];

  score += stat.mission_status == 1 ? 3000 : 0;
  score -= stat.lostlifes * 200;
  if (stat.bullets_shot == 0) stat.bullets_shot = 1;
  score += int( float(((float)stat.bullets_hit / (float)stat.bullets_shot) * 100.0) * 20.0);
  score += stat.extras_taken * 50;
  score += stat.enemy_bonus;

  if (stat.bonus != 0) bonus_flag = 1;
  score += stat.bonus;

  score -= stat.used_time * globals->score_per_second;

  return score;
}

typedef struct t_field {
  int x;
  int y;
} t_field;

void m1PStats::init(int n)
{
  if (!globals->use_user_stats) init_normal(n);
  else init_user(n);
}

void m1PStats::init_hiscore_notification(int mode, char *saveret)
{
  char tmp[250];
  int ypos = 395;

  // HISCORE NOTIFICATION TEXT
  button[M_1PSTATS_LASTHI] = new mNormalText();
  button[M_1PSTATS_LASTHI]->create(65, ypos, 65-640, ypos);
  button[M_1PSTATS_LASTHI]->set_int(FONT_IMPACT10);
  button[M_1PSTATS_LASTHI]->set_flag(MC_INACTIVE);

  if (mode == -2) // no display (new scores)
  {
    button[M_1PSTATS_LASTHI]->set_flag(MC_INVISIBLE);
  }
  else if (mode == -1) // hiscores disabled
  {
    sprintf(tmp, "%s", language->get_string(T_STATS_HISCORE_DISABLED));
    button[M_1PSTATS_LASTHI]->set_text(tmp);
    button[M_1PSTATS_LASTHI]->set_int(globals->col_orange);
  }
  else if (mode == 0) // not better
  {
    sprintf(tmp, "%s (%s)", language->get_string(T_STATS_HISCORE_OLD), saveret);
    button[M_1PSTATS_LASTHI]->set_text(tmp);
    button[M_1PSTATS_LASTHI]->set_int(globals->col_yellow);
  }
  else // normal score
  {
    sprintf(tmp, "%s (%s)", language->get_string(T_STATS_HISCORE_NEW), saveret);
    button[M_1PSTATS_LASTHI]->set_text(tmp);
    button[M_1PSTATS_LASTHI]->set_int(globals->col_green);
  }

  FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  int l = text_length(f, tmp);

  button[M_1PSTATS_LASTHI]->set_dest_x(globals->menu_resolution_x/2 - l/2);
}

void m1PStats::init_normal(int scatter)
{
  char tmp[50];

  statistics stat;
  int pl = 0;
  stat = globals->player_stats[pl];
  int datafont = FONT_IMPACT10;
  long int score[2];
  score[0] = calc_score(0);

  // hiscores
  sprintf(globals->score_string, "%d", (int)score[0]);
  char saveret[50]; memset(&saveret, 0, sizeof(saveret));
  strcpy(saveret, "");
  int himode = hiscore->save_hiscores(saveret);
  init_hiscore_notification(himode, saveret);

  // back
  button[M_1PSTATS_OKAY] = new mMainButton();
  if (scatter) button[M_1PSTATS_OKAY]->create(245, 430, 245, 500, 150, 30);
  else button[M_1PSTATS_OKAY]->create(245, 430, 0, 0, 150, 30);
  button[M_1PSTATS_OKAY]->set_text(language->get_string(T_MENU_OK));

  // caption
  button[M_1PSTATS_CAPTION] = new mNormalText();
  button[M_1PSTATS_CAPTION]->create(286, 64, 800, 64);
  button[M_1PSTATS_CAPTION]->set_int(FONT_BANK);
  button[M_1PSTATS_CAPTION]->set_text(language->get_string(T_STATS_CAP_1PLAYER));
  button[M_1PSTATS_CAPTION]->set_int(globals->col_yellow);
  button[M_1PSTATS_CAPTION]->set_flag(MC_INACTIVE);

  int ty = 0, tx = 35;
  t_field f[10][10]; // x and y pos

  // left row
  f[0][1].x = 30+tx; f[0][1].y = 175+ty;
  f[0][2].x = 13+tx; f[0][2].y = 200+ty;
  f[0][3].x = 13+tx; f[0][3].y = 225+ty;
  f[0][4].x = 13+tx; f[0][4].y = 250+ty;
  f[0][5].x = 13+tx; f[0][5].y = 275+ty;
  f[0][6].x = 13+tx; f[0][6].y = 300+ty;
  f[0][7].x = 30+tx; f[0][7].y = 325+ty;
  f[0][8].x = 30+tx; f[0][8].y = 360+ty;

  // top row
  f[1][0].x = 410+tx; f[1][0].y = 120+ty;

  // data fields
  for (int n=1; n <= 1; n++)
  {
    f[n][1].x = f[n][0].x; f[n][1].y = f[0][1].y +5;
    f[n][2].x = f[n][0].x; f[n][2].y = f[0][2].y +5;
    f[n][3].x = f[n][0].x; f[n][3].y = f[0][3].y +5;
    f[n][4].x = f[n][0].x; f[n][4].y = f[0][4].y +5;
    f[n][5].x = f[n][0].x; f[n][5].y = f[0][5].y +5;
    f[n][6].x = f[n][0].x; f[n][6].y = f[0][6].y +5;
    f[n][7].x = f[n][0].x; f[n][7].y = f[0][7].y +5;
    f[n][8].x = f[n][0].x; f[n][8].y = f[0][8].y +5;
  }

  // left row (description of items)
  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][1].x, f[0][1].y, f[0][1].x-640, f[0][1].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, language->get_string(T_STATS_MISSION_STATUS));
  if (stat.mission_status == 1)
    sprintf(tmp, "%s (%s)", tmp, language->get_string(T_STATS_COMPLETED));
  else
    sprintf(tmp, "%s (%s)", tmp, language->get_string(T_STATS_FAILED));
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][2].x, f[0][2].y, f[0][2].x-640, f[0][2].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, "- "); strcat(tmp, language->get_string(T_STATS_LOSTLIFES)); strcat(tmp, " (x200)");
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][3].x, f[0][3].y, f[0][3].x-640, f[0][3].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_HITPERCENT)); strcat(tmp, " (x20)");
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][4].x, f[0][4].y, f[0][4].x-640, f[0][4].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_EXTRASTAKEN)); strcat(tmp, " (x50)");
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][5].x, f[0][5].y, f[0][5].x-640, f[0][5].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_ENEMY_BONUS));
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  int npos = 6;
  if (bonus_flag)
  {
    button[button_count] = new mNormalText();
    button[button_count]->create(f[0][6].x, f[0][6].y, f[0][6].x-640, f[0][6].y);
    button[button_count]->set_int(FONT_ICE);
    strcpy(tmp, "+ "); strcat(tmp, language->get_string(T_STATS_BONUS));
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_green);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;
    npos = 7;
  }
  else npos = 6;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][npos].x, f[0][npos].y, f[0][npos].x-640, f[0][npos].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, "- "); strcat(tmp, language->get_string(T_STATS_TIMEPLAYED));
  sprintf(tmp, "%s (x%d)", tmp, globals->score_per_second);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);
  if (globals->score_per_second == 0) button[button_count]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[0][8].x, f[0][8].y, f[0][8].x-640, f[0][8].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, language->get_string(T_STATS_SCORE));
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_white);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  // top row (player name)
  button[button_count] = new mNormalText();
  button[button_count]->create(f[1][0].x, f[1][0].y, 640+f[1][0].x, f[1][0].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, globals->pplayer[0].pname);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  // Score Data

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][1].x, f[pl+1][1].y, 640+f[pl+1][1].x, f[pl+1][1].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%d", stat.mission_status == 1 ? 3000 : 0);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_yellow);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][2].x, f[pl+1][2].y, 640+f[pl+1][2].x, f[pl+1][2].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%d", stat.lostlifes);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_orange);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][3].x, f[pl+1][3].y, 640+f[pl+1][3].x, f[pl+1][3].y);
  button[button_count]->set_int(datafont);
  if (stat.bullets_shot == 0) stat.bullets_shot = 1;
  sprintf(tmp, "%.1f%%", float((float)stat.bullets_hit / (float)stat.bullets_shot) * 100.0);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_yellow);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][4].x, f[pl+1][4].y, 640+f[pl+1][4].x, f[pl+1][4].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%d", stat.extras_taken);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_yellow);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][5].x, f[pl+1][5].y, 640+f[pl+1][5].x, f[pl+1][5].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%ld", stat.enemy_bonus);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_yellow);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  if (bonus_flag)
  {
    npos = 7;
    button[button_count] = new mNormalText();
    button[button_count]->create(f[pl+1][6].x, f[pl+1][6].y, 640+f[pl+1][6].x, f[pl+1][6].y);
    button[button_count]->set_int(datafont);
    sprintf(tmp, "%ld", stat.bonus);
    button[button_count]->set_text(tmp);
    button[button_count]->set_int(globals->col_yellow);
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;
  }
  else npos = 6;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][npos].x, f[pl+1][npos].y, 640+f[pl+1][npos].x, f[pl+1][npos].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%ds", stat.used_time);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_orange);
  button[button_count]->set_flag(MC_INACTIVE);
  if (globals->score_per_second == 0) button[button_count]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button_count++;

  button[button_count] = new mNormalText();
  button[button_count]->create(f[pl+1][8].x, f[pl+1][8].y, 640+f[pl+1][8].x, f[pl+1][8].y);
  button[button_count]->set_int(datafont);
  sprintf(tmp, "%ld", score[pl]);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_white);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;
} // for players

void m1PStats::init_user(int scatter)
{
  int n, n2;
  char tmp[50];

  // hiscores
  if (strlen(globals->score_string) == 0)
    sprintf(globals->score_string, "0");
  char saveret[50]; strcpy(saveret, "");
  int hiret = hiscore->save_hiscores(saveret);

  init_hiscore_notification(hiret, saveret);

  // back
  button[M_1PSTATS_OKAY] = new mMainButton();
  if (scatter) button[M_1PSTATS_OKAY]->create(245, 430, 245, 500, 150, 30);
  else button[M_1PSTATS_OKAY]->create(245, 430, 0, 0, 150, 30);
  button[M_1PSTATS_OKAY]->set_text(language->get_string(T_MENU_OK));

  // caption
  button[M_1PSTATS_CAPTION] = new mNormalText();
  button[M_1PSTATS_CAPTION]->create(286, 64, 800, 64);
  button[M_1PSTATS_CAPTION]->set_int(FONT_BANK);
  button[M_1PSTATS_CAPTION]->set_text(language->get_string(T_STATS_CAP_1PLAYER));
  button[M_1PSTATS_CAPTION]->set_int(globals->col_yellow);
  button[M_1PSTATS_CAPTION]->set_flag(MC_INACTIVE);

  int ty = 0, tx = 35;
  t_field f[15][15]; // x and y pos
  memset(&f, 0, sizeof(f));

  // left row
  for (n=1; n <= 10; n++)
  {
    f[0][n].x = 13+tx; f[0][n].y = 150+ty+(n-1)*25;
  }

  // top row
  f[1][0].x = 350+tx; f[1][0].y = 110+ty;

  // data fields
  for (n=1; n <= 2; n++)
  {
    for (n2=1; n2 <= 10; n2++)
    {
      f[n][n2].x = f[n][0].x;
      f[n][n2].y = f[0][n2].y +5;
    }
  }

  // left row (description of items)
  for (n=0; n < 10; n++)
  {
    button[button_count] = new mNormalText();
    button[button_count]->create(f[0][n+1].x+globals->user_stats[n].get_plusx(), f[0][n+1].y, f[0][n+1].x-640, f[0][n+1].y);
    button[button_count]->set_int(FONT_ICE);
    button[button_count]->set_text(globals->user_stats[n].get_text());
    button[button_count]->set_int(globals->user_stats[n].get_textcolor());
    button[button_count]->set_flag(MC_INACTIVE);
  
    button_count++;
  }

  // top row (players)
  button[button_count] = new mNormalText();
  button[button_count]->create(f[1][0].x, f[1][0].y, 640+f[1][0].x, f[1][0].y);
  button[button_count]->set_int(FONT_ICE);
  strcpy(tmp, globals->pplayer[0].pname);
  button[button_count]->set_text(tmp);
  button[button_count]->set_int(globals->col_green);
  button[button_count]->set_flag(MC_INACTIVE);

  button_count++;

  // data fields
  int datafont = FONT_IMPACT10;

  for (n=0; n < 10; n++)
  {
    button[button_count] = new mNormalText();
    button[button_count]->create(f[1][n+1].x, f[1][n+1].y, 640+f[1][n+1].x, f[1][n+1].y);
    button[button_count]->set_int(datafont);
    button[button_count]->set_text(globals->user_stats[n].get_value(0));
    button[button_count]->set_int(globals->user_stats[n].get_valuecolor(0));
    button[button_count]->set_flag(MC_INACTIVE);

    button_count++;
  }
}

int m1PStats::check()
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

int m1PStats::handle_controls()
{
  int quit = 0;

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_1PSTATS_OKAY]->get_flags() & MC_PUSHED)
  {
    esc_pressed = 1;
    gsound->menu_select();
    switch_to_menu = globals->last_menu;
    button[M_1PSTATS_OKAY]->set_flag(MC_INVISIBLE);
    button[M_1PSTATS_OKAY]->set_flag(MC_INACTIVE);
    button[M_1PSTATS_OKAY]->del_flag(MC_PUSHED);
    button[M_1PSTATS_OKAY]->do_explosion();
    move_out();
  }

  if (move() == 0 && moving) return switch_to_menu;

  if (quit) return MENU_QUIT;

  return 0;
}

void m1PStats::draw(BITMAP *v)
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

int m1PStats::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

//  if (r) moving = 1;

  return r;
}

void m1PStats::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_1PSTATS_CAPTION]->set_dest_x( button[M_1PSTATS_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}


