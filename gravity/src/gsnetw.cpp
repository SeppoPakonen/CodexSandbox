/*
 *   GRAVITY STRIKE -- gsnetw.cpp
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsnetw.h"
#include "gserror.h"
#include "gsglob.h"
#include "gshelper.h"
#include "gsplayer.h"
#include "gscontrl.h"
#include "gsobject.h"
#include "gseffect.h"
#include "gscrater.h"
#include "gsenemy.h"
#include "gsmap.h"
#include "gsbase.h"
#include "gslng.h"
#include "network/StringCompressor.h"
#include "network/GetTime.h"
#include "network/UtilityFunctions.h"

gsChat::gsChat()
{
  for (int n=0; n < MAX_CHAT_MESSAGES; n++)
    memset(&message[n], 0, sizeof(ChatMessage));

  count = 0;
}

gsChat::~gsChat()
{
  clear();
}

void gsChat::add(char *msg, char *player, int col)
{
  if (col == 0) col = globals->col_white;
  if (message[count].message) delete[] message[count].message;
  message[count].message = new char[strlen(msg)+1];
  strcpy(message[count].message, msg);
  strncpy(message[count].player, player, 19);
  message[count].color = col;

  count++;

  // overflow: delete first and push back
  if (count == MAX_CHAT_MESSAGES)
  {
    delete[] message[0].message;

    for (int n=1; n < MAX_CHAT_MESSAGES; n++)
    {
      message[n-1] = message[n];
    }
    message[MAX_CHAT_MESSAGES-1].message = NULL;
    count = MAX_CHAT_MESSAGES - 1;
  }
}

char *gsChat::get_message(int num)
{
  if (message[num].message) return message[num].message;
  return "n/a";
}

char *gsChat::get_player(int num)
{
  if (message[num].player) return message[num].player;
  return "n/a";
}

char *gsChat::get_message_with_playername(int num, char *mymsg)
{
  if (message[num].message)
  {
    sprintf(mymsg, "%s: %s", message[num].player, message[num].message);
    return mymsg;
  }
  return "n/a";
}

int gsChat::get_color(int num)
{
  return message[num].color;
}

void gsChat::clear()
{
  for (int n=0; n < MAX_CHAT_MESSAGES; n++)
  {
    if (message[n].message) delete[] message[n].message;
    memset(&message[n], 0, sizeof(ChatMessage));
  }

  count = 0;
}

int gsChat::process_commands(char *msg)
{
  char tmp[1000];

  if (msg[0] == '*')
  {
    if (msg[1] == '*')
    {
      strncpy(tmp, msg, 1000);
      strrepl(tmp, 1000, "** ", "");
      script->do_string(tmp);
    }

    return 1;
  }

  return 0;
}

gsNetwork::gsNetwork()
{
  errors->log(2, "network", "initialization");
  mode = -1;

  for (int n=0; n < MAX_NET_PLAYERS; n++)
    netplayer[n] = NULL;

  netplayers = 1;
  reset_status_vars();
}

gsNetwork::~gsNetwork()
{
  reset();
}

int gsNetwork::init(int mymode)
{
  int ret;

  if (mymode == NET_CLIENT)
  {
    mode = NET_CLIENT;

    client = new gsNetClient();

    ret = client->connect_to_server(globals->server_ip);
    //ret = client->connect_to_server("80.139.96.120");

    return ret;
  }
  else if (mymode == NET_SERVER)
  {
    mode = NET_SERVER;
    
    server = new gsNetServer();
    ret = server->start();

    create_local_player(globals->pplayer[0].pname, globals->pplayer[0].pship);
    netplayer[0]->ready = true;
    return ret;
  }
   
  return 0;
}

void gsNetwork::reset()
{
  for (int n=0; n < MAX_NET_PLAYERS; n++)
  {
    if (netplayer[n]) delete netplayer[n]; 
    netplayer[n] = NULL;
  }

  if (mode == NET_CLIENT)
  {
    delete client;
  }
  else if (mode == NET_SERVER)
  {
    delete server;
  }

  errors->log(2, "network", "reset");
  
  mode = -1;
  netplayers = 1;
  reset_status_vars();
}

void gsNetwork::reset_status_vars()
{
  for (int n=0; n < MAX_NET_PLAYERS; n++)
    if (netplayer[n])
    {
      netplayer[n]->ready = false;
      netplayer[n]->in_game = false;
  //    if (is_server() && n == 0) netplayer[n]->ready = true; // server is always ready
      memset(&netplayer[n]->player_stats, 0, sizeof(statistics));
      last_timestamp[n] = 0;
    }

  set_position_set(false);
  set_quit_game(false);
  set_start_game(false);
  set_server_in_game(false);
}

int gsNetwork::client_check_connection_attempt()
{
  if (is_server()) return -1;

  if (is_client())
    return client->check_connection_attempt();

  return -1;
}

void gsNetwork::process_packets()
{
  if (is_server()) server->process_packets();
  else if (is_client()) client->process_packets();
}

void gsNetwork::create_local_player(char *name, char *ship)
{
  PlayerID id;
  //memset(&id, 99, sizeof(PlayerID));
  id.binaryAddress = SERVER_BIN_ID;
  id.port = 0;

  if (is_client()) id = client->interface->GetPlayerID();

  if (netplayer[0]) delete netplayer[0];
  netplayer[0] = new gsNetPlayer(name, ship, id);
}

void gsNetwork::write_string_to_bitstream(char *myString, BitStream *output)
{
  StringCompressor::Instance()->EncodeString(myString, 256, output);
}

void gsNetwork::write_bitstream_to_string(char *myString, BitStream *input)
{
  StringCompressor::Instance()->DecodeString(myString, 256, input);
}

void gsNetwork::create_timestamped_bitstream_packet(BitStream *bs)
{
  unsigned long timeStamp;

  timeStamp = RakNetGetTime();

  bs->Write((unsigned char)ID_TIMESTAMP);
  bs->Write(timeStamp);
}

int gsNetwork::process_timestamped_bitstream_packet(BitStream *bs, unsigned long *timestamp)
{
  unsigned long timeStamp;
  unsigned char tempid;

  // ID_TIMESTAMP
  bs->Read(tempid);
  if (tempid != ID_TIMESTAMP) return -1;

  // timestamp
  bs->Read(timeStamp);

  if (timestamp) *timestamp = timeStamp;

  // return delay
  return RakNetGetTime() - timeStamp;
}

void gsNetwork::add_random_chat_message(int msgnum, char *repl1, char *repl2, char *system, int maxmsg, int col)
{
  char mymsg[500];
  
  int mymsgnum = rand() % maxmsg; 
  int cnt=6;
  while (strlen(language->get_string(TEXT_STRINGS(msgnum+mymsgnum))) <= 4 && --cnt > 0)
    mymsgnum = rand() % maxmsg;

  if (cnt <= 0) mymsgnum = 0;
  strncpy(mymsg, language->get_string(TEXT_STRINGS(msgnum+mymsgnum)), 249);

  strrepl(mymsg, 500, "%1%", repl1);
  if (repl2 != "") strrepl(mymsg, 500, "%2%", repl2);

  chat.add(mymsg, system, col);
}

void gsNetwork::add_frag_message(char *playername, char *killername, int weapon)
{
  int msgnum;
  int maxmsg = 5;

  switch (weapon)
  {
    case -1          : msgnum = T_NET_FRAGMSG_SUICIDE_1; break;
    case -2          : msgnum = T_NET_FRAGMSG_PLSUICIDE_1; break;
    case -3          : msgnum = T_NET_FRAGMSG_QUIT_1; break;
    case W_SINGLE    : msgnum = T_NET_FRAGMSG_SINGLE_BLASTER_1; break;
    case W_DOUBLE    : msgnum = T_NET_FRAGMSG_DOUBLE_BLASTER_1; break;
    case W_BOMB      : msgnum = T_NET_FRAGMSG_BOMB_1; break;
    case W_BIGBOMB   : msgnum = T_NET_FRAGMSG_BIGBOMB_1; break;
    case W_ROCKET    : msgnum = T_NET_FRAGMSG_ROCKET_1; break;
    case W_MINE      : msgnum = T_NET_FRAGMSG_MINE_1; break;
    case W_SHRINKER  : msgnum = T_NET_FRAGMSG_SHRINKER_1; break;
    case W_PROBE     : msgnum = T_NET_FRAGMSG_PROBE_1; break;
    case W_GAS       : msgnum = T_NET_FRAGMSG_GAS_1; break;
    case W_BUMPER    : msgnum = T_NET_FRAGMSG_BUMPER_1; break;
    case W_SHOTGUN   : msgnum = T_NET_FRAGMSG_SHOTGUN_1; break;
    case W_ACID      : msgnum = T_NET_FRAGMSG_ACID_1; break;
    default          : return;
  }

  char mykillername[50];
  char myplayername[50];

  sprintf(mykillername, "[g]%s[/]", killername);
  sprintf(myplayername, "[o]%s[/]", playername);

  add_random_chat_message(msgnum, mykillername, myplayername, "FRAG", 5, globals->col_white);
}


void gsNetwork::create_shipfile_packet(BitStream *p)
{
  PlayerID tempid;
//  memset(&tempid, 0, sizeof(PlayerID));
  tempid.binaryAddress = SERVER_BIN_ID;
  tempid.port = 0;

  if (is_client()) tempid = client->get_our_id();

  create_shipfile_packet(p, tempid);
}


void gsNetwork::create_shipfile_packet(BitStream *p, PlayerID player_id)
{ 
  // structure
  // ID_MENU_SHIPFILE (unsigned char)
  // net_id (PlayerID)
  // name of ship image
  // sizeof SHP file (int)
  // sizeof pic file (int)
  // SHP file
  // pic file (if needed)

  gsNetPlayer *myplayer = get_netplayer_by_id(player_id);  
  if (!myplayer)
  {
    errors->log(1, "create_shipfile error", "netplayer with this id does not exist!", player_id.binaryAddress);
    p = NULL;
    return;
  }

  // write ID
  p->Write((unsigned char)ID_MENU_SHIPFILE);
  
  // write PlayerID
  p->Write((char*)&myplayer->net_id, sizeof(PlayerID));

  // write ship image filename
  write_string_to_bitstream(myplayer->player->get_shipimage_filename(), p);

  // paths
  char shippath[40], shipimgpath[40];
  strcpy(shippath, globals->ship_dir);
  strcat(shippath, myplayer->ship_file);

  int shipimg = 0;
  if (myplayer->player->get_ship_type() == SHIP_CUSTOM) shipimg = 1;

  if (shipimg)
  {
    strcpy(shipimgpath, globals->ship_dir);
    strcat(shipimgpath, myplayer->player->get_shipimage_filename());
  }

  // packed ship?
  int packed_ship = 0;
  if (strchr(shippath, '#'))
  {
    char temppath[40];
    char tempfile[20];
    strcpy(tempfile, strchr(shippath, '#')+1);

    if (strstr(tempfile, "def1_shp")) packed_ship = 1069;
    else if (strstr(tempfile, "def2_shp")) packed_ship = 1005;
    else if (strstr(tempfile, "def3_shp")) packed_ship = 1029;

    sprintf(temppath, "%s%s#%s", globals->data_dir, globals->shipdata_name, tempfile);
    //errors->log(2, "new ship path", temppath);
    strcpy(shippath, temppath);
  }


//errors->log(2, "myplayer shipimgpath", shipimgpath);
//errors->log(2, "myplayer shippath", shippath);

  // do it!
  if ((exists(shippath) || packed_ship) && (!shipimg || exists(shipimgpath)))
  {
    int shipsize = (packed_ship == 0) ? file_size(shippath) : packed_ship;
    int shipimgsize = shipimg ? file_size(shipimgpath) : 0;

//errors->log(2, "sending ship", shippath, shipsize);

    // write SHP size
    p->Write(shipsize);

    // write SHPIMG size
    p->Write(shipimgsize);

    // write ship file
    char *shipbuf = new char[shipsize];
    if (packed_ship)
    {
      packfile_password(globals->pwd);
      PACKFILE *shp = pack_fopen(shippath, "rp");
      pack_fread(shipbuf, sizeof(char), shipsize, shp);
      pack_fclose(shp);
      packfile_password(NULL);
    }
    else
    {
      FILE *shp = fopen(shippath, "rb");
      fread(shipbuf, sizeof(char), shipsize, shp);
      fclose(shp);
    }

    p->Write(shipbuf, shipsize);
    delete[] shipbuf;

    
    // write shipimg file
    char *shipimgbuf;
    if (shipimg)
    {
      shipimgbuf = new char[shipimgsize];

      FILE *shp = fopen(shipimgpath, "rb");
      fread(shipimgbuf, sizeof(char), shipimgsize, shp);
      fclose(shp);

      p->Write(shipimgbuf, shipimgsize);
      delete[] shipimgbuf;
    }
  }
}


void gsNetwork::receive_shipfile(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  // structure
  // ID_MENU_SHIPFILE (unsigned char)
  // net_id (PlayerID)
  // name of shipimg file
  // sizeof SHP file (int)
  // sizeof pic file (int)
  // SHP file
  // pic file (if needed)

  // read ID
  p.Read(typeId);

  // read Player ID
  PlayerID player_id;
  p.Read((char*)&player_id, sizeof(PlayerID));

  // read ship image filename
  char shipimg_filename[40];
  write_bitstream_to_string(&shipimg_filename[0], &p);

  // read shipsize
  int shipsize;
  p.Read(shipsize);

  // read shipimgsize
  int shipimgsize;
  p.Read(shipimgsize);

  // read ship data
  char *shipbuf = new char[shipsize + 20 /* for filename change, just 2 be sure */];
  memset(shipbuf, 0, shipsize+20);
  p.Read(shipbuf, shipsize);

  // read shipimg data (if needed)
  char *shipimgbuf;
  if (shipimgsize > 0)
  {
    shipimgbuf = new char[shipimgsize];
    p.Read(shipimgbuf, shipimgsize);
  }
    

  // create temporary ship with buffered data
  char tempship_path[40];
  sprintf(tempship_path, "%stmp%d.shp", globals->ship_dir, player_id.binaryAddress / 10000);

  // change ship image filename in ship buffer
  int len_oldname = strlen(shipimg_filename);
  int len_newname = len_oldname;
  if (shipimgsize > 0)
  {
    char newstr[50];
    sprintf(newstr, "tmp%lu.%s", player_id.binaryAddress / 10000, get_extension(shipimg_filename));
    len_newname = strlen(newstr);
    char *ret = strrepl(shipbuf, shipsize+20, shipimg_filename, newstr);
//    errors->log(2, "receive ship replace", shipimg_filename, newstr);
    if (!ret) errors->log(2, "receive ship replace", "ERROR strrepl bailed out!");
  }

  // write ship file
  FILE *tf = fopen(tempship_path, "wb");
  fwrite(shipbuf, sizeof(char), shipsize+len_newname-len_oldname, tf);
  fclose(tf);
  delete[] shipbuf;

  // write ship image file
  if (shipimgsize > 0)
  {
    char tempp[50];
    sprintf(tempp, "%stmp%d.%s", globals->ship_dir, player_id.binaryAddress / 10000, strlwr(get_extension(shipimg_filename)));
    FILE *tf = fopen(tempp, "wb");
    fwrite(shipimgbuf, sizeof(char), shipimgsize, tf);
    fclose(tf);
    delete[] shipimgbuf;
  }

  errors->log(2, "ship file received", tempship_path, shipsize);
  if (shipimgsize) errors->log(2, "ship image received", shipimg_filename, shipimgsize);

  
  // update netplayer with new data
  gsNetPlayer *myplayer = get_netplayer_by_id(player_id);

  if (!myplayer)
  {
    errors->log(1, "receive_shipfile error", "netplayer with this id does not exist!", player_id.binaryAddress);
    return;
  }

  sprintf(myplayer->ship_file, "tmp%d.shp", player_id.binaryAddress /  10000);
  if (myplayer->player)
  {
    myplayer->player->load_ship(tempship_path);
    myplayer->player->set_network(1);
    strncpy(myplayer->shipimg_file, myplayer->player->get_shipimage_filename(), 50);
  }

  myplayer->network = true;


  // chat msg
  char jointext[200];
  sprintf(jointext, "%s %s", myplayer->name, language->get_string(T_NET_JOIN_LOBBY));
  net->chat.add(jointext, "SYSTEM", globals->col_white);
}

