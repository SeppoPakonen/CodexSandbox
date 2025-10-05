/*
 *   GRAVITY STRIKE -- gslevel.cpp
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
#include "gslevel.h"
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
#include "allegttf.h"

RGB_MAP     rgb_table;
COLOR_MAP   light_table;
COLOR_MAP   trans_table;

gsLevel::gsLevel()
{
  exit_level = FALSE;
  strcpy(error_str, "");
  globals->exit_level = 0;
  globals->pause = 0;

  globals->save_values();
}

gsLevel::~gsLevel()
{
  clear_bitmap(screen);
  globals->pause = 0;

  script->clear_ingame_script();

//  script->kill_script_commands(glob_lua);

  gsound->destroy_music();

  if (gpanel[0])     delete(gpanel[0]);  gpanel[0] = NULL;
  if (gpanel[1])     delete(gpanel[1]);  gpanel[1] = NULL;
  if (bases)         delete(bases);      bases = NULL;
  if (objects)       delete(objects);    objects = NULL;
  if (enemies)       delete(enemies);    enemies = NULL;
  if (playmap[0])    delete(playmap[0]); playmap[0] = NULL;
  if (playmap[1])    delete(playmap[1]); playmap[1] = NULL;
  if (effects)       delete(effects);    effects = NULL;
  if (fade)          delete(fade);       fade = NULL;
  if (globals->vscreen)  destroy_bitmap(globals->vscreen); globals->vscreen = NULL;
  if (globals->vscreen2) destroy_bitmap(globals->vscreen2); globals->vscreen2 = NULL;
  if (images)        delete(images);     images = NULL;
  if (comp_player[0])   delete(comp_player[0]); comp_player[0] = NULL;

  // save statistics
  int n;
  for (n=0; n < globals->players; n++)
    globals->player_stats[n] = player[n]->get_statistics_copy();

  gsound->stop_player_thrusters(player[0]->get_sig());
  gsound->stop_player_thrusters(player[1]->get_sig());

  for (n=0; n < 8; n++)
  {
    if (player[n])   delete player[n];
    player[n] = NULL;
  }

  clearlist->clear_list();
  antialias_exit();

  gsound->destroy_user_samples();
  globals->restore_values();
  
  delete glob_lua;
}

int gsLevel::init_stuff()
{
  errors->log(2, "init", "TWOPLAYER LEVEL");

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
    globals->splitscreen = 1;
  }

  errors->log(2, "init", "map(s)");
  playmap[0] = new gsMap();
  if (globals->splitscreen) playmap[1] = new gsMap();

  // effects
  errors->log(2, "init", "effects");
  effects = new gsEffect;

  // player
  errors->log(2, "init", "player(s)");
  int id = gsrand()%10000 + 500;
  player[0] = new gsPlayer(id , 0, 0, globals->pplayer[0].pship, playmap[0]);
  id = gsrand()%10000 + 500;
  if (!globals->onescreen_map)
  {
    player[1] = new gsPlayer(id, 0, 0, globals->pplayer[1].pship, playmap[1]);
  }
  else
  {
    player[1] = new gsPlayer(id, 0, 0, globals->pplayer[1].pship, playmap[0]);
  }

  player[0]->set_name(globals->pplayer[0].pname);
  if (globals->players >= 2) player[1]->set_name(globals->pplayer[1].pname);

  errors->log(2, "init", "loading player controls");
  player[0]->controls->set_type(LOCAL_SINGLE);
  player[0]->controls->load_key_cfg();
  if ((globals->splitscreen || globals->onescreen_map) && globals->players >= 2)
  {
    player[0]->controls->set_type(LOCAL1);
    player[1]->controls->set_type(LOCAL2);
    player[1]->controls->load_key_cfg();
  }

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
  else
  {
    globals->playscreen_width = globals->playscreen_split_width;

    errors->log(2, "init", "vscreen & vscreen2 (splitscreen)");
    globals->vscreen  = create_bitmap(globals->playscreen_width + globals->plus_x + 100,
                                      globals->playscreen_height + globals->plus_y + 100);
    globals->vscreen2 = create_bitmap(globals->playscreen_width + globals->plus_x + 100,
                                      globals->playscreen_height + globals->plus_y + 100);

    if (!globals->vscreen || !globals->vscreen2)
    {
      strcpy(error_str, "Can't initialize virtual screen!");
      return 1;
    }

    clear_bitmap(globals->vscreen);
    clear_bitmap(globals->vscreen2);
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
  if (globals->splitscreen)
  {
    playmap[1]->set(globals->vscreen2, "", "");
  }

  // panel
  errors->log(2, "init", "panel(s)");
  if (globals->splitscreen)
  {
    gpanel[0] = new gsPanel(1, player[0], playmap[0]);
    if (player[1]) gpanel[1] = new gsPanel(2, player[1], playmap[1]);
  }
  else
  {
    if (globals->onescreen_map)
    {
      gpanel[0] = new gsPanel(1, player[0], playmap[0]);
      gpanel[1] = new gsPanel(2, player[1], playmap[0]);
    }
    else
    {
      gpanel[0] = new gsPanel(0, player[0], playmap[0]);
    }
  }

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

  // execute init scripts (global script, base create etc.)
  errors->log(2, "init", "executing level init script", globals->level_files[globals->selected_level].initscriptfile);
  script->do_init_script(glob_lua,
                         globals->level_files[globals->selected_level].leveldir,
                         globals->level_files[globals->selected_level].initscriptfile);
  if (globals->level_files[globals->selected_level].activate_global_script)
  {
    errors->log(2, "init", "executing global init script", globals->global_ini);
    script->do_init_script(glob_lua, NULL, globals->global_ini);
  }

  script->init_hooks(glob_lua);

  // load computer player
  if (globals->computer_player)
  {
    comp_player[0] = new gsCompPlayer(glob_lua, 2);
    comp_player[0]->run_ini();
  }

  // initialize
  init();

  // start fade in
  fade->initiate(FADE_TO_MAIN, 1, 60);
  fading_in = 1;

  script->do_hook_init_level(glob_lua);

  return 0;
}

void gsLevel::flip()
{
  int h1 = (gpanel[0]->get_active()) ? globals->playscreen_height : globals->playscreen_full_height;
  int h2 = (gpanel[1]->get_active()) ? globals->playscreen_height : globals->playscreen_full_height;

  if (!globals->splitscreen)
  {
    acquire_bitmap(screen);

    blit(playmap[0]->get_vscreen(), screen, (int)playmap[0]->get_fscroll_x(), (int)playmap[0]->get_fscroll_y(), 0, 0,
         globals->playscreen_width, h1);

    if (gpanel[0]->get_active()) gpanel[0]->draw(screen);

    if (globals->onescreen_map)
    {
      if (gpanel[1]->get_active()) 
      {
        gpanel[1]->draw(screen);

        blit((BITMAP*)globals->paneldat[panel_div].dat, screen, 
             0, 0,
             globals->playscreen_full_width/2-3, globals->playscreen_full_height-globals->panel_half_height, 
             5, globals->panel_half_height);
      }
    }

    release_bitmap(screen);
  }
  else
  {
    acquire_bitmap(screen);

    blit(playmap[0]->get_vscreen(), screen, (int)playmap[0]->get_fscroll_x(), (int)playmap[0]->get_fscroll_y(),
         globals->playscreen1_sx, 0,
         globals->playscreen_width, h1);

    blit(playmap[1]->get_vscreen(), screen, (int)playmap[1]->get_fscroll_x(), (int)playmap[1]->get_fscroll_y(),
         globals->playscreen2_sx, 0,
         globals->playscreen_width, h2);

    if (gpanel[0]->get_active()) gpanel[0]->draw(screen);
    if (gpanel[1]->get_active()) gpanel[1]->draw(screen);

    stretch_blit((BITMAP*)globals->paneldat[panel_div].dat, screen, 
                  0, 0,
                  5, 480,
                  globals->playscreen_split_width, 0, 
                  5, globals->playscreen_full_height);

    release_bitmap(screen);
  }
}

void gsLevel::init()
{
  int n;

  for (n=0; n < globals->players; n++)
  {
    if (player[n]->get_x() == 0 && player[n]->get_y() == 0)
    {
      player[n]->set_homebase(n);
      player[n]->return_home();
    }

    player[n]->set_status_mode(1);
  }

  if (globals->splitscreen)
  {
    player[0]->show_status_set_map(playmap[1]);
    if (globals->players >= 2) player[1]->show_status_set_map(playmap[0]);
  }
  else
  {
    for (n=1; n < globals->players; n++)
    {
      player[n]->set_status_mode(1);
      player[n]->show_status_set_map(playmap[0]);
    }

    if (globals->onescreen_map)
    {
      player[0]->set_status_mode(1);
      player[0]->show_status_set_map(playmap[0]);
    }
  }

  gpanel[0]->update_all();
  if (globals->splitscreen || globals->onescreen_map) gpanel[1]->update_all();

  if (!globals->onescreen_map)
  {
    playmap[0]->scroll_to(player[0]);
    if (globals->splitscreen) playmap[1]->scroll_to(player[1]);
    playmap[0]->scroll();
    if (globals->splitscreen) playmap[1]->scroll();
  }
  else
  {
    playmap[0]->scroll_to(globals->level_files[globals->selected_level].map_start_x,
                          globals->level_files[globals->selected_level].map_start_y);
    playmap[0]->scroll();
  }

  playmap[0]->request_redraw();
  if (globals->splitscreen) playmap[1]->request_redraw();

  if (globals->onescreen_map) playmap[0]->redraw();

  // TEMP //

  // TEMP //
  
}

void gsLevel::draw_stuff()
{
  int n;

  if (fading_in && !fade->get_fade_active())
  {
    fading_in = 0;
    gpanel[0]->update_weapons(); gpanel[0]->update_shields();
    if (globals->splitscreen || globals->onescreen_map)
    {
      gpanel[1]->update_weapons(); gpanel[1]->update_shields();
    }
  }

  effects->explosions->save_background();

  for (n=0; n < globals->players; n++)
  {
    player[n]->save_background();
  }

  objects->save_background(playmap[0]);
  if (globals->splitscreen) objects->save_background(playmap[1]);

  bases->draw(playmap[0]);
  if (globals->splitscreen) bases->draw(playmap[1]);

  objects->draw(playmap[0]);
  if (globals->splitscreen) objects->draw(playmap[1]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->check_map_collisions();
  }

  bases->draw_cargo(playmap[0]);
  if (globals->splitscreen) bases->draw_cargo(playmap[1]);

  objects->draw_special(playmap[0]);
  if (globals->splitscreen) objects->draw_special(playmap[1]);

  effects->pixels->check_collisions(playmap[0]);
  if (globals->splitscreen) effects->pixels->check_collisions(playmap[1]);

  effects->pixels->draw(playmap[0]);
  if (globals->splitscreen) effects->pixels->draw(playmap[1]);

  effects->bullets->save_background(playmap[0]);
  
  effects->bullets->draw(playmap[0]);
  if (globals->splitscreen) effects->bullets->draw(playmap[1]);

  effects->explosions->draw(playmap[0]);
  if (globals->splitscreen) effects->explosions->draw(playmap[1]);

  enemies->draw(playmap[0]);
  if (globals->splitscreen) enemies->draw(playmap[1]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->display(playmap[0]);
    if (globals->splitscreen) player[n]->display(playmap[1]);
//        player[n]->display_exhaust_pixels(playmap[0], playmap[1]);
  }

  effects->explosions->draw_shield(playmap[0]);
  if (globals->splitscreen) effects->explosions->draw_shield(playmap[1]);

  for (n=0; n < globals->players; n++) player[n]->save_overlay_back();
  for (n=0; n < globals->players; n++) player[n]->draw_overlay();

  playmap[0]->osd->save_background();
  playmap[0]->osd->draw();
  if (globals->splitscreen)
  {
    playmap[1]->osd->save_background();
    playmap[1]->osd->draw();
  }

}

void gsLevel::clear_stuff()
{
  int n;

  playmap[0]->osd->restore_background();
  if (globals->splitscreen)
    playmap[1]->osd->restore_background();

  for (n=0; n < globals->players; n++) player[n]->restore_overlay();

  effects->explosions->clear(playmap[0]);
  if (globals->splitscreen) effects->explosions->clear(playmap[1]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->clear(playmap[0]);
    if (globals->splitscreen) player[n]->clear(playmap[1]);
  }

  objects->restore_background(playmap[0]);
  if (globals->splitscreen) objects->restore_background(playmap[1]);

  effects->bullets->restore_background(playmap[0]);
  if (globals->splitscreen) effects->bullets->restore_background(playmap[1]);

  clearlist->clear_screen();

  gpanel[0]->panel_clear();
  if (globals->splitscreen) gpanel[1]->panel_clear();

}

void gsLevel::do_logic()
{
  int n;

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
  }

  if (globals->splitscreen)
    if (player[1]->get_dead() == 0)
      playmap[1]->scroll_to(player[1]);
    else if (player[1]->get_dead() == 100)
      playmap[1]->scroll_to(player[1], globals->player_respawn-100);

  if (!globals->onescreen_map)
  {
    playmap[0]->scroll();
    if (globals->splitscreen) playmap[1]->scroll();
  }
  else
  {
    if (playmap[0]->is_redraw_needed())
      playmap[0]->redraw();
  }

  effects->pixels->move(globals->particle_gravity);
  effects->explosions->update();
  effects->bullets->move();

  objects->update();
  bases->update();
  enemies->update();

//  effects->pixels->check_collisions(playmap[0]);
//  if (globals->splitscreen) effects->pixels->check_collisions(playmap[1]);

  if (globals->splitscreen) effects->bullets->check_collisions(playmap[0], playmap[1]);
  else effects->bullets->check_collisions(playmap[0]);

  for (n=0; n < globals->players; n++)
  {
    player[n]->check_player_collisions();
    player[n]->check_stuff();
  }

  gpanel[0]->update_debug();
  if (globals->splitscreen || globals->onescreen_map) gpanel[1]->update_debug();

  gpanel[0]->update_anims();
  if (globals->splitscreen || globals->onescreen_map) gpanel[1]->update_anims();

  gpanel[0]->update_radar();
  if (globals->splitscreen || globals->onescreen_map) gpanel[1]->update_radar();

  playmap[0]->osd->update();
  if (globals->splitscreen) playmap[1]->osd->update();

  script->do_ingame_script(glob_lua);

  // computer player
  if (globals->computer_player)
    comp_player[0]->run_script();

  //if (globals->game_time % 30 == 0)
  {
    int th = script->get_gcthreshold(glob_lua);
    script->set_gcthreshold(glob_lua, 0);
    script->set_gcthreshold(glob_lua, th);
  }

}

void gsLevel::play()
{
  int actual_time = 0;
  int endthis = FALSE;
  int esc_pressed = FALSE;
  int update_graphics = FALSE;
  int display_esc_osd = FALSE;
  globals->game_time = globals->overall_game_time = globals->second_time = 0;
  globals->level_restart = 0;

  // create ESCape-OSD
  OsdBitmap *escosd = playmap[0]->osd->add(9999, globals->playscreen_width/2 - 100, 100, 200, 70, OSDBIT_SCREEN);
  escosd->clear();
  escosd->draw_rectangle();
  escosd->fill_area(globals->col_blue, 1, 1);
  escosd->draw_text(FONT_IMPACT14, language->get_string(T_KEY_ESC), globals->col_yellow, 5, 5, 0);
  escosd->draw_text(FONT_IMPACT10, language->get_string(T_GAME_ESC_EXIT), globals->col_orange, 80, 5+3, 0);

  escosd->draw_text(FONT_IMPACT14, language->get_string(T_KEY_F1), globals->col_yellow, 5, 25, 0);
  escosd->draw_text(FONT_IMPACT10, language->get_string(T_GAME_ESC_RESTART), globals->col_yellow, 80, 25+3, 0);

  escosd->draw_text(FONT_IMPACT14, language->get_string(T_GAME_ESC_OTHER_KEY), globals->col_yellow, 5, 45, 0);
  escosd->draw_text(FONT_IMPACT10, language->get_string(T_GAME_ESC_CONTINUE), globals->col_green, 80, 45+3, 0);
  escosd->set_visible(0);

  while (!(endthis))
  {
    while (globals->game_time > actual_time)
    {
      // game logic
      actual_time++;

      if (key[KEY_ESC] && !esc_pressed)
      {
        esc_pressed = TRUE;
        clear_keybuf();
        key[KEY_ESC] = 0;
      }

      // TEMP!!
//      if (key[KEY_P]) { player[0]->set_handicap(player[0]->get_handicap()+1); clear_keybuf(); }
//      if (key[KEY_O]) { player[0]->set_handicap(player[0]->get_handicap()-1); clear_keybuf(); }
            
      // ESC = Pause --> ESC=Exit, F1=Restart, other key=continue
      if (esc_pressed)
      {
        globals->pause = 1;
        
        if (!display_esc_osd)
        {
          display_esc_osd = TRUE;
          escosd->set_visible(1);
        }  

        if (key[KEY_ESC])
        {
          exit_level = 1;
          clear_keybuf();
        }
        
        else if (key[KEY_F1])
        {
          globals->level_restart = 1;
          exit_level = 1;
          clear_keybuf();
          esc_pressed = FALSE;
        }
        
        else if (keypressed())
        {
          globals->pause = 0;
          esc_pressed = FALSE;
          clear_keybuf();
          display_esc_osd = FALSE;
          escosd->set_visible(0);
        }
      }  

      if (exit_level == 1)
      {
        fade->initiate(FADE_TO_BLACK, 1, 50);
      }

      if (exit_level > 0)
      if (++exit_level && !endthis && !fade->get_fade_active())
      {
        exit_level = FALSE;
        endthis = TRUE;
        script->do_hook_exit_level(glob_lua);
      }

      if (player[0]->controls->get_input(SP_DEBUG, 1, 0))
      {
        if (gpanel[0]->get_debug()) { gpanel[0]->set_debug(0); gpanel[0]->blit_orig_paneldat(); gpanel[0]->update_all(); }
        else { gpanel[0]->set_debug(1); gpanel[0]->update_all(); }
      }

      if (player[0]->controls->get_input(SP_PANEL, 1, 0))
      {
        if (gpanel[0]->get_active()) gpanel[0]->set_active(0);
        else gpanel[0]->set_active(1);
        if (gpanel[1]->get_active()) gpanel[1]->set_active(0);
        else gpanel[1]->set_active(1);
        
        clear_bitmap(screen);
      }

      // globals->pause pressed?
      if (player[0]->controls->get_input(SP_PAUSE, 1, 0))
      {
        globals->pause = 1 - globals->pause;
      }
      
     
      // do game logic
      if (!globals->pause) do_logic();

      fade->update();

      update_graphics = TRUE;
    } // while game_time > actual_time

    if (update_graphics)
    {
      // draw graphics
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

  // delete ESC-OSD
  playmap[0]->osd->remove(escosd);

  if (replay_mode > 0) replay->close_file();

}

