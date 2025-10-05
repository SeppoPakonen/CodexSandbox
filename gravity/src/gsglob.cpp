/*
 *   GRAVITY STRIKE -- gsglob.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <string.h>
#include <stdio.h>
#include "gsglob.h"
#include "gshelper.h"
#include "gslng.h"
#include "gserror.h"
#include "contrib/md5class.h"

extern gsError *errors;

gsGlobals::gsGlobals()
{
  int n;
  
  // control flow
  language              = LANG_EN;
  fps = fpst = game_time = overall_game_time = second_time = 0;
  max_craters           = 100;
  splitscreen           = 1;
  players               = 1;
  onescreen_map         = FALSE;
  game_mode             = GAME_DOGFIGHT;
  singleplayer          = 0;
  minigame              = 0;
  network_game          = 0;
  ready                 = 0;
  exit_level            = 0;
  use_joystick          = 0;
  joystick_installed    = 0;
  rand_ini              = 0;
  handicap              = 0;

  // graphics
  
  game_resolution[0].x  =  640; game_resolution[0].y =  480;
  game_resolution[1].x  =  800; game_resolution[1].y =  600;
  game_resolution[2].x  = 1024; game_resolution[2].y =  768;
  game_resolution[3].x  = 1280; game_resolution[3].y = 1024;
  game_resolution[4].x  = 1600; game_resolution[4].y = 1200;
  use_resolution_nr     = 0;
  
  game_resolution_x     = 640;
  game_resolution_y     = 480;
  menu_resolution_x     = 640;
  menu_resolution_y     = 480;
  
  tile_w                = 48;
  tile_h                = 48;
  map_w                 = 100;
  map_h                 = 100;
  plus_x                = 500;
  plus_y                = 500;

  strcpy(server_ip, "192.168.5.1");

  init_size_vars();

  // environment
  gravity_percent       = 1.0;
  particle_gravity      = 0.015;
  level_gravity         = 0.006;
//  level_gravity = 0; // temp
  bomb_gravity          = 0.040;

  // setup options
  pixamount             = 100;
  soundvol              = 100;
  musicvol              = 100;

  activate_extra        = 0;
  activate_extra_bar    = 0;
  capture_the_flag      = 0;
  hunter_hunted         = 0;
  race                  = 0;
  race_laps             = 3;

  // player
  shoot_speed           = 5;
  shoot_speed_bomb      = 3;
  player_respawn        = 60 * 5; // 5 seconds
  player_indestructible = 60 * 3;

  // other
  do_all_hooks          = 1;
  slow_computer         = 0;
  computer_player       = 0;
  computer_player_strength = 5;
  use_script_computer_player = 0;
  pause                 = 0;
  level_restart         = 0;

  // datafiles
  strcpy(pwd,             "tanja");
  strcpy(data_dir,        "./dat/");
  strcpy(level_dir,       "./levels/");
  strcpy(hiscore_dir,     "./hiscores/");
  strcpy(levelstate_dir,  "./state/");
  strcpy(ship_dir,        "./ships/");
  strcpy(map_dir,         "./maps/");
  strcpy(script_dir,      "./scripts/");
  strcpy(level_mission_dir,   "mission");
  strcpy(level_multiplay_dir, "dogfight");
  strcpy(level_race_dir,      "race");
  strcpy(level_minigame_dir,  "minigame");
  
/*
  strcpy(data_dir,        "d:/anwend/djgpp/source/gs/dat/");
  strcpy(level_dir,       "d:/anwend/djgpp/source/gs/levels/");
  strcpy(hiscore_dir,     "d:/anwend/djgpp/source/gs/hiscores/");
  strcpy(ship_dir,        "d:/anwend/djgpp/source/gs/ships/");
  strcpy(map_dir,         "d:/anwend/djgpp/source/gs/maps/");
  strcpy(script_dir,      "d:/anwend/djgpp/source/gs/scripts/");
*/
  strcpy(configfile_name, "gs.cfg");
  strcpy(datafile_name,   "gs.dat");
  strcpy(soundfile_name,  "gssnd.dat");
  strcpy(shipdata_name,   "ships.dat");
  strcpy(fontdata_name,   "fonts.dat");
  strcpy(menu_datafile_name, "gsmenu.dat");
  strcpy(language_file_name, "text.lua");
  strcpy(keyboard_file_name, "keys.dat");
  strcpy(hiscore_file_name, "hiscore.txt");
  strcpy(crc_file_name,   "crc.dat");
  strcpy(global_ini,      "global.ini");
  strcpy(global_sc,       "global.sc");
  strcpy(logfile_name,    "gs.log");
  strcpy(def_ship,        "def1.shp");
  strcpy(def_level,       "level.i00");

  // twoplayer
  max_frags             = 20;
  max_time              = 20;
  extra_amount          = 5;

  score_per_second      = 2;

  // temp vars
  for (n=0; n < 2; n++)
  {
    sprintf(pplayer[n].pname, "Player %d", n+1);
    strcpy(pplayer[n].pship, "");
    pplayer[n].sig = 0;
  }
  for (n=0; n < 8; n++)
  {
    memset(&player_stats[n], 0, sizeof(statistics));
  }
  last_menu = -1;
  ship_num = -1;
  crc_failed = 0;
  temp_minigame_2player = 0;

  // NULLs
  vscreen               = NULL;
  vscreen2              = NULL;
  paneldat              = NULL;
  shipdat               = NULL;
  menudat               = NULL;
  gamedat               = NULL;
  sounddat              = NULL;
  fontdat               = NULL;

  // level info
  selected_level        = 0;
  strcpy(last_level, "");
  strcpy(last_splevel, "");
  strcpy(last_leveltype, "");
  strcpy(last_minigame, "");
  max_levels            = 0;
  memset(&level_files, 0, sizeof(level_files));
  strcpy(temp_last_ship_name, "");

  memset(&crc_levelfail, 0, sizeof(crc_levelfail));

