#ifndef _Socket_PS_h_
#define _Socket_PS_h_

#define QR_MAGIC_1			0x80085FAC
#define QR_MAGIC_2			0x07177135
#define NATNEG_MAGIC_LEN	4
#define NN_MAGIC_0			'F'
#define NN_MAGIC_1			'S'
#define NN_MAGIC_2			'p'
#define NN_MAGIC_3			'e'
#define NN_MAGIC_4			'e'
#define NN_MAGIC_5			'd'

// unknown
#define NumRooms			64
#define SSHOST				"host"

enum {
	GroupRoom
};

enum {
	NormalMessage
};

enum {
	PEERFullRoom,
	PEERInviteOnlyRoom,
	PEERBannedFromRoom,
	PEERBadPassword,
	PEERAlreadyInRoom,
	PEERNoTitleSet,
	PEERJoinFailed,
};

enum {
	HOSTNAME_KEY,
	GAMENAME_KEY,
	GAMEMODE_KEY,
	HOSTPORT_KEY,
	MAPNAME_KEY,
	GAMETYPE_KEY,
	TEAMPLAY_KEY,
	NUMPLAYERS_KEY,
	MAXPLAYERS_KEY,
	PASSWORD_KEY,
	NUMOBSERVERS_KEY,
	MAXOBSERVERS_KEY,
	SKILLLEVEL_KEY,
	STARTED_KEY,
	HOSTED_MODE_KEY,
	RANKED_KEY,
};

enum {
	PEER_IN_USE,
	PEER_INVALID,
};

enum {
	nr_success,
	nr_deadbeatpartner,
	nr_inittimeout,
	nr_unknownerror,
};

enum {
	ns_initack,
	ns_connectping,
};

typedef enum {
	GHTTPReceivingFile,
	GHTTPPosting,
	
} GHTTPState;

typedef enum {
	GHTTPFalse,
	GHTTPTrue,
	GHTTPSuccess,
	
} GHTTPResult;

enum {
	GHTTPFileNotFound,
	
};

typedef int NegotiateError;
typedef int SOCKET;
//typedef int GHTTPResult;
//typedef int GHTTPState;
typedef bool GHTTPBool;

struct GHTTPRequest {};
struct GHTTPPost {};

typedef void (*ErrorCallback)(PEER, int, const char*, void*);
typedef void (*ConnectCallback2)(PEER, PEERBool, void*);
typedef void (*ConnectCallback3)(PEER, PEERBool, int, int, const char*, int, void*);
typedef void (*NegiotiateProgress)(NegotiateState, void*);
typedef void (*NegiotiateComplete)(NegotiateResult, SOCKET, SOSockAddrIn*, void*);
typedef GHTTPBool (*ghttpCompletedCallback) (
	GHTTPRequest request,
	GHTTPResult result,
	char* buffer,
	int buffer_len,
	void * param
	);
typedef void (*RoomKeysCb)(PEER,bool,int,const char*,int,char**,char**,void*);
typedef void (*GroupRoomsCb)(PEER,PEERBool,int,SBServer,const char*,int,int,int,int,void*);
typedef void (*JoinRoomCb)(PEER,PEERBool,PEERJoinResult,RoomType,void*);

typedef void (*DisconnectCallback)(PEER, const char*, void*);
typedef void (*RoomMessageCallback)(PEER, int, const char*, const char*, MessageType, void*);
typedef void (*PlayerMessageCallback)(PEER, const char*, const char*, MessageType, void*);
typedef void (*PlayerJoinedCallback)(PEER, RoomType, const char*, void*);
typedef void (*PlayerLeftCallback)(PEER, RoomType, const char*, const char*, void*);
typedef void (*PlayerInfoCallback)(PEER, RoomType, const char*, unsigned int, int, void*);
typedef void (*NewPlayerListCallback)(PEER, RoomType, void*);
typedef void (*RoomKeyCallback)(PEER, RoomType, const char*, const char*, const char*, void*);
typedef void (*NatNegotiateCallback)(PEER, int, void*);
typedef void (*KeyListCallback)(PEER, qr2_key_type, qr2_keybuffer_t, void*);
typedef void (*ServerKeyCallback)(PEER, int, qr2_buffer_t, void*);
typedef void (*PlayerKeyCallback)(PEER, int, int, qr2_buffer_t, void*);
typedef int  (*KeyCountCallback)(PEER, qr2_key_type, void*);
typedef void (*AddErrorCallback)(PEER, qr2_error_t, char*, void*);
typedef void (*PublicAddrCallback)(PEER, unsigned int, unsigned short, void*);

