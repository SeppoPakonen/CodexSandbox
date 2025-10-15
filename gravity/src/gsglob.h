/*
 *   GRAVITY STRIKE -- gsglob.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#define ALLEGRO_NO_COMPATIBILITY
#include "allegro5_wrapper.h"


#ifndef _GSGLOBALS_
#define _GSGLOBALS_

#include "gsd.h"
#include "gssd.h"
#include "fonts.h"
#include "gsplayer.h"
//#include <mss.h>

#include <stdio.h>

#define VERSION   "0.94"

#define MAX_LEVELS  150

enum GAME_MODES {
  GAME_DOGFIGHT,
  GAME_RACE
};

enum GS_FONTS {
  FONT_ICE        = font_ice,
  FONT_BANK       = font_bank,
  FONT_IMPACT10   = font_impact,
  FONT_IMPACT14   = font_impact14,
  FONT_LCD        = font_lcd,
  FONT_KEYPUNCH   = font_keypun,
  FONT_VERDANA    = font_verdana,
  FONT_MINIFONT   = font_minifont,
  FONT_FIXED      = font_fixed
};

typedef struct RESOLUTION {
  int x;
  int y;
} RESOLUTION;

typedef struct level_info {
  char   map_style[30];                            // filename of user style (fmp file)
  int    onescreen_map;                            // is it a onescreen map?
  int    map_start_x;                              // x offset for os map
  int    map_start_y;                              // y offset for os map
  char   activate_extra_str[35];                   // string for "activate" button
  char   activate_extra_bar_str[35];               // string for "activate" bar
  int    activate_ctf;                             // activate capture the flag?
  int    activate_hh;                              // activate hunter-hunted?
  int    activate_global_script;                   // activate global script functions?
  int    activate_singleplayer;                    // activate single player mode?
  int    activate_race;                            // activate race mode?
  int    activate_minigame;                        // activate mini game?
  float  min_version;                              // minimum game version for this level?
  int    max_players;                              // maximum of players to handle (1-6)
  int    crc_failed;                               // crc check for this level failed?
  char   name[50];                                 // name of level
  char   desc[400];                                // description of level
  char   missiondesc[1000];                        // mission description of level
  char   leveldir[100];                            // directory with level files
  char   levelfile[40];                            // filename of level file
  char   mapfile[40];                              // filename of map file (.mar)
  char   initscriptfile[40];                       // filename of init script
  char   scriptfile[40];                           // filename of game script
  char   author[50];                               // level author
  char   depends_on[100];                          // depends on this level to be finished
  char   depends_on_txt[250];                      // text to display when level depends on another
  int    require_resolution_nr;                    // require this resolution
} level_info;

typedef struct PLAYERGLOB {
  int    sig;                                      // signature of this player
  int    ready;                                    // player ready to start?
  char   pname[21];                                // name of player
  char   pship[21];                                // filename of ship
} PLAYERGLOB;

class UserStats {
  private:
    char    text[201];                             // display this text
    int     textcolor;                             // color for text
    char    value[6][21];                          // values (20 chars) for max. 6 players
    int     valuecolor[6];                         // color of values
    int     plusx;                                 // value to add on left side

  public:
    UserStats();
    ~UserStats();

    void    clear();

    void    set_text(char *n)             { strncpy(text, n, 200); }
    void    set_textcolor(int n)          { textcolor = n; }
    void    set_value(int pl, char *n)    { if (pl >= 0 && pl <= 5) strncpy(value[pl], n, 20); }
    void    set_value(int pl, int n)      { if (pl >= 0 && pl <= 5) sprintf(value[pl], "%d", n); }
    void    set_valuecolor(int pl, int n) { if (pl >= 0 && pl <= 5) valuecolor[pl] = n; }
    void    set_plusx(int n)              { plusx = n; }

    char*   get_text()                    { return text; }
    int     get_textcolor()               { return textcolor; }
    char*   get_value(int pl)             { if (pl >= 0 && pl <= 5) return value[pl]; return NULL; }
    int     get_valuecolor(int pl)        { if (pl >= 0 && pl <= 5) return valuecolor[pl]; return 0; }
    int     get_plusx()                   { return plusx; }
};

typedef struct _saved_globals {
  float f[10];
  int i[30];
} _saved_globals;

class gsGlobals
{
  private:
    _saved_globals saved_globals;
    
    void init_size_vars();
    
  public:
    // Funktionen
    gsGlobals();
    ~gsGlobals();
    
    void save_values();
    void restore_values();
    
    void switch_to_game_resolution();
    void switch_to_menu_resolution();

    // control flow
    volatile int  fps, fpst;                       // frames per seconds
    volatile int  game_time, overall_game_time;    // game timers
    volatile int  second_time;                     // second timer
    char          logfile_name[21];                // name of logfile
    int           splitscreen;                     // splitscreen game?
    int           onescreen_map;                   // map only one screen big?
    int           players;                         // how much players?
    int           game_mode;                       // mode of game
    int           singleplayer;                    // single player mode?
    int           minigame;                        // mini game?
    int           network_game;                    // is this a network game?
    int           ready;                           // ok, i'm ready
    int           use_joystick;                    // joystick active?
    int           joystick_installed;              // is joystick installed? 
    int           rand_ini;                        // initialization for random number generator
    char          server_ip[20];                   // ip address of game server


    // graphics
    RESOLUTION    game_resolution[5];              // storage for resolutions
    int           use_resolution_nr;               // use game_resolution[use_resolution_nr]
    int           game_resolution_x;               // game resolution (x)
    int           game_resolution_y;               // game resolution (y)
    int           menu_resolution_x;
    int           menu_resolution_y;
    int           tile_w;                          // width of tile in pixels
    int           tile_h;                          // height of tile
    int           map_w;                           // width of map in tiles
    int           map_h;                           // height of map
    int           playscreen_width;                // width of vis. screen
    int           playscreen_full_width;           // width of full screen
    int           playscreen_split_width;          // width of splitted screen
    int           playscreen_height;               // height of vis. screen
    int           playscreen_full_height;          // height of full screen
    int           playscreen1_sx;                  // x start of playscr1
    int           playscreen2_sx;                  // x start of playscr2
    int           panel_full_width;                // full width of panel
    int           panel_full_height;               // full height of panel
    int           panel_half_width;                // half width of panel (2p)
    int           panel_half_height;               // 2p panel height
    int           panel_full_x;                    // panel coordinates
    int           panel_full_y;
    int           panel_half1_x;                   // panel coordinates (2p)
    int           panel_half1_y;
    int           panel_half2_x;
    int           panel_half2_y;
    BITMAP        *vscreen;                        // virtual screen
    BITMAP        *vscreen2;                       // virtual screen 2nd pl.

    int           col_white;                       // some color definitions
    int           col_grey;
    int           col_dgrey;
    int           col_yellow;
    int           col_red;
    int           col_bred;
    int           col_blue;
    int           col_lblue;
    int           col_green;
    int           col_black;
    int           col_orange;
    int tbl_blue[21];
    int tbl_white[21];
    int tbl_bluewhite[21];
    int tbl_green[21];
    int tbl_yellow[21];
    int tbl_red[21];
    int tbl_all[256];
    PALETTE        *mainpal;                       // pointer to main pal data

    // environment
    int           slow_computer;
    int           use_script_computer_player;      // use script player instead of built in

    // setup options
    int           activate_extra;                  // activate level extras?
    int           activate_extra_bar;              // value of extra bar
    int           capture_the_flag;                // ctf mode activated?
    int           hunter_hunted;                   // hh mode activated?
    int           race;                            // race mode activated?
    int           handicap;                        // handicap mode ativated?

    // player
    int           color[10];                       // colors of players

    // datafiles
    char          pwd[21];                         // password
    char          data_dir[50];                    // dir with data files
    char          ship_dir[50];                    // dir with ship defs
    char          level_dir[50];                   // dir with level defs
    char          level_mission_dir[50];           // dir with mission files (singleplayer)
    char          level_multiplay_dir[50];         // dir with multiplayer files
    char          level_race_dir[50];              // dir with race files
    char          level_minigame_dir[50];          // dir with minigames
    char          map_dir[50];                     // dir with map files
    char          script_dir[50];                  // dir with script files
    char          hiscore_dir[50];                 // dir with hiscore files
    char          levelstate_dir[50];              // dir with level states (.fin files)
    char          configfile_name[21];             // name of globconfig file
    char          datafile_name[21];               // name of data file
    char          soundfile_name[21];              // name of sound data file
    char          shipdata_name[21];               // name of ship data file
    char          fontdata_name[21];               // name of font data file
    char          menu_datafile_name[21];          // name of menu data file
    char          language_file_name[21];          // name of language file
    char          keyboard_file_name[21];          // name of keyboard config file
    char          global_ini[21];                  // name of global init file
    char          global_sc[21];                   // name of global script file
    char          hiscore_file_name[21];           // name of hiscore file
    DATAFILE      *menudat;                        // menu datafile
    DATAFILE      *gamedat;                        // game datafile
    DATAFILE      *paneldat;                       // panel data
    DATAFILE      *shipdat;                        // ship data
    DATAFILE      *sounddat;                       // sound data
    DATAFILE      *fontdat;                        // font data
    char          def_ship[21];                    // name of default ship
    char          def_level[21];                   // name of default level

    // twoplayer options

    // other options
    int           pause;                           // pause active?
    int           level_restart;                   // restart level after Escaping?

    // crc
    int           crc_failed;                      // crc checks failed?
    char          crc_file_name[21];
    char          *crc_levelfail[MAX_LEVELS];      // crc failed for level ... ?


    // temporary variables
    PLAYERGLOB    pplayer[2];                      // temporary global player data
    int           last_menu;                       // menu we come from...
    int           ship_num;                        // fill what ship var?
    statistics    player_stats[8];                 // temp storage for player statistics
    char          score_string[50];                // string with scores for hiscore :)
    char          temp_last_ship_name[50];         // name of last first player's ship... (for hiscores)
    int           temp_minigame_2player;           // 2 player mode activated?


    level_info    level_files[MAX_LEVELS];                // array containing info about level files
    int           selected_level;                  // selected level (num in level_files[])
    char          last_level[30];                  // last level played (twoplayer)
    char          last_leveltype[2];               // type of last level played
    char          last_splevel[30];                // last single player level played
    char          last_minigame[30];               // last minigame level
    int           max_levels;                      // how much level files?

    // functions
    int  make_color(int r, int g, int b);          // (for scripts)
    void make_standard_colors();                   // make color-constants
    void make_color_tables();                      // fills tbl_ arrays
    void save_config_file();                       // saves script file with current globals
    int  get_level_num_from_filename(char *n);     // see left :-)
    void delete_level_temps();                     // delete function before level start
    void set_highscore(char *n)                    { strncpy(score_string, n, 49); }
    void set_highscore(int n)                      { sprintf(score_string, "%d", n); }
    void create_level_done_file();                 // saves lvlname.fin file for current level & player
    int  check_level_done_file(char *lvlname, char *plname); // checks the file with given level file and player name
    
    // script-changable values
    float         gravity_percent;                 // glob gravity in percent (-1 to +2)
    float         level_gravity;                   // global gravity
    float         particle_gravity;                // gravity for particles
    float         bomb_gravity;                    // gravity for bombs
    int           exit_level;                      // exit current level
    int           max_craters;                     // maximum craters allowed
    int           language;                        // language to use (defined in gslng.h)
    int           plus_x;                          // increm. of map
    int           plus_y;                          // - " -
    int           pixamount;                       // amount of pixels to display (0-200)
    int           soundvol;                        // volume of sound
    int           musicvol;                        // volume of menu music
    int           race_laps;                       // race laps
    int           shoot_speed;                     // speed of normal bullets
    int           shoot_speed_bomb;                // speed of bombs
    int           player_respawn;                  // time for players to respawn (frames)
    int           player_indestructible;           // time players are indestructible after respawn
    int           extra_amount;                    // how much extras? (1 to 10)
    int           max_frags;                       // frags at which game ends?
    int           max_time;                        // minutes game lasts?
    int           do_all_hooks;                    // do all hook functions (objects & enemies?)
    int           computer_player;                 // activate computer player?
    int           computer_player_strength;        // strength of computer player (0-10)
    int           use_user_stats;
    UserStats     user_stats[10];                  // 10 rows of user stats
    int           score_per_second;                // minus score per second (default: 5)
    
};



extern gsGlobals *globals;

#endif

