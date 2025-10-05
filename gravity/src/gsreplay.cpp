/*
 *   GRAVITY STRIKE -- gsreplay.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsreplay.h"
#include "gsglob.h"
#include "gsplayer.h"
#include "gserror.h"
#include "gscontrl.h"
#include "gsmap.h"

gsReplay::gsReplay()
{
  strcpy(filename, "");
  replay_file = NULL;
  memset(&current_header, 0, sizeof(REPLAY_HEADER));
}

gsReplay::~gsReplay()
{
  if (replay_file) pack_fclose(replay_file);
}

void gsReplay::create_new_file(char *f)
{
  char myfname[30];
  strcpy(myfname, "test.rpl");

  if (f) strncpy(myfname, f, 29);

  replay_file = pack_fopen(myfname, "wb");
  tick = 0;
  
  current_data = NULL;
}

int gsReplay::load_file(char *f)
{
  // get file size
  int fsize = file_size(f) - sizeof(REPLAY_HEADER);

  replay_file = pack_fopen(f, "rb");
  if (!replay_file) return 1;

  // read header
  pack_fread(&current_header, sizeof(REPLAY_HEADER), replay_file);
  tick = 0;
  
  // get record count
  record_count = fsize / sizeof(REPLAY_DATA);

  // allocate memory
  current_data = new REPLAY_DATA[record_count];
   
  // read data
  pack_fread(current_data, sizeof(REPLAY_DATA) * record_count, replay_file);

  pack_fclose(replay_file);
  replay_file = NULL;
 
  return 0;
}

void gsReplay::start_recording()
{
  memset(&current_header, 0, sizeof(REPLAY_HEADER));

  current_header.players = globals->players;
  strcpy(current_header.map, globals->level_files[globals->selected_level].levelfile);
  for (int n=0; n < globals->players; n++)
  {
    strncpy(current_header.playership[n], player[n]->get_shipfile_filename(), 29);
    strncpy(current_header.playername[n], player[n]->get_name(), 29);
  }

  pack_fwrite(&current_header, sizeof(REPLAY_HEADER), replay_file);
}

void gsReplay::close_file()
{
  if (current_data) { delete[] current_data; current_data = NULL; }
  
  if (replay_file) pack_fclose(replay_file);
  replay_file = NULL;
}

void gsReplay::write_position_data()
{
  int n;
  REPLAY_DATA dat;

  for (n=0; n < globals->players; n++)
  {
    dat.tick = globals->game_time;
    dat.x = player[n]->get_x();
    dat.y = player[n]->get_y();
    dat.xspd = player[n]->get_xspd();
    dat.yspd = player[n]->get_yspd();
    dat.head = (int)player[n]->get_head();

    dat.left   = player[n]->controls->get_input(LEFT, 0) ? 1 : 0;
    dat.right  = player[n]->controls->get_input(RIGHT, 0) ? 1 : 0;
    dat.thrust = player[n]->controls->get_input(THRUST, 0) ? 1 : 0;
    dat.shoot  = player[n]->controls->get_input(SHOOT, 0) ? 1 : 0;
    dat.next_weapon = player[n]->controls->get_input(NEXT_WEAPON, 0) ? 1 : 0;
    dat.prev_weapon = player[n]->controls->get_input(PREV_WEAPON, 0) ? 1 : 0;
    dat.weapon_1 = player[n]->controls->get_input(WEAPON_1, 0) ? 1 : 0;
    dat.weapon_2 = player[n]->controls->get_input(WEAPON_2, 0) ? 1 : 0;
    dat.weapon_3 = player[n]->controls->get_input(WEAPON_3, 0) ? 1 : 0;
    dat.weapon_4 = player[n]->controls->get_input(WEAPON_4, 0) ? 1 : 0;
    dat.weapon_5 = player[n]->controls->get_input(WEAPON_5, 0) ? 1 : 0;
    dat.weapon_6 = player[n]->controls->get_input(WEAPON_6, 0) ? 1 : 0;
    dat.extra_action = player[n]->controls->get_input(EXTRA_ACTION, 0) ? 1 : 0;
    dat.special = player[n]->controls->get_input(SPECIAL, 0) ? 1 : 0;
    
    pack_fwrite(&dat, sizeof(REPLAY_DATA), replay_file);
  }
}

void gsReplay::write_player_action(int player, PLAYER_ACTIONS action)
{
}

void gsReplay::read_data()
{
  if (tick < record_count)
  for (int n=0; n < globals->players; n++)
  {
/*    if (globals->game_time % 120 == 0)
    {
//  errors->log(2, "replay", globals->game_time, current_data[tick].tick);
      player[n]->set_x(current_data[tick].x);
      player[n]->set_y(current_data[tick].y);
      player[n]->set_xspd(current_data[tick].xspd);
      player[n]->set_yspd(current_data[tick].yspd);
      player[n]->set_head(current_data[tick].head);
    }
    */

    player[n]->controls->set_input(LEFT, current_data[tick].left);
    player[n]->controls->set_input(RIGHT, current_data[tick].right);
    player[n]->controls->set_input(THRUST, current_data[tick].thrust);
    player[n]->controls->set_input(SHOOT, current_data[tick].shoot);
    player[n]->controls->set_input(NEXT_WEAPON, current_data[tick].next_weapon);
    player[n]->controls->set_input(PREV_WEAPON, current_data[tick].prev_weapon);
    player[n]->controls->set_input(WEAPON_1, current_data[tick].weapon_1);
    player[n]->controls->set_input(WEAPON_2, current_data[tick].weapon_2);
    player[n]->controls->set_input(WEAPON_3, current_data[tick].weapon_3);
    player[n]->controls->set_input(WEAPON_4, current_data[tick].weapon_4);
    player[n]->controls->set_input(WEAPON_5, current_data[tick].weapon_5);
    player[n]->controls->set_input(WEAPON_6, current_data[tick].weapon_6);
    player[n]->controls->set_input(EXTRA_ACTION, current_data[tick].extra_action);
    player[n]->controls->set_input(SPECIAL, current_data[tick].special);
    
    tick++;
  } // for players
}

