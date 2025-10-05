/*
 *   GRAVITY STRIKE -- gsnetw.h
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _GSNETWORK_
#define _GSNETWORK_

#include "gsnetcli.h"
#include "gsnetsrv.h"
#include "gsplayer.h"

#define SERVER_PORT            60047
#define CLIENT_PORT            60047

#define MAX_NET_PLAYERS        6
#define MAX_NET_TIMEOUT        60*5

#define NET_CLIENT             0
#define NET_SERVER             1

#define SERVER_BIN_ID          999999

enum NET_ID {
  ID_WELCOME = 100,           // server  -> client: server welcomes client (and client sends his data)
  ID_MENU_SEND_PLAYER,        // client <-> server: ship data is sent
  ID_MENU_KILL_PLAYER,        // server  -> client: client has to delete ship of player with included playerId
  ID_MENU_UPDATE,             // server <-> client: update global data (player name, level options etc.)
  ID_MENU_SHIPFILE,           // client <-> server: ship file data
  ID_MENU_REQUEST_SHIPFILES,  // server <-> client: server or client requests shipfiles
  ID_MENU_START_GAME,         // server  -> client: send start game message
  ID_CHAT_MESSAGE,            // server <-> client: a chat message

  ID_GAME_SET_PLAYER_HOME,    // server  -> client: sent each player his start position
  ID_GAME_I_AM_IN,            // client  -> server: client reached game (after getting position) and requests other players
  ID_GAME_QUIT,               // client <-> server: client or server leaves the game
  ID_GAME_SERVER_MISC,        // server  -> clients: server updates clients with misc. data (e.g. remaining time)

  ID_GAME_ADD_PLAYER,         // server  -> clients: add a new player to current running game
  ID_GAME_PLAYER_POSITION,    // client <-> server: player position update (timestamped) (sent x times a second or so, UNRELIABLE SEQUENCED)
  ID_GAME_PLAYER_INPUT,       // client <-> server: player moves (presses a key), send RELIABLE ORDERED
  ID_GAME_PLAYER_DEAD,        // client <-> server: local player is dead, inform others
  /*ID_GAME_PLAYER_SHOOTS,      // client <-> server: local player shoots, transmit coordinates */
  ID_GAME_EXTRA,              // client <-> server: an extra was created
  ID_GAME_PLAYER_DAMAGE,      // client <-> server: local player is damaged, inform other clients
  ID_GAME_PLAYER_STATUS,      // client <-> server: player status (ammo, hull and shield)
  ID_GAME_CRATER,             // client <-> server: a crater was created
  ID_GAME_OBJECT_DEAD,        // client <-> server: an object was destroyed by local player
  ID_GAME_ENEMY_DEAD,         // client <-> server: an enemy was destroyed by local player
  ID_GAME_PLAYER_AUTHORATIVE_UPDATE,  // server  -> client: e.g. after collision decided by server
  ID_PLAYER_STATS,            // client <-> server: final statistics
  ID_GAME_ENEMY_OBJECT_UPDATE,// server -> clients: pos/spd update of an enemy or an object
  ID_GAME_MAP_STATE           // server -> clients: which enemies/objects/craters are still active (and crater pos.)
};

enum NET_STREAMS {
  STREAM_DEFAULT        = 0,
  STREAM_PLAYER_UPDATE,       // ordering stream for player movement updates
  STREAM_WORLD_UPDATE,        // stream for updates of world (enemy/object deaths etc.)
  STREAM_STATISTICS,          // stream for player statistics (in game and end-game)
  STREAM_EFFECTS              // stream for effects (adding craters...)
};

class gsNetPlayer
{
 public: 

  gsNetPlayer(char *pname, char *pship, PlayerID net_id);
  ~gsNetPlayer();

  void delete_shipfiles();

  gsPlayer   *player;                              // temporary player class
  bool       network;                              // this is a non-local network player
  char       name[50];                             // player name
  char       ship_file[50];                        // ship file
  char       shipimg_file[50];                     // ship img file
  PlayerID   net_id;                               // RakNet network id
  bool       ready;                                // player is ready
  bool       in_game;                              // player is in game

  statistics player_stats;                         // last stats
};

#define MAX_CHAT_MESSAGES 50

struct ChatMessage {
  char  *message;
  char  player[20];
  int   color;
};