/* gsNetPlayer class */
gsNetPlayer::gsNetPlayer(char *pname, char *pship, PlayerID pnet_id)
{
  errors->log(2, "gsNetPlayer created", pship, pnet_id.binaryAddress);
  strncpy(name, pname, 49);

  strcpy(ship_file, "");
  strcpy(shipimg_file, "");

  // create the player
  if (pship) strncpy(ship_file, pship, 49);
  player = new gsPlayer(0, SHIP_PIRANHA, 0, pship);
  player->set_name(name);
  player->set_sig(pnet_id.binaryAddress / 10000);

  net_id = pnet_id;
  ready = false;
  in_game = false;
  network = false;

  memset(&player_stats, 0, sizeof(statistics));
}

gsNetPlayer::~gsNetPlayer()
{
  errors->log(2, "gsNetPlayer", "deleted", net_id.binaryAddress);

  delete_shipfiles();
  
  if (in_game)
    net->delete_player_from_game(net_id);

  if (player)
    delete player;
}

void gsNetPlayer::delete_shipfiles()
{
  if (network)
  {
    // delete temporary player ship
    char shippath[255];
    char shipimgpath[255];

    sprintf(shippath, "%s%s", globals->ship_dir, ship_file);
    sprintf(shipimgpath, "%s%s", globals->ship_dir, shipimg_file);

    errors->log(2, "network", "delete player ship", shippath);
    errors->log(2, "network", "delete player ship image", shipimgpath);

    if (exists(shippath))
      delete_file(shippath);

    if (exists(shipimgpath))
      delete_file(shipimgpath);
  }
}

