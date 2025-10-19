/*
 *   GRAVITY STRIKE -- gsnetsrv.cpp
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
#include "gsnetsrv.h"

// Include the statistics header
#include "network/RakNetStatistics.h"

gsNetServer::gsNetServer()
{
  interface = RakNetworkFactory::GetRakServerInterface();
  interface->SetAllowedPlayers(MAX_NET_PLAYERS-1); // max players = players - (1 for server)
  interface->StartOccasionalPing(); // its off by default (contrary to docs!)
  interface->StartSynchronizedRandomInteger(); // to keep RNGs in sync

  memset(&our_id, 0, sizeof(PlayerID));
  our_id.binaryAddress = SERVER_BIN_ID;
  our_id.port = 0;
}

gsNetServer::~gsNetServer()
{
  interface->Disconnect(300);
  RakNetworkFactory::DestroyRakServerInterface(interface);
  errors->log(2, "network", "server shutdown");
}

int gsNetServer::start()
{
  int ret = interface->Start(MAX_NET_PLAYERS, 0, 1, SERVER_PORT);

  if (!ret)
  {
    errors->log(1, "network", "error: can't start server");
    return 1;
  }
  else
  {
    errors->log(2, "network", "server start successful!");
    return 0;
  }
}

unsigned char gsNetServer::get_packet_identifier(Packet *p)
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


void gsNetServer::process_packets()
{
	Packet* p;
  unsigned char packetIdentifier;

  // TEMP - Pings
  if (globals->game_time % 300 == 0 && net->get_netplayer(1))
    errors->log(2, "net server", "ping to client", interface->GetLastPing(net->get_netplayer(1)->net_id));

	p = interface->Receive();

	while (p)
	{
    packetIdentifier = get_packet_identifier(p);

		// Check if this is a native packet
		switch (packetIdentifier)
		{
		  case ID_REMOTE_DISCONNECTION_NOTIFICATION:  // Another system has disconnected.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_REMOTE_DISCONNECTION_NOTIFICATION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_CONNECTION_LOST:             // Another system has been dropped by the server.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_REMOTE_CONNECTION_LOST", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_NEW_INCOMING_CONNECTION:     // Another system has connected.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_REMOTE_NEW_INCOMING_CONNECTION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_REMOTE_EXISTING_CONNECTION:         // We just connected to the server and are getting a list of players already connected
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_REMOTE_EXISTING_CONNECTION", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_CONNECTION_BANNED:                  // We are banned from connecting to the system specified in packet->playerId
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_CONNECTION_BANNED", p->playerId.binaryAddress);
        #endif
		    break;
		  case ID_CONNECTION_REQUEST_ACCEPTED:        // Our request to connect to another system has been accepted.  Client only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_CONNECTION_REQUEST_ACCEPTED", p->playerId.binaryAddress);
        #endif
			  break;
		  
      case ID_NEW_INCOMING_CONNECTION:            // Another system has requested to connect to us, which we have accepted.  Server or peer only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_NEW_INCOMING_CONNECTION", p->playerId.binaryAddress);
        #endif

        send_simple_message(ID_WELCOME, p->playerId);
			  break;

		  case ID_NO_FREE_INCOMING_CONNECTIONS:       // The system we connected to has no free slots to connect to. Client or peer only.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_NO_FREE_INCOMING_CONNECTIONS", p->playerId.binaryAddress);
        #endif
			  break;
		  
      case ID_DISCONNECTION_NOTIFICATION:         // A remote system terminated the connection.  packet->playerId specifies which remote system
        // player has disconnected: kill him (identified by p->playerId)
        menu_server_kill_player(p);

        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_DISCONNECTION_NOTIFICATION", p->playerId.binaryAddress);
        #endif
			  break;

		  case ID_CONNECTION_LOST:                    // The network cannot send reliable packets so has terminated the connection. packet->playerId specifies which remote system
        // player has disconnected: kill him
        menu_server_kill_player(p);
      
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_CONNECTION_LOST", p->playerId.binaryAddress);
        #endif
			  break;

		  case ID_RECEIVED_STATIC_DATA:               // Another system has just sent their static data to us (which we recorded automatically)
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_RECEIVED_STATIC_DATA", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_INVALID_PASSWORD:                   // Our connection to another system was refused because the passwords do not match
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_INVALID_PASSWORD", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_MODIFIED_PACKET:                    // The network layer has detected packet tampering
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_MODIFIED_PACKET", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_REMOTE_PORT_REFUSED:                // The remote system has responded with ICMP_PORT_UNREACHABLE
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_REMOTE_PORT_REFUSED", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_PONG:                               // Peer or client.  Response from a ping for an unconnected system.
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_PONG", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_UPDATE_DISTRIBUTED_NETWORK_OBJECT:
//      	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacket(p);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_UPDATE_DISTRIBUTED_NETWORK_OBJECT", p->playerId.binaryAddress);
        #endif
	  		break;
  		case ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_ACCEPTED:
//       	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationAccepted(p);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_ACCEPTED", p->playerId.binaryAddress);
        #endif
  			break;
		  case ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_REJECTED:
//       	DistributedNetworkObjectManager::Instance()->HandleDistributedNetworkObjectPacketCreationRejected(p);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_DISTRIBUTED_NETWORK_OBJECT_CREATION_REJECTED", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_REQUEST_FILE_LIST:      // Request for a list of files served by the autopatcher. Send to Autopatcher::SendDownloadableFileList(packet->playerId).
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_AUTOPATCHER_REQUEST_FILE_L�ST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_FILE_LIST:              // Got a list of files served by a remote autopatcher. Send to Autopatcher::OnAutopatcherFileList(packet, bool);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_AUTOPATCHER_FILE_LIST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_REQUEST_FILES:          // Got a request for a set of files from our autopatcher. Send to Autopatcher::OnAutopatcherRequestFiles(packet);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_AUTOPATCHER_REQUEST_FILES", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_SET_DOWNLOAD_LIST:      // Got confirmation of what files we are about to get from the remote autopatcher. Send to Autopatcher::OnAutopatcherSetDownloadList(packet);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_AUTOPATCHER_SET_DOWNLOAD_LIST", p->playerId.binaryAddress);
        #endif
			  break;
		  case ID_AUTOPATCHER_WRITE_FILE:             // Got a file from a remote autopatcher. Send to Autopatcher::OnAutopatcherWriteFile(packet);
        #ifdef _DEBUG 
          errors->log(2, "net server", "ID_AUTOPATCHER_WRITE_FILE", p->playerId.binaryAddress);
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

void gsNetServer::process_unhandled_packet(Packet *packet, unsigned char packetIdentifier)
{
  switch (packetIdentifier)
  {
    case ID_MENU_SEND_PLAYER : // client sends us his ship
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_MENU_SEND_PLAYER", packet->playerId.binaryAddress);
      #endif

      // receive ship of player
      menu_server_receive_player(packet);

      // send all other ships to client
      menu_send_players_to_client(packet);

      // send client a packet with a "send me your ship files" request
      send_simple_message(ID_MENU_REQUEST_SHIPFILES, packet->playerId);
      break;

    case ID_MENU_SHIPFILE : // client sends us his shipfile
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_MENU_SHIPFILE", packet->playerId.binaryAddress);
      #endif
      menu_server_receive_shipfile(packet);
      break;

    case ID_MENU_REQUEST_SHIPFILES : // client requests our shipfiles
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_MENU_REQUEST_SHIPFILES", packet->playerId.binaryAddress);
      #endif
      menu_send_shipfiles_to_client(packet);
      break;

    case ID_MENU_UPDATE :    // client sends updated data (name etc.)
      #ifdef _DEBUG
        //errors->log(2, "net server", "ID_MENU_UPDATE (from client)", packet->playerId.binaryAddress);
      #endif
      menu_server_receive_update(packet);
      break;

    case ID_MENU_START_GAME :   // client sends "start game" message (he needs a home location)
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_MENU_START_GAME", packet->playerId.binaryAddress);
      #endif
      game_server_receive_start_game(packet);
      errors->log(2, "netplayers", net->get_netplayers());
      break;

    case ID_GAME_I_AM_IN :   // client sends "I'm in game" message (and requests other players)
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_GAME_I_AM_IN", packet->playerId.binaryAddress);
      #endif
      game_server_receive_i_am_in(packet);
      net->game_send_map_state(packet->playerId);
      break;

    case ID_GAME_QUIT :      // a client has left the game
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_GAME_QUIT (from client)", packet->playerId.binaryAddress);
      #endif
      game_receive_quit(packet);
      break;

    case ID_GAME_PLAYER_POSITION :   // client sends us a position update
      #ifdef _DEBUG
        //errors->log(2, "net server", "ID_GAME_PLAYER_POSITION (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_position(packet);
      break;

    case ID_GAME_PLAYER_INPUT :   // client sends us a key
      #ifdef _DEBUG
        //errors->log(2, "net server", "ID_GAME_PLAYER_POSITION (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_input(packet);
      break;

    case ID_GAME_PLAYER_DEAD :      // a client is dead
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_GAME_PLAYER_DEAD (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_dead(packet);      
      break;

    case ID_GAME_EXTRA :      // a client created an extra
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_GAME_EXTRA (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_extra(packet);
      break;

    case ID_GAME_PLAYER_DAMAGE :      // a client was damaged
      #ifdef _DEBUG
        errors->log(2, "net server", "ID_GAME_PLAYER_DAMAGE (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_damage(packet);
      break;

    case ID_GAME_PLAYER_STATUS :      // client sends us his playship status
      #ifdef _DEBUG
        //errors->log(2, "net server", "ID_GAME_PLAYER_STATUS (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_status(packet);
      break;

    case ID_GAME_CRATER :      // a crater was created
      #ifdef _DEBUG 
        //errors->log(2, "net server", "ID_GAME_CRATER (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_crater(packet);
      break;

    case ID_CHAT_MESSAGE :      // chat coming in
      #ifdef _DEBUG 
        //errors->log(2, "net server", "ID_GAME_CRATER (from client)", packet->playerId.binaryAddress);
      #endif
      net->game_process_chat_message(packet);
      break;

    case ID_GAME_OBJECT_DEAD :      // an object is dead
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_GAME_OBJECT_DEAD", packet->playerId.binaryAddress);
      #endif
      net->game_process_object_dead(packet);
      break;

    case ID_GAME_ENEMY_DEAD :      // an enemy is dead
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_GAME_ENEMY_DEAD", packet->playerId.binaryAddress);
      #endif
      net->game_process_enemy_dead(packet);
      break;

    case ID_PLAYER_STATS :      // (final) player statistics
      #ifdef _DEBUG 
        errors->log(2, "net server", "ID_PLAYER_STATS", packet->playerId.binaryAddress);
      #endif
      net->game_process_player_statistics(packet);
      break;

    case 247 : // test
       net->receive_test_data(packet);
       break;

    default:
      errors->log(2, "net server", "server got unhandled packet!", packetIdentifier);
  }
}

void gsNetServer::send_simple_message(unsigned char msgid, PlayerID player_id, bool broadcast)
{
  BitStream bs;
  net->create_timestamped_bitstream_packet(&bs);
  bs.Write((unsigned char)msgid);
  interface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, player_id, broadcast);
}

void gsNetServer::menu_send_players_to_client(Packet *packet)
{
  for (int n=0; n < MAX_NET_PLAYERS; n++)
    if (net->get_netplayer(n) && net->get_netplayer(n)->net_id != packet->playerId)
    {
      BitStream p;
      // id
      p.Write((unsigned char)ID_MENU_SEND_PLAYER);

      // player data
      net->write_string_to_bitstream(net->get_netplayer(n)->name, &p);
      net->write_string_to_bitstream(net->get_netplayer(n)->ship_file, &p);

      // sender id
      p.Write((char*)&net->get_netplayer(n)->net_id, sizeof(PlayerID));

      // server id (not really needed)
      p.Write((char*)&our_id, sizeof(PlayerID));

      // send to player which sent us the originating packet
      int ret = interface->Send(&p, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, 0);
      if (!ret)
        errors->log(2, "net server", "menu send ship failed", ret);
      else
        errors->log(2, "net server", "ship sent to client", packet->playerId.binaryAddress);
    }  
}

void gsNetServer::menu_send_shipfiles_to_client(Packet *packet)
{
  for (int n=0; n < MAX_NET_PLAYERS; n++)
    if (net->get_netplayer(n) && net->get_netplayer(n)->net_id != packet->playerId)
    {
      BitStream sp;
      net->create_shipfile_packet(&sp, net->get_netplayer(n)->net_id);

      // send to player which sent us the originating packet
      int ret = interface->Send(&sp, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, 0);
      if (!ret)
        errors->log(2, "net server", "menu send shipfiles failed", ret);
      else
        errors->log(2, "net server", "shipfiles sent to client", packet->playerId.binaryAddress);
    }
}

void gsNetServer::menu_send_our_shipfile_to_clients()
{
  BitStream sp;
  net->create_shipfile_packet(&sp);

  // send to all players
  int ret = interface->Send(&sp, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true);
  if (!ret)
    errors->log(2, "net server", "menu send OUR shipfiles failed", ret);
  else
    errors->log(2, "net server", "OUR shipfiles sent to all clients", ret);
}

void gsNetServer::menu_server_receive_player(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  p.Read(typeId);

  char tmpname[50];
  char tmpship[50];
  PlayerID server_id, sender_id;

  // read player data
  net->write_bitstream_to_string(&tmpname[0], &p);
  net->write_bitstream_to_string(&tmpship[0], &p);
  
  // originating client's id
  p.Read((char*)&sender_id, sizeof(PlayerID));
  // if client id is 0, then replace with client's real id
  if (sender_id.binaryAddress == 0)
  {
    errors->log(2, "net server", "received player, id was 0, replacing with", packet->playerId.binaryAddress);
    sender_id = packet->playerId;
  }

  // read our own id (as server) and store it
  p.Read((char*)&server_id, sizeof(PlayerID));
  //our_id = server_id; //net->get_netplayer(0)->net_id = our_id; (deprecated: server is always 0)

  // create the netplayer
  gsNetPlayer *pl = net->menu_create_netplayer(tmpname, tmpship, packet->playerId);

  errors->log(2, "net server", "ship created", packet->playerId.binaryAddress);

  // create new packet
  BitStream bs;
  bs.Write(typeId);
  net->write_string_to_bitstream(&tmpname[0], &bs);
  net->write_string_to_bitstream(&tmpship[0], &bs);
  bs.Write((char*)&sender_id, sizeof(PlayerID));
  bs.Write((char*)&server_id, sizeof(PlayerID));
  // send the packet to all other clients
  interface->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, true);
  //int ret = interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, true);
}

void gsNetServer::menu_server_receive_shipfile(Packet *packet)
{
  // receive and create the file on disk
  net->receive_shipfile(packet);

  // relay the packet to all other clients
  int ret = interface->Send((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, true);
}

void gsNetServer::menu_server_kill_player(Packet *packet)
{
  // delete the netplayer
  net->menu_delete_netplayer(packet->playerId);

  errors->log(2, "net server", "player deleted", packet->playerId.binaryAddress / 10000);

  // create a new ship deletion package
  BitStream p;
  p.Write((unsigned char)ID_MENU_KILL_PLAYER);
  p.Write((char*)&packet->playerId, sizeof(PlayerID));

  // broadcast the message to all clients EXCEPT to originating one (packet->playerId);
  int ret = interface->Send(&p, HIGH_PRIORITY, RELIABLE, 0, packet->playerId, true);
}

void gsNetServer::menu_server_update_clients()
{
  // if no clients, just return
  if (interface->GetConnectedPlayers() == 0) return;

  BitStream p;

  // ID
  p.Write((unsigned char)ID_MENU_UPDATE);

  // player count
  p.Write((unsigned char)net->get_netplayers());
  
  // all player names and in_game state
  for (int n=0; n < MAX_NET_PLAYERS; n++)
    if (net->get_netplayer(n))
    {
      p.Write((char*)&net->get_netplayer(n)->net_id, sizeof(PlayerID));
      net->write_string_to_bitstream(net->get_netplayer(n)->name, &p);
      //p.Write((bool)net->get_netplayer(n)->ready);
      p.Write((bool)net->get_netplayer(n)->in_game);
    }

  // globals
  p.Write(globals->gravity_percent);
  p.Write(globals->extra_amount);
  p.Write(globals->max_frags);
  p.Write(globals->max_time);
  p.Write(globals->activate_extra);
  p.Write(globals->capture_the_flag);
  p.Write(globals->hunter_hunted);
  p.Write((bool)net->get_server_in_game()); // are we in game?
  
  // levelfile name of current level
  net->write_string_to_bitstream(globals->level_files[globals->selected_level].levelfile, &p);
  
  // send to all clients
  int ret = interface->Send(&p, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);

  //errors->log(2, "net server", "sent menu globals update", ret);
}

void gsNetServer::menu_server_receive_update(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;

  p.Read(typeId);

  gsNetPlayer *myplayer = net->get_netplayer_by_id(packet->playerId);
  if (myplayer) 
  {
    // player name
    net->write_bitstream_to_string(&myplayer->name[0], &p);

    // player ready?
    p.Read(myplayer->ready);

    // player in game?
//    p.Read(myplayer->in_game);
  }
}

void gsNetServer::game_send_set_player_home(PlayerID pid, int x, int y, int base)
{
  BitStream p;
  net->create_timestamped_bitstream_packet(&p);
  
  // id
  p.Write((unsigned char)ID_GAME_SET_PLAYER_HOME);

  // player id
  p.Write((char*)&pid, sizeof(PlayerID));

  // x position
  p.Write(x);

  // y position
  p.Write(y);

  // homebase number
  p.Write(base);

  // elapsed gametime
  p.Write((unsigned long)globals->game_time);

  // send to client
  interface->Send(&p, HIGH_PRIORITY, RELIABLE, 0, pid, false);

  errors->log(2, "sent home location to client (base num, client id)", base, pid.binaryAddress / 10000);
}



void gsNetServer::game_server_receive_start_game(Packet *packet)
{
  gsNetPlayer *myplayer = net->get_netplayer_by_id(packet->playerId);
  if (myplayer)
  {
    HomeLocation *myloc = NULL;
    myplayer->in_game = true;
    
    // find a home location
    bool found = false;
    for (int n=1; n < globals->level_files[globals->selected_level].max_players; n++)
    {
      myloc = net->get_home_location(n);
      if (myloc && myloc->id == 0) // this one is free
      {
        myloc->id = packet->playerId.binaryAddress / 10000;
        found = true;

        // send to this client
        game_send_set_player_home(packet->playerId, myloc->x, myloc->y, myloc->num);

        // add player locally
        net->add_player_to_game(myplayer->net_id);

        // set player home
        myplayer->player->set_homebase(myloc->num);
        myplayer->player->set_home_x(myloc->x);
        myplayer->player->set_home_y(myloc->y);
        myplayer->player->return_home();
        errors->log(2, "position", myloc->num, myloc->x);
        break;
      }
    }
    
    myplayer->in_game = false; // really ingame has to wait until "I'm in" message comes in

    if (!found)
      errors->log(1, "net server", "ERROR: no home location available for client!!", packet->playerId.binaryAddress / 10000);
  }
  else
    errors->log(1, "net server", "game_server_receive_start_game ERROR: id is not valid!", packet->playerId.binaryAddress);
}

void gsNetServer::game_server_receive_i_am_in(Packet *packet)
{
  if (!net->get_server_in_game()) return;

  gsNetPlayer *myplayer = net->get_netplayer_by_id(packet->playerId);
  if (myplayer)
  {
    // set this player to be fully "in game"
    myplayer->in_game = true;

    // send active players
    BitStream p;
    
    // id
    p.Write((unsigned char)ID_GAME_ADD_PLAYER);

    // player count
    p.Write((char)globals->players);

    for (int n=0; n < net->get_netplayers(); n++)
    {
      if (net->get_netplayer(n)->in_game || n == 0)
      {
        PlayerID pid = net->get_netplayer(n)->net_id;
        
        // pid
        p.Write((char*)&pid, sizeof(PlayerID));

        // x position
        p.Write((int)net->get_netplayer(n)->player->get_home_x());

        // y position
        p.Write((int)net->get_netplayer(n)->player->get_home_y());

        // homebase number
        p.Write((char)net->get_netplayer(n)->player->get_homebase());

        errors->log(2, "net server", "receive_i_am_in is sending active player no. pid", pid.binaryAddress / 10000);
      }
    }

    // send to all players!
    net->send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID);
  }
  else
    errors->log(1, "net server", "game_server_i_am_in ERROR: id is not valid!", packet->playerId.binaryAddress);
}

void gsNetServer::game_send_quit()
{
  BitStream p;

  // id
  p.Write((unsigned char)ID_GAME_QUIT);
  
  // include our id
  p.Write((char*)&our_id, sizeof(PlayerID));

  // send to all clients
  net->send_to_ingame_players(&p, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID);
}

void gsNetServer::game_receive_quit(Packet *packet)
{
  BitStream p((char*)packet->data, packet->length, false);
  unsigned char typeId;
  PlayerID player_id;

  // read id
  p.Read(typeId);

  // read player id
  p.Read((char*)&player_id, sizeof(PlayerID));

  // kick player locally
  net->delete_player_from_game(player_id);
  
  // relay packet to all other clients
  net->send_to_ingame_players((char*)packet->data, packet->length, HIGH_PRIORITY, RELIABLE, 0, packet->playerId); 
}

void gsNetServer::game_send_server_misc()
{
  BitStream p;
  
  // id
  p.Write((unsigned char)ID_GAME_SERVER_MISC);

  // game time
  p.Write((int)globals->second_time);

  // send to all
  interface->Send(&p, LOW_PRIORITY, UNRELIABLE_SEQUENCED, 0, UNASSIGNED_PLAYER_ID, true);
}


void gsNetServer::game_log_netstats(PlayerID pid)
{
  RakNetStatisticsStruct *mystats;
  mystats = interface->GetStatistics(pid);
  if (!mystats) return;

  char buffer[4000];
  StatisticsToString(mystats, buffer, 2);
  gsNetPlayer *mypl = net->get_netplayer_by_id(pid);
  if (!mypl) return;
  errors->log(2, mypl->name, buffer);
}
