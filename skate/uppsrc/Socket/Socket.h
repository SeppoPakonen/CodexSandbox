#ifndef _Socket_Socket_h_
#define _Socket_Socket_h_

#ifdef flagPOSIX
	#include <sys/socket.h>
	#include <netdb.h>
#endif

#include <Dolphin/dolphin.h>


// Unknown values
#define SO_INADDR_ANY 0
#define SO_EWOULDBLOCK 0

enum {
	GP_NEWUSER_BAD_PASSWORD,
	GP_NEWUSER_BAD_NICK,
	GP_ADDBUDDY_ALREADY_BUDDY,
	GP_LOGIN_CONNECTION_FAILED,
	GP_FIREWALL,
	GP_NON_BLOCKING,
};

// Errors
enum {
	GP_NO_ERROR,
	GP_ERROR,
	GP_MEMORY_ERROR,
	GP_PARAMETER_ERROR,
	GP_NETWORK_ERROR,
	GP_SERVER_ERROR,
};

// Status
enum {
	GP_ONLINE,
	GP_CHATTING,
};

// ???
enum {
	GP_RECV_BUDDY_REQUEST,
	GP_RECV_BUDDY_STATUS,
	GP_RECV_BUDDY_MESSAGE,
	GP_RECV_GAME_INVITE,
	GP_TRANSFER_CALLBACK,
};



typedef int GPProfile;
typedef int GPResult;
typedef int GPEnum;

struct GPConnection {
	
};

struct GPErrorArg {
	int errorCode;
	const char* errorString;
	bool fatal;
	GPProfile* profile;
	
};

struct GPRecvBuddyStatusArg {
	int index;
	
};

struct GPRecvBuddyRequestArg {
	GPProfile* profile;
	
};

struct GPConnectResponseArg {
	int result;
	GPProfile profile;
	
};

struct GPBuddyStatus {
	int status;
	const char* statusString;
	const char* locationString;
	int profile;
	
};


typedef void (*ConnectCallback)(GPConnection* connection, void* arg, void* param );


void gpProcess(GPConnection* conn);
void gpGetBuddyStatus(GPConnection* conn, int index, GPBuddyStatus* status);
void gpAuthBuddyRequest(GPConnection* conn, GPProfile* status);
GPResult gpConnect(GPConnection* conn, const char* name, const char* email, const char* pass, int, int, ConnectCallback, void* cb_arg);
GPResult gpConnectNewUser(GPConnection* conn, const char* name, const char* email, const char* pass, int, int, ConnectCallback, void* cb_arg);
GPResult gpDisconnect(GPConnection* conn);
GPResult gpSendBuddyRequest(GPConnection* conn, int profile, const char* msg);
GPResult gpDeleteBuddy(GPConnection* conn, int profile);
GPResult gpInitialize(GPConnection* conn, int game_id);
GPResult gpSetCallback(GPConnection* conn, int code, ConnectCallback cb_ptr, void* arg);
GPResult gpSetStatus(GPConnection* conn, int status, const char* a, const char* b);

enum {
	SO_PF_INET
};

struct SOSockAddr {
	u32 addr;
	u32 len;
	
};

struct SOInAddr {
	u32 addr;
	
};

struct SOSockAddrIn {
	SOSockAddr addr;
	u32 family;
	u32 port;
	u32 len;
	
};



struct MessageType {
	
};


struct qr2_keybuffer_t {
	
};

struct qr2_buffer_t {
	
};


typedef bool PEERBool;
typedef int PEER;
typedef int RoomType;
typedef int PEERJoinResult;
typedef int NegotiateState;
typedef int NegotiateResult;

typedef int qr2_key_type;
typedef int qr2_error_t;

void qr2_buffer_add(qr2_buffer_t buf, const char* server_name);
void qr2_buffer_add_int(qr2_buffer_t buf, int i);
void qr2_keybuffer_add(qr2_keybuffer_t buf, int i);
void qr2_register_key(int key, const char* desc);

enum {
	key_server,
	key_player,
	key_team,
};

struct SBServer {
	
};

#define SO_VENDOR_NINTENDO	0
#define SO_VERSION			0

enum {
	SO_FLAG_DHCP		= (1 << 0),
	
};

typedef void* (*SOAllocator)( u32 name, s32 size );
typedef void (*SOFree)( u32 name, void* prt, s32 size );

struct SOConfig {
	int vendor;
	int version;
	u32 flag;
	u32 addr;
	u32 netmask;
	u32 router;
	u32 dns1;
	u32 dns2;
	SOAllocator alloc;
	SOFree free;
	
};

struct hostent;

int SBServerGetIntValue(SBServer server, const char* key, int def);

int SOStartup(SOConfig* conf);
int SOCleanup();
int SOConnect(int sockfd, const SOSockAddr *addr);
int SORecvFrom(int sockfd, void* buf, u32 size, int flags, const SOSockAddr *addr);
int SORecv(int sockfd, void* buf, u32 size, int flags);
int SOClose(int sockfd);
int SOAccept(int sockfd, SOSockAddr* addr);
int SOBind(int sockfd, SOSockAddr* addr);
int SOGetPeerName(int sockfd, SOSockAddr* addr);
int SOGetSockName(int sockfd, SOSockAddr* addr);
int SOGetSockOpt(int sockfd, int level, int optname, void* optval);
int SOListen(int sockfd, int backlog);
int SOSend(int sockfd, const void* buf, int len, u32 flags);
int SOSendTo(int sockfd, const void* buf, int len, u32 flags, SOSockAddr* to);
int SOSetSockOpt(int sockfd, int level, int optname, const void* optval);
int SOShutdown(int sockfd, int how);
int SOSocket(int family, int type, int proto);
hostent* SOGetHostByName(const char* name);

u32 SOHtoNs(u32 ip);
u32 SONtoHs(u32 ip);
u32 SONtoHl(u32 ip);
u32 SOHtoNl(u32 ip);
u32 SOHtoHl(u32 ip);
char* SOInetNtoA(SOInAddr addr);
char* SOInetNtoA(SOSockAddr addr);
int SOInetAtoN(const char* cp, SOInAddr* addr);
int SOInetAtoN(const char* cp, SOSockAddr* addr);
int SOFcntl(int sockfd, int key, int value);

int sockAPIderegother(int thread_id);

enum {
	SO_F_GETFL,
	SO_F_SETFL,
	SO_O_NONBLOCK,
	SO_EADDRINUSE,
	
};

#include "PS.h"


#endif