gsNetPlayer* gsNetwork::menu_create_netplayer(char *pname, char *pship, PlayerID net_id)
{
  int n;

  // check if player already exists
  for (n=1; n < MAX_NET_PLAYERS; n++)
  {
    if (netplayer[n] && netplayer[n]->net_id == net_id)
    {
      errors->log(2, "gsNetPlayer", "player still exists!", net_id.binaryAddress);
      return NULL;
    }
  }

  for (n=1; n < MAX_NET_PLAYERS; n++)
  {
    if (!netplayer[n])
    {
      errors->log(2, "gsNetPlayer", "adding at position", n);
      netplayer[n] = new gsNetPlayer(pname, /*pship*/ NULL, net_id);
      netplayers++;
      return netplayer[n];
    }
  }

  return NULL;
}

int gsNetwork::menu_delete_netplayer(PlayerID net_id)
{
  for (int n=1; n < MAX_NET_PLAYERS; n++)
  {
    if (netplayer[n] && netplayer[n]->net_id == net_id)
    {
      // chat msg
      char leavetext[200];
      sprintf(leavetext, "%s %s", netplayer[n]->name, language->get_string(T_NET_LEAVE_LOBBY));
      net->chat.add(leavetext, "SYSTEM", globals->col_white);

      errors->log(2, "gsNetPlayer", "deleting from position", n);
      delete netplayer[n]; netplayer[n] = NULL;
      reorganize_netplayer_array(n);
      netplayers--;
      return 0;
    }
  }

  return -1;
}

void gsNetwork::reorganize_netplayer_array(int n)
{
  int o;
  for (o=n+1; o < MAX_NET_PLAYERS; o++)
  {
    netplayer[o-1] = netplayer[o];
    netplayer[o] = NULL;
  }
}

void gsNetwork::reorganize_player_array(int n)
{
  int o;
  for (o=n+1; o < MAX_NET_PLAYERS; o++)
  {
    player[o-1] = player[o];
    player[o] = NULL;
  }
}


bool gsNetwork::get_all_players_ready()
{
  for (int n=0; n < get_netplayers(); n++)
    if (!netplayer[n]->ready)
      return false;

  return true;
}


void gsNetwork::fill_home_locations()
{
  // fill array with base ids and coordinates
  for (int n=1; n < globals->level_files[globals->selected_level].max_players; n++)
  {
    // get base
    Base *mybase = bases->get_base(n);
    if (!mybase)
    {
      errors->log(2, "fill home locations", "error, base X does not exist!", n);
      homeloc[n].num = homeloc[n].x = homeloc[n].y = 0;
    }

    homeloc[n].num = n;
    player[0]->set_homebase(n); player[0]->return_home();
    homeloc[n].x = player[0]->get_x();
    homeloc[n].y = player[0]->get_y();
    homeloc[n].id = 0;
  }

  // server to base 0
  player[0]->set_homebase(0);
  player[0]->return_home();
  homeloc[0].num = 0;
  homeloc[0].x = player[0]->get_x();
  homeloc[0].y = player[0]->get_y();
  homeloc[0].id = SERVER_BIN_ID;
}


