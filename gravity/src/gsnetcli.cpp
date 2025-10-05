/*
 *   GRAVITY STRIKE -- gsnetcli.cpp
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsglob.h" 
#include "gserror.h"
#include "gsnetw.h"
#include "gsnetcli.h"

gsNetClient::gsNetClient()
{
  interface = RakNetworkFactory::GetRakClientInterface();
  interface->StartOccasionalPing(); // its off by default! (contrary to docs...)

  connection_attempt_timer = -1;
  connected = 0;

  memset(&our_id, 0, sizeof(PlayerID));
  memset(&server_id, 0, sizeof(PlayerID));
}

gsNetClient::~gsNetClient()
{
/*  RakNetStatisticsStruct *rss;
  rss = interface->GetStatistics();
  char message[2048];
  StatisticsToString(rss, message, 2);
  errors->log(2, "endtime", globals->game_time);
  errors->log(2, "stats", message);
*/

  interface->Disconnect(500);
  RakNetworkFactory::DestroyRakClientInterface(interface);
  errors->log(2, "network", "client shutdown");
}

int gsNetClient::connect_to_server(char *ip)
{
  int ret = interface->Connect(ip, SERVER_PORT, CLIENT_PORT, 0, 1);
  
  if (!ret)
  {
    errors->log(1, "network", "error: connection attempt to server failed on initialization");
    return 1;
  }
  else
  {
    errors->log(2, "network", "connection attempt to server started", ip);
    connection_attempt_timer = 0;
    return 0;
  }
}

int gsNetClient::check_connection_attempt()
{
  if (!connected && connection_attempt_timer >= 0)
  {
    if (++connection_attempt_timer >= MAX_NET_TIMEOUT)
    {
      connection_attempt_timer = -1;
      errors->log(2, "network", "client connection to server failed");
      return 0;
    }
    else
    {
      if (interface->IsConnected())
      {
        connected = 1; // client has successfully connected to server
        connection_attempt_timer = -1;
        errors->log(2, "network", "client successfully connected to server!", interface->GetPlayerID().binaryAddress);
        errors->log(2, "network", "server id is", interface->GetServerID().binaryAddress);
        our_id = interface->GetPlayerID();
        server_id = interface->GetServerID();
        return 1;
      }
    }
  }

  return -1;
}

unsigned char gsNetClient::get_packet_identifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		if (p->length > sizeof(unsigned char) + sizeof(unsigned long))
		  return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
    else
      return 255;
	}
	else
		return (unsigned char) p->data[0];
}

