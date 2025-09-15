#include "Socket.h"


void gpProcess(GPConnection* conn) {
	TODO
}

void gpGetBuddyStatus(GPConnection* conn, int index, GPBuddyStatus* status) {
	TODO
}

void gpAuthBuddyRequest(GPConnection* conn, GPProfile* status) {
	TODO
}

GPResult gpConnect(GPConnection* conn, const char* name, const char* email, const char* pass, int, int, ConnectCallback, void* cb_arg) {
	TODO
}

GPResult gpConnectNewUser(GPConnection* conn, const char* name, const char* email, const char* pass, int, int, ConnectCallback, void* cb_arg) {
	TODO
}

GPResult gpDisconnect(GPConnection* conn) {
	TODO
}

GPResult gpSendBuddyRequest(GPConnection* conn, int profile, const char* msg) {
	TODO
}

GPResult gpDeleteBuddy(GPConnection* conn, int profile) {
	TODO
}

GPResult gpInitialize(GPConnection* conn, int game_id) {
	TODO
}

GPResult gpSetCallback(GPConnection* conn, int code, ConnectCallback cb_ptr, void* arg) {
	TODO
}

GPResult gpSetStatus(GPConnection* conn, int status, const char* a, const char* b) {
	TODO
}






void qr2_buffer_add(qr2_buffer_t buf, const char* server_name) {
	TODO
}

void qr2_buffer_add_int(qr2_buffer_t buf, int i) {
	TODO
}

void qr2_keybuffer_add(qr2_keybuffer_t buf, int i) {
	TODO
}

void qr2_register_key(int key, const char* desc) {
	TODO
}






int SBServerGetIntValue(SBServer server, const char* key, int def) {
	TODO
}


int SOStartup(SOConfig* conf) {
	TODO
}

int SOCleanup() {
	TODO
}

int SOConnect(int sockfd, const SOSockAddr *addr) {
	TODO
}

int SORecvFrom(int sockfd, void* buf, u32 size, int flags, const SOSockAddr *addr) {
	TODO
}

int SORecv(int sockfd, void* buf, u32 size, int flags) {
	TODO
}

int SOClose(int sockfd) {
	TODO
}

int SOAccept(int sockfd, SOSockAddr* addr) {
	TODO
}

int SOBind(int sockfd, SOSockAddr* addr) {
	TODO
}

int SOGetPeerName(int sockfd, SOSockAddr* addr) {
	TODO
}

int SOGetSockName(int sockfd, SOSockAddr* addr) {
	TODO
}

int SOGetSockOpt(int sockfd, int level, int optname, void* optval) {
	TODO
}

int SOListen(int sockfd, int backlog) {
	TODO
}

int SOSend(int sockfd, const void* buf, int len, u32 flags) {
	TODO
}

int SOSendTo(int sockfd, const void* buf, int len, u32 flags, SOSockAddr* to) {
	TODO
}

int SOSetSockOpt(int sockfd, int level, int optname, const void* optval) {
	TODO
}

int SOShutdown(int sockfd, int how) {
	TODO
}

int SOSocket(int family, int type, int proto) {
	TODO
}

hostent* SOGetHostByName(const char* name) {
	TODO
}


u32 SOHtoNs(u32 ip) {
	TODO
}

u32 SONtoHs(u32 ip) {
	TODO
}

u32 SONtoHl(u32 ip) {
	TODO
}

u32 SOHtoNl(u32 ip) {
	TODO
}

u32 SOHtoHl(u32 ip) {
	TODO
}

char* SOInetNtoA(SOInAddr addr) {
	TODO
}

char* SOInetNtoA(SOSockAddr addr) {
	TODO
}

int SOInetAtoN(const char* cp, SOInAddr* addr) {
	TODO
}

int SOInetAtoN(const char* cp, SOSockAddr* addr) {
	TODO
}

int SOFcntl(int sockfd, int key, int value) {
	TODO
}


int sockAPIderegother(int thread_id) {
	TODO
}