class gsChat
{
  private:
    ChatMessage  message[MAX_CHAT_MESSAGES];       // message storage
    int          count;                            // how much messages in array?

  public:
    gsChat();
    ~gsChat();

    void add(char *msg, char *player, int color=0);// add a message to the array
    char *get_message(int num);                    // return message no num
    char *get_player(int num);                     // return player name who posted message num
    char *get_message_with_playername(int num, char *buf);    // return message with preceding player name
    int  get_color(int num);                       // get color of message num
    int  get_count()                               { return count; }

    int  process_commands(char *msg);              // checks if there is a command in the chat msg and processes it. returns 1 if there was a command.

    void clear();                                  // clear all messages
};

struct HomeLocation {
  int x;
  int y;
  int num;
  int id;
};

class gsNetwork
{
  private:
    gsNetClient  *client;                          // if client
    gsNetServer  *server;                          // if server

    int          mode;                             // NET_CLIENT or NET_SERVER

    gsNetPlayer  *netplayer[MAX_NET_PLAYERS];      // temporary network players
    int          netplayers;                       // how much network players?

    unsigned long last_timestamp[MAX_NET_PLAYERS]; // last timestamp received

    bool         server_in_game;                   // true = server is in game
    bool         start_game;                       // server startet a game
    bool         position_set;                     // true = we received our start position
    unsigned long lets_go_delay;                   // for client: time (ticks) server is in the future
    bool         quit_game;                        // true = we need to quit the current game
    
    HomeLocation homeloc[MAX_NET_PLAYERS];         // array holding home locations (for dynamically joining players)

  public:   
    gsNetwork();
    ~gsNetwork();

    gsChat       chat;                             // chat message handler

    // get/set/is
    int is_server()                                { if (mode == NET_SERVER) return 1; else return 0; }
    int is_client()                                { if (mode == NET_CLIENT) return 1; else return 0; }

    gsNetPlayer *get_netplayer(int n)              { if (netplayer[n]) return netplayer[n]; else return NULL; }
    gsNetPlayer *get_netplayer_by_id(PlayerID id)  { for (int n=0; n < MAX_NET_PLAYERS; n++) if (netplayer[n] && netplayer[n]->net_id == id) return netplayer[n]; return NULL; }
    int get_netplayer_nr_by_id(PlayerID id)        { for (int n=0; n < MAX_NET_PLAYERS; n++) if (netplayer[n] && netplayer[n]->net_id == id) return n; return -1; }
    int get_netplayers()                           { return netplayers; }

    bool get_all_players_ready();                  // returns true when all netplayers are "ready"
    bool get_start_game()                          { return start_game; }
    void set_start_game(bool n)                    { start_game = n; }
    bool get_quit_game()                           { return quit_game; }
    void set_quit_game(bool n)                     { quit_game = n; }
    bool get_server_in_game()                      { return server_in_game; }
    void set_server_in_game(bool n)                { server_in_game = n; }
    
    unsigned long get_lets_go_delay()              { return lets_go_delay; }
    void set_lets_go_delay(unsigned long n)        { lets_go_delay = n; }
   
    bool get_position_set()                        { return position_set; }
    void set_position_set(bool n)                  { position_set = n; }

    void add_random_chat_message(int msgnum, char *repl1, char *repl2, char *system, int maxmsg, int col);
    void add_frag_message(char *plname, char *killername, int weapon);

    // functions
    int  init(int mode);                           // init as client or server
    void reset();                                  // reset/disconnect network
    void reset_status_vars();                      // reset bools (returning to menu - from game)

    void send_to_ingame_players(char *data, const long length, PacketPriority priority, PacketReliability reliability, char ordering_channel, PlayerID player_id);
    void send_to_ingame_players(BitStream *data, PacketPriority priority, PacketReliability reliability, char ordering_channel, PlayerID player_id);

    void add_player_to_game(PlayerID pid);         // adds netplayer pid to game
    void delete_player_from_game(PlayerID pid);    // removes netplayer pid from game

    int  client_check_connection_attempt();        // check if client is connected
    int  client_is_connected()                     { if (is_client()) return client->is_connected(); else return 0; }
    void process_packets();                        // processing of received packets
    void write_string_to_bitstream(char *myString, BitStream *output);
    void write_bitstream_to_string(char *myString, BitStream *input);
    void create_timestamped_bitstream_packet(BitStream *bs);
    int  process_timestamped_bitstream_packet(BitStream *bs, unsigned long *timestamp=NULL); // process a timestamped bitstream and return its delay

