#ifndef _Dolphin_compat_h_
#define _Dolphin_compat_h_

char *strupr(char *str);
int stricmp(const char* s1, const char* s2);
       
#define strcmpi stricmp


// Unknown
enum {
	GP_NULL,
	GP_PLAYING
};

unsigned int PtrIndex_FindAdd(void* ptr);



void StartThread(int arg, void* obj);
int GetThreadId();

enum {
	GE_NODNS,
	GE_NOSOCKET,
	GE_NOCONNECT,
	GE_DATAERROR,
	GE_NOERROR,
	
};

struct PersistType {
	
};

struct StatsGame {
	
};

typedef PersistType* persisttype_t;
typedef StatsGame* statsgame_t;

int PersistThink();


extern char* gcd_gamename;
extern char* gcd_secret_key;
extern char* bo_set;
extern char* bl_server;
extern char* bl_player;

int InitStatsConnection(int port);
int IsStatsConnected();
void CloseStatsConnection();
statsgame_t NewGame(int);
void FreeGame(statsgame_t);
const char* GetChallenge(statsgame_t game);
char* GenerateAuth(char* challenge, char* password, char* response);
void NewPlayer(statsgame_t game, const char* name);
void NewPlayer(statsgame_t game, int id, const char* name);
void RemovePlayer(statsgame_t game, int id);
void BucketStringOp(statsgame_t game, const char* key, const char* set, const char* value, const char* server, int);
void BucketIntOp(statsgame_t game, const char* key, const char* set, int value, const char* server, int);
int SendGameSnapShot(statsgame_t game, void*, int);

struct SemaParam {
	int initCount;
	int maxCount;
	
};

int  CreateSema(SemaParam* params);
void DeleteSema(int sema_id);
void WaitSema(int sema_id);
void SignalSema(int sema_id);

// Some weird threading stuff
int DI();
int EI();




enum {
	SN_ENOTINIT			= 1000,
	SN_ETHNOTREG,
	SN_EMAXTHREAD,
	SN_EIOPNORESP,
	SN_ENOMEM,
	SN_EBINDFAIL,
	SN_EINVTHREAD,
	SN_EALRDYINIT,
	SN_ESTKDOWN,
	SN_EIFSTATE,
	SN_EDNSFAIL,
	SN_SWERROR,
	SN_EDNSBUSY,
	SN_REQSIZE,
	SN_REQINV,
	SN_REQSYNC,
	SN_RPCBAD,
	SN_NOTREADY,
	
};

int sn_errno(int sock_id);
int sn_h_errno();

/*struct hostent {
	char**			h_addr_list;
	char**			h_aliases;
	char*			h_name;
	int				h_addrtype;
	int				h_length;
	
	
};*/

void snPause();

enum {
	AX_MODE_DPL2,
};

enum {
	AX_COMPRESSOR_OFF,
};

enum {
	DTK_MODE_NOREPEAT,
	DTK_MODE_REPEAT
};

enum {
	sceDNAS2_T_INET,
	sceDNAS2_S_END,
	sceDNAS2_S_NG,
	sceDNAS2_S_COM_ERROR,
	sceDNAS2_SS_SERVER_BUSY,
	sceDNAS2_SS_BEFORE_SERVICE,
	sceDNAS2_SS_OUT_OF_SERVICE,
	sceDNAS2_SS_END_OF_SERVICE,
	sceDNAS2_SS_SESSION_TIME_OUT,
	sceDNAS2_SS_INVALID_SERVER,
	sceDNAS2_SS_INTERNAL_ERROR,
	sceDNAS2_SS_EXTERNAL_ERROR,
	sceDNAS2_SS_ID_NOUSE,
	sceDNAS2_SS_ID_NOT_JOIN_TO_CAT,
	sceDNAS2_SS_DL_NODATA,
	sceDNAS2_SS_DL_BEFORE_SERVICE,
	sceDNAS2_SS_DL_OUT_OF_SERVICE,
	sceDNAS2_SS_DL_NOT_UPDATED,
	sceDNAS2_SS_INVALID_PS2,
	sceDNAS2_SS_INVALID_HDD_BINDING,
	sceDNAS2_SS_INVALID_MEDIA,
	sceDNAS2_SS_INVALID_AUTHDATA,
	sceDNAS2_ERR_GLUE_ABORT,
	sceDNAS2_ERR_NET_PROXY,
	sceDNAS2_ERR_NET_SSL,
	sceDNAS2_ERR_NET_DNS_HOST_NOT_FOUND,
	sceDNAS2_ERR_NET_DNS_TRY_AGAIN,
	sceDNAS2_ERR_NET_DNS_NO_RECOVERY,
	sceDNAS2_ERR_NET_DNS_NO_DATA,
	sceDNAS2_ERR_NET_DNS_OTHERS,
	sceDNAS2_ERR_NET_EISCONN,
	sceDNAS2_ERR_NET_ETIMEOUT,
	sceDNAS2_ERR_NET_TIMEOUT,
	sceDNAS2_ERR_NET_ECONNREFUSED,
	sceDNAS2_ERR_NET_ENETUNREACH,
	sceDNAS2_ERR_NET_ENOTCONN,
	sceDNAS2_ERR_NET_ENOBUFS,
	sceDNAS2_ERR_NET_EMFILE,
	sceDNAS2_ERR_NET_EBADF,
	sceDNAS2_ERR_NET_EINVAL,
	sceDNAS2_ERR_NET_OTHERS,
};