void gsNetwork::send_to_ingame_players(char *data, const long length, PacketPriority priority, PacketReliability reliability, char ordering_channel, PlayerID player_id)
{
  bool send_to_all = false;

  // broadcast to all if UNASSIGNED_PLAYER_ID
  if (player_id == UNASSIGNED_PLAYER_ID) send_to_all = true;

  // loop through all other netplayers who are in game
  for (int n=1; n < get_netplayers(); n++)
  {
    if (get_netplayer(n) && get_netplayer(n)->in_game)
    {
      if (get_netplayer(n)->net_id != player_id || send_to_all)
        server->interface->Send(data, length, priority, reliability, ordering_channel, get_netplayer(n)->net_id, false);
    }
  }
}

void gsNetwork::send_to_ingame_players(BitStream *data, PacketPriority priority, PacketReliability reliability, char ordering_channel, PlayerID player_id)
{
  bool send_to_all = false;

  // broadcast to all if UNASSIGNED_PLAYER_ID
  if (player_id == UNASSIGNED_PLAYER_ID) send_to_all = true;

  // loop through all other netplayers who are in game
  for (int n=1; n < get_netplayers(); n++)
  {
    if (get_netplayer(n) && get_netplayer(n)->in_game)
    {
      if (get_netplayer(n)->net_id != player_id || send_to_all)
        server->interface->Send(data, priority, reliability, ordering_channel, get_netplayer(n)->net_id, false);
    }
  }
}

void gsNetwork::add_player_to_game(PlayerID pid)
{
  gsNetPlayer *mypl = get_netplayer_by_id(pid);
  if (!mypl)
  {
    errors->log(1, "network", "ERROR add_player_to_game: player does not exist!", pid.binaryAddress / 10000);
    return;
  }

  mypl->in_game = true;

  globals->players++;
  int n = globals->players - 1;

  if (player[n]) delete player[n];
  player[n] = new gsPlayer(mypl->net_id.binaryAddress / 10000, 0, 0, mypl->ship_file, playmap[0]);
  player[n]->set_name(mypl->name);
  mypl->player = player[n];

  player[n]->controls->set_type(NETWORK_INPUT);
  player[n]->set_network(1);
  
  player[n]->set_status_mode(1);
  player[n]->show_status_set_map(playmap[0]);

  gpanel[0]->update_netplayers();

  // add message
  add_random_chat_message(T_NET_FRAGMSG_JOIN_1, mypl->name, "", "SYSTEM", 5, globals->col_white);

  errors->log(2, "network", "added player to current game", pid.binaryAddress / 10000);
}

void gsNetwork::delete_player_from_game(PlayerID pid)
{
  gsNetPlayer *mypl = get_netplayer_by_id(pid);

  if (!mypl)
  {
    errors->log(1, "network", "ERROR delete_player_from_game: Player ID is not valid!", pid.binaryAddress / 10000);
    return;
  }

  // set in game to false
  mypl->in_game = false;
  
  // set player[x] to NULL
  int num = get_player_nr_by_sig(pid.binaryAddress / 10000);
  if (num < 0)
  {
    errors->log(1, "network", "ERROR delete_player_from_game: No player[x] matches signature", pid.binaryAddress / 10000);
    return;
  }
  player[num] = NULL;

  // delete player structure
  delete mypl->player;
  mypl->player = NULL;

  // decrease player count
  globals->players--;

  // reorganize
  reorganize_player_array(num);

  // update panel
  gpanel[0]->update_netplayers();

  // reset filled homebase (server)
  if (is_server())
  {
    for (int n=0; n < MAX_NET_PLAYERS; n++)
    {
      HomeLocation *myloc = net->get_home_location(n);
      if (myloc->id == pid.binaryAddress / 10000) myloc->id = 0;
    }
  }

  // add message
  add_random_chat_message(T_NET_FRAGMSG_QUIT_1, mypl->name, "", "SYSTEM", 5, globals->col_white);

  errors->log(2, "network", "deleted player from current running game", pid.binaryAddress / 10000);
}

void gsNetwork::game_send_player_position()
{
  gsNetPlayer *mypl = get_netplayer(0);
  BitStream p;
  create_timestamped_bitstream_packet(&p);

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_POSITION);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // heading
  p.Write((int)mypl->player->get_head());

  // if no movement (player landed or dead), just send one bit indicating our status
  // for syncing also send one packet every second
  //if (mypl->player->get_land() || mypl->player->get_dead())
  //if (mypl->player->get_xspd() == 0 && mypl->player->get_yspd() == 0)
  if (globals->game_time % 60 != 0 && (mypl->player->get_dead() /*|| mypl->player->get_land() */ || (mypl->player->get_xspd() == 0 && mypl->player->get_yspd() == 0)))
  {
    p.Write((bool)false);
  }
  else // send complete position update
  {
    p.Write((bool)true);
  
    // x position
    p.Write((float)mypl->player->get_x());

    // y position
    p.Write((float)mypl->player->get_y());

    // x speed
    p.Write((float)mypl->player->get_xspd());

    // y speed
    p.Write((float)mypl->player->get_yspd());
  }


  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE);
  }

}

void gsNetwork::game_process_player_position(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;
  unsigned long this_timestamp;

  int delay = process_timestamped_bitstream_packet(&p, &this_timestamp);
  
  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));
  
  // read heading
  int head;
  p.Read(head);

  // read movement status flag
  bool is_moving;
  p.Read(is_moving);

  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // update old timestamp
  int nr = 0;
  if (mypl && mypl->player)
  {
    nr = get_player_nr_by_sig(mypl->player->get_sig());
    if (last_timestamp[nr] > this_timestamp)
      errors->log(2, "process_player_position: last timestamp > this_timestamp", last_timestamp[nr], this_timestamp);
    last_timestamp[nr] = this_timestamp;
  }

  // read and apply position data
  float x,y;
  float newx, newy;
  float xspd=0, yspd=0;

  if (mypl->player)
    mypl->player->set_head(head);


  if (is_moving)
  {
    p.Read(x);
    p.Read(y);
    p.Read(xspd);
    p.Read(yspd);

    newx = x; newy = y;

    //errors->log(2, "net", "receive player position update", delay);
    
    // interpolation
    if (delay > 10000) delay = 0;
    if (delay > 0 && mypl->player)
    {
      float ox = mypl->player->get_x();
      float oy = mypl->player->get_y();
      float srvx, srvy;
      
      float delaymult = 60.0 / float(1000.0/(float)delay);
      srvx = x + xspd * delaymult;
      srvy = y + yspd * delaymult;

      // set server position if position differs too much
      if (distance(ox, oy, srvx, srvy) > 200)
      {
        newx = srvx;
        newy = srvy;
      }
      else
      {
        // blend client position and server position together (40/60)
        newx = (0.6 * srvx) + (0.4 * ox);
        newy = (0.6 * srvy) + (0.4 * oy);
      }
    }

    if (mypl->player)
    {
      mypl->player->set_x(newx);
      mypl->player->set_y(newy);
      mypl->player->set_xspd(xspd);
      mypl->player->set_yspd(yspd);
      mypl->player->set_land(0);
    }
  }
  

  // if server: relay packet to all other clients (if there are more than 2 netplayers)
  if (is_server() && get_netplayers() > 2)
  {
    BitStream srv;
    create_timestamped_bitstream_packet(&srv);
    srv.Write(typeId);
    srv.Write((char*)&player_id, sizeof(PlayerID));
    srv.Write(head);
    srv.Write(is_moving);
    if (is_moving)
    {
      srv.Write(newx);
      srv.Write(newy);
      srv.Write(xspd);
      srv.Write(yspd);
    }

    // do the relay
    //server->interface->Send(&srv, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, packet->playerId);
    //server->interface->Send((char*)packet->data, packet->length, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, packet->playerId, true);
  }
}


