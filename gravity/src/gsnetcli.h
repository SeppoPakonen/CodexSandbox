/*
 *   GRAVITY STRIKE -- menu/gsmenu.h
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */


#ifndef _GSNETCLIENT_
#define _GSNETCLIENT_

#include "network/BitStream.h"
#include "network/RakNetworkFactory.h"
#include "network/RakClientInterface.h"
#include "network/RakServerInterface.h"
#include "network/NetworkTypes.h"
#include "network/PacketPriority.h"
#include "network/PacketEnumerations.h"

class gsNetClient
{
 private:

  int connection_attempt_timer;                  // >0 on connection attempt (max=MAX_NET_TIMEOUT)
  int connected;                                 // 1 if client is connected
  PlayerID our_id;                               // our player id
  PlayerID server_id;                            // ID of the server

 public:

  RakClientInterface *interface;                 // our connection handle

  int is_connected()                             { return connected; }
  int get_connection_attempt_timer()             { return connection_attempt_timer; }
  PlayerID get_our_id()                          { return our_id; }

  // functions
  gsNetClient();
  ~gsNetClient();

  int  connect_to_server(char *ip);
  int  check_connection_attempt();               // if attempt_timer >= 0, increase until MAX_NET_TIMEOUT, then set client_connected
  unsigned char get_packet_identifier(Packet *p);
  void process_packets();
  void process_unhandled_packet(Packet *packet, unsigned char packetIdentifier);
  void send_simple_message(unsigned char msgid);

  void menu_send_our_player_to_server();         // server wants to know us...
  void menu_send_our_shipfile_to_server();       // ...so send our ship files to him
  void menu_client_receive_player(Packet *p);    // we receive a player and create him
  void menu_client_receive_shipfile(Packet *p);  // we also receive his shipfiles
  void menu_client_kill_player(Packet *p);       // we have to kill a player

  void menu_send_start_game();                   // send start game message to server
  void game_send_i_am_in();                      // send I'm in message to server (requests other player positions)

  void menu_client_receive_update(Packet *p);    // server sends us updated globals
  void menu_client_update_server();              // sends globals update to server

  void game_receive_set_player_home(Packet *p);  // server sends us our start position
  void game_receive_add_player(Packet *p);       // server sends us other active players (in current game)
  void game_send_quit();                         // client leaves game, inform server
  void game_receive_quit(Packet *p);             // a player has left the game, kill him or quit game (if player was server)
  void game_receive_server_misc(Packet *p);      // client receives misc. data from server
};

#endif