enum {
	SCECdINIT,
};

enum {
	SCECdDVD,
};

struct ThreadParam {
	int currentPriority;
	
};

struct sceDNAS2TimeoutInfo_t {
	int timeout;
	int priority;
	
};

struct AuthData {
	char* ptr;
	int size;
	
};

struct PassPhrase {
	unsigned char* ptr;
	int size;
	
};

struct sceDNAS2TitleAuthInfo_t {
	AuthData auth_data;
	PassPhrase pass_phrase;
	int line_type;
	
};

struct sceDNAS2Status_t {
	int code;
	int sub_code;
	
};

void __hwInitForEmu();
void AXSetMode(int mode);
void AXSetCompressor(int mode);
void DTKInit();
void DTKSetRepeatMode(int mode);

void ReferThreadStatus(int thread_id, ThreadParam* info);
void ChangeThreadPriority(int thread_id, int prio);

int sceDNAS2InitNoHDD(sceDNAS2TitleAuthInfo_t* auth_info, int, int, int, sceDNAS2TimeoutInfo_t* to);
int sceDNAS2GetStatus(sceDNAS2Status_t* status);
int sceDNAS2SetProxy(int, const char *proxy_host, unsigned short port);
int sceDNAS2GetProxy(int* state_check, char *proxy_host, int len, unsigned short* port);
int sceDNAS2AuthNetStart();
int sceDNAS2Shutdown();
int sceCdInit(int);
int sceCdMmode(int);
int sceCdDiskReady(int);
int sceSifLoadModule(char* buf, int arg_count, char* args);
int sceSifInitIopHeap();
int scePadInit(int);
int scePadGetSlotMax(int);
int scePadEnd();

namespace std {


void OurPrintf(const char* txt);


}


/*
enum {
	TX_SETMODE0_I0_ID,
	TX_SETMODE0_I1_ID,
	TX_SETMODE0_I2_ID,
	TX_SETMODE0_I3_ID,
	TX_SETMODE0_I4_ID,
	TX_SETMODE0_I5_ID,
	TX_SETMODE0_I6_ID,
	TX_SETMODE0_I7_ID,
};

enum {
	TX_SETMODE1_I0_ID,
	TX_SETMODE1_I1_ID,
	TX_SETMODE1_I2_ID,
	TX_SETMODE1_I3_ID,
	TX_SETMODE1_I4_ID,
	TX_SETMODE1_I5_ID,
	TX_SETMODE1_I6_ID,
	TX_SETMODE1_I7_ID,
};

enum {
	TX_SETIMAGE0_I0_ID,
	TX_SETIMAGE0_I1_ID,
	TX_SETIMAGE0_I2_ID,
	TX_SETIMAGE0_I3_ID,
	TX_SETIMAGE0_I4_ID,
	TX_SETIMAGE0_I5_ID,
	TX_SETIMAGE0_I6_ID,
	TX_SETIMAGE0_I7_ID,
};

enum {
	TX_SETIMAGE1_I0_ID,
	TX_SETIMAGE1_I1_ID,
	TX_SETIMAGE1_I2_ID,
	TX_SETIMAGE1_I3_ID,
	TX_SETIMAGE1_I4_ID,
	TX_SETIMAGE1_I5_ID,
	TX_SETIMAGE1_I6_ID,
	TX_SETIMAGE1_I7_ID,
};

enum {
	TX_SETIMAGE2_I0_ID,
	TX_SETIMAGE2_I1_ID,
	TX_SETIMAGE2_I2_ID,
	TX_SETIMAGE2_I3_ID,
	TX_SETIMAGE2_I4_ID,
	TX_SETIMAGE2_I5_ID,
	TX_SETIMAGE2_I6_ID,
	TX_SETIMAGE2_I7_ID,
};

enum {
	TX_SETIMAGE3_I0_ID,
	TX_SETIMAGE3_I1_ID,
	TX_SETIMAGE3_I2_ID,
	TX_SETIMAGE3_I3_ID,
	TX_SETIMAGE3_I4_ID,
	TX_SETIMAGE3_I5_ID,
	TX_SETIMAGE3_I6_ID,
	TX_SETIMAGE3_I7_ID,
};

enum {
	TX_SETTLUT_I0_ID,
	TX_SETTLUT_I1_ID,
	TX_SETTLUT_I2_ID,
	TX_SETTLUT_I3_ID,
	TX_SETTLUT_I4_ID,
	TX_SETTLUT_I5_ID,
	TX_SETTLUT_I6_ID,
	TX_SETTLUT_I7_ID,
};
*/




#ifdef flagPOSIX
inline int BitScanForward(uint u) {
	return (int)__builtin_ffsll(u) - 1;
}
inline s64 SysGetTickCount() {
	struct timespec ts;
	s64 theTick = 0LL;
	clock_gettime( CLOCK_REALTIME, &ts );
	theTick  = ts.tv_nsec / 1000000LL;
	theTick += ts.tv_sec * 1000LL;
	return theTick;
}
#else
inline int BitScanForward(uint u) {
	unsigned long idx;
	if (_BitScanForward(&idx, u))
		return idx;
	return -1;
}
inline s64 SysGetTickCount() {
	return GetTickCount();
}
#endif

#endif