//  for (n=0; n < 100; n++) user_int[n] = 0;
//  for (n=0; n < 50; n++) user_float[n] = 0;
//  for (n=0; n < 50; n++) strcpy(user_string[n], "");
}

void gsGlobals::init_size_vars()
{
  panel_full_height     = 56;
  panel_full_width      = 640;
  panel_half_height     = 56;
  panel_half_width      = 317;

  playscreen_full_width = game_resolution_x;
  playscreen_full_height = game_resolution_y;
  playscreen_width      = playscreen_full_width;
  playscreen_height     = playscreen_full_height - panel_full_height;
  playscreen_split_width= playscreen_width/2 - 3;

  playscreen1_sx        = 0;
  playscreen2_sx        = playscreen_full_width/2 + 2;

  panel_full_x          = 0;
  panel_full_y          = playscreen_height;
  
/* deprecated
  panel_half1_x         = 0;
  panel_half2_x         = playscreen_full_width/2 + 2;
  panel_half1_y         = playscreen_height - panel_full_height;
  panel_half2_y         = playscreen_height - panel_full_height;
*/
}

void gsGlobals::save_values()
{
  _saved_globals *s = &saved_globals;

  if (singleplayer == 1)
  {
    gravity_percent       = 1.0;
    particle_gravity      = 0.015;
    level_gravity         = 0.006;
    bomb_gravity          = 0.040;
    score_per_second      = 2;
  }
  
  s->f[1]  =  gravity_percent;
  s->f[2]  =  level_gravity;
  s->f[3]  =  particle_gravity;
  s->f[4]  =  bomb_gravity;
  
  s->i[1]  =  exit_level;
  s->i[2]  =  max_craters;
  s->i[3]  =  language;
  s->i[4]  =  plus_x;
  s->i[5]  =  plus_y;
  s->i[6]  =  pixamount;
  s->i[7]  =  soundvol; 
  s->i[8]  =  race_laps;
  s->i[9]  =  shoot_speed;
  s->i[10] =  shoot_speed_bomb;
  s->i[11] =  player_respawn;
  s->i[12] =  player_indestructible;
  s->i[13] =  extra_amount;
  s->i[14] =  max_frags;
  s->i[15] =  max_time;
  s->i[16] =  computer_player;
  s->i[17] =  computer_player_strength;
  
  delete_level_temps();
}