void gsNetwork::game_send_player_input(int key, bool state, bool reset)
{
  gsNetPlayer *mypl = get_netplayer(0);
  BitStream p;
  create_timestamped_bitstream_packet(&p);

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_INPUT);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // pressed key
  p.Write((unsigned char)key);

  // state of key
  p.Write((bool)state);

  // reset key (one-timer)
  p.Write((bool)reset);


  // Position update!
  // active weapon (test)
//  p.Write((char)mypl->player->get_active_weapon());

  // heading
  p.Write((int)mypl->player->get_head());

  // x position
  p.Write((float)mypl->player->get_x());

  // y position
  p.Write((float)mypl->player->get_y());

  // x speed
  p.Write((float)mypl->player->get_xspd());

  // y speed
  p.Write((float)mypl->player->get_yspd());

  //errors->log(2, "send key", key, reset);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE);
  }
}


void gsNetwork::game_process_player_input(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;
  unsigned long this_timestamp;

  int delay = process_timestamped_bitstream_packet(&p, &this_timestamp);

  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // read key
  unsigned char key;
  p.Read(key);

  // read key state
  bool state;
  p.Read(state);

  // reset key?
  bool reset;
  p.Read(reset);

  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // position
  int head;
  float x, y, xspd, yspd;
  float srvx, srvy;
  char active_weapon;
//  p.Read(active_weapon);
  p.Read(head);
  p.Read(x);
  p.Read(y);
  p.Read(xspd);
  p.Read(yspd);

//  mypl->player->set_active_weapon(active_weapon);

  srvx = x; srvy = y;

  // interpolation (only if no newer player_update packet arrived earlier)
  int nr = 0;
  if (mypl && mypl->player)
  {
    nr = get_player_nr_by_sig(mypl->player->get_sig());
    if (nr >= 0)
    if (this_timestamp < last_timestamp[nr])
    {
      errors->log(2, "pressed key / state", key, state);
      errors->log(2, "this_ts / last_ts", this_timestamp, last_timestamp[nr]);
    }
  }

  //if (this_timestamp > last_timestamp[nr] && delay > 0 && mypl->player)
  if (delay > 0)
  {
    float ox = mypl->player->get_x();
    float oy = mypl->player->get_y();
    float newx, newy;
   
    last_timestamp[nr] = this_timestamp;
    
    float delaymult = 60.0 / float(1000.0/(float)delay);
//   xspd = mypl->player->get_xspd();
//   yspd = mypl->player->get_yspd();
    srvx = x + xspd * delaymult;
    srvy = y + yspd * delaymult;

    // blend client position and server position together (40/60)
    //newx = (0.6 * srvx) + (0.4 * ox);
    //newy = (0.6 * srvy) + (0.4 * oy);

    newx = srvx;
    newy = srvy;

    mypl->player->set_head(head);
    mypl->player->set_x(newx);
    mypl->player->set_y(newy);
    mypl->player->set_xspd(xspd);
    mypl->player->set_yspd(yspd);
    if (xspd != 0 || yspd != 0) mypl->player->set_land(0);
  }

  //errors->log(2, "received key", key, mypl->net_id.binaryAddress / 10000);

  // process!
  if (mypl && mypl->player && mypl->player->controls)
  {
    if (state) 
      mypl->player->controls->add_to_pressed_keys(key, reset);
    else 
      mypl->player->controls->delete_from_pressed_keys(key);
  }
  else
    errors->log(2, "process_player_input: mypl does not exist! (should not happen)");

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
  {
    BitStream srv;
    create_timestamped_bitstream_packet(&srv);
    srv.Write(typeId);
    srv.Write((char*)&player_id, sizeof(PlayerID));
    srv.Write(key);
    srv.Write(state);
    srv.Write(reset);
    
    srv.Write(head);
    srv.Write(srvx);
    srv.Write(srvy);
    srv.Write(xspd);
    srv.Write(yspd);

    // do the relay
    //server->interface->Send(&srv, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE, packet->playerId, true);
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_PLAYER_UPDATE, packet->playerId);
  }

}

void gsNetwork::game_send_player_dead(long sig, int x, int y)
{
  gsNetPlayer *mypl = get_netplayer(get_player_nr_by_sig(sig));
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_DEAD);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // x and y position
  p.Write(x);
  p.Write(y);

  // hitby/killedby
  p.Write((int)mypl->player->get_killedby());
  p.Write((int)mypl->player->get_hitbyweap());

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_PLAYER_UPDATE);
  }

}


void gsNetwork::game_process_player_dead(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;

  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // read x and y position (death place)
  int x, y;
  p.Read(x);
  p.Read(y);

  // hitby
  int killedby;
  p.Read(killedby);
  int hitbyweap;
  p.Read(hitbyweap);

  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // process!
  if (mypl && mypl->player)
  {
    mypl->player->set_x(x);
    mypl->player->set_y(y);
    mypl->player->set_dead();
    mypl->player->set_killedby(killedby);
    mypl->player->set_hitbyweap(hitbyweap);
    
    if (killedby == -1)
    {
      mypl->player->inc_suicides();
    }
    else
    {
      for (int n=0; n < globals->players; n++)
        if (player[n]->get_sig() == killedby)
          player[n]->inc_frags();

      mypl->player->set_hitby(mypl->player->get_killedby());
    }
    
    errors->log(2, "sig/killedby", mypl->player->get_sig(), killedby);
    player[0]->get_ppanel()->update_netplayer_stats();
  }

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_PLAYER_UPDATE, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_PLAYER_UPDATE, packet->playerId);
}

void gsNetwork::game_send_extra(int extratype, int framespeed, int x, int y, int extrarand)
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_EXTRA);

  // extra type
  p.Write((char)extratype);

  // framespeed
  p.Write((char)framespeed);

  // position
  p.Write(x);
  p.Write(y);

  // extra random number (for processing of additional "random" settings like which weapon to upgrade)
  p.Write(extrarand);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS);
  }
}

void gsNetwork::game_process_extra(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  if (!objects) return;

  // read id
  p.Read(typeId);

  // extra type
  char extratype;
  p.Read(extratype);

  // framespeed
  char framespeed;
  p.Read(framespeed);

  // position
  int x, y;
  p.Read(x);
  p.Read(y);

  // extra random number
  int extrarand;
  p.Read(extrarand);

  // process! (see player->put_extra())
  Object *myob = objects->add_special(1000, SPOBJ_EXTRA, x, y, extratype, framespeed+1);
  myob->set_user2(extrarand);

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId);
}


