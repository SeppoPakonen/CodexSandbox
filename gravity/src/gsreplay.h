/*
 *   GRAVITY STRIKE -- gsreplay.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSREPLAY_
#define _GSREPLAY_

#include <string.h>

enum PLAYER_ACTIONS {
  RPL_LEFT,
  RPL_RIGHT,
  RPL_THRUST,
  RPL_SHOOT,
  RPL_W1,
  RPL_W2,
  RPL_W3,
  RPL_W4,
  RPL_W5,
  RPL_W6,
  RPL_NW,
  RPL_PW,
  RPL_EXTRA,
  RPL_SPECIAL
};

typedef struct REPLAY_HEADER {
  int players;
  char map[30];
  char playership[6][30];
  char playername[6][30];
  int year, month, day, hour, minute;
  int temp1, temp2, temp3, temp4, temp5;
  char tempc1[50], tempc2[50], tempc3[50];
} REPLAY_HEADER;

typedef struct REPLAY_DATA_OLD {
  long int tick;
  float x;
  float y;
  float xspd;
  float yspd;
  int head;
  char left;
  char right;
  char thrust;
  char shoot;
} REPLAY_DATA_OLD;  

typedef struct REPLAY_DATA {
  long int tick;
  float x;
  float y;
  float xspd;
  float yspd;
  int head;
  unsigned int left     : 1;
  unsigned int right    : 1;
  unsigned int shoot    : 1;
  unsigned int thrust   : 1;
  unsigned int next_weapon : 1;
  unsigned int prev_weapon : 1;
  unsigned int weapon_1 : 1;
  unsigned int weapon_2 : 1;
  unsigned int weapon_3 : 1;
  unsigned int weapon_4 : 1;
  unsigned int weapon_5 : 1;
  unsigned int weapon_6 : 1;
  unsigned int extra_action : 1;
  unsigned int special : 1;
} REPLAY_DATA;

class gsReplay
{
  private:
    // Variables
    PACKFILE *replay_file;
    char filename[20];
    REPLAY_HEADER current_header;
    long int record_count;
    int tick;
    REPLAY_DATA *current_data;
    
    // functions

  public:
    // constructor, destructor
    gsReplay();
    ~gsReplay();

    // variables

    // get/put
    char *get_filename()                       { return filename; }

    // functions
    void create_new_file(char *f=NULL);
    int load_file(char *);

    void start_recording();
    void close_file();

    void write_position_data();
    void write_player_action(int player, PLAYER_ACTIONS action);

    void read_data();
};

extern gsReplay *replay;

#endif