void gsGlobals::restore_values()
{
  _saved_globals *s = &saved_globals;
  
  gravity_percent     = s->f[1];
  level_gravity       = s->f[2];
  particle_gravity    = s->f[3];
  bomb_gravity        = s->f[4];
  
  max_craters         = s->i[2];
  language            = s->i[3];
  plus_x              = s->i[4];
  plus_y              = s->i[5];
  pixamount           = s->i[6];
  soundvol            = s->i[7]; 
  race_laps           = s->i[8];
  shoot_speed         = s->i[9];
  shoot_speed_bomb    = s->i[10];
  player_respawn      = s->i[11];
  player_indestructible = s->i[12];
  extra_amount        = s->i[13];
  max_frags           = s->i[14];
  max_time            = s->i[15];
  computer_player     = s->i[16];
  computer_player_strength = s->i[17];

  do_all_hooks        = 1;
  exit_level          = 0;
}

gsGlobals::~gsGlobals()
{
 int m=0;
 while (crc_levelfail[m])
 {
   delete[] crc_levelfail[m];
   m++;
 }
}

void gsGlobals::make_color_tables()
{
  int nr;

  for (nr=0; nr <= 10; nr++)
  {
    tbl_blue[nr] = makecol8(nr*13,nr*16,nr*25);
    tbl_bluewhite[nr] = makecol8(nr*18,nr*20,nr*25);
    tbl_white[nr] = makecol8(nr*23,nr*23,nr*23);
    tbl_green[nr] = makecol8(0,nr*25,0);
    tbl_red[nr] = makecol8(nr*25,nr*18,nr*15);
    tbl_yellow[nr] = makecol8(nr*25,nr*25,0);
  }

  for (nr=0; nr < 256; nr++)
    tbl_all[nr] = makecol8(fxrand() % 255,fxrand() % 255,fxrand() % 255);
}

void gsGlobals::save_config_file()
{
  char path[40];
  strcpy(path, data_dir);
  strcat(path, configfile_name);

  FILE *f = fopen(path, "wt");

  if (f)
  {
    fprintf(f, "-- GRAVITY STRIKE v%s: Global Configuration\n\n", VERSION);

    switch (language)
    {
      case LANG_EN : fprintf(f, "%-20s = LANG_EN\n", "language"); break;
      case LANG_DE : fprintf(f, "%-20s = LANG_DE\n", "language"); break;
      case LANG_CR : fprintf(f, "%-20s = LANG_CR\n", "language"); break;
      case LANG_DK : fprintf(f, "%-20s = LANG_DK\n", "language"); break;
      case LANG_IT : fprintf(f, "%-20s = LANG_IT\n", "language"); break;
    }

    fprintf(f, "%-20s = %d\n", "pixels", pixamount);
    fprintf(f, "%-20s = %d\n", "sound_volume", soundvol);
    fprintf(f, "%-20s = %d\n", "music_volume", musicvol);

    fprintf(f, "%-20s = \"%s\"\n", "player1_name", pplayer[0].pname);
    fprintf(f, "%-20s = \"%s\"\n", "player2_name", pplayer[1].pname);
    fprintf(f, "%-20s = \"%s\"\n", "player1_ship", pplayer[0].pship);
    fprintf(f, "%-20s = \"%s\"\n", "player2_ship", pplayer[1].pship);
    fprintf(f, "%-20s = \"%s\"\n", "default_map", last_level);
    fprintf(f, "%-20s = \"%s\"\n", "default_spmap", last_splevel);
    fprintf(f, "%-20s = \"%s\"\n", "default_minigame", last_minigame);

    fprintf(f, "%-20s = %d\n", "gravity", int(gravity_percent*100.0));
    fprintf(f, "%-20s = %d\n", "maxfrags", max_frags);
    fprintf(f, "%-20s = %d\n", "maxtime", max_time);
    fprintf(f, "%-20s = %d\n\n", "extras", extra_amount);
    fprintf(f, "%-20s = %d\n", "slow_computer", slow_computer);
    fprintf(f, "%-20s = %d\n", "use_script_computer_player", use_script_computer_player);
    fprintf(f, "%-20s = %d\n", "resolution", globals->use_resolution_nr);
    fprintf(f, "%-20s = \"%s\"\n", "server_ip", globals->server_ip);

    fclose(f);
  } // if f

}