void gsNetClient::process_packets()
{
	Packet* p;
	unsigned char packetIdentifier;

  /*
  if (globals->game_time % 600 == 0)
  {
    //interface->PingServer();
    //errors->log(2, "net client", "last ping", interface->GetLastPing());
    RakNetStatisticsStruct *stats = interface->GetStatistics();
    char mybuf[10000]; memset(mybuf, 0, sizeof(mybuf));
    StatisticsToString(stats, mybuf, 2);
    errors->log(2, "net stats", mybuf);
  }
  */

	p = interface->Receive();

  while (p)
	{
    packetIdentifier = get_packet_identifier(p);

		// Check if this is a native packet
		switch (packetIdentifier)
		{
		  case ID_REMOTE_DISCONNECTION_NOTIFICATION:  // Another system has disconnected.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_REMOTE_DISCONNECTION_NOTIFICATION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_CONNECTION_LOST:             // Another system has been dropped by the server.  Client only.
        #ifdef _DEBUG
          errors->log(2, "net client", "ID_REMOTE_CONNECTION_LOST", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_NEW_INCOMING_CONNECTION:     // Another system has connected.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_REMOTE_NEW_INCOMING_CONNECTION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_EXISTING_CONNECTION:         // We just connected to the server and are getting a list of players already connected
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_REMOTE_EXISTING_CONNECTION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_CONNECTION_BANNED:                  // We are banned from connecting to the system specified in packet->playerId
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_CONNECTION_BANNED", p->playerId.binaryAddress);
        #endif
		    break;
		  
      case ID_CONNECTION_REQUEST_ACCEPTED:        // Our request to connect to another system has been accepted.  Client only.
         
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_CONNECTION_REQUEST_ACCEPTED", p->playerId.binaryAddress);
        #endif
 			  break;

		  case ID_NEW_INCOMING_CONNECTION:            // Another system has requested to connect to us, which we have accepted.  Server or peer only.
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_NEW_INCOMING_CONNECTION", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_NO_FREE_INCOMING_CONNECTIONS:       // The system we connected to has no free slots to connect to. Client or peer only.
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_NO_FREE_INCOMING_CONNECTIONS", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_DISCONNECTION_NOTIFICATION:         // A remote system terminated the connection.  packet->playerId specifies which remote system
        connected = 0;
        net->set_quit_game(true);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_DISCONNECTION_NOTIFICATION", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_CONNECTION_LOST:                    // The network cannot send reliable packets so has terminated the connection. packet->playerId specifies which remote system
        connected = 0;
        net->set_quit_game(true);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_CONNECTION_LOST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_RECEIVED_STATIC_DATA:               // Another system has just sent their static data to us (which we recorded automatically)
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_RECEIVED_STATIC_DATA", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_INVALID_PASSWORD:                   // Our connection to another system was refused because the passwords do not match
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_INVALID_PASSWORD", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_MODIFIED_PACKET:                    // The network layer has detected packet tampering
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_MODIFIED_PACKET", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_REMOTE_PORT_REFUSED:                // The remote system has responded with ICMP_PORT_UNREACHABLE
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_REMOTE_PORT_REFUSED", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_PONG:                               // Peer or client.  Response from a ping for an unconnected system.
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_PONG", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_UPDATE_DISTRIBUTED_NETWORK_OBJECT:
//      	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacket(p);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_UPDATE_DISTRIBUTED_NETWORK_OBJECT", p->playerId.binaryAddress);
        #endif
	  		break;
  		case ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_ACCEPTED:
//       	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationAccepted(p);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_ACCEPTED", p->playerId.binaryAddress);
        #endif
  			break;
		  case ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_REJECTED:
//       	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationRejected(p);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_REJECTED", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_REQUEST_FILE_LIST:      // Request for a list of files served by the autopatcher. Send to Autopatcher::SendDownloadableFileList(packet->playerId).
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_AUTOPATCHER_REQUEST_FILE_LÌST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_FILE_LIST:              // Got a list of files served by a remote autopatcher. Send to Autopatcher::OnAutopatcherFileList(packet, bool);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_AUTOPATCHER_FILE_LIST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_REQUEST_FILES:          // Got a request for a set of files from our autopatcher. Send to Autopatcher::OnAutopatcherRequestFiles(packet);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_AUTOPATCHER_REQUEST_FILES", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_SET_DOWNLOAD_LIST:      // Got confirmation of what files we are about to get from the remote autopatcher. Send to Autopatcher::OnAutopatcherSetDownloadList(packet);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_AUTOPATCHER_SET_DOWNLOAD_LIST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_WRITE_FILE:             // Got a file from a remote autopatcher. Send to Autopatcher::OnAutopatcherWriteFile(packet);
        #ifdef _DEBUG 
          errors->log(2, "net client", "ID_AUTOPATCHER_WRITE_FILE", p->playerId.binaryAddress);
        #endif
			  break;
		  default:
			  process_unhandled_packet(p, packetIdentifier);
			  break;
		}

		interface->DeallocatePacket(p);

		p = interface->Receive();
	}  
}

void gsNetClient::process_unhandled_packet(Packet *packet, unsigned char packetIdentifier)
{
  int delay = 0;
  BitStream bs((char*)packet->data, packet->length, false);

  switch (packetIdentifier)
  {
    case ID_WELCOME : // server sends a welcome, we create our player and send our name
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_WELCOME", packet->playerId.binaryAddress);
      #endif

      // create local player
      net->create_local_player(globals->pplayer[0].pname, globals->pplayer[0].pship);
      errors->log(2, "net client", "net id", net->get_netplayer(0)->net_id.binaryAddress);

      // send our player to the server
      menu_send_our_player_to_server();

      errors->log(2, "net client ip", (char*)interface->PlayerIDToDottedIP(net->get_netplayer(0)->net_id));
      errors->log(2, "net server ip", (char*)interface->PlayerIDToDottedIP(packet->playerId));

      break;

    case ID_MENU_SEND_PLAYER : // server sends us a ship
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_MENU_SEND_PLAYER", packet->playerId.binaryAddress);
      #endif
      menu_client_receive_player(packet);
      break;

    case ID_MENU_SHIPFILE : // server sends us ship files
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_MENU_SHIPFILE", packet->playerId.binaryAddress);
      #endif
      menu_client_receive_shipfile(packet);
      break;

    case ID_MENU_REQUEST_SHIPFILES : // server requests our ship files
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_MENU_REQUEST_SHIPFILES", packet->playerId.binaryAddress);
      #endif
      menu_send_our_shipfile_to_server();

      // request shipfiles from server
      send_simple_message(ID_MENU_REQUEST_SHIPFILES);
      break;

    case ID_MENU_KILL_PLAYER : // we have to completely delete a player
      #ifdef _DEBUG
        errors->log(2, "net client", "ID_MENU_KILL_PLAYER", packet->playerId.binaryAddress);
      #endif
      menu_client_kill_player(packet);
      break;

    case ID_MENU_UPDATE :    // server sends us updated globals
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_MENU_UPDATE", packet->playerId.binaryAddress);
      #endif
      menu_client_receive_update(packet);
      break;

    case ID_GAME_SET_PLAYER_HOME :   // server sends us a starting position
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_SET_PLAYER_HOME", packet->playerId.binaryAddress);
      #endif
      game_receive_set_player_home(packet);
      break;

    case ID_GAME_ADD_PLAYER :   // server sends us a game-joining player
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_ADD_PLAYER", packet->playerId.binaryAddress);
      #endif
      game_receive_add_player(packet);
      break;

    case ID_GAME_QUIT :      // a player has left the game
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_QUIT", packet->playerId.binaryAddress);
      #endif
      game_receive_quit(packet);
      break;

    case ID_GAME_SERVER_MISC :     // server sends misc. data
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_GAME_SERVER_MISC", packet->playerId.binaryAddress);
      #endif
      game_receive_server_misc(packet);
      break;

    case ID_GAME_PLAYER_POSITION :      // server sends us a position update for a player
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_GAME_PLAYER_POSITION", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_position(packet);
      break;

    case ID_GAME_PLAYER_INPUT :      // server sends us a input key of a player
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_GAME_PLAYER_POSITION", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_input(packet);
      break;

    case ID_GAME_PLAYER_DEAD :      // a player is dead
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_PLAYER_DEAD", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_dead(packet);
      break;

    case ID_GAME_EXTRA :      // an extra was created
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_EXTRA", packet->playerId.binaryAddress);
      #endif
      net->game_process_extra(packet);
      break;

    case ID_GAME_PLAYER_DAMAGE :      // a player was damaged
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_PLAYER_DAMAGE", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_damage(packet);
      break;

    case ID_GAME_PLAYER_STATUS :      // player status coming in
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_PLAYER_STATUS", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_status(packet);
      break;

    case ID_GAME_CRATER :      // a crater was created
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_GAME_CRATER", packet->playerId.binaryAddress);
      #endif
      net->game_process_crater(packet);
      break;

    case ID_CHAT_MESSAGE :      // chat message
      #ifdef _DEBUG 
        //errors->log(2, "net client", "ID_GAME_CRATER", packet->playerId.binaryAddress);
      #endif
      net->game_process_chat_message(packet);
      break;

    case ID_GAME_OBJECT_DEAD :      // an object is dead
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_OBJECT_DEAD", packet->playerId.binaryAddress);
      #endif
      net->game_process_object_dead(packet);
      break;

    case ID_GAME_ENEMY_DEAD :      // an enemy is dead
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_ENEMY_DEAD", packet->playerId.binaryAddress);
      #endif
      net->game_process_enemy_dead(packet);
      break;

    case ID_GAME_PLAYER_AUTHORATIVE_UPDATE :      // server sends us an important-updated player
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_PLAYER_AUTHORATIVE_UPDATE", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_authorative_update(packet);
      break;

    case ID_PLAYER_STATS :      // (final) player statistics
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_PLAYER_STATS", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_statistics(packet);
      break;

    case ID_GAME_ENEMY_OBJECT_UPDATE :  // server sends us an enemy or object position
      #ifdef _DEBUG
        //errors->log(2, "net client", "ID_GAME_ENEMY_OBJECT_UPDATE", packet->playerId.binaryAddress);
      #endif
      net->game_process_enemy_object_update(packet);
      break;

    case ID_GAME_MAP_STATE :      // server is sending us his map state
      #ifdef _DEBUG 
        errors->log(2, "net client", "ID_GAME_MAP_STATE", packet->playerId.binaryAddress);
      #endif
      net->game_process_map_state(packet);
      break;

    case 247 : // test
       net->receive_test_data(packet);
       break;

    default:
      errors->log(2, "net client", "client got unhandled packet!", packetIdentifier);
  }

}

void gsNetClient::send_simple_message(unsigned char msgid)
{
  BitStream bs;
  bs.Write((unsigned char)msgid);
  interface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void gsNetClient::menu_send_our_player_to_server()
{
  BitStream p;
  p.Write((unsigned char)ID_MENU_SEND_PLAYER);

  // player data
  net->write_string_to_bitstream(net->get_netplayer(0)->name, &p);
  net->write_string_to_bitstream(net->get_netplayer(0)->ship_file, &p);

  // our id
  p.Write((char*)&our_id, sizeof(PlayerID));

  // server id (only needed because there's no api function in the server interface?!)
  p.Write((char*)&server_id, sizeof(PlayerID));

  // send
  int ret = interface->Send(&p, HIGH_PRIORITY, RELIABLE, 0);
  if (!ret)
    errors->log(2, "net client", "menu send player failed", ret);
  else
    errors->log(2, "net client", "player sent to server (our_id)", our_id.binaryAddress);
}

void gsNetClient::menu_send_our_shipfile_to_server()
{
  // send shipfile
  BitStream sp;
  net->create_shipfile_packet(&sp);
  int ret = interface->Send(&sp, HIGH_PRIORITY, RELIABLE, 0);
  if (!ret)
    errors->log(2, "net client", "menu send ship file failed", our_id.binaryAddress);
  else
    errors->log(2, "net client", "ship file sent to server (bytes)", sp.GetNumberOfBytesUsed());
}

void gsNetClient::menu_client_receive_shipfile(Packet *packet)
{
  // receive and create the file on disk
  net->receive_shipfile(packet);
}

void gsNetClient::menu_client_receive_player(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  char tmpname[50];
  char tmpship[50];
  PlayerID sender_id, server_id;

  // type
  p.Read(typeId);
  
  // player data
  net->write_bitstream_to_string(&tmpname[0], &p);
  net->write_bitstream_to_string(&tmpship[0], &p);
  
  // sender id
  p.Read((char*)&sender_id, sizeof(PlayerID));

  // server id (not needed)
  p.Read((char*)&server_id, sizeof(PlayerID));

  // create the netplayer
  gsNetPlayer *pl = net->menu_create_netplayer(tmpname, tmpship, sender_id);

  errors->log(2, "net client", "ship created", sender_id.binaryAddress);
}

void gsNetClient::menu_client_kill_player(Packet *packet)
{
  PlayerID sender_id;
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  p.Read(typeId);
  p.Read((char*)&sender_id, sizeof(PlayerID));

  // delete the netplayer
  net->menu_delete_netplayer(sender_id);

  errors->log(2, "net client", "player deleted", sender_id.binaryAddress / 10000);
}

void gsNetClient::menu_send_start_game()
{
  errors->log(2, "net client", "send ID_MENU_START_GAME");
  send_simple_message(ID_MENU_START_GAME);
}

void gsNetClient::game_send_i_am_in()
{
  errors->log(2, "net client", "send ID_GAME_I_AM_IN");
  send_simple_message(ID_GAME_I_AM_IN);
}

void gsNetClient::menu_client_receive_update(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  char tempstring[100];
  unsigned char netplayer_count;
  PlayerID temp_id;
  int n;

  // ID
  p.Read(typeId);

  // player count
  p.Read(netplayer_count);
  // all player data
  for (n=0; n < netplayer_count; n++)
  {
    p.Read((char*)&temp_id, sizeof(PlayerID)); // get id
    gsNetPlayer *temp_player = net->get_netplayer_by_id(temp_id); // get player by this id
    
    if (temp_player)
    {
      net->write_bitstream_to_string(&temp_player->name[0], &p);
      p.Read((bool)temp_player->ready);
    }
    else return;
  }

  // globals
  p.Read(globals->gravity_percent);
  p.Read(globals->extra_amount);
  p.Read(globals->max_frags);
  p.Read(globals->max_time);
  p.Read(globals->activate_extra);
  p.Read(globals->capture_the_flag);
  p.Read(globals->hunter_hunted);
  bool server_in_game;
  p.Read((bool)server_in_game);
  net->set_server_in_game(server_in_game);

  // set server to ready
  for (n=0; n < netplayer_count; n++)
    if (net->get_netplayer(n)->net_id.binaryAddress == SERVER_BIN_ID)
      net->get_netplayer(n)->ready = server_in_game;
  
  // levelfile name of current level
  net->write_bitstream_to_string(&tempstring[0], &p);

  // find level!
  for (n=0; n < globals->max_levels; n++)
    if (!strcmp(globals->level_files[n].levelfile, tempstring))
    {
      globals->selected_level = n;
      break;
    }
}

void gsNetClient::menu_client_update_server()
{
  BitStream p;
  p.Write((unsigned char)ID_MENU_UPDATE);

  // player name
  net->write_string_to_bitstream(net->get_netplayer(0)->name, &p);

  // are we ready?
  p.Write(net->get_netplayer(0)->ready);

  // are we in a game?
  //p.Write((bool)net->get_lets_go());

  // send
  int ret = interface->Send(&p, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0);
}


void gsNetClient::game_receive_set_player_home(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;
  int x, y, base;

  unsigned long this_timestamp;
  int delay = net->process_timestamped_bitstream_packet(&p, &this_timestamp);

  // id
  p.Read(typeId);

  // player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // x position
  p.Read(x);

  // y position
  p.Read(y);

  // homebase
  p.Read(base);

  // elapsed gametime
  unsigned long gametime;
  p.Read(gametime);

  net->set_lets_go_delay((delay / (1000.0/60.0)) + gametime);
  //errors->log(2, "receive_set_player_home (delay, gametime)", delay, gametime);

  // set home coordinates now!
  gsNetPlayer *myplayer = net->get_netplayer_by_id(player_id);
  if (!myplayer)
  {
    errors->log(1, "net client", "set_player_home ERROR: player with this id does not exist!", player_id.binaryAddress);
    return;
  }
  errors->log(2, "home coordinates", base, y);

  myplayer->player->set_home_x(x);
  myplayer->player->set_home_y(y);
  if (base != -1) myplayer->player->set_homebase(base);
  myplayer->player->return_home();

  net->set_position_set(true);
}

void gsNetClient::game_receive_add_player(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  // id
  p.Read(typeId);

  // player count
  char player_count;
  p.Read(player_count);

  errors->log(2, "net client", "game_receive_add_player (player_count)", player_count);
  // read all active players
  for (int n=0; n < player_count; n++)
  {
    PlayerID pid;
    int x, y;
    char base;

    // player id
    p.Read((char*)&pid, sizeof(PlayerID));

    // x, y, base
    p.Read(x);
    p.Read(y);
    p.Read(base);

    // is player already in game?
    gsNetPlayer *mypl = net->get_netplayer_by_id(pid);
    if (!mypl)
    {
      errors->log(1, "net client", "ERROR game_receive_add_player: id is not valid!", pid.binaryAddress / 10000);
      return;
    }

    if (mypl->in_game || mypl->net_id == net->get_netplayer(0)->net_id)
      errors->log(2, "net client", "game_receive_add_player: player is already in game", pid.binaryAddress / 10000);
    else
    {
      net->add_player_to_game(pid);
      mypl->player->set_home_x(x);
      mypl->player->set_home_y(y);
      mypl->player->set_homebase(base);
      mypl->player->return_home();
      errors->log(2, "net client", "game_receive_add_player: added player and returned him to his home location", pid.binaryAddress / 10000);
      errors->log(2, "net client", "home base", base);
      errors->log(2, "net client", x, y);
    }
  }
}

void gsNetClient::game_send_quit()
{
  BitStream p;
  
  // id
  p.Write((unsigned char)ID_GAME_QUIT);

  // include our id
  p.Write((char*)&our_id, sizeof(PlayerID));

  // send to server
  interface->Send(&p, HIGH_PRIORITY, RELIABLE, 0); 
}

void gsNetClient::game_receive_quit(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;

  // id
  p.Read(typeId);

  // player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // if player id is server, quit game
  if (player_id.binaryAddress == SERVER_BIN_ID)
  {
    net->set_quit_game(true);
  }
  // else if player id is another client, delete this player
  else
  {
    net->delete_player_from_game(player_id);
  }
}

void gsNetClient::game_receive_server_misc(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
//  PlayerID player_id;

  // id
  p.Read(typeId);

  // game time
  p.Read((int)globals->second_time);
}