void gsNetwork::game_send_player_damage(long plsig, int damage)
{
  BitStream p;
  gsNetPlayer *mypl = get_netplayer(get_player_nr_by_sig(plsig));

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_DAMAGE);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // kind of damage 
  p.Write((char)damage);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS);
  }
}

void gsNetwork::game_process_player_damage(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;

  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // damage
  char damage;
  p.Read(damage);

  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  if (mypl)
  {
    mypl->player->do_damage(damage);
  }

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId);
}



void gsNetwork::game_send_player_status()
{
  gsNetPlayer *mypl = get_netplayer(0);
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_STATUS);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // ammo and upgrade status of all weapons
  int n;
  for (n=0; n < 6; n++)
  {
    weaponslot *ws = mypl->player->get_weapon_slot(n);
    p.Write((int)ws->bullets);
    p.Write((char)ws->upgrade);
  }

  // hull and (wall)shield
  p.Write(mypl->player->get_hull());
  p.Write(mypl->player->get_shield());
  p.Write(mypl->player->get_wall_shield());

  // frags and suicides
  p.Write((char)mypl->player->get_frags());
  p.Write((char)mypl->player->get_suicides());

  // freezed or shrinked?
  if (mypl->player->get_freezed())
  {
    p.Write((bool)true);
    p.Write((int)mypl->player->get_freezed());
  }
  else
    p.Write((bool)false);

  if (mypl->player->get_shrinked())
  {
    p.Write((bool)true);
    p.Write((int)mypl->player->get_shrinked());
  }
  else
    p.Write((bool)false);


  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_STATISTICS);
  }

}

void gsNetwork::game_process_player_status(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;
  
  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));
  
  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // weapon bullets / upgrade status
  if (mypl && mypl->player)
  {
    int n;
    for (n=0; n < 6; n++)
    {
      weaponslot *ws = mypl->player->get_weapon_slot(n);
      int bullets; 
      char upgrade;
      int temp_bullets;
      char temp_upgrade;
      p.Read(temp_bullets);
      p.Read(temp_upgrade);
      bullets = temp_bullets;
      upgrade = temp_upgrade;

      ws->bullets = bullets;
      ws->upgrade = upgrade;
    }

    // hull and (wall)shield
    float hull, shield, wall_shield;
    p.Read(hull);
    p.Read(shield);
    p.Read(wall_shield);
    mypl->player->set_hull(hull);
    mypl->player->set_shield(shield);
    mypl->player->set_wall_shield(wall_shield);

    // frags & suicides
    char frags, suicides;
    p.Read(frags);
    p.Read(suicides);
    mypl->player->set_frags(frags);
    mypl->player->set_suicides(suicides);

    // freezed / shrinked
    bool freezed;
    p.Read(freezed);
    if (freezed)
    {
      int freezetime;
      p.Read(freezetime);
      mypl->player->set_freezed(freezetime);
    }

    bool shrinked;
    p.Read(shrinked);
    if (shrinked)
    {
      int shrinktime;
      p.Read(shrinktime);
      mypl->player->set_shrinked(shrinktime);
    }
  }

  // if server: relay packet to all other clients (if there are more than 2 netplayers)
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_STATISTICS, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_STATISTICS, packet->playerId);
}


void gsNetwork::game_send_crater(int xpos, int ypos, int size)
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_CRATER);

  // position
  p.Write(xpos);
  p.Write(ypos);

  // size
  p.Write(size);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS);
  }
}

void gsNetwork::game_process_crater(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  if (!effects->craters) return;

  // read id
  p.Read(typeId);

  // position
  int x, y;
  p.Read(x);
  p.Read(y);

  // size
  int size;
  p.Read(size);

  // process!
  effects->craters->add(x, y, size, 1, 0 /* 0 == dont send over network (again) */);

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, STREAM_STATISTICS, packet->playerId);
}

void gsNetwork::game_send_chat_message(char *msg)
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_CHAT_MESSAGE);

  // local player id
  gsNetPlayer *mypl = get_netplayer(0);
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // message
  write_string_to_bitstream(msg, &p);

  // server sends to all clients
  if (is_server())
  {
    server->interface->Send(&p, LOW_PRIORITY, RELIABLE_ORDERED, STREAM_DEFAULT, UNASSIGNED_PLAYER_ID, true);
    //send_to_ingame_players(&p, LOW_PRIORITY, RELIABLE_ORDERED, STREAM_DEFAULT, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, LOW_PRIORITY, RELIABLE_ORDERED, STREAM_DEFAULT);
  }
}

void gsNetwork::game_process_chat_message(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;

  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // message
  char msgbuf[260];
  write_bitstream_to_string(msgbuf, &p);

  // find color 
  int col = globals->col_white;

  for (int n=1; n < MAX_NET_PLAYERS; n++)
    if (netplayer[n] == mypl)
    {
      switch (n)
      {
        case 1:
          col = globals->col_yellow;
          break;
        case 2:
          col = globals->col_green;
          break;
        case 3:
          col = globals->col_red;
          break;
        case 4:
          col = globals->col_orange;
          break;
        case 5:
          col = globals->col_lblue;
          break;
        default:
          break;
      }
      break;
    }

  // process
  if (mypl)
    chat.add(msgbuf, mypl->name, col);

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    server->interface->Send((char*)packet->data, packet->length, LOW_PRIORITY, RELIABLE_ORDERED, STREAM_DEFAULT, packet->playerId, true);
    //send_to_ingame_players((char*)packet->data, packet->length, LOW_PRIORITY, RELIABLE_ORDERED, STREAM_DEFAULT, packet->playerId);
}



void gsNetwork::game_send_object_dead(int objsig, bool special)
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_OBJECT_DEAD);

  Object *myob;
  if (!special)
    myob = objects->get_object(objsig);
  else
    myob = objects->get_spobject(objsig);

  if (!myob) return;

  p.Write((int)objsig);
  p.Write((bool)special);

  //errors->log(2, "network", "game_send_object_dead", objsig);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE);
  }

}


void gsNetwork::game_process_object_dead(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  if (!objects) return;

  // read id
  p.Read(typeId);

  // read object's signature and if its a special object
  int objsig;
  bool special;
  p.Read(objsig);
  p.Read(special);

  Object *myob;
  if (!special)
    myob = objects->get_object(objsig);
  else
    myob = objects->get_spobject(objsig);

  if (!myob) return;

  myob->set_hit(myob->get_maxhit()+1);
  myob->set_dead_by_network();

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, packet->playerId);
}


void gsNetwork::game_send_enemy_dead(int ensig)
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_ENEMY_DEAD);

  Enemy *myen;
  myen = enemies->get_enemy(ensig);

  if (!myen) return;

  p.Write((int)ensig);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE);
  }

}