int gsGlobals::make_color(int r, int g, int b)
{
  return makecol8(r, g, b);
}

void gsGlobals::make_standard_colors()
{
  mainpal = (PALETTE *)gamedat[gamepal].dat;
  set_palette((RGB *)mainpal);
  
  color[0]  = makecol8(255,255,255);        // white
  color[1]  = makecol8(95,255,95);          // light green
  color[2]  = makecol8(255,255,0);          // yellow
  color[3]  = makecol8(255,100,100);        // light red
  color[4]  = makecol8(100,100,255);        // light blue
  color[5]  = makecol8(255,255,255);        // white
  color[6]  = makecol8(255,255,0);          // yellow
  color[7]  = makecol8(255,100,100);        // light red
  color[8]  = makecol8(100,255,100);        // light green
  color[9]  = makecol8(100,100,255);        // light blue

  col_red       = makecol8(255,0,0);
  col_bred      = makecol8(255,255,0);
  col_green     = makecol8(0,255,0);
  col_blue      = makecol8(0,0,255);
  col_black     = makecol8(0,0,0);
  col_white     = makecol8(255,255,255);
  col_yellow    = makecol8(255,255,0);
  col_orange    = makecol8(255,128,0);
  col_lblue     = makecol8(20,20,200);
  col_grey      = makecol8(147,147,147);
  col_dgrey     = makecol8(47,47,47);

}

void gsGlobals::switch_to_game_resolution()
{
  extern int gfxdriver;
  
  // require special resolution?
  if (level_files[selected_level].require_resolution_nr >= 0)
  {
    game_resolution_x = game_resolution[ level_files[selected_level].require_resolution_nr ].x;
    game_resolution_y = game_resolution[ level_files[selected_level].require_resolution_nr ].y;
  }
  // else use configured resolution
  else
  {
    game_resolution_x = game_resolution[ use_resolution_nr ].x;
    game_resolution_y = game_resolution[ use_resolution_nr ].y;
  }
  
  if (game_resolution_x == screen->w && game_resolution_y == screen->h)
  {
    init_size_vars();
    return; // nothing else to do
  }
  
  if (set_gfx_mode(gfxdriver, game_resolution_x, game_resolution_y, 0, 0))
  {
    game_resolution_x = 640; 
    game_resolution_y = 480;
    set_gfx_mode(gfxdriver, game_resolution_x, game_resolution_y, 0, 0);
    errors->log(1, "ERROR", "game resolution not supported! (using 640x480)");
  }

  init_size_vars();
}

void gsGlobals::switch_to_menu_resolution()
{
  extern int gfxdriver;

  if (menu_resolution_x == screen->w && menu_resolution_y == screen->h)
    return; // nothing to do
  
  if (set_gfx_mode(gfxdriver, menu_resolution_x, menu_resolution_y, 0, 0))
  {
    set_gfx_mode(gfxdriver, 640, 480, 0, 0);
    errors->log(1, "ERROR", "menu resolution not supported! (using 640x480)");
  }

  init_size_vars();
}


