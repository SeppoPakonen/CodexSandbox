#include "dolphin.h"
#include <Core/Core.h>

FifoBase GXWGFifo;


int stricmp(const char* s1, const char* s2) {
	assert(s1 != NULL);
	assert(s2 != NULL);
	
#ifdef HAVE_STRCASECMP
	return strcasecmp(s1, s2)
#else
	while (tolower((unsigned char) *s1) == tolower((unsigned char) *s2)) {
		if (*s1 == '\0')
			return 0;
		s1++; s2++;
	}
	
	return (int) tolower((unsigned char) *s1) -
		   (int) tolower((unsigned char) *s2);
#endif
}

char *strupr(char *str) {
	TODO
}

void StartThread(int arg, void* obj) {TODO}
int GetThreadId() {TODO}

int DI() {TODO}
int EI() {TODO}

char* gcd_gamename;
char* gcd_secret_key;
char* bo_set;
char* bl_server;
char* bl_player;

int PersistThink() {TODO}

int InitStatsConnection(int port) {TODO}
int IsStatsConnected() {TODO}
void CloseStatsConnection() {TODO}
statsgame_t NewGame(int) {TODO}
void FreeGame(statsgame_t) {TODO}
const char* GetChallenge(statsgame_t game) {TODO}
char* GenerateAuth(char* challenge, char* password, char* response) {TODO}
void NewPlayer(statsgame_t game, const char* name) {TODO}
void NewPlayer(statsgame_t game, int id, const char* name) {TODO}
void RemovePlayer(statsgame_t game, int id) {TODO}
void BucketStringOp(statsgame_t game, const char* key, const char* set, const char* value, const char* server, int) {TODO}
void BucketIntOp(statsgame_t game, const char* key, const char* set, int value, const char* server, int) {TODO}
int SendGameSnapShot(statsgame_t game, void*, int) {TODO}

/*void __hwInitForEmu() {TODO}
void AXSetMode(int mode) {TODO}
void AXSetCompressor(int mode) {TODO}
void DTKInit() {TODO}
void DTKSetRepeatMode(int mode) {TODO}*/

void ReferThreadStatus(int thread_id, ThreadParam* info) {TODO}
void ChangeThreadPriority(int thread_id, int prio) {TODO}

int sceDNAS2InitNoHDD(sceDNAS2TitleAuthInfo_t* auth_info, int, int, int, sceDNAS2TimeoutInfo_t* to) {TODO}
int sceDNAS2GetStatus(sceDNAS2Status_t* status) {TODO}
int sceDNAS2SetProxy(int, const char *proxy_host, unsigned short port) {TODO}
int sceDNAS2GetProxy(int* state_check, char *proxy_host, int len, unsigned short* port) {TODO}
int sceDNAS2AuthNetStart() {TODO}
int sceDNAS2Shutdown() {TODO}
int sceCdInit(int) {TODO}
int sceCdMmode(int) {TODO}
int sceCdDiskReady(int) {TODO}
int sceSifLoadModule(char* buf, int arg_count, char* args) {TODO}
int sceSifInitIopHeap() {TODO}
int scePadInit(int) {TODO}
int scePadGetSlotMax(int) {TODO}
int scePadEnd() {TODO}


int sn_errno(int sock_id) {TODO}
int sn_h_errno() {TODO}



