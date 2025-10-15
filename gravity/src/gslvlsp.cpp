/*
 *   GRAVITY STRIKE -- gslvlsp.cpp
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
#include "gslvlsp.h"
#include "gslng.h"
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
#include "gshiscor.h"
#include "allegro5_wrapper.h"

#define MISSION_DONE_PAUSE      60*4

extern RGB_MAP     rgb_table;
extern COLOR_MAP   light_table;
extern COLOR_MAP   trans_table;

gsSingleLevel::gsSingleLevel()
{
  exit_level = FALSE;
  strcpy(error_str, "");
  globals->exit_level = 0;
  globals->pause = 0;
  mission_done_counter = MISSION_DONE_PAUSE;
  time_to_player_start = -1;

  // aktuelle globals sichern
  globals->save_values();

  // TEMP!
  //sgsrand(47);
}

gsSingleLevel::~gsSingleLevel()
{
  clear_bitmap(screen);
  globals->pause = 0;

  statistics *tmpstat = player[0]->get_statistics();
  if (time_to_player_start >= 0)
    tmpstat->used_time = globals->second_time - time_to_player_start;
  else
    tmpstat->used_time = 0;
  player[0]->set_statistics(tmpstat);

  globals->max_time = old_max_time;

  script->clear_ingame_script();
//  script->kill_script_commands(&lua);

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

  // save statistics
  int n;
  for (n=0; n < globals->players; n++)
    globals->player_stats[n] = player[n]->get_statistics_copy();

  gsound->stop_player_thrusters(player[0]->get_sig());

  delete player[0];
  player[0] = NULL;

  clearlist->clear_list();
  antialias_exit();

  gsound->destroy_user_samples();

  globals->restore_values();
  
  delete glob_lua;
}

int gsSingleLevel::init_stuff()
{
  extern gsHiscore *hiscore;
  globals->crc_failed = 0;

  errors->log(2, "init", "SINGLEPLAYER LEVEL");

  // change resolution to globals->game_resolution_x/y
  globals->switch_to_game_resolution();

  // init scripting
  glob_lua = new Lua();

  old_max_time = globals->max_time;
  globals->max_time = 0;

  // crc check!
  if (hiscore->do_crc_checks(globals->level_files[globals->selected_level].leveldir) ||
      hiscore->check_crc_failed_for_current_level())
  {
    strcpy(error_str, "Level checksum failure!");
    globals->crc_failed = 1;
  }
  
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

  // image handler
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

  errors->log(2, "init", "map");
  playmap[0] = new gsMap();

  // effects
  errors->log(2, "init", "effects");
  effects = new gsEffect;

  // player
  errors->log(2, "init", "player");
  int id = gsrand()%10000 + 500;
  player[0] = new gsPlayer(id , 0, 0, globals->pplayer[0].pship, playmap[0]);
  player[0]->set_name(globals->pplayer[0].pname);

  strncpy(globals->temp_last_ship_name, player[0]->get_ship_name(), 49); // for hiscores

  errors->log(2, "init", "loading player controls");
  player[0]->controls->set_type(LOCAL_SINGLE);
  player[0]->controls->load_key_cfg();

  // virtual screens
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

  // initialize
  init();

  // start fade in
  fade->initiate(FADE_TO_MAIN, 1, 60);
  fading_in = 1;

  // start getting replay -- TEMP!!!
  replay_mode = 0;

  if (replay_mode == 1)
  {
    replay->create_new_file();
    replay->start_recording();
  }

  if (replay_mode == 2)
  {
    replay->load_file("test.rpl");
  }

  // start script
  script->init_ingame_script();

  script->do_hook_init_level(glob_lua);

  return 0;
}

void gsSingleLevel::flip()
{
  int h = (gpanel[0]->get_active()) ? globals->playscreen_height : globals->playscreen_full_height;
  
  acquire_bitmap(screen);

  blit(playmap[0]->get_vscreen(), screen, 
       (int)playmap[0]->get_fscroll_x(), (int)playmap[0]->get_fscroll_y(), 
       0, 0,
       globals->playscreen_width, h);

  if (gpanel[0]->get_active()) gpanel[0]->draw(screen);

  release_bitmap(screen);
}

void gsSingleLevel::init()
{
  if (player[0]->get_x() == 0 && player[0]->get_y() == 0)
  {
    player[0]->set_homebase(0);
    player[0]->return_home();
  }

  player[0]->set_status_mode(1);

  gpanel[0]->update_all();

  if (!globals->onescreen_map)
  {
    playmap[0]->scroll_to(player[0]);
    playmap[0]->scroll();
  }
  else if (playmap[0]->get_scroll_to_x() < 0 && playmap[0]->get_scroll_to_y() < 0)
  {
    playmap[0]->scroll_to(globals->level_files[globals->selected_level].map_start_x,
                          globals->level_files[globals->selected_level].map_start_y);
    playmap[0]->scroll();
  }

  playmap[0]->request_redraw();
  if (globals->onescreen_map) playmap[0]->redraw();

  OsdBitmap *myosd = playmap[0]->osd->get_osdbit(1000);
  myosd->draw_typewriter_text(globals->level_files[globals->selected_level].missiondesc, 10, 5, 1, 1);

//  OsdBitmap *myosd2 = gpanel[0]->osd->get_osdbit(1000);
//  myosd2->draw_typewriter_text("[f:minifont][c:yellow]Initialisiere Schiffscomputer...[w:120][br]Starte Antrieb...[w:120][br]Ueberpruefe Toilettenspuelung...[w:120][br]Checke Pilotenstatus...[w:180][c:orange] Pilot reagiert nicht![w:60][c:yellow][br]Starte Wiederbelebungsmassnahmen...[w:60][br][c:white]Level 1: Kuenstliche Beatmung...[w:180][c:orange] keine Reaktion[w:60][br]Level2: Herzstimulation...[w:180][c:orange] fehlgeschlagen[w:240][br][c:red]Alarmstufe Rot: [c:white]Pilotenfehler",
//                               5, 5, 0, 0);

}

void gsSingleLevel::draw_stuff()
{
  if (fading_in && !fade->get_fade_active())
  {
    fading_in = 0;
    gpanel[0]->update_weapons(); gpanel[0]->update_shields();
  }

  effects->explosions->save_background();

  player[0]->save_background();

  objects->save_background(playmap[0]);
  bases->draw(playmap[0]);
  objects->draw(playmap[0]);

  player[0]->check_map_collisions();
  bases->draw_cargo(playmap[0]);

  objects->draw_special(playmap[0]);
  effects->pixels->check_collisions(playmap[0]);
  effects->pixels->draw(playmap[0]);

  effects->bullets->save_background(playmap[0]);

  effects->bullets->draw(playmap[0]);
  effects->explosions->draw(playmap[0]);
  enemies->draw(playmap[0]);

  player[0]->display(playmap[0]);

  effects->explosions->draw_shield(playmap[0]);

  player[0]->save_overlay_back();
  player[0]->draw_overlay();

  playmap[0]->osd->save_background();
  playmap[0]->osd->draw();

  gpanel[0]->osd->save_background();
  gpanel[0]->osd->draw();
}

void gsSingleLevel::clear_stuff()
{
  gpanel[0]->osd->restore_background();
  playmap[0]->osd->restore_background();
  player[0]->restore_overlay();

  effects->explosions->clear(playmap[0]);

  player[0]->clear(playmap[0]);

  effects->bullets->restore_background(playmap[0]);
  objects->restore_background(playmap[0]);

  clearlist->clear_screen();

  gpanel[0]->panel_clear();
}

void gsSingleLevel::do_logic()
{
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

  // TEMP!!!
  if (replay_mode == 1) replay->write_position_data();
  // TEMP!!!
  if (replay_mode == 2) replay->read_data();

  player[0]->update();

  if (player[0]->get_mission_status() != 0 || player[0]->get_lifes() == 0)
  {
    if (mission_done_counter == MISSION_DONE_PAUSE)
    {
      OsdBitmap *myosd = playmap[0]->osd->get_osdbit(1000);
      char mytext[100];

      if (player[0]->get_mission_status() == 1)
        sprintf(mytext, "[center][f:impact14][c:green]%s", language->get_string(T_GAME_MISSION_DONE));
      else if (player[0]->get_mission_status() == -1)
        sprintf(mytext, "[center][f:impact14][c:orange]%s", language->get_string(T_GAME_MISSION_FAILED));
      else
        sprintf(mytext, "[center][f:impact14][c:white]%s", language->get_string(T_GAME_YOU_ARE_DEAD));

      myosd->draw_typewriter_text(mytext, 30, 6);
    }

    mission_done_counter--;
  }

  if (!mission_done_counter) { if (!exit_level) exit_level = 1; }
  if (globals->exit_level) if (!exit_level) exit_level = 1;

  if (!globals->onescreen_map)
  {
    if (player[0]->get_dead() == 0)
      playmap[0]->scroll_to(player[0]);
    else if (player[0]->get_dead() == 100 && player[0]->get_lifes() && player[0]->get_mission_status() == 0)
      playmap[0]->scroll_to(player[0], globals->player_respawn-100);
  }
  else
  {
    if (playmap[0]->is_redraw_needed())
      playmap[0]->redraw();
  }

  if (!globals->onescreen_map)
  {
    playmap[0]->scroll();
  }

  effects->pixels->move(globals->particle_gravity);
  effects->explosions->update();
  effects->bullets->move();

  objects->update();
  bases->update();
  enemies->update();

  effects->bullets->check_collisions(playmap[0]);

  player[0]->check_player_collisions();
  player[0]->check_stuff();

  gpanel[0]->update_debug();
  gpanel[0]->update_anims();

  gpanel[0]->update_radar();

  playmap[0]->osd->update();
  gpanel[0]->osd->update();

  if (time_to_player_start < 0 && player[0]->get_thrust())
    time_to_player_start = globals->second_time;

  script->do_ingame_script(glob_lua);

  int th = script->get_gcthreshold(glob_lua);
//  if (globals->game_time % 60 == 0)
    script->set_gcthreshold(glob_lua, 0);
  script->set_gcthreshold(glob_lua, th);

}

void gsSingleLevel::play()
{
  int actual_time = 0;
  int endthis = FALSE;
  int esc_pressed = FALSE;
  int update_graphics = FALSE;
  int display_esc_osd = FALSE;
  globals->game_time = globals->overall_game_time = globals->second_time = 0;
  globals->level_restart = 0;

  // create ESCape-OSD
  OsdBitmap *escosd = playmap[0]->osd->add(9999, screen->w/2 - 100, 100, 200, 70, OSDBIT_SCREEN);
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
        else 
        {
          clear_bitmap(screen);
          gpanel[0]->set_active(1);
        }
      }

      // pause pressed?
      if (player[0]->controls->get_input(SP_PAUSE, 1, 0) && !globals->network_game)
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

