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


#ifndef _GSNETSERVER_
#define _GSNETSERVER_

#include "network/RakNetworkFactory.h"
#include "network/RakClientInterface.h"
#include "network/RakServerInterface.h"
#include "network/NetworkTypes.h"
#include "network/PacketPriority.h"
#include "network/PacketEnumerations.h"
#include "network/BitStream.h"

//using namespace RakNet;  // Comment out since RakNet not available

class gsNetServer
{
 private:
   PlayerID our_id;                        // our (server) id on the network

 public:

  // variables
  RakServerInterface *interface;           // our connection handle

  // functions
  gsNetServer();
  ~gsNetServer();

  int  start();                            // start server
  unsigned char get_packet_identifier(Packet *p);
  void process_packets();
  void process_unhandled_packet(Packet *packet, unsigned char packetIdentifier);
  void send_simple_message(unsigned char msgid, PlayerID player_id, bool broadcast=false);

  void menu_send_players_to_client(Packet *p);    // send all ships to new client
  void menu_send_shipfiles_to_client(Packet *p);  // send all shipfiles to new client
  void menu_send_our_shipfile_to_clients();       // send our shipfiles to all clients (when changed)
  void menu_server_receive_player(Packet *p);     // create a player and relay packet
  void menu_server_receive_shipfile(Packet *p);   // receive a shipfile and relay packet
  void menu_server_kill_player(Packet *p);        // delete a player and relay packet

  void menu_server_update_clients();              // update all clients with menu data
  void menu_server_receive_update(Packet *p);     // server receives globals update from client

  void game_server_receive_start_game(Packet *p); // server receives a game start request and honors with location send
  void game_server_receive_i_am_in(Packet *p);    // server receives a I'm in message
  void game_send_set_player_home(PlayerID sig, int x, int y, int base); // send home coordinates to all players
  void game_process_request_player_home(Packet *p); // process client's "give me a base" request
  void game_send_quit();                          // server leaves game, inform clients
  void game_receive_quit(Packet *p);              // a client leaves the game
  void game_send_server_misc();                   // server sends updated misc. data

  void game_log_netstats(PlayerID pid);           // logs current stats for pid via errors:log
};

#endif;