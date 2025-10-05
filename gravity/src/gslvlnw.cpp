/*
 *   GRAVITY STRIKE -- gslvlnw.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsscript.h"
#include "gscontrl.h"
#include "gslvlnw.h"
#include "gserror.h"
#include "gsmap.h"
#include "gsglob.h"
#include "gspanel.h"
#include "gsplayer.h"
#include "gseffect.h"
#include "gsclear.h"
#include "gsbase.h"
#include "gsobject.h"
#include "gshelper.h"
#include "gsfade.h"
#include "gssound.h"
#include "gsosd.h"
#include "gsreplay.h"
#include "gsenemy.h"
#include "gsplcomp.h"
#include "gslng.h"
#include "gsnetw.h"
#include "allegttf.h"

extern RGB_MAP     rgb_table;
extern COLOR_MAP   light_table;
extern COLOR_MAP   trans_table;


gsNetLevel::gsNetLevel()
{
  exit_level = FALSE;
  strcpy(error_str, "");
  globals->exit_level = 0;
  globals->pause = 0;

  globals->save_values();
  //globals->players = net->get_netplayers();
  globals->players = 1;

  net->set_quit_game(false);

  strcpy(chat_input_text, "");

  net->chat.clear();

  cur_send_enemy = 0;
  cur_send_object = 0;
  cur_send_spobject = 0;

  client_quit = false;
}

gsNetLevel::~gsNetLevel()
{
  clear_bitmap(screen);
  globals->pause = 0;

  script->clear_ingame_script();
  net->chat.clear();

//  script->kill_script_commands(glob_lua);

  gsound->destroy_music();

  playmap[0]->osd->remove(osd_chat_input);
  playmap[0]->osd->remove(osd_chat_messages);

  if (gpanel[0])     delete(gpanel[0]);  gpanel[0] = NULL;
  if (bases)         delete(bases);      bases = NULL;
  if (objects)       delete(objects);    objects = NULL;
  if (enemies)       delete(enemies);    enemies = NULL;
  if (playmap[0])    delete(playmap[0]); playmap[0] = NULL;
  if (effects)       delete(effects);    effects = NULL;
  if (fade)          delete(fade);       fade = NULL;
  if (globals->vscreen)  destroy_bitmap(globals->vscreen); globals->vscreen = NULL;
  if (images)        delete(images);     images = NULL;

  // send our stats and request others if client quit game
  net->game_send_player_statistics(net->get_netplayer(0)->net_id, client_quit);

  // wait for stats of other players
  int n;
  int waitstart = globals->game_time;
  bool all_received = false;
  while (globals->game_time < waitstart + 60*5 && !all_received)
  {
    net->process_packets();
    all_received = true;
    for (n=1; n < net->get_netplayers(); n++)
      if (net->get_netplayer(n))
      {
        //errors->log(2, "netplayer x stats set?", n, net->get_netplayer(n)->player_stats.set);
        if (net->get_netplayer(n)->player_stats.set == 0)
          all_received = false;
      }
  }
  
  for (n=0; n < globals->players; n++)
  {
    gsound->stop_player_thrusters(player[n]->get_sig());
  }

  for (n=0; n < globals->players; n++)
  {
    if (net->get_netplayer(n))
    {
      if (net->get_netplayer(n)->player) delete net->get_netplayer(n)->player;
      net->get_netplayer(n)->player = NULL;
    }
    player[n] = NULL;
  }

  clearlist->clear_list();
  antialias_exit();

  gsound->destroy_user_samples();
  globals->restore_values();
  
  delete glob_lua;
}

int gsNetLevel::init_stuff()
{
  errors->log(2, "init", "NETWORK LEVEL");

  // synchronize Random Number Generators
  long test = net->get_synchronized_random_number_seed();
  errors->log(2, "network", "synchronized random number seed", test);
  seed_gsrand(test);

  // change resolution to globals->game_resolution_x/y
  globals->switch_to_game_resolution();

  // init scripting
  glob_lua = new Lua();

  // paneldat
  globals->paneldat = (DATAFILE*)globals->gamedat[panel].dat;
  
  clear_bitmap(screen);

  errors->log(2, "init", "color tables");
  create_rgb_table(&rgb_table, (RGB*)globals->gamedat[gamepal].dat, NULL);
  rgb_map = &rgb_table;
  create_trans_table(&trans_table, (RGB*)globals->gamedat[gamepal].dat, 128, 128, 128, NULL);
  create_light_table(&light_table, (RGB*)globals->gamedat[gamepal].dat, 0, 0, 200, NULL);
  color_map = &light_table;

  // color stuff
  errors->log(2, "init", "player colors / sprites");
  globals->make_standard_colors();
  globals->make_color_tables();

  // antialias routines
  errors->log(2, "init", "antialiased text");
  antialias_init((RGB*)globals->gamedat[gamepal].dat);

  // imge handler
  errors->log(2, "init", "image handler");
  images = new gsImage();

  // bases
  errors->log(2, "init", "bases");
  bases = new gsBase();

  // objects
  errors->log(2, "init", "objects");
  objects = new gsObject();

  // enemies
  errors->log(2, "init", "enemies");
  enemies = new gsEnemy();

  // maps
  if (globals->level_files[globals->selected_level].onescreen_map)
  {
    globals->onescreen_map = TRUE;
    globals->splitscreen = 0;
  }
  else
  {
    globals->onescreen_map = 0;
    globals->splitscreen = 0;
  }

  errors->log(2, "init", "map(s)");
  playmap[0] = new gsMap();

  // effects
  errors->log(2, "init", "effects");
  effects = new gsEffect;

  // init players
  errors->log(2, "init", "local player");
  long plsig = net->get_netplayer(0)->net_id.binaryAddress / 10000;
  player[0] = new gsPlayer(plsig, 0, 0, net->get_netplayer(0)->ship_file, playmap[0]);
  player[0]->set_name(net->get_netplayer(0)->name);
  net->get_netplayer(0)->player = player[0];

  errors->log(2, "init", "loading player controls");
  player[0]->controls->set_type(LOCAL_SINGLE);
  player[0]->controls->load_key_cfg();

  // virtual screens
  if (!globals->splitscreen)
  {
    globals->playscreen_width = globals->playscreen_full_width;

    errors->log(2, "init", "vscreen");
    globals->vscreen = create_bitmap(globals->playscreen_width + globals->plus_x + 100,
                                     globals->playscreen_height + globals->plus_y + 100);

    if (!globals->vscreen)
    {
      strcpy(error_str, "Can't initialize virtual screen!");
      return 1;
    }

    clear_bitmap(globals->vscreen);
  }

  // load maps
  errors->log(2, "init", "loading map");
  char tileset[150], map[150];
  sprintf(tileset, "%s%s/%s", globals->level_dir,
                              globals->level_files[globals->selected_level].leveldir,
                              globals->level_files[globals->selected_level].map_style);
  sprintf(map, "%s%s/%s", globals->level_dir,
                          globals->level_files[globals->selected_level].leveldir,
                          globals->level_files[globals->selected_level].mapfile);
  playmap[0]->set(globals->vscreen, tileset, map);

  // chat osd
  errors->log(2, "init", "chat osd");
  osd_chat_input = playmap[0]->osd->add(9999, globals->playscreen_width/2 - 200, 350, 400, 20, OSDBIT_SCREEN);
  osd_chat_input->clear();
  osd_chat_input->draw_rectangle();
  osd_chat_input->fill_area(globals->col_blue, 1, 1);
  osd_chat_input->draw_line(globals->col_yellow, 0+3, 3, 0+3, 17);
  osd_chat_input->set_visible(0);

  osd_chat_messages = playmap[0]->osd->add(9998, 80, 10, 400, 60, OSDBIT_SCREEN);
  osd_chat_messages->clear();
  
  // panel
  errors->log(2, "init", "panel");
  gpanel[0] = new gsPanel(0, player[0], playmap[0]);

  // set to black palette
  set_palette(black_palette);

  // fading
  errors->log(2, "init", "fading");
  fade = new gsFade();
 
  // load ingame script
  script->init_script_commands(glob_lua);

  errors->log(2, "init", "loading ingame script", globals->level_files[globals->selected_level].scriptfile);

  if (globals->level_files[globals->selected_level].activate_global_script)
    script->load_ingame_script(globals->level_files[globals->selected_level].leveldir,
                               globals->level_files[globals->selected_level].scriptfile,
                               globals->global_sc);
  else
    script->load_ingame_script(globals->level_files[globals->selected_level].leveldir,
                               globals->level_files[globals->selected_level].scriptfile);

  // execute init scripts (base create etc.)
  errors->log(2, "init", "executing level init script", globals->level_files[globals->selected_level].initscriptfile);
  script->do_init_script(glob_lua,
                         globals->level_files[globals->selected_level].leveldir,
                         globals->level_files[globals->selected_level].initscriptfile);
  
  int starttime = 0;
  bool timeout = false;
  
  // (clients only) wait until position setting comes in
  if (net->is_client())
  {
    // we have started our game: inform server and request position!
    net->menu_send_start_game();
  
    // now wait for answer
    globals->game_time = 0;
    int oldtime = 0;
    starttime = globals->game_time;
    timeout = false;
    while (!net->get_position_set() && !timeout)
      if (globals->game_time > oldtime)
      {
        net->process_packets();
        oldtime = globals->game_time;
        if (globals->game_time > starttime+60*10) 
        {
          timeout = true;
          net->set_quit_game(true);
          errors->log(1, "network", "TIMEOUT while waiting for our position");
        }
      }  
  }

  // initialize
  init();

  // global init script
  if (globals->level_files[globals->selected_level].activate_global_script)
  {
    errors->log(2, "init", "executing global init script", globals->global_ini);
    script->do_init_script(glob_lua, NULL, globals->global_ini);
  }

  script->init_hooks(glob_lua);

  // start fade in
  fade->initiate(FADE_TO_MAIN, 1, 60);
  fading_in = 1;

  if (net->is_client())
    errors->log(2, "network", "lets_go_delay", net->get_lets_go_delay());

  // start
  script->do_hook_init_level(glob_lua);

  // forward "lets go delay" cycles (if client)
  if (net->is_client())
  {
    unsigned long lets_go_delay = net->get_lets_go_delay();
    if (lets_go_delay < 60*60*5)
    {
      if (lets_go_delay < 0) lets_go_delay = 0;

      errors->log(2, "network", "forwarding game state X cycles", lets_go_delay);
      for (int d=0; d < lets_go_delay; d++)
      {
        objects->update();
        bases->update();
        enemies->update();
        //script->do_ingame_script(glob_lua);
      }
    }
    else
      errors->log(2, "network", "lets_go_delay is > 5 minutes. Not forwarding game.", lets_go_delay);
  }

  // I'm in!
  if (net->is_client())
    net->game_send_i_am_in();

  return 0;
}

void gsNetLevel::flip()
{
  int h1 = (gpanel[0]->get_active()) ? globals->playscreen_height : globals->playscreen_full_height;

  if (!globals->splitscreen)
  {
    acquire_bitmap(screen);

    blit(playmap[0]->get_vscreen(), screen, (int)playmap[0]->get_fscroll_x(), (int)playmap[0]->get_fscroll_y(), 0, 0,
         globals->playscreen_width, h1);

    if (gpanel[0]->get_active()) gpanel[0]->draw(screen);

    release_bitmap(screen);
  }
}

void gsNetLevel::init()
{
  if (net->is_server())
    net->fill_home_locations();

  player[0]->set_status_mode(1);
  player[0]->show_status_set_map(playmap[0]);


  gpanel[0]->update_all();

  if (!globals->onescreen_map)
  {
    playmap[0]->scroll_to(player[0]);
    playmap[0]->scroll();
  }
  else
  {
    playmap[0]->scroll_to(globals->level_files[globals->selected_level].map_start_x,
                          globals->level_files[globals->selected_level].map_start_y);
    playmap[0]->scroll();
  }

  playmap[0]->request_redraw();
  if (globals->onescreen_map) playmap[0]->redraw();

  // TEMP //

  // TEMP //
  
}

void gsNetLevel::draw_stuff()
{
  int n;

  if (fading_in && !fade->get_fade_active())
  {
    fading_in = 0;
    gpanel[0]->update_weapons(); gpanel[0]->update_shields();
  }

  effects->explosions->save_background();

  for (n=0; n < globals->players; n++)
  {
    player[n]->save_background();
  }

  objects->save_background(playmap[0]);
  bases->draw(playmap[0]);
  objects->draw(playmap[0]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->check_map_collisions();
  }

  bases->draw_cargo(playmap[0]);
  objects->draw_special(playmap[0]);
  effects->pixels->check_collisions(playmap[0]);
  effects->pixels->draw(playmap[0]);
  effects->bullets->save_background(playmap[0]);
  effects->bullets->draw(playmap[0]);
  effects->explosions->draw(playmap[0]);
  enemies->draw(playmap[0]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->display(playmap[0]);
  }

  effects->explosions->draw_shield(playmap[0]);

  for (n=0; n < globals->players; n++) player[n]->save_overlay_back();
  for (n=0; n < globals->players; n++) player[n]->draw_overlay();

  playmap[0]->osd->save_background();
  playmap[0]->osd->draw();
}

void gsNetLevel::clear_stuff()
{
  int n;

  playmap[0]->osd->restore_background();

  for (n=0; n < globals->players; n++) player[n]->restore_overlay();

  effects->explosions->clear(playmap[0]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->clear(playmap[0]);
  }

  objects->restore_background(playmap[0]);

  effects->bullets->restore_background(playmap[0]);

  clearlist->clear_screen();

  gpanel[0]->panel_clear();

}

void gsNetLevel::do_logic()
{
  int n;
 
  // network messages
  net->process_packets();
  do_network_stuff();
  rest(0);

  
  if (keyboard_needs_poll()) poll_keyboard();
  if (globals->use_joystick) poll_joystick();

  // keyboard hit callback
  if (script->is_flag(script->flag_hook_keyboard_hit))
  {
    while (keypressed())
      script->do_hook_keyboard_hit(glob_lua, readkey() >> 8);
  }

  if (globals->max_time > 0)
    if (globals->second_time == globals->max_time) if (!exit_level) exit_level = 1;

  for (n=0; n < globals->players; n++)
  {
    player[n]->update();
    if (player[n]->get_frags() >= globals->max_frags) if (!exit_level) exit_level = 1;
  }

  if (globals->exit_level) if (!exit_level) exit_level = 1;

  if (!globals->onescreen_map)
  {
    if (player[0]->get_dead() == 0)
      playmap[0]->scroll_to(player[0]);
    else if (player[0]->get_dead() == 100)
      playmap[0]->scroll_to(player[0], globals->player_respawn-100);

    playmap[0]->scroll();
  }
  else
  {
    if (playmap[0]->is_redraw_needed())
      playmap[0]->redraw();

    playmap[0]->scroll();
  }

  effects->pixels->move(globals->particle_gravity);
  effects->explosions->update();
  effects->bullets->move();

  objects->update();
  bases->update();
  enemies->update();
  effects->bullets->check_collisions(playmap[0]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->check_player_collisions();
    player[n]->check_stuff();
  }

  gpanel[0]->update_debug();
  gpanel[0]->update_anims();
  gpanel[0]->update_radar();
  playmap[0]->osd->update();

  script->do_ingame_script(glob_lua);


  //if (globals->game_time % 30 == 0)
  {
    int th = script->get_gcthreshold(glob_lua);
    script->set_gcthreshold(glob_lua, 0);
    script->set_gcthreshold(glob_lua, th);
  }
}


void gsNetLevel::update_osd_chat_input()
{
  FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  int tlen = text_length(f, chat_input_text);

  osd_chat_input->clear();
  osd_chat_input->draw_rectangle();
  osd_chat_input->fill_area(globals->col_blue, 1, 1);

  BITMAP *tmp = create_bitmap(tlen+2, osd_chat_input->get_height());
  clear_bitmap(tmp);
  aatextout(tmp, f, chat_input_text, 0, 0, globals->col_white);
  vline(tmp, tlen+1, 0, 14, globals->col_yellow);

  int sx = tmp->w - osd_chat_input->get_width() + 8;
  if (sx < 0) sx = 0;
  masked_blit(tmp, osd_chat_input->get_bitmap(), sx, 0, 4, 3, osd_chat_input->get_width()-5, osd_chat_input->get_height());

  destroy_bitmap(tmp);

  //osd_chat_input->draw_text(FONT_IMPACT10, chat_input_text, globals->col_white, 3, 3, 0);
  //int fontlen = text_length((FONT*)globals->fontdat[FONT_IMPACT10].dat, chat_input_text);
  //osd_chat_input->draw_line(globals->col_yellow, fontlen+3, 3, fontlen+3, 17);
}

void gsNetLevel::update_osd_chat_messages()
{
  // chat messages
  char msgbuf[1000];
  //FONT *f = (FONT *)globals->fontdat[FONT_MINIFONT].dat;

  osd_chat_messages->clear();
  //osd_chat_messages->draw_rectangle();

  for (int n=net->chat.get_count(); n > 0 && n > (net->chat.get_count()-7); n--)
  {
    osd_chat_messages->draw_colored_text(FONT_MINIFONT, strupr(net->chat.get_message_with_playername(n-1, msgbuf)), globals->col_black, 4, 49-(net->chat.get_count()-n)*8, 1);
    osd_chat_messages->draw_colored_text(FONT_MINIFONT, strupr(net->chat.get_message_with_playername(n-1, msgbuf)), globals->col_black, 6, 51-(net->chat.get_count()-n)*8, 1);
    osd_chat_messages->draw_colored_text(FONT_MINIFONT, strupr(net->chat.get_message_with_playername(n-1, msgbuf)), net->chat.get_color(n-1), 5, 50-(net->chat.get_count()-n)*8, 0);
  }
}

void gsNetLevel::update_chat_osd()
{
  // update chat message when chat osd is visible
  if (osd_chat_input->get_visible())
  {
    int k = 0, kascii = 0;

    if (keypressed())
    {
      k = readkey();
      kascii = k & 0xff;
      k = k >> 8;
      if (k == 0) k = readkey();
    }

    if (k)
    {
      if (k == KEY_BACKSPACE)
      {
        if (strlen(chat_input_text) > 0) chat_input_text[strlen(chat_input_text)-1] = '\0';
        update_osd_chat_input();
      }
      else if (k == KEY_ENTER && strlen(chat_input_text) > 0)
      {
        if (!net->chat.process_commands(chat_input_text))
        {
          net->chat.add(chat_input_text, net->get_netplayer(0)->name, globals->col_white);
          net->game_send_chat_message(chat_input_text);
        }

        chat_input_text[0] = '\0';
        osd_chat_input->set_visible(0);

        update_osd_chat_input();
        update_osd_chat_messages();
      }
      else
      if ( (k >= KEY_A && k <= KEY_9) || k == KEY_SPACE ||
           k == KEY_STOP || k == KEY_COMMA || k == KEY_COLON ||
           k == KEY_MINUS || k == KEY_PLUS_PAD || k == KEY_EQUALS ||
           k == KEY_QUOTE || k == KEY_SLASH || k == KEY_ASTERISK ||
           k == KEY_OPENBRACE || k == KEY_CLOSEBRACE)
      {
        if (strlen(chat_input_text) < 80)
        {
          int l = strlen(chat_input_text);
          if (!(k == KEY_SPACE && l == 0))
          {
            chat_input_text[l] = kascii;
            chat_input_text[l+1] = '\0';
            update_osd_chat_input();
          }
          key[k] = 0;
        }
      }
      clear_keybuf();
    }        

    key[KEY_LSHIFT] = 0;
    key[KEY_RSHIFT] = 0;
    key[KEY_ALT] = 0;
    key[KEY_ALTGR] = 0;
    key[KEY_LCONTROL] = 0;
    key[KEY_RCONTROL] = 0;
    key_shifts = 0;
  }
}

void gsNetLevel::play()
{
  int actual_time = 0;
  int endthis = FALSE;
  int esc_pressed = FALSE;
  int update_graphics = FALSE;
  int display_esc_osd = FALSE;
  globals->game_time = globals->overall_game_time = globals->second_time = 0;
  globals->level_restart = 0;

  while (!(endthis))
  {
    while (globals->game_time > actual_time)
    {
      // game logic
      actual_time++;

      if (key[KEY_ESC] && !esc_pressed && exit_level == 0)
      {
        esc_pressed = TRUE;
        clear_keybuf();
        key[KEY_ESC] = 0;
      }

      if (esc_pressed)
      {  
        exit_level = 1;
        esc_pressed = false;
        if (net->is_client()) client_quit = true;
      }

      if (exit_level == 0 && net->get_quit_game())
        exit_level = 1;

      if (exit_level == 1)
      {
        exit_level = 1;
        if (!net->get_quit_game()) net->game_send_quit();
        fade->initiate(FADE_TO_BLACK, 1, 50);
      }

      if (exit_level > 0)
      if (++exit_level && !endthis && !fade->get_fade_active())
      {
        exit_level = FALSE;
        endthis = TRUE;
        script->do_hook_exit_level(glob_lua);
      }

      if (player[0]->controls->get_input(SP_CHAT, 1, 0))
      {
        if (osd_chat_input->get_visible())
          osd_chat_input->set_visible(0);
        else
          osd_chat_input->set_visible(1);
        
        key[KEY_TAB] = 0;
      }

      if (key[KEY_Q])
      {
        if (net->get_netplayer(1))
          net->game_log_netstats(net->get_netplayer(1)->net_id);
        key[KEY_Q] = 0;
      }

      if (key[KEY_W])
      {
        if (net->get_netplayer(2))
          net->game_log_netstats(net->get_netplayer(2)->net_id);
        key[KEY_W] = 0;
      }
      

      // chat osd
      update_chat_osd();

      if (player[0]->controls->get_input(SP_DEBUG, 1, 0))
      {
        if (gpanel[0]->get_debug()) { gpanel[0]->set_debug(0); gpanel[0]->blit_orig_paneldat(); gpanel[0]->update_all(); }
        else { gpanel[0]->set_debug(1); gpanel[0]->update_all(); }
      }

      if (player[0]->controls->get_input(SP_PANEL, 1, 0))
      {
        if (gpanel[0]->get_active()) gpanel[0]->set_active(0);
        else gpanel[0]->set_active(1);
        
        clear_bitmap(screen);
      } 
     
      // do game logic
      if (!globals->pause) do_logic();

      fade->update();

      update_graphics = TRUE;
    } // while game_time > actual_time

    if (update_graphics)
    {
      // draw graphics
      rest(0);
      draw_stuff();
      if (player[0]->controls->get_input(SP_SCREENSHOT, 1, 0)) make_screenshot();

      // flip to visible screen
      flip();

      // clear stuff on virtual screen
      clear_stuff();

      update_graphics = FALSE;

      globals->fps++;
    } // if update_graphics

  } // while !endthis

  if (replay_mode > 0) replay->close_file();

}


void gsNetLevel::do_network_stuff()
{
  int sendtime = 40;
  float sendmult = 0.08; //(float)sendtime / 500.0;

  // if another player is closer, send updates more frequently
  for (int n=1; n < globals->players; n++)
  {
    float d = distance(player[0]->get_x(), player[0]->get_y(), player[n]->get_x(), player[n]->get_y());
    
    sendtime = fast_ftol(d * sendmult);

    if (sendtime < 15)
      sendtime = 15;
    else if (sendtime > 40)
      sendtime = 40;
  }

  // send a normal player position update
  if (globals->game_time % sendtime == 0)
    net->game_send_player_position();

  // send player status update (ammo, hull/shield, frags etc.)
  if (globals->game_time % 180 == 0)
    net->game_send_player_status();

  // update chat osd every second
  if (globals->game_time % 60 == 0)
    update_osd_chat_messages();

  // send object and enemy updates from server to clients (1-2 times per second)
  int cur = 0;
  if (net->is_server() && globals->game_time % 80 == 0)
  {
    Enemy *enemy = enemies->get_first_enemy();
    cur = 0;
    while (enemy && cur >= 0)
    {
      if (enemy->get_xspd() != 0 || enemy->get_yspd() != 0)
      {
        if (cur == cur_send_enemy)
        {
          cur_send_enemy = ++cur;
          net->game_send_enemy_object_update(0, enemy->get_nr());
          //errors->log(2, "net send", "enemy nr", cur);
          cur = -1;
        }
        else
          cur++;
      }
      enemy = enemy->get_next();
    }
    if (cur != -1) cur_send_enemy = 0;
  }

  if (net->is_server() && globals->game_time % 100 == 0)
  {
    Object *object = objects->get_first_object();
    cur = 0;
    while (object && cur >= 0)
    {
      if (object->get_xspd() != 0 || object->get_yspd() != 0)
      {
        if (cur == cur_send_object)
        {
          cur_send_object = ++cur;
          net->game_send_enemy_object_update(1, object->get_nr());
          //errors->log(2, "net send", "object nr", cur);
          cur = -1;
        }
        else
          cur++;
      }

      object = object->get_next();
    }
    if (cur != -1) cur_send_object = 0;
  }

  if (net->is_server() && globals->game_time % 110 == 0)
  {
    Object *object = objects->get_first_spobject();
    cur = 0;
    while (object && cur >= 0)
    {
      if (object->get_xspd() != 0 || object->get_yspd() != 0)
      {
        if (cur == cur_send_spobject)
        {
          cur_send_spobject = ++cur;
          net->game_send_enemy_object_update(2, object->get_nr());
          //errors->log(2, "net send", "sp object nr", cur);
          cur = -1;
        }
        else
          cur++;
      }

      object = object->get_next();
    }
    if (cur != -1) cur_send_spobject = 0;
  }


  // send menu updates for new players (if some of them are not in the game)
//  for (int n=0; n < net->get_netplayers(); n++)
  if (net->is_server() && globals->game_time % (60*5) == 0)
    net->menu_server_update_clients();

  // send some miscellaneus data (as server)
  if (net->is_server() && globals->game_time % 280 == 0)
    net->game_send_server_misc();

  /*
  if (globals->game_time % 60*10 == 0)
  {
    for (int n=0; n < net->get_netplayers(); n++)
    {
      if (net->get_netplayer(n)->in_game)
        errors->log(2, "network", "player with in_game status", net->get_netplayer(n)->net_id.binaryAddress / 10000);
    }
  }
  */
}