void gsNetwork::game_process_enemy_dead(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  if (!enemies) return;

  // read id
  p.Read(typeId);

  // read enemy's signature
  int ensig;
  p.Read(ensig);

  Enemy *myen;
  myen = enemies->get_enemy(ensig);

  if (!myen) return;

  myen->set_hit(myen->get_maxhit()+1);
  myen->set_dead_by_network();

  // if server: relay!
  if (is_server() && get_netplayers() > 2)
    //server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, packet->playerId, true);
    send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_WORLD_UPDATE, packet->playerId);
}


void gsNetwork::game_send_player_authorative_update(long plsig)
{
  gsNetPlayer *mypl = get_netplayer(get_player_nr_by_sig(plsig));
  BitStream p;
  create_timestamped_bitstream_packet(&p);

  // id
  p.Write((unsigned char)ID_GAME_PLAYER_AUTHORATIVE_UPDATE);

  // player id
  p.Write((char*)&mypl->net_id, sizeof(PlayerID));

  // heading
  p.Write((int)mypl->player->get_head());

  // x position
  p.Write((float)mypl->player->get_x());

  // y position
  p.Write((float)mypl->player->get_y());

  // x speed
  p.Write((float)mypl->player->get_xspd());

  // y speed
  p.Write((float)mypl->player->get_yspd());

  // wall shield
  p.Write((char)mypl->player->get_wall_shield());

  // player dead?
  if (mypl->player->get_dead())
  {
    p.Write((bool)true);
    p.Write((int)mypl->player->get_killedby());
    p.Write((int)mypl->player->get_hitbyweap());
  }
  else
    p.Write((bool)false);


  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_SEQUENCED, STREAM_PLAYER_UPDATE, UNASSIGNED_PLAYER_ID);
  }
}


void gsNetwork::game_process_player_authorative_update(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;
  unsigned long this_timestamp;

  int delay = process_timestamped_bitstream_packet(&p, &this_timestamp);
  
  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));
  
  // read heading
  int head;
  p.Read(head);

  // get player
  gsNetPlayer *mypl = get_netplayer_by_id(player_id);

  // read and apply position data
  float x,y;
  float newx, newy;
  float xspd=0, yspd=0;

  if (mypl->player)
    mypl->player->set_head(head);

  p.Read(x);
  p.Read(y);
  p.Read(xspd);
  p.Read(yspd);

  newx = x; newy = y;

  // interpolation
  if (delay > 0 && mypl->player)
  {
    float ox = mypl->player->get_x();
    float oy = mypl->player->get_y();
    float srvx, srvy;
    
    float delaymult = 60.0 / float(1000.0/(float)delay);
    srvx = x + xspd * delaymult;
    srvy = y + yspd * delaymult;

    // blend client position and server position together (20/80)
    //newx = (0.8 * srvx) + (0.2 * ox);
    //newy = (0.8 * srvy) + (0.2 * oy);
    newx = srvx;
    newy = srvy;

    mypl->player->set_x(newx);
    mypl->player->set_y(newy);
    mypl->player->set_xspd(xspd);
    mypl->player->set_yspd(yspd);
    mypl->player->set_land(0);
  }

  // wall shield
  char wshield;
  p.Read(wshield);
  if (mypl) mypl->player->set_wall_shield(wshield);

  // if player is dead, then kill...
  bool dead;
  p.Read(dead);

  if (dead && mypl)
  {
    int killedby;
    int hitbyweap;
    p.Read(killedby);
    p.Read(hitbyweap);
    
    mypl->player->set_dead();
    mypl->player->set_authorative_dead(true);
    mypl->player->set_killedby(killedby);
    mypl->player->set_hitbyweap(hitbyweap);

    if (hitbyweap == -2)
      mypl->player->inc_suicides();
    else
    {
      for (int n=0; n < globals->players; n++)
        if (killedby == player[n]->get_sig())
          player[n]->inc_frags();
    }

//    errors->log(2, "sig/killedby", mypl->player->get_sig(), killedby);
    player[0]->get_ppanel()->update_netplayer_stats();
    player[0]->get_ppanel()->update_frags();
  }
  
  // no relay cause packet is only sent by server
}


void gsNetwork::game_send_player_statistics(PlayerID pid, bool request)
{
  gsNetPlayer *mypl = get_netplayer_by_id(pid);
  if (!mypl || !mypl->player) 
  {
    errors->log(1, "network", "ERROR game_send_player_statistics: Player ID not valid!", pid.binaryAddress / 10000);
    return;
  }

  BitStream p;

  // id
  p.Write((unsigned char)ID_PLAYER_STATS);

  // player id
  p.Write((char*)&pid, sizeof(PlayerID));

  // request stats from others?
  p.Write((bool)request);

  // stats
  statistics myplstats;
  myplstats = mypl->player->get_statistics_copy();
  p.Write((char*)&myplstats, sizeof(statistics));

  // save locally
  mypl->player_stats = myplstats;

  // server sends to all clients and saves locally
  if (is_server())
  {
    server->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID, true);
    //send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, STREAM_STATISTICS, UNASSIGNED_PLAYER_ID);
    errors->log(2, "net server", "game_send_player_statistics: sent stats to all players", pid.binaryAddress / 10000);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE, STREAM_STATISTICS);
    errors->log(2, "net client", "game_send_player_statistics: sent stats to server", pid.binaryAddress / 10000);
  }

}


void gsNetwork::game_process_player_statistics(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  // read id
  p.Read(typeId);

  // read player sig
  PlayerID pid;
  p.Read((char*)&pid, sizeof(PlayerID));

  // other player requests our stats?
  bool request;
  bool temp_request;
  p.Read(temp_request);
  request = temp_request;

  // read stats
  statistics myplstats;
  p.Read((char*)&myplstats, sizeof(statistics));

  // process!
  gsNetPlayer *mypl = net->get_netplayer_by_id(pid);
  if (!mypl)
  {
    errors->log(1, "network", "ERROR game_process_player_statistics: Player ID not valid!", pid.binaryAddress / 10000);
    return;
  }

  errors->log(2, "network", "setting stats for player", pid.binaryAddress / 10000);
  mypl->player_stats = myplstats;
  mypl->player_stats.set = true;

  if (is_server())
  {
    // send our stats and relay
    if (request && get_netplayer(0))
      game_send_player_statistics(get_netplayer(0)->net_id, false);
    
    server->interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_STATISTICS, packet->playerId, true);
    //send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, STREAM_STATISTICS, packet->playerId);
  }
  else if (is_client())
  {
    // send our stats
    if (request && get_netplayer(0))
      game_send_player_statistics(get_netplayer(0)->net_id, false);
  }
}

