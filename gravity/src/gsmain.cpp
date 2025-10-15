/*
 *   GRAVITY STRIKE -- gsmain.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#define DO_CRC_CHECKS  1
#define MAKE_CRC_FILE  0

#include "gsmain.h"             // main
#include "gsglob.h"
#include "gserror.h"
#include "gsmap.h"
#include "gsclear.h"
#include "gsplayer.h"
#include "gspanel.h"
#include "gseffect.h"
#include "gscontrl.h"
#include "gslevel.h"
#include "gslvlnw.h"
#include "gslvlsp.h"
#include "gsbase.h"
#include "gsobject.h"
#include "gslng.h"
#include "gsfade.h"
#include "gssound.h"
#include "gsnetw.h"
#include "gsreplay.h"
#include "gsenemy.h"
#include "gshelper.h"
#include "allegro5_wrapper.h"
#include "gsframes.h"
#include "gshiscor.h"
#include "gsplcomp.h"
#include "menu/gsmenu.h"
#include "contrib/md5class.h"
#include "gsscript.h"

gsGlobals   *globals    = NULL;
gsError     *errors     = NULL;
gsLanguage  *language   = NULL;
gsScript    *script     = NULL;
gsFade      *fade       = NULL;
gsSound     *gsound     = NULL;
gsNetwork   *net        = NULL;
gsMap       *playmap[2];
gsClear     *clearlist  = NULL;
gsPlayer    *player[8];             // maximum of 8 players
gsPanel     *gpanel[2];
gsEffect    *effects    = NULL;
gsBase      *bases      = NULL;
gsObject    *objects    = NULL;
gsEnemy     *enemies    = NULL;
gsImage     *images     = NULL;
gsReplay    *replay     = NULL;
gsHiscore   *hiscore    = NULL;
gsCompPlayer *comp_player[2];

void fps_timer(void)
{
  globals->fpst = globals->fps;
  globals->fps  = 0;
}
END_OF_FUNCTION(fps_timer);

void game_timer(void)
{
  globals->game_time++;
  globals->overall_game_time++;
}
END_OF_FUNCTION(game_timer);

void second_timer(void)
{
  if (!globals->pause)
    globals->second_time++;
}
END_OF_FUNCTION(second_timer);

gsMain::gsMain()
{
}

gsMain::~gsMain()
{
  exit();
}


int gsMain::init()
{
  globals = new gsGlobals;
  errors  = new gsError;

  set_uformat(U_ASCII);

  errors->log(2, "init", "allegro");
  allegro_init();
  errors->log(2, "init", "library version", allegro_id);

  set_window_title("Gravity Strike");

  // scripting
  errors->log(2, "init", "scripting");
  script = new gsScript;

  // game config
  errors->log(2, "init", "loading game config");
  script->load_game_config();

  // set keyboard layout
  char my_layout[2];
  switch (globals->language)
  {
    case LANG_DE :
      strcpy(my_layout, "de");
      break;
    case LANG_EN :
      strcpy(my_layout, "en");
      break;
    case LANG_CR :
      strcpy(my_layout, "en");
      break;
    default:
      strcpy(my_layout, "en");
      break;
  }
  char s[100];
  sprintf(s, "[system]\nkeyboard = %s", my_layout);
  set_config_data(s, strlen(s));

  errors->log(2, "init", "setting keyboard layout", my_layout);


  errors->log(2, "init", "keyboard");
  install_keyboard();

  // level files and crc
  script->read_level_files(1);

  errors->log(2, "init", "doing crc checks");
  hiscore = new gsHiscore();
  if (MAKE_CRC_FILE) hiscore->make_crc_file();
  if (DO_CRC_CHECKS)
  {
    if (hiscore->do_crc_checks())
    {
      extern int nocrcmessage;
      if (!nocrcmessage) allegro_message("ERROR: One or more of your global game files are invalid. Highscores will not work at all!\n");
      errors->log(2, "ERROR", "One or more global files are invalid. Highscores will not work at all!");
      globals->crc_failed = 1;
    }
  }

  errors->log(2, "init", "mouse");
  if (install_mouse() <= 0)
  {
    allegro_message("ERROR: No mouse detected.\nPlease install a mouse driver!\n");
    errors->log(2, "ERROR", "no mouse detected!");
    return 1;
  }

  #ifdef ALLEGRO_WINDOWS
    set_mouse_speed(4,4);
  #endif

  errors->log(2, "init", "trying to initialize joystick");
  #ifdef ALLEGRO_DOS
    if (install_joystick(JOY_TYPE_4BUTTON) == 0) globals->joystick_installed = 1; else install_joystick(JOY_TYPE_NONE);
  #else
    if (install_joystick(JOY_TYPE_AUTODETECT) == 0) globals->joystick_installed = 1; else install_joystick(JOY_TYPE_NONE);
  #endif
  if (globals->joystick_installed) errors->log(2, "init", "joystick(s) found!", num_joysticks);
  else errors->log(2, "init", "sorry, no joystick available");

  errors->log(2, "init", "timer");
  install_timer();

  LOCK_VARIABLE(globals->game_time);
  LOCK_VARIABLE(globals->overall_game_time);
  LOCK_VARIABLE(globals->fps);
  LOCK_VARIABLE(globals->fpst);
  LOCK_VARIABLE(globals->second_time);
  LOCK_FUNCTION(fps_timer);
  LOCK_FUNCTION(game_timer);
  LOCK_FUNCTION(second_timer);

  install_int(fps_timer,1000);
  install_int_ex(game_timer,BPS_TO_TIMER(60));
  install_int_ex(second_timer,SECS_TO_TIMER(1));

  // language files
  errors->log(2, "init", "language files");
  language = new gsLanguage;
  script->load_text();

  // clearlist
  errors->log(2, "init", "clear list");
  clearlist = new gsClear;

  errors->log(2, "init", "sound");
  gsound = new gsSound();

  // network
  errors->log(2, "init", "networking");
  net = new gsNetwork();

  // init datafiles
  char tmp_dir[255];
  errors->log(2, "init", "datafiles");
  packfile_password(globals->pwd);
  strcpy(tmp_dir, globals->data_dir);
  globals->menudat = load_datafile((char*)strcat(tmp_dir, globals->menu_datafile_name));
  strcpy(tmp_dir, globals->data_dir);
  globals->gamedat = load_datafile((char*)strcat(tmp_dir, globals->datafile_name));
  strcpy(tmp_dir, globals->data_dir);
  globals->shipdat = load_datafile((char*)strcat(tmp_dir, globals->shipdata_name));
  strcpy(tmp_dir, globals->data_dir);
  globals->fontdat = load_datafile((char*)strcat(tmp_dir, globals->fontdata_name));
  packfile_password(NULL);
  if (!globals->menudat) { allegro_message("Can't load menu datafile!"); return 1; }
  if (!globals->gamedat) { allegro_message("Can't load game datafile!"); return 1; }
  if (!globals->shipdat) { allegro_message("Can't load ship datafile!"); return 1; }
  if (!globals->fontdat) { allegro_message("Can't load font datafile!"); return 1; }

  // delete temporary ships (from previous crashes while network gaminng :)
//  net->delete_tmp_ship_files();

  // init graphics
  extern int gfxdriver;

  errors->log(2, "init", "gfx mode");
  if (set_gfx_mode(gfxdriver, globals->menu_resolution_x, globals->menu_resolution_y, 0, 0))
  {
    errors->log(0, "FATAL ERROR", allegro_error);
    return 1;
  }
  set_display_switch_mode(SWITCH_BACKGROUND);

  errors->log(2, "init", "looking for map images to be created...");
  create_map_images();

  // TEMP
  replay = new gsReplay();
  // TEMP

  return 0;
}

void gsMain::exit()
{
errors->log(2, "game", "removing timer");
  remove_timer(); 

errors->log(2, "game", "unloading datafiles");
  if (globals->menudat) unload_datafile(globals->menudat); globals->menudat = NULL;
  if (globals->gamedat) unload_datafile(globals->gamedat); globals->gamedat = NULL;
  if (globals->shipdat) unload_datafile(globals->shipdat); globals->shipdat = NULL;
  if (globals->fontdat) unload_datafile(globals->fontdat); globals->fontdat = NULL;

errors->log(2, "game", "destroying sound, net, scripts, clearlist, hiscore");
  if (gsound)        delete(gsound);      gsound = NULL;
  if (net)           delete(net);         net = NULL;
  if (script)        delete(script);      script = NULL;
  if (clearlist)     delete(clearlist);   clearlist = NULL;
  if (hiscore)       delete(hiscore);     hiscore = NULL;

  // TEMP
  if (replay)        delete(replay);      replay = NULL;
  // TEMP

errors->log(2, "game", "destroying errors, language, globals");
  if (errors)        delete(errors);      errors = NULL;
  if (language)      delete(language);    language = NULL;
  if (globals)       delete(globals);     globals = NULL;

  allegro_exit();
}

void gsMain::create_map_image(int levnum)
{
  char tileset[255];
  char marpath[255];
  int zoom;
  BITMAP *tempbit, *minimap;
  int error = 0;
  int image_size = 115;

  sprintf(marpath, "%s%s/%s", globals->level_dir,
                              globals->level_files[levnum].leveldir,
                              globals->level_files[levnum].mapfile);

  char outfile[255];
  replace_extension(outfile, marpath, "tga", 50);

  //errors->log(2, "outfile", outfile, marpath);

  if (exists(outfile))
  {
    error = 1; // map is already created, nothing to do...
  }

  if (!error)
  {
    errors->log(2, "init", "creating map image for map...", get_filename(marpath));

    // path for tileset
    strcpy(tileset, globals->data_dir);
    strcat(tileset, globals->level_files[levnum].map_style);

    if (MapLoad(tileset))
    {
      // alternative path for tileset
      sprintf(tileset, "%s%s/%s", globals->level_dir,
                                  globals->level_files[levnum].leveldir,
                                  globals->level_files[levnum].map_style);
     if (MapLoad(tileset))
     {
        errors->log(0, "ERROR", "loading tileset", globals->level_files[levnum].map_style);
        return;
      }
    }

    if (MapLoadMAR(marpath, 0))
    {
      errors->log(0, "ERROR", "loading map", marpath);
      return;
    }

    // get farthest "0" tile
    int far_x = 0;
    int far_y = 0;
    for (int xe=99; xe > 0; xe--)
      for (int ye=99; ye > 0; ye--)
        if (!(BLKSTR*)MapGetBlock(xe, ye)->tl)
        {
          if (xe > far_x) far_x = xe;
          if (ye > far_y) far_y = ye;
        }

    //errors->log(2, get_filename(marpath), far_x, far_y);

    int farthest_line = MAX(far_x, far_y);
    if (farthest_line < 90) farthest_line += 10; else farthest_line = 100;

    // set up new zoom
    zoom = (farthest_line * globals->tile_w) / image_size;
    
    int draw_w = globals->tile_w * 10;
    int draw_h = globals->tile_h * 10;
  
    minimap = create_bitmap(image_size, image_size);
    tempbit = create_bitmap(draw_w, draw_h);
    clear_bitmap(tempbit); clear_bitmap(minimap);

    int newsize = (farthest_line/10)+1;
    if (newsize > 10) newsize = 10;

    for (int w=0; w < newsize; w++)
      for (int h=0; h < newsize; h++)
      {
        // draw 100 tiles
        MapDrawBG(tempbit, draw_w * w, draw_h * h, 0, 0, draw_w, draw_h);
      
        // scale
        stretch_blit(tempbit, minimap,
                     0, 0, draw_w, draw_h,
                     (draw_w / zoom) * w, (draw_h / zoom) * h,
                     draw_w / zoom, draw_h / zoom);
      }
  
    destroy_bitmap(tempbit);

    char str_mapsize[10];
    sprintf(str_mapsize, "%02dx%02d", far_x+1, far_y+1);
    text_mode(-1);

    aatextout_right(minimap, (FONT*)globals->fontdat[font_impact].dat, str_mapsize, image_size-4, image_size-17, makecol8(0, 0, 0));
    aatextout_right(minimap, (FONT*)globals->fontdat[font_impact].dat, str_mapsize, image_size-3, image_size-16, makecol8(89, 102, 251));

    save_tga(outfile, minimap, (RGB *)globals->mainpal);
  
    destroy_bitmap(minimap);
  
    MapFreeMem();
  }
}

void gsMain::create_map_images()
{
  char datafile[255];

  strcpy(datafile, globals->data_dir);
  strcat(datafile, globals->datafile_name);

  packfile_password(globals->pwd);
  DATAFILE *dat = load_datafile_object(datafile, "gamepal");
  packfile_password(NULL);
  globals->mainpal = (PALETTE *)dat->dat;
  set_palette((RGB *)globals->mainpal);

  for (int n=0; n < globals->max_levels; n++)
  {
    create_map_image(n);
  }

  unload_datafile_object(dat);
}

void gsMain::do_sp_level()
{
  splevel = new gsSingleLevel();

  if ( splevel->init_stuff() )
  {
    errors->log(0, "FATAL ERROR", splevel->get_error_str());
  }
  else
  {
    errors->log(2, "Game", "single player level created, starting");
    splevel->play();
    errors->log(2, "Game", "single player level done");
  }

  delete(splevel);
}


void gsMain::do_level()
{
  level = new gsLevel();

  if ( level->init_stuff() )
  {
    errors->log(0, "FATAL ERROR", level->get_error_str());
  }
  else
  {
    errors->log(2, "Game", "level created, starting");
    level->play();
    errors->log(2, "Game", "level done");
  }

  delete(level);
}

void gsMain::do_network_level()
{
  nwlevel = new gsNetLevel();

  if ( nwlevel->init_stuff() )
  {
    errors->log(0, "FATAL ERROR", nwlevel->get_error_str());
  }
  else
  {
    errors->log(2, "Game", "network level created, starting");
    nwlevel->play();
    errors->log(2, "Game", "network level done");
  }

  delete(nwlevel);
}

void gsMain::play()
{
  int mode = MENU_MAIN;

  while (mode)
  {
    menu = new gsMenu();
  
    menu->init(mode);
    mode = menu->do_it();
  
    delete(menu);

    if (mode == MENU_1PSTATISTICS) 
    {
      do 
        do_sp_level();
      while(globals->level_restart);
    }  

    if (mode == MENU_2PSTATISTICS) 
    {
      do 
        do_level();
      while(globals->level_restart);
    }  
    
    if (mode == MENU_NETSTATISTICS) do_network_level();
  }
}