    gsNetPlayer *menu_create_netplayer(char *name, char *ship, PlayerID net_id); // add a network player
    int  menu_delete_netplayer(PlayerID net_id);        // delete a network player
    void reorganize_netplayer_array(int n);             // player n left - reorganize netplayer array (menu)
    void reorganize_player_array(int n);                // player n left - reorganize player array (game)
    void create_local_player(char *name, char *ship);   // creates local player

    void receive_shipfile(Packet *p);              // receive a shipfile (ini & pic)
    void create_shipfile_packet(BitStream *p);                 // create a packet with shipfile of local player
    void create_shipfile_packet(BitStream *p, PlayerID pid);   // create a packet with shipfile of player with ID pid

    void send_test_data(int sz);
    void receive_test_data(Packet *p);

    void fill_home_locations();                    // fills home location array (run after init script)
    HomeLocation *get_home_location(int n)         { if (n >= 0 && n < MAX_NET_PLAYERS) return &homeloc[n]; else return NULL; }

    // wrappers
    void game_log_netstats(PlayerID pid)           { if (is_server()) server->game_log_netstats(pid); }

    void menu_server_update_clients()              { if (is_server()) server->menu_server_update_clients(); }
    void menu_client_update_server()               { if (is_client()) client->menu_client_update_server(); }
    void menu_update_shipfiles()                   { if (is_server()) server->menu_send_our_shipfile_to_clients(); else if (is_client()) client->menu_send_our_shipfile_to_server(); }
    void menu_send_start_game()                    { if (is_client()) client->menu_send_start_game(); }
    void game_send_i_am_in()                       { if (is_client()) client->game_send_i_am_in(); }

    unsigned long get_synchronized_random_number_seed()     { if (is_server()) return server->interface->GetSynchronizedRandomInteger(); else return client->interface->GetSynchronizedRandomInteger(); }

    void game_send_set_player_home(PlayerID pid, int x, int y, int base) { if (is_server()) server->game_send_set_player_home(pid, x, y, base); }

    void game_send_quit()                          { if (is_client()) client->game_send_quit(); else if (is_server()) server->game_send_quit(); }

    void game_send_server_misc()                   { if (is_server()) server->game_send_server_misc(); }

    void game_send_player_position();              // send the position and speed of our player
    void game_process_player_position(Packet *p);  // process a player position packet

    void game_send_player_input(int key, bool state, bool reset); // send a pressed key (true = key pressed, false = key released)
    void game_process_player_input(Packet *p);     // process the network input of a player

    void game_send_player_dead(long sig, int x, int y); // player with "sig" is dead (at position x/y)
    void game_process_player_dead(Packet *p);      // kill a network player cause he's dead

    void game_send_extra(int extratype, int framespeed, int x, int y, int extrarandom); // send a "create an extra" packet (because player is dead)
    void game_process_extra(Packet *p);            // create an extra

    void game_send_player_damage(long plsig, int damage); // send a "player sig is damaged" packet
    void game_process_player_damage(Packet *p);    // damage a player

    void game_send_player_status();                // send status of our player (invoked every few seconds)
    void game_process_player_status(Packet *p);

    void game_send_crater(int x, int y, int radius); // send a "create crater" packet
    void game_process_crater(Packet *p);

    void game_send_chat_message(char *msg);        // send a chat message
    void game_process_chat_message(Packet *p);

    void game_send_object_dead(int objsig, bool specialob); // send death of an object
    void game_process_object_dead(Packet *p);

    void game_send_enemy_dead(int ensig);           // send death of an enemy
    void game_process_enemy_dead(Packet *p);

    void game_send_player_authorative_update(long plsig);  // server sends authorative player update (after collision)
    void game_process_player_authorative_update(Packet *p);

    void game_send_player_statistics(PlayerID pid, bool request);  // send statistics of player pid and optionally request stats from others
    void game_process_player_statistics(Packet *p);    // process stats

    void game_send_enemy_object_update(int enob, int sig); // sends an update for enemy or object with sig to clients
    void game_process_enemy_object_update(Packet *p);

    void game_send_map_state(PlayerID toplayer);      // sends an updated map state to new players
    void game_process_map_state(Packet *p);
};


extern gsNetwork *net;

#endif