int gsGlobals::get_level_num_from_filename(char *nm)
{
  for (int n=0; n < max_levels; n++)
  {
    if (stricmp(level_files[n].levelfile, nm) == 0)
      return n;
  }

  return -1;
}

void gsGlobals::delete_level_temps()
{
  strcpy(score_string, "");
  use_user_stats = 0;
  for (int n=0; n < 10; n++)
    user_stats[n].clear();
}

void gsGlobals::create_level_done_file()
{
  gsMD5 md5;
  char checksumstring[200];
  char checksumbuf[100];
  char checksum[100];
  char lvlname[300];
  
  memset(checksum, 0, sizeof(checksum));
  memset(checksumbuf, 0, sizeof(checksumbuf));
  memset(checksumstring, 0, sizeof(checksumstring));

  sprintf(lvlname, "%s%s/%s", level_dir,
                              level_files[selected_level].leveldir,
                              level_files[selected_level].levelfile);
 
  // get md5 from lvl file
  md5.get_md5_from_file(lvlname, checksumbuf);
  
  // make checksum string
  sprintf(checksumstring, "%s-%s", checksumbuf, player[0]->get_name());
  
  // get final md5
  md5.get_md5(checksumstring, strlen(checksumstring), checksum);
  
  // write to lvlname.fin file
  char levelfin[100];
  replace_extension(levelfin, lvlname, "fin", 100);
  
  char nlevelfin[200];
  sprintf(nlevelfin, "%s%s", globals->levelstate_dir, get_filename(levelfin));
  
  FILE *FIN = fopen(nlevelfin, "wt");
  if (!FIN) return;
  fprintf(FIN, "%s", checksum);
  fclose(FIN);
}

int gsGlobals::check_level_done_file(char *depends_on, char *plname)
{
  gsMD5 md5;
  char lvlname[300];
  char file_checksum[100];
  char checksumstring[200];
  char checksumbuf[100];
  char checksum[100];

  memset(file_checksum, 0, sizeof(file_checksum));
  memset(checksum, 0, sizeof(checksum));
  memset(checksumbuf, 0, sizeof(checksumbuf));
  memset(checksumstring, 0, sizeof(checksumstring));

  sprintf(lvlname, "%s%s", level_dir, depends_on);

  char levelfin[100];
  replace_extension(levelfin, lvlname, "fin", 100);

  char nlevelfin[200];
  sprintf(nlevelfin, "%s%s", globals->levelstate_dir, get_filename(levelfin));
  
  FILE *FIN = fopen(nlevelfin, "rt");
  // file not found? level not finished...
  if (!FIN) return 0; 

  // get lvl md5
  md5.get_md5_from_file(lvlname, checksumbuf);

//errors->log(2, "checksumbuf", checksumbuf);

  // make checksum string
  char mplname[100];
  strncpy(mplname, plname, 99);
  sprintf(checksumstring, "%s-%s", checksumbuf, strupr(mplname));

//errors->log(2, "checksumstring", checksumstring);
  
  // get final md5
  md5.get_md5(checksumstring, strlen(checksumstring), checksum);

//errors->log(2, "checksum", checksum);

  // get checksum from file
  fgets(file_checksum, 100, FIN);
  fclose(FIN);

//errors->log(2, "file_checksum", file_checksum);
  
  // compare checksums...
  if (strcmp(checksum, file_checksum) == 0)
    return 1;
  else
    return 0;

}

UserStats::UserStats()
{
}

UserStats::~UserStats()
{
}

void UserStats::clear()
{
  strcpy(text, " ");
  textcolor = globals->col_green;
  plusx = 0;

  for (int n=0; n < 6; n++)
  {
    strcpy(value[n], " ");
    valuecolor[n] = globals->col_yellow;
  }
}


