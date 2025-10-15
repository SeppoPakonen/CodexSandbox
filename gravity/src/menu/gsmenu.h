/*
 *   GRAVITY STRIKE -- menu/gsmenu.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSMENU_
#define _GSMENU_

enum MENU_TYPES {
  MENU_QUIT = 1,
  MENU_MAIN,
  MENU_OPTIONS,
  MENU_CONTROLS,
  MENU_2PCONTROLS,
  MENU_SPCONTROLS,
  MENU_SINGLEPLAYER,
  MENU_MINIGAME,
  MENU_SPLITSCREEN,
  MENU_NETGAMEINI,
  MENU_NETGAME,
  MENU_CHOOSESHIP,
  MENU_SHIPBUILDER,
  MENU_1PSTATISTICS,
  MENU_2PSTATISTICS,
  MENU_NETSTATISTICS,
  START_GAME,
  START_NETGAME
};

class gsMenu
{
  private:
    int actual_time;                               // some variables for main loop
    int endthis;
    int esc_pressed;
    int update_graphics;
    int last_second;
    int exit_mode;                                 // 0 for quit, 1 for start game
    int crosshair_count;                           // animation frame of mouse cross

    char   error_str[100];                         // string with error message
    int    current_menu;                           // current menu we are in
    class  mStars   *stars;                        // background stars

    // menus
    class  mMain    *menu_main;                    // main menu
    class  mOptions *menu_options;                 // options menu
    class  mPControls *menu_controls;              // normal controls
    class  mPControls2P *menu_2pcontrols;          // 2player controls
    class  mPControlsSP *menu_spcontrols;          // special controls
    class  mChooseShip *menu_chooseship;           // Choose ship
    class  mShipBuilder *menu_shipbuilder;         // Ship Builder
    class  mOnePlayer *menu_oneplayer;             // 1player match
    class  mTwoPlayer *menu_twoplayer;             // 2player match
    class  mMiniGame *menu_minigame;               // minigame
    class  mNetGameInit *menu_netgameini;          // network match init
    class  mNetGame *menu_netgame;                 // network match
    class  m1PStats *menu_1pstats;                 // 1player statistics
    class  m2PStats *menu_2pstats;                 // 2player statistics
    class  mNetStats *menu_netstats;               // network statistics

    // functions
    void show_player_stats();                      // show player frags... :)
    void update_mouse_crosshair();                 // updates crosshair anim of mouse

  public:
    // constr. / destr.
    gsMenu();
    ~gsMenu();

    // functions
    int  init(int n);                              // initializes menu
    int  do_it();                                  // main menu function
    void do_graphics(int=0);                       // draw graphics
    void do_logic(int=0);                          // do menu logic
    void exit_menu();                              // exits menu
};

extern gsMenu *menu;

#endif