void gsNetwork::game_send_enemy_object_update(int enob, int sig)
{
  BitStream p;

  create_timestamped_bitstream_packet(&p);

  float x, y, xspd, yspd;
  int curwp;

  if (enob == 0) // enemy
  {
    Enemy *myen;
    myen = enemies->get_enemy(sig);
    if (!myen) return;

    // only send if enemy is moving
    if (myen->get_xspd() != 0 || myen->get_yspd() != 0)
    {
      x = myen->get_x();
      y = myen->get_y();
      xspd = myen->get_xspd();
      yspd = myen->get_yspd();
      curwp = myen->wp->get_current();
    }
    else return;
  }
  else if (enob == 1) // object
  {
    Object *myob;
    myob = objects->get_object(sig);
    if (!myob) return;

    // only send if this is a moving object
    if (myob->get_xspd() != 0 || myob->get_yspd() != 0)
    {
      x = myob->get_x();
      y = myob->get_y();
      xspd = myob->get_xspd();
      yspd = myob->get_yspd();
      curwp = myob->wp->get_current();
    }
    else return;
  }
  else if (enob == 2) // special object
  {
    Object *myob;
    myob = objects->get_spobject(sig);
    if (!myob) return;

    // only send if this is a moving object
    if (myob->get_xspd() != 0 || myob->get_yspd() != 0)
    {
      x = myob->get_x();
      y = myob->get_y();
      xspd = myob->get_xspd();
      yspd = myob->get_yspd();
      curwp = myob->wp->get_current();
    }
    else return;
  }

  // create packet
  // id
  p.Write((unsigned char)ID_GAME_ENEMY_OBJECT_UPDATE);

  // enemy or object?
  p.Write((char)enob);

  // sig
  p.Write((int)sig);
/*
if (enob == 0) errors->log(2, "net send", "enemy nr", sig);
if (enob == 1) errors->log(2, "net send", "object nr", sig);
if (enob == 2) errors->log(2, "net send", "sp object nr", sig);
*/
  // position
  p.Write((float)x);
  p.Write((float)y);
  p.Write((float)xspd);
  p.Write((float)yspd);
  p.Write((int)curwp);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID);
  }

}

void gsNetwork::game_process_enemy_object_update(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  unsigned long this_timestamp;

  int delay = process_timestamped_bitstream_packet(&p, &this_timestamp);
  
  // read id
  p.Read(typeId);

  // enemy or object?
  char type;
  p.Read(type);

  // signature
  int sig;
  p.Read(sig);

  // pos & spd
  float x, y, xspd, yspd;
  p.Read(x);
  p.Read(y);
  p.Read(xspd);
  p.Read(yspd);

  // current waypoint
  int curwp;
  p.Read(curwp);

  // interpolation
  float delaymult = 60.0 / float(1000.0/(float)delay);
  float srvx = x + xspd * delaymult;
  float srvy = y + yspd * delaymult;

  int dist;
  
  // process!
  if (type == 0) // enemy
  {
    Enemy *myen = enemies->get_enemy(sig);
    if (!myen) return;
    dist = distance(myen->get_x(), myen->get_y(), srvx, srvy);
    if (abs(dist) > 10)
    {
      myen->set_x(srvx);
      myen->set_y(srvy);
      myen->set_xspd(xspd);
      myen->set_yspd(yspd);
      myen->wp->set_current(curwp);
    }
  }
  else if (type == 1) // object
  {
    Object *myob = objects->get_object(sig);
    if (!myob) return;
    dist = distance(myob->get_x(), myob->get_y(), srvx, srvy);
    if (abs(dist) > 10)
    {
      myob->set_x(srvx);
      myob->set_y(srvy);
      myob->set_xspd(xspd);
      myob->set_yspd(yspd);
      myob->wp->set_current(curwp);
    }
  }
  else if (type == 2) // special object
  {
    Object *myob = objects->get_spobject(sig);
    if (!myob) return;
    dist = distance(myob->get_x(), myob->get_y(), srvx, srvy);
    if (abs(dist) > 10)
    {
      myob->set_x(srvx);
      myob->set_y(srvy);
      myob->set_xspd(xspd);
      myob->set_yspd(yspd);
      myob->wp->set_current(curwp);
    }
  }
}

void gsNetwork::game_send_map_state(PlayerID toplayer)
{
  BitStream p;

  // create packet
  // id
  p.Write((unsigned char)ID_GAME_MAP_STATE);
/*
  // send active enemy IDs
  p.Write((int)enemies->get_count());
  Enemy *myen = enemies->get_first_enemy();
  while (myen)
  {
    p.Write((int)myen->get_nr());
    myen = myen->get_next();
  }

  // send active object/spobject IDs
  p.Write((int)objects->get_count());
  Object *myob = objects->get_first_object();
  while (myob)
  {
    p.Write((int)myob->get_nr());
    myob = myob->get_next();
  }

  // send active spobject IDs
  p.Write((int)-4747);
  myob = objects->get_first_spobject();
  while (myob)
  {
    p.Write((int)myob->get_nr());
    myob = myob->get_next();
  }
*/
  // send craters
  p.Write((int)effects->craters->get_count());
  Crater *mycr = effects->craters->get_first();
  while (mycr)
  {
    p.Write((int)mycr->get_x());
    p.Write((int)mycr->get_y());
    p.Write((int)mycr->get_size());
    mycr = mycr->get_next();
  }

  // server sends to requesting client
  if (is_server())
  {
    server->interface->Send(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_WORLD_UPDATE, toplayer, false);
    //send_to_ingame_players(&p, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, STREAM_WORLD_UPDATE, UNASSIGNED_PLAYER_ID);
  }

}

void gsNetwork::game_process_map_state(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  int n;
  
  // read id
  p.Read(typeId);

  /* active enemy IDs
  int enemy_count;
  p.Read(enemy_count);

  Enemy *myen = enemies->get_first_enemy();
  while (myen)
  {
    p.Write((int)myen->get_nr());
    myen = myen->get_next();
  }

  // send active object/spobject IDs
  p.Write((int)objects->get_count());
  Object *myob = objects->get_first_object();
  while (myob)
  {
    p.Write((int)myob->get_nr());
    myob = myob->get_next();
  }

  // send active spobject IDs
  p.Write((int)-4747);
  myob = objects->get_first_spobject();
  while (myob)
  {
    p.Write((int)myob->get_nr());
    myob = myob->get_next();
  }
  */

  // create craters
  int crater_count;
  p.Read(crater_count);

  for (n=0; n < crater_count; n++)
  {
    int x, y, size;
    p.Read(x);
    p.Read(y);
    p.Read(size);
    effects->craters->add(x, y, size, 1, 0);
  }
  
}


void gsNetwork::send_test_data(int size)
{
  BitStream p;

  char *test = new char[size];
  memset(test, 47, size);
  static int counter = 0;
  
  // id
  p.Write((unsigned char)247);

  // count
  p.Write((int)counter);
  counter++;

  // test data
  p.Write(test, size);

  // server sends to all clients
  if (is_server())
  {
    //server->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, 33, UNASSIGNED_PLAYER_ID, true);
    send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE_ORDERED, 33, UNASSIGNED_PLAYER_ID);
  }
  // client sends to server
  else if (is_client()) 
  {
    client->interface->Send(&p, HIGH_PRIORITY, RELIABLE_ORDERED, 33);
  }

  errors->log(2, "sent test data (counter, bytes)", counter, p.GetNumberOfBytesUsed());

  delete[] test;
}

void gsNetwork::receive_test_data(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  // id
  p.Read(typeId);

  // counter
  int counter;
  p.Read(counter);

  errors->log(2, "received test data (counter, bytes)", counter, p.GetNumberOfBytesUsed());
}
