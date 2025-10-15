/*
 *   GRAVITY STRIKE -- menu/mnetgame.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mnetgame.h"
#include "allegro5_wrapper.h"
#include "controls/mtypes.h"
#include "../gshelper.h"
#include "../gslng.h"
#include "../gsplayer.h"
#include "../gsfade.h"
#include "../gssound.h"
#include "../gserror.h"
#include "../gsscript.h"
#include "../gsnetw.h"
#include "../gsclear.h"
#include "../gsglob.h"
#include "gsmd.h"
#include "gsmenu.h"
#include <stdio.h>

mNetGame::mNetGame(int mode)
{
  active_button = 1;
  button_count = 43;
  moving = 0;
  switch_to_menu = -1;
  no_input = 0;
  minimap = NULL;
  moved_in = FALSE;
  esc_pressed = 0;

  globals->minigame = 0;
  globals->singleplayer = 0;
  globals->network_game = 1;

  net->reset_status_vars();

  last_level = -99;
  globals->ready = 0;

  int n;

  for (n=0; n < button_count; n++)
    button[n] = NULL;

  script->read_level_files();
  globals->selected_level = globals->get_level_num_from_filename(globals->last_level);

  // reinit deleted players (when returning from game)
  for (n=0; n < net->get_netplayers(); n++)
    if (net->get_netplayer(n) && !net->get_netplayer(n)->player)
    {
      net->get_netplayer(n)->player = new gsPlayer(net->get_netplayer(n)->net_id.binaryAddress / 10000, 0, 0, net->get_netplayer(n)->ship_file);
      if (n > 0) net->get_netplayer(n)->player->set_network(1);
      strncpy(net->get_netplayer(n)->shipimg_file, net->get_netplayer(n)->player->get_shipimage_filename(), 50);
    }
}

mNetGame::~mNetGame()
{
  globals->last_menu = MENU_NETGAME;

  strcpy(globals->pplayer[0].pname, button[M_NETGAME_MYNAME]->get_text());

  for (int n=0; n < button_count; n++)
    if (button[n]) delete(button[n]);

  if (minimap)  destroy_bitmap(minimap);
  
  if (switch_to_menu != START_NETGAME && switch_to_menu != MENU_CHOOSESHIP && switch_to_menu != MENU_SHIPBUILDER) 
    net->reset();
}

void mNetGame::init(int scatter, int ship_changed)
{
  int col = makecol8(58, 64, 191);

  if (ship_changed) 
  { 
    net->create_local_player(globals->pplayer[0].pname, globals->pplayer[0].pship); 
    if (net->is_server()) net->get_netplayer(0)->ready = true;
    net->menu_update_shipfiles();
  }

  button[M_NETGAME_CAPTION] = new mNormalText();
  button[M_NETGAME_CAPTION]->create(286, 64, 800, 64);
  button[M_NETGAME_CAPTION]->set_int(FONT_BANK);
  if (net->is_server())
    button[M_NETGAME_CAPTION]->set_text(language->get_string(T_MENU_CAP_NETGAME_SERVER));
  else
    button[M_NETGAME_CAPTION]->set_text(language->get_string(T_MENU_CAP_NETGAME_CLIENT));
  button[M_NETGAME_CAPTION]->set_int(globals->col_yellow);
  button[M_NETGAME_CAPTION]->set_flag(MC_INACTIVE);

  button[M_NETGAME_BACK] = new mMainButton();
  if (scatter) button[M_NETGAME_BACK]->create(30, 430, 30, 500, 80, 30);
  else button[M_NETGAME_BACK]->create(30, 430, 0, 0, 80, 30);
  button[M_NETGAME_BACK]->set_text(language->get_string(T_MENU_BACK));

  // I'm ready!
  button[M_NETGAME_READY] = new mCheckBox();
  button[M_NETGAME_READY]->create(450, 440, 640, 500, 17, 12);
  button[M_NETGAME_READY]->set_int(150);
  button[M_NETGAME_READY]->set_text(language->get_string(T_MENU_I_AM_READY));
  button[M_NETGAME_READY]->set_color1(globals->col_yellow);
  //if (net->is_server())
    button[M_NETGAME_READY]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  button[M_NETGAME_PLAY] = new mMainButton();
  button[M_NETGAME_PLAY]->create(450, 430, 450, 500, 150, 30);
  button[M_NETGAME_PLAY]->set_text(language->get_string(T_MENU_PLAY));
  button[M_NETGAME_PLAY]->set_color1(globals->col_yellow);
  if (net->is_client())
    button[M_NETGAME_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);

  // chat message input line
  button[M_NETGAME_CHAT_INPUT] = new mTextField();
  button[M_NETGAME_CHAT_INPUT]->create(145, 438, 145, 700, 300, 15, 40);
  button[M_NETGAME_CHAT_INPUT]->set_int(80);
  button[M_NETGAME_CHAT_INPUT]->set_text("Chat:");


  char playerstr[20];
  int xab=0, yab=25, xab2=30;

  button[M_NETGAME_MYSHIP] = new mShipIcon2();
  button[M_NETGAME_MYSHIP]->create(30+xab, 5+200+yab, -300+30+xab, 5+200+yab, 60, 51);
  button[M_NETGAME_MYSHIP]->set_int(1);
  button[M_NETGAME_MYSHIP]->set_player(net->get_netplayer(0)->player);

  button[M_NETGAME_MYCHOOSE] = new mSmallButton();
  button[M_NETGAME_MYCHOOSE]->create(30+xab+65, 5+200+yab, -300+130+xab, 5+200+yab, 100, 15);
  button[M_NETGAME_MYCHOOSE]->set_text(language->get_string(T_MENU_2P_CHOOSESHIP));

  button[M_NETGAME_MYEDIT] = new mSmallButton();
  button[M_NETGAME_MYEDIT]->create(30+xab+65, 5+218+yab, -300+130+xab, 5+220+yab, 100, 15);
  button[M_NETGAME_MYEDIT]->set_text(language->get_string(T_MENU_2P_EDITSHIP));

  button[M_NETGAME_MYNAME] = new mTextField();
  button[M_NETGAME_MYNAME]->create(30+xab+65, 5+236+yab, -300+130+xab, 5+240+yab, 100, 15, 0);
  button[M_NETGAME_MYNAME]->set_int(20);
  button[M_NETGAME_MYNAME]->set_text(" ");
  button[M_NETGAME_MYNAME]->set_text(net->get_netplayer(0)->name);

  button[M_NETGAME_P2SHIP] = new mShipIcon2();
  button[M_NETGAME_P2SHIP]->create(200+xab+xab2, 5+200+yab, 640+xab+xab2, 5+200+yab, 60, 51);
  button[M_NETGAME_P2SHIP]->set_int(1);
  button[M_NETGAME_P2SHIP]->set_player(NULL);

  button[M_NETGAME_P2CAP] = new mNormalText();
  button[M_NETGAME_P2CAP]->create(200+xab+xab2+65, 5+200+yab, 640+100+xab+xab2, 5+200+yab);
  button[M_NETGAME_P2CAP]->set_int(FONT_IMPACT10);
  sprintf(playerstr, "%s %d", language->get_string(T_STATS_PLAYER), 2);
  button[M_NETGAME_P2CAP]->set_text(playerstr);
  button[M_NETGAME_P2CAP]->set_int(col);

  button[M_NETGAME_P2NAME] = new mNormalText();
  button[M_NETGAME_P2NAME]->create(200+xab+xab2+65, 5+220+yab, 640+100+xab2+xab, 5+220+yab);
  button[M_NETGAME_P2NAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P2NAME]->set_text("n/a");
  button[M_NETGAME_P2NAME]->set_int(globals->col_green);

  button[M_NETGAME_P2READY] = new mNormalText();
  button[M_NETGAME_P2READY]->create(200+xab+xab2+65, 1+240+yab, 640+100+xab2+xab, 5+240+yab);
  button[M_NETGAME_P2READY]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P2READY]->set_text("playing");
  button[M_NETGAME_P2READY]->set_int(globals->col_orange);

  button[M_NETGAME_P3SHIP] = new mShipIcon2();
  button[M_NETGAME_P3SHIP]->create(370+xab+xab2+xab2, 5+200+yab, 640+xab+xab2, 5+200+yab, 60, 51);
  button[M_NETGAME_P3SHIP]->set_int(1);
  button[M_NETGAME_P3SHIP]->set_player(NULL);

  button[M_NETGAME_P3CAP] = new mNormalText();
  button[M_NETGAME_P3CAP]->create(370+xab+xab2+xab2+65, 5+200+yab, 640+100+xab+xab2, 5+200+yab);
  button[M_NETGAME_P3CAP]->set_int(FONT_IMPACT10);
  sprintf(playerstr, "%s %d", language->get_string(T_STATS_PLAYER), 3);
  button[M_NETGAME_P3CAP]->set_text(playerstr);
  button[M_NETGAME_P3CAP]->set_int(col);

  button[M_NETGAME_P3NAME] = new mNormalText();
  button[M_NETGAME_P3NAME]->create(370+xab+xab2+xab2+65, 5+220+yab, 640+100+xab2+xab, 5+220+yab);
  button[M_NETGAME_P3NAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P3NAME]->set_text("n/a");
  button[M_NETGAME_P3NAME]->set_int(globals->col_green);

  button[M_NETGAME_P3READY] = new mNormalText();
  button[M_NETGAME_P3READY]->create(370+xab+xab2+xab2+65, 1+240+yab, 640+100+xab2+xab, 5+240+yab);
  button[M_NETGAME_P3READY]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P3READY]->set_text("playing");
  button[M_NETGAME_P3READY]->set_int(globals->col_orange);

  button[M_NETGAME_P4SHIP] = new mShipIcon2();
  button[M_NETGAME_P4SHIP]->create(30+xab, 5+255+yab, 640+xab+xab2, 5+255+yab, 60, 51);
  button[M_NETGAME_P4SHIP]->set_int(1);
  button[M_NETGAME_P4SHIP]->set_player(NULL);

  button[M_NETGAME_P4CAP] = new mNormalText();
  button[M_NETGAME_P4CAP]->create(30+xab+65, 5+255+yab, 640+100+xab+xab2, 5+255+yab);
  button[M_NETGAME_P4CAP]->set_int(FONT_IMPACT10);
  sprintf(playerstr, "%s %d", language->get_string(T_STATS_PLAYER), 4);
  button[M_NETGAME_P4CAP]->set_text(playerstr);
  button[M_NETGAME_P4CAP]->set_int(col);

  button[M_NETGAME_P4NAME] = new mNormalText();
  button[M_NETGAME_P4NAME]->create(30+xab+65, 5+275+yab, 640+100+xab2+xab, 5+275+yab);
  button[M_NETGAME_P4NAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P4NAME]->set_text("n/a");
  button[M_NETGAME_P4NAME]->set_int(globals->col_green);

  button[M_NETGAME_P4READY] = new mNormalText();
  button[M_NETGAME_P4READY]->create(30+xab+65, 1+295+yab, 640+100+xab2+xab, 5+295+yab);
  button[M_NETGAME_P4READY]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P4READY]->set_text("playing");
  button[M_NETGAME_P4READY]->set_int(globals->col_orange);

  button[M_NETGAME_P5SHIP] = new mShipIcon2();
  button[M_NETGAME_P5SHIP]->create(200+xab+xab2, 5+255+yab, 640+xab+xab2, 5+255+yab, 60, 51);
  button[M_NETGAME_P5SHIP]->set_int(1);
  button[M_NETGAME_P5SHIP]->set_player(NULL);

  button[M_NETGAME_P5CAP] = new mNormalText();
  button[M_NETGAME_P5CAP]->create(200+xab+xab2+65, 5+255+yab, 640+100+xab+xab2, 5+255+yab);
  button[M_NETGAME_P5CAP]->set_int(FONT_IMPACT10);
  sprintf(playerstr, "%s %d", language->get_string(T_STATS_PLAYER), 5);
  button[M_NETGAME_P5CAP]->set_text(playerstr);
  button[M_NETGAME_P5CAP]->set_int(col);

  button[M_NETGAME_P5NAME] = new mNormalText();
  button[M_NETGAME_P5NAME]->create(200+xab+xab2+65, 5+275+yab, 640+100+xab2+xab, 5+275+yab);
  button[M_NETGAME_P5NAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P5NAME]->set_text("n/a");
  button[M_NETGAME_P5NAME]->set_int(globals->col_green);

  button[M_NETGAME_P5READY] = new mNormalText();
  button[M_NETGAME_P5READY]->create(200+xab+xab2+65, 1+295+yab, 640+100+xab2+xab, 5+295+yab);
  button[M_NETGAME_P5READY]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P5READY]->set_text("playing");
  button[M_NETGAME_P5READY]->set_int(globals->col_orange);

  button[M_NETGAME_P6SHIP] = new mShipIcon2();
  button[M_NETGAME_P6SHIP]->create(370+xab+xab2+xab2, 5+255+yab, 640+xab+xab2, 5+255+yab, 60, 51);
  button[M_NETGAME_P6SHIP]->set_int(1);
  button[M_NETGAME_P6SHIP]->set_player(NULL);

  button[M_NETGAME_P6CAP] = new mNormalText();
  button[M_NETGAME_P6CAP]->create(370+xab+xab2+xab2+65, 5+255+yab, 640+100+xab+xab2, 5+255+yab);
  button[M_NETGAME_P6CAP]->set_int(FONT_IMPACT10);
  sprintf(playerstr, "%s %d", language->get_string(T_STATS_PLAYER), 6);
  button[M_NETGAME_P6CAP]->set_text(playerstr);
  button[M_NETGAME_P6CAP]->set_int(col);

  button[M_NETGAME_P6NAME] = new mNormalText();
  button[M_NETGAME_P6NAME]->create(370+xab+xab2+xab2+65, 5+275+yab, 640+100+xab2+xab, 5+275+yab);
  button[M_NETGAME_P6NAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P6NAME]->set_text("n/a");
  button[M_NETGAME_P6NAME]->set_int(globals->col_green);

  button[M_NETGAME_P6READY] = new mNormalText();
  button[M_NETGAME_P6READY]->create(370+xab+xab2+xab2+65, 1+295+yab, 640+100+xab2+xab, 5+295+yab);
  button[M_NETGAME_P6READY]->set_int(FONT_IMPACT10);
  button[M_NETGAME_P6READY]->set_text("playing");
  button[M_NETGAME_P6READY]->set_int(globals->col_orange);  

  int n;
  for (n=M_NETGAME_P2SHIP; n <= M_NETGAME_P6READY; n++)
    button[n]->set_flag(MC_INVISIBLE | MC_INACTIVE);

  button[M_NETGAME_LEVCAP] = new mNormalText();
  button[M_NETGAME_LEVCAP]->create(30, 100, -300-90, 120);
  button[M_NETGAME_LEVCAP]->set_int(FONT_IMPACT10);
  button[M_NETGAME_LEVCAP]->set_text("Level");
  button[M_NETGAME_LEVCAP]->set_int(col);
  button[M_NETGAME_LEVCAP]->set_flag(MC_INACTIVE);

  button[M_NETGAME_LEVSELECT] = new mLevelSelect();
  button[M_NETGAME_LEVSELECT]->create(120, 100, -300, 120, 170, 65);
  button[M_NETGAME_LEVSELECT]->set_int(globals->selected_level);
  if (!net->is_server()) button[M_NETGAME_LEVSELECT]->set_flag(MC_INVISIBLE);

  button[M_NETGAME_LEVELNAME] = new mNormalText();
  button[M_NETGAME_LEVELNAME]->create(120, 100, -300, 120);
  button[M_NETGAME_LEVELNAME]->set_int(FONT_IMPACT10);
  button[M_NETGAME_LEVELNAME]->set_text("n/a");
  button[M_NETGAME_LEVELNAME]->set_int(globals->col_yellow);
  button[M_NETGAME_LEVELNAME]->set_flag(MC_INACTIVE);
  if (net->is_server()) button[M_NETGAME_LEVELNAME]->set_flag(MC_INVISIBLE);

  button[M_NETGAME_LEVELEXTRA] = new mCheckBox();
  button[M_NETGAME_LEVELEXTRA]->create(330, 100, 640, 120, 17, 12);
  button[M_NETGAME_LEVELEXTRA]->set_int(-1);
  button[M_NETGAME_LEVELEXTRA]->set_text(language->get_string(T_MENU_LEVELEXTRA));
  if (globals->activate_extra)
    button[M_NETGAME_LEVELEXTRA]->set_flag(MC_PUSHED);

  button[M_NETGAME_CTF] = new mCheckBox();
  button[M_NETGAME_CTF]->create(330, 120, 640, 140, 17, 12);
  button[M_NETGAME_CTF]->set_int(-1);
  button[M_NETGAME_CTF]->set_text(language->get_string(T_MENU_CAPTURE_THE_FLAG));
  if (globals->capture_the_flag)
    button[M_NETGAME_CTF]->set_flag(MC_PUSHED);

  button[M_NETGAME_HH] = new mCheckBox();
  button[M_NETGAME_HH]->create(330, 140, 640, 160, 17, 12);
  button[M_NETGAME_HH]->set_int(-1);
  button[M_NETGAME_HH]->set_text(language->get_string(T_MENU_HUNTER_HUNTED));
  if (globals->hunter_hunted)
    button[M_NETGAME_HH]->set_flag(MC_PUSHED);

  // options
  yab = 180;

  button[M_NETGAME_OPT_GRAVITY] = new mTextSlider();
  button[M_NETGAME_OPT_GRAVITY]->set_int(1);
  button[M_NETGAME_OPT_GRAVITY]->create(30, 0+yab, -400, 0+yab, 220, 14, 90);
  button[M_NETGAME_OPT_GRAVITY]->set_text(language->get_string(T_MENU_GRAVITY));
  button[M_NETGAME_OPT_GRAVITY]->get_bar()->set_int((int(globals->gravity_percent * 100.0) + 100) / 3);

  button[M_NETGAME_OPT_GRAVITY_TXT] = new mNormalText();
  button[M_NETGAME_OPT_GRAVITY_TXT]->create(260, 0+yab, -400-30, 0+yab);
  button[M_NETGAME_OPT_GRAVITY_TXT]->set_int(FONT_IMPACT10);
  button[M_NETGAME_OPT_GRAVITY_TXT]->set_text("n/a");
  button[M_NETGAME_OPT_GRAVITY_TXT]->set_int(globals->col_green);
  button[M_NETGAME_OPT_GRAVITY_TXT]->set_flag(MC_INACTIVE);

  button[M_NETGAME_OPT_MAXFRAGS] = new mTextSlider();
  button[M_NETGAME_OPT_MAXFRAGS]->set_int(1);
  button[M_NETGAME_OPT_MAXFRAGS]->create(330, 0+yab, 640, 0+yab, 220, 14, 90);
  button[M_NETGAME_OPT_MAXFRAGS]->set_text(language->get_string(T_MENU_MAXFRAGS));
  button[M_NETGAME_OPT_MAXFRAGS]->get_bar()->set_int(globals->max_frags*2);

  button[M_NETGAME_OPT_MAXFRAGS_TXT] = new mNormalText();
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->create(560, 0+yab, 640, 0+yab);
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->set_int(FONT_IMPACT10);
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->set_text("n/a");
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->set_int(globals->col_green);
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->set_flag(MC_INACTIVE);

  button[M_NETGAME_OPT_EXTRAS] = new mTextSlider();
  button[M_NETGAME_OPT_EXTRAS]->set_int(1);
  button[M_NETGAME_OPT_EXTRAS]->create(30, 17+yab, -400, 17+yab, 220, 14, 90);
  button[M_NETGAME_OPT_EXTRAS]->set_text(language->get_string(T_MENU_EXTRAS));
  button[M_NETGAME_OPT_EXTRAS]->get_bar()->set_int(globals->extra_amount * 10);

  button[M_NETGAME_OPT_EXTRAS_TXT] = new mNormalText();
  button[M_NETGAME_OPT_EXTRAS_TXT]->create(260, 17+yab, -400-30, 17+yab);
  button[M_NETGAME_OPT_EXTRAS_TXT]->set_int(FONT_IMPACT10);
  button[M_NETGAME_OPT_EXTRAS_TXT]->set_text("n/a");
  button[M_NETGAME_OPT_EXTRAS_TXT]->set_int(globals->col_green);
  button[M_NETGAME_OPT_EXTRAS_TXT]->set_flag(MC_INACTIVE);

  button[M_NETGAME_OPT_MAXTIME] = new mTextSlider();
  button[M_NETGAME_OPT_MAXTIME]->set_int(1);
  button[M_NETGAME_OPT_MAXTIME]->create(330, 17+yab, 640, 17+yab, 220, 14, 90);
  button[M_NETGAME_OPT_MAXTIME]->set_text(language->get_string(T_MENU_MAXTIME));
  button[M_NETGAME_OPT_MAXTIME]->get_bar()->set_int((globals->max_time)/30);

  button[M_NETGAME_OPT_MAXTIME_TXT] = new mNormalText();
  button[M_NETGAME_OPT_MAXTIME_TXT]->create(560, 17+yab, 640, 17+yab);
  button[M_NETGAME_OPT_MAXTIME_TXT]->set_int(FONT_IMPACT10);
  button[M_NETGAME_OPT_MAXTIME_TXT]->set_text("n/a");
  button[M_NETGAME_OPT_MAXTIME_TXT]->set_int(globals->col_green);
  button[M_NETGAME_OPT_MAXTIME_TXT]->set_flag(MC_INACTIVE);

  if (!net->is_server())
    for (n=M_NETGAME_LEVCAP; n <= M_NETGAME_OPT_MAXTIME_TXT; n++)
      button[n]->set_flag(MC_INACTIVE);

  button[M_NETGAME_OPT_GRAVITY]->get_bar()->set_int((globals->gravity_percent*100+100)/3);
  button[M_NETGAME_OPT_EXTRAS]->get_bar()->set_int(globals->extra_amount*10);
  button[M_NETGAME_OPT_MAXFRAGS]->get_bar()->set_int(globals->max_frags*2-1);
  button[M_NETGAME_OPT_MAXTIME]->get_bar()->set_int((globals->max_time)/30);

}

int mNetGame::check()
{
  // network messages
  net->process_packets();

  // updates
  if (net->is_server() && globals->game_time % 60 == 0)
    net->menu_server_update_clients();

  if (net->is_client() && globals->game_time % 30 == 0)
    net->menu_client_update_server();

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
    else if (key[KEY_ENTER] && active_button != M_NETGAME_CHAT_INPUT)
    {
      if (active_button >= 0)
        button[active_button]->set_flag(MC_PUSHED);
      clear_key(KEY_ENTER);
    }
  }

  return(handle_controls());
}

int mNetGame::handle_controls()
{
  int i;

  if (!moving) if (net->is_client() && !net->client_is_connected()) key[KEY_ESC] = 1;
/*
  if (net->is_server() && globals->game_time % 60 == 0)
  {
    net->send_test_data(10000);
  }
*/
  // chat (get_align() stands for is_enter_pressed() ;-))
  if (button[M_NETGAME_CHAT_INPUT]->get_align())
  {
    net->chat.add(button[M_NETGAME_CHAT_INPUT]->get_text(), net->get_netplayer(0)->name, globals->col_white);
    net->game_send_chat_message(button[M_NETGAME_CHAT_INPUT]->get_text());
    button[M_NETGAME_CHAT_INPUT]->set_text("");
  }

  // update menu items
  int n;
  for (n=0; n < MAX_NET_PLAYERS-1; n++)
  {
    gsNetPlayer *ok = net->get_netplayer(n+1);
    
    if (ok)
      button[M_NETGAME_P2SHIP+n*4]->set_player(ok->player);
    else
      button[M_NETGAME_P2SHIP+n*4]->set_player(NULL);

    if (globals->game_time % 30 == 0)
    {
      if (ok)
      {
        if (net->get_netplayer(n+1)->player)
        {
          button[M_NETGAME_P2NAME+n*4]->set_text(net->get_netplayer(n+1)->name);
          button[M_NETGAME_P2SHIP+n*4]->del_flag(MC_INVISIBLE | MC_INACTIVE);
          button[M_NETGAME_P2NAME+n*4]->del_flag(MC_INVISIBLE | MC_INACTIVE);
          button[M_NETGAME_P2CAP+n*4]->del_flag(MC_INVISIBLE | MC_INACTIVE);
          if (net->get_netplayer(n+1)->ready)
            button[M_NETGAME_P2READY+n*4]->del_flag(MC_INVISIBLE);
          else
            button[M_NETGAME_P2READY+n*4]->set_flag(MC_INVISIBLE);
        }
      }
      else
      {
        button[M_NETGAME_P2NAME+n*4]->set_text("n/a");
        button[M_NETGAME_P2SHIP+n*4]->set_flag(MC_INVISIBLE | MC_INACTIVE);
        button[M_NETGAME_P2NAME+n*4]->set_flag(MC_INVISIBLE | MC_INACTIVE);
        button[M_NETGAME_P2CAP+n*4]->set_flag(MC_INVISIBLE | MC_INACTIVE);
        button[M_NETGAME_P2READY+n*4]->set_flag(MC_INVISIBLE);
      }
    }
  }

  // update some other stuff
  if (globals->game_time % 60 == 0)
  {
    strncpy(net->get_netplayer(0)->name, button[M_NETGAME_MYNAME]->get_text(), 20);

    // show/hide "play" button
    if (net->is_client())
    {
      if (net->get_server_in_game())
        button[M_NETGAME_PLAY]->del_flag(MC_INACTIVE | MC_INVISIBLE);
      else
        button[M_NETGAME_PLAY]->set_flag(MC_INACTIVE | MC_INVISIBLE);
    }
  }

  if (net->is_client())
  {
    if (globals->activate_extra)
      button[M_NETGAME_LEVELEXTRA]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_LEVELEXTRA]->del_flag(MC_PUSHED);

    if (globals->capture_the_flag)
      button[M_NETGAME_CTF]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_CTF]->del_flag(MC_PUSHED);

    if (globals->hunter_hunted)
      button[M_NETGAME_HH]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_HH]->del_flag(MC_PUSHED);

    if (globals->selected_level == -1)
      button[M_NETGAME_READY]->del_flag(MC_PUSHED);

    if (button[M_NETGAME_READY]->get_flags() & MC_PUSHED)
    {
      net->get_netplayer(0)->ready = true;
    }
    else
      net->get_netplayer(0)->ready = false;
  }
  else // server
  {
    // all players ready? then activate button
    if (net->get_all_players_ready() && net->get_netplayers() > 1 && globals->selected_level >= 0 &&
        net->get_netplayers() <= globals->level_files[globals->selected_level].max_players)
      button[M_NETGAME_PLAY]->del_flag(MC_INVISIBLE | MC_INACTIVE);
    else
      ;//button[M_NETGAME_PLAY]->set_flag(MC_INVISIBLE | MC_INACTIVE);

    // activate level extra?
    if (button[M_NETGAME_LEVELEXTRA]->get_flags() & MC_ACTIVE)
    {
      if (button[M_NETGAME_LEVELEXTRA]->get_flags() & MC_PUSHED)
      {
        globals->activate_extra = 1;
        mouse_b = 0;
      }
      else
      {
        globals->activate_extra = 0;
        mouse_b = 0;
      }
    }
  
    // activate ctf?
    if (button[M_NETGAME_CTF]->get_flags() & MC_ACTIVE)
    {
      if (button[M_NETGAME_CTF]->get_flags() & MC_PUSHED)
      {
        button[M_NETGAME_HH]->del_flag(MC_PUSHED);
        globals->capture_the_flag = 1;
        globals->hunter_hunted = 0;
        mouse_b = 0;
      }
      else
      {
        globals->capture_the_flag = 0;
        mouse_b = 0;
      }
    }

    // activate hunter hunted?
    if (button[M_NETGAME_HH]->get_flags() & MC_ACTIVE)
    {
      if (button[M_NETGAME_HH]->get_flags() & MC_PUSHED)
      {
        button[M_NETGAME_CTF]->del_flag(MC_PUSHED);
        globals->hunter_hunted = 1;
        globals->capture_the_flag = 0;
        mouse_b = 0;
      }
      else
      {
        globals->hunter_hunted = 0;
        mouse_b = 0;
      }
    }

    if (globals->level_files[globals->selected_level].activate_race == 1)
      globals->race = 1;
    else
      globals->race = 0;

  } // else server

  // check if level changed
  if (net->is_server())
    i = button[M_NETGAME_LEVSELECT]->get_int();
  else
    i = globals->selected_level;

  if (i >= 0 && i != last_level)
  {
    last_level = i;

    if (moved_in) gsound->menu_select();
      else moved_in = TRUE;

    globals->selected_level = i;
    strncpy(globals->last_level, globals->level_files[i].levelfile, 20);

    button[M_NETGAME_LEVELNAME]->set_int(globals->col_yellow);
    char myleveltext[100];
    sprintf(myleveltext, "%s [%d]", globals->level_files[globals->selected_level].name, globals->level_files[globals->selected_level].max_players);
    button[M_NETGAME_LEVELNAME]->set_text(myleveltext);

    if (strlen(globals->level_files[globals->selected_level].activate_extra_str) > 1)
    {
      button[M_NETGAME_LEVELEXTRA]->del_flag(MC_INVISIBLE);
      button[M_NETGAME_LEVELEXTRA]->del_flag(MC_INACTIVE);
      button[M_NETGAME_LEVELEXTRA]->set_text(globals->level_files[globals->selected_level].activate_extra_str);
    }
    else
    {
      button[M_NETGAME_LEVELEXTRA]->set_flag(MC_INVISIBLE);
      button[M_NETGAME_LEVELEXTRA]->set_flag(MC_INACTIVE);
      globals->activate_extra = 0;
    }

    if (globals->level_files[globals->selected_level].activate_ctf == 1)
    {
      button[M_NETGAME_CTF]->del_flag(MC_INVISIBLE);
      button[M_NETGAME_CTF]->del_flag(MC_INACTIVE);
    }
    else
    {
      button[M_NETGAME_CTF]->set_flag(MC_INVISIBLE);
      button[M_NETGAME_CTF]->set_flag(MC_INACTIVE);
      globals->capture_the_flag = 0;
    }

    if (globals->level_files[globals->selected_level].activate_hh == 1)
    {
      button[M_NETGAME_HH]->del_flag(MC_INVISIBLE);
      button[M_NETGAME_HH]->del_flag(MC_INACTIVE);
    }
    else
    {
      button[M_NETGAME_HH]->set_flag(MC_INVISIBLE);
      button[M_NETGAME_HH]->set_flag(MC_INACTIVE);
      globals->hunter_hunted = 0;
    }

    if (globals->activate_extra)
      button[M_NETGAME_LEVELEXTRA]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_LEVELEXTRA]->del_flag(MC_PUSHED);

    if (globals->capture_the_flag)
      button[M_NETGAME_CTF]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_CTF]->del_flag(MC_PUSHED);

    if (globals->hunter_hunted)
      button[M_NETGAME_HH]->set_flag(MC_PUSHED);
    else
      button[M_NETGAME_HH]->del_flag(MC_PUSHED);
      
  }
  else if (i != last_level && i == -1)
  {
    last_level = i;
    button[M_NETGAME_LEVELNAME]->set_int(globals->col_orange);
    button[M_NETGAME_LEVELNAME]->set_text(language->get_string(T_MENU_LEVEL_NOT_AVAILABLE));
  }

  // back
  if ((key[KEY_ESC] && esc_pressed == 0) || button[M_NETGAME_BACK]->get_flags() & MC_PUSHED)
  {
    key[KEY_ESC] = 0;
    esc_pressed = 1;
    globals->network_game = 0;
    gsound->menu_select();
    switch_to_menu = MENU_MAIN;
    button[M_NETGAME_BACK]->set_flag(MC_INVISIBLE);
    button[M_NETGAME_BACK]->do_explosion();
    button[M_NETGAME_BACK]->del_flag(MC_PUSHED);
    move_out();
  }

  // play!
  if ((button[M_NETGAME_PLAY]->get_flags() & MC_PUSHED) && !moving)
  {
    switch_to_menu = START_NETGAME;
    fade->initiate(FADE_TO_BLACK, 1, 60);
    
    if (net->is_server())
    {
      net->set_server_in_game(true);
      net->get_netplayer(0)->ready = true;
    }

    gsound->menu_select();
    button[M_NETGAME_PLAY]->set_flag(MC_INVISIBLE);
    button[M_NETGAME_PLAY]->do_explosion();
    button[M_NETGAME_PLAY]->del_flag(MC_PUSHED);

    // send random number over network
//    net->game_send_rand();
    move_out();
  }

  // local player choose ship
  if (button[M_NETGAME_MYCHOOSE]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_CHOOSESHIP;
    globals->ship_num = 0;
    button[M_NETGAME_MYCHOOSE]->set_flag(MC_INVISIBLE);
    button[M_NETGAME_MYCHOOSE]->do_explosion();
    button[M_NETGAME_MYCHOOSE]->del_flag(MC_PUSHED);
    move_out();

    if (net->is_client())
      button[M_NETGAME_READY]->del_flag(MC_PUSHED);
  }

  // local player edit ship
  if (button[M_NETGAME_MYEDIT]->get_flags() & MC_PUSHED)
  {
    gsound->menu_select();
    switch_to_menu = MENU_SHIPBUILDER;
    globals->ship_num = 0;
    button[M_NETGAME_MYEDIT]->set_flag(MC_INVISIBLE);
    button[M_NETGAME_MYEDIT]->do_explosion();
    button[M_NETGAME_MYEDIT]->del_flag(MC_PUSHED);
    move_out();

    if (net->is_client())
      button[M_NETGAME_READY]->del_flag(MC_PUSHED);
  }

  if (net->is_server())
  {
    // gravity
    static float f = button[M_NETGAME_OPT_GRAVITY]->get_bar()->get_int();
    if (button[M_NETGAME_OPT_GRAVITY]->get_arrow1()->get_flags() & MC_PUSHED)
      if (f > 0)  { f-=0.2; globals->gravity_percent = (f*3 - 100)/100.0; gsound->menu_changebar(); }
    if (button[M_NETGAME_OPT_GRAVITY]->get_arrow2()->get_flags() & MC_PUSHED)
      if (f < 100)  { f+=0.2; globals->gravity_percent = (f*3 - 100)/100.0; gsound->menu_changebar(); }
    button[M_NETGAME_OPT_GRAVITY]->get_bar()->set_int(int(f));

    // extras
    i = button[M_NETGAME_OPT_EXTRAS]->get_bar()->get_int();
    if (button[M_NETGAME_OPT_EXTRAS]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { i-=10; globals->extra_amount = i / 10; gsound->menu_changebar(); mouse_b = 0; }
    if (button[M_NETGAME_OPT_EXTRAS]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100)  { i+=10; globals->extra_amount = i / 10; gsound->menu_changebar(); mouse_b = 0; }
    button[M_NETGAME_OPT_EXTRAS]->get_bar()->set_int(i);

    // max frags
    i = button[M_NETGAME_OPT_MAXFRAGS]->get_bar()->get_int();
    if (button[M_NETGAME_OPT_MAXFRAGS]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { i-=2; globals->max_frags = (i / 2)+1; gsound->menu_changebar(); mouse_b = 0; }
    if (button[M_NETGAME_OPT_MAXFRAGS]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100)  { i+=2; globals->max_frags = (i / 2)+1; gsound->menu_changebar(); mouse_b = 0; }
    button[M_NETGAME_OPT_MAXFRAGS]->get_bar()->set_int(i);

    // max time
    i = button[M_NETGAME_OPT_MAXTIME]->get_bar()->get_int();
    if (button[M_NETGAME_OPT_MAXTIME]->get_arrow1()->get_flags() & MC_PUSHED)
      if (i > 0)  { i-=1; globals->max_time = (i*30); gsound->menu_changebar(); mouse_b = 0; }
    if (button[M_NETGAME_OPT_MAXTIME]->get_arrow2()->get_flags() & MC_PUSHED)
      if (i < 100)  { i+=1; globals->max_time = (i*30); gsound->menu_changebar(); mouse_b = 0; }
    button[M_NETGAME_OPT_MAXTIME]->get_bar()->set_int(i);
  }
  else
  {
    button[M_NETGAME_OPT_GRAVITY]->get_bar()->set_int((globals->gravity_percent*100+100)/3);
    button[M_NETGAME_OPT_EXTRAS]->get_bar()->set_int(globals->extra_amount*10);
    button[M_NETGAME_OPT_MAXFRAGS]->get_bar()->set_int(globals->max_frags*2);
    button[M_NETGAME_OPT_MAXTIME]->get_bar()->set_int((globals->max_time)/30);
  }

  char tmps[10];
  sprintf(tmps, "%.0f%%", globals->gravity_percent*100.0);
  button[M_NETGAME_OPT_GRAVITY_TXT]->set_text(tmps);

  sprintf(tmps, "%d", globals->extra_amount);
  button[M_NETGAME_OPT_EXTRAS_TXT]->set_text(tmps);
  
  sprintf(tmps, "%d", globals->max_frags);
  button[M_NETGAME_OPT_MAXFRAGS_TXT]->set_text(tmps);

  sprintf(tmps, "%.1f min", globals->max_time/60.0);
  button[M_NETGAME_OPT_MAXTIME_TXT]->set_text(tmps);


  if (move() == 0 && moving)
  {
    return switch_to_menu;
  }

  return 0;
}

void mNetGame::draw(BITMAP *v)
{
  int n;

  for (n=0; n < button_count; n++)
  {
    button[n]->draw(v);
  }
  
  // chat messages
  char msgbuf[1000];
  FONT *f = (FONT *)globals->fontdat[FONT_IMPACT10].dat;

  if (!moving)
  for (n=net->chat.get_count(); n > 0 && n > (net->chat.get_count()-7); n--)
  {
    aatextout(globals->vscreen, f, (net->chat.get_message_with_playername(n-1, msgbuf)), 30, 415-(net->chat.get_count()-n)*12, net->chat.get_color(n-1));
  }

  clearlist->add(v, 30, 305, 580, 130);
}

int mNetGame::move()
{
  int r = 0;

  for (int n=0; n < button_count; n++)
    if (button[n]->update()) r = 1;

  return r;
}

void mNetGame::move_out()
{
  for (int n=1; n < button_count; n++)
  {
    button[n]->set_dest_y( button[n]->get_y() + 400 );
  }
  button[M_NETGAME_CAPTION]->set_dest_x( button[M_NETGAME_CAPTION]->get_x() + 400 );

  moving = 1;
  no_input = 1;
}