typedef void (*ProgressCallback)(GHTTPRequest, GHTTPState, const char*, int, int, int, void*);
typedef GHTTPBool (*CompletedCallback)(GHTTPRequest, GHTTPResult, char*, int, void*);

typedef void (*StatsRetrievalCallback)(int,int,persisttype_t,int,int,char*,int,void*);

struct PEERCallbacks {
	DisconnectCallback		disconnected;
	RoomMessageCallback		roomMessage;
	PlayerMessageCallback	playerMessage;
	PlayerJoinedCallback	playerJoined;
	PlayerLeftCallback		playerLeft;
	PlayerInfoCallback		playerInfo;
	NewPlayerListCallback	newPlayerList;
	RoomKeyCallback			roomKeyChanged;
	
	NatNegotiateCallback	qrNatNegotiateCallback;
	KeyListCallback			qrKeyList;
	ServerKeyCallback		qrServerKey;
	PlayerKeyCallback		qrPlayerKey;
	KeyCountCallback		qrCount;
	AddErrorCallback		qrAddError;
	PublicAddrCallback		qrPublicAddressCallback;
	
	void* param;
	
};

PEER peerInitialize(PEERCallbacks* cb);
void peerShutdown(PEER peer);
void peerEnumPlayers(PEER peer, int room, ConnectCallback3 cb, void* arg);
unsigned int peerGetLocalIP(PEER peer);
void peerConnect(PEER peer, const char* lobby_name, int profile, ErrorCallback,
	ConnectCallback2, void*, bool);
bool peerGetPlayerInfoNoWait(PEER, const char*, void*, int* profile);
u32 peerGetPrivateIP(PEER peer);
void peerRetryWithNick(PEER peer, const char* nick);
void chatFixNick(PEER peer, const char* nick);
void chatFixNick(const char* new_nick, const char* nick);
const char* peerGetNick(PEER peer);
void peerSetRoomKeys(PEER peer, int roomtype, const char* nick, int limit, const char** keys, const char** values);
void peerGetRoomKeys(PEER peer, int roomtype, const char* match, int limit, const char** keys, RoomKeysCb, void*, int);
void peerThink(PEER peer);
void peerStateChanged(PEER peer);
void peerStopGame(PEER peer);
void peerParseQuery(PEER peer, const char* data, int data_len, SOSockAddr* sender);
PEERBool peerStartReportingWithSocket(PEER peer, int socket, int host_port);
void peerListGroupRooms(PEER peer, const char* rooms, GroupRoomsCb, void*, bool);
void peerStopListingGames(PEER peer);
void peerSendNatNegotiateCookie(PEER peer, u32 addr, u32 port, u32 cookie);
void peerJoinGroupRoom(PEER peer, int id, JoinRoomCb, void*, bool);
void peerLeaveRoom(PEER peer, int room, int);
void peerSetGroupID(PEER peer, int lobby_id);
void peerClearTitle(PEER peer);
void peerDisconnect(PEER peer);
void peerSetTitle(PEER peer, const char* title, const char* key, const char* title2, const char* key2, int version, int, bool, PEERBool*, PEERBool*);
void peerSetQuietMode(PEER peer, bool value);
void peerMessageRoom(PEER peer, int room, const char* text, int msg);


int NNBeginNegotiationWithSocket(SOCKET, int cookie, int, NegiotiateProgress, NegiotiateComplete, void* arg);
void NNCancel(int cookie);
void NNThink();
void NNFreeNegotiateList();
void NNProcessData(const char* data, int data_len, SOSockAddrIn* addr);


void ghttpCancelRequest(GHTTPRequest req);
void ghttpThink();
void ghttpCleanup();
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
	void* arg);
GHTTPRequest ghttpPostEx(
	const char* url,
	void*,
	GHTTPPost post,
	int,
	int,
	ProgressCallback,
	CompletedCallback,
	void* arg);
GHTTPPost ghttpNewPost();
void ghttpPostAddString(GHTTPPost post, const char* key, const char* value);
void ghttpPostAddFileFromMemory(GHTTPPost post, const char* fname, char* buf, int size, char* fname2, void*);
const char* GOAGetUniqueID();

void GetPersistDataValues(
	int,
	int profile,
	const char* pub_ro,
	int,
	const char* rooms,
	StatsRetrievalCallback,
	void* arg);






extern int ne_noerror;
extern const char* pd_public_ro;





#define SIF_RPCM_NOWAIT 0

struct sceSifClientData {
	int serve;
	
};

int sceSifSearchModuleByName(const char* name);
int sceSifBindRpc(sceSifClientData* data, u32 rpc_id, int value);
int sceSifInitRpc(int);
int sceSifCallRpc(sceSifClientData* data, int fn, int arg, int** buf0, int size0, int** buf1, int size1, int, int);

#endif
