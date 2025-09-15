#include "Socket.h"


int ne_noerror;
const char* pd_public_ro;

PEER peerInitialize(PEERCallbacks* cb) {
	TODO
}

void peerShutdown(PEER peer) {
	TODO
}

void peerEnumPlayers(PEER peer, int room, ConnectCallback3 cb, void* arg) {
	TODO
}

unsigned int peerGetLocalIP(PEER peer) {
	TODO
}

void peerConnect(PEER peer, const char* lobby_name, int profile, ErrorCallback,
	ConnectCallback2, void*, bool) {
	TODO
}

bool peerGetPlayerInfoNoWait(PEER, const char*, void*, int* profile) {
	TODO
}

u32 peerGetPrivateIP(PEER peer) {
	TODO
}

void peerRetryWithNick(PEER peer, const char* nick) {
	TODO
}

void chatFixNick(PEER peer, const char* nick) {
	TODO
}

void chatFixNick(const char* new_nick, const char* nick) {
	TODO
}

const char* peerGetNick(PEER peer) {
	TODO
}

void peerSetRoomKeys(PEER peer, int roomtype, const char* nick, int limit, const char** keys, const char** values) {
	TODO
}

void peerGetRoomKeys(PEER peer, int roomtype, const char* match, int limit, const char** keys, RoomKeysCb, void*, int) {
	TODO
}

void peerThink(PEER peer) {
	TODO
}

void peerStateChanged(PEER peer) {
	TODO
}

void peerStopGame(PEER peer) {
	TODO
}

void peerParseQuery(PEER peer, const char* data, int data_len, SOSockAddr* sender) {
	TODO
}

PEERBool peerStartReportingWithSocket(PEER peer, int socket, int host_port) {
	TODO
}

void peerListGroupRooms(PEER peer, const char* rooms, GroupRoomsCb, void*, bool) {
	TODO
}

void peerStopListingGames(PEER peer) {
	TODO
}

void peerSendNatNegotiateCookie(PEER peer, u32 addr, u32 port, u32 cookie) {
	TODO
}

void peerJoinGroupRoom(PEER peer, int id, JoinRoomCb, void*, bool) {
	TODO
}

void peerLeaveRoom(PEER peer, int room, int) {
	TODO
}

void peerSetGroupID(PEER peer, int lobby_id) {
	TODO
}

void peerClearTitle(PEER peer) {
	TODO
}

void peerDisconnect(PEER peer) {
	TODO
}

void peerSetTitle(PEER peer, const char* title, const char* key, const char* title2, const char* key2, int version, int, bool, PEERBool*, PEERBool*) {
	TODO
}

void peerSetQuietMode(PEER peer, bool value) {
	TODO
}

void peerMessageRoom(PEER peer, int room, const char* text, int msg) {
	TODO
}



int NNBeginNegotiationWithSocket(SOCKET, int cookie, int, NegiotiateProgress, NegiotiateComplete, void* arg) {
	TODO
}

void NNCancel(int cookie) {
	TODO
}

void NNThink() {
	TODO
}

void NNFreeNegotiateList() {
	TODO
}

void NNProcessData(const char* data, int data_len, SOSockAddrIn* addr) {
	TODO
}



void ghttpCancelRequest(GHTTPRequest req) {
	TODO
}

void ghttpThink() {
	TODO
}

void ghttpCleanup() {
	TODO
}

GHTTPRequest ghttpGetEx(
	const char* dl_path,
	const char*,
	char* buffer,
	int,
	void*,
	int,
	int,
	ProgressCallback,
	CompletedCallback,
	void* arg) {
	TODO
}

GHTTPRequest ghttpPostEx(
	const char* url,
	void*,
	GHTTPPost post,
	int,
	int,
	ProgressCallback,
	CompletedCallback,
	void* arg) {
	TODO
}

GHTTPPost ghttpNewPost() {
	TODO
}

void ghttpPostAddString(GHTTPPost post, const char* key, const char* value) {
	TODO
}

void ghttpPostAddFileFromMemory(GHTTPPost post, const char* fname, char* buf, int size, char* fname2, void*) {
	TODO
}

const char* GOAGetUniqueID() {
	TODO
}


void GetPersistDataValues(
	int,
	int profile,
	const char* pub_ro,
	int,
	const char* rooms,
	StatsRetrievalCallback,
	void* arg) {
	TODO
}



int sceSifSearchModuleByName(const char* name) {
	TODO
}

int sceSifBindRpc(sceSifClientData* data, u32 rpc_id, int value) {
	TODO
}

int sceSifInitRpc(int) {
	TODO
}

int sceSifCallRpc(sceSifClientData* data, int fn, int arg, int** buf0, int size0, int** buf1, int size1, int, int) {
	TODO
}




