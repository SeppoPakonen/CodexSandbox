#ifndef _SimpleMedia_Types_h_
#define _SimpleMedia_Types_h_


#define VID_CHUNK_LEN		VidChunkLen
#define VID_CHUNK_ID(x)		*((u32*)x)

// Not known...
#define VID_CHUNK_HEADER_SIZE	64
#define VID_NUM_READ_BUFFERS	32

// Known
#define MAX_FNAME_LEN			64


enum {
	AX_PB_MIXCTRL_L			= 1 << 0,
	AX_PB_MIXCTRL_R			= 1 << 1,
	AX_PB_MIXCTRL_B_DPL2	= 1 << 2,
	AX_PB_MIXCTRL_B_L		= 1 << 3,
	AX_PB_MIXCTRL_B_R		= 1 << 4,
	
};

enum {
	GQR_SCALE_64,
	GQR_TYPE_S16,
};

enum {
	VID_DECODER_B_FRAMES		= (1 << 0),
	VID_DECODER_AUDIO			= (1 << 1),
	
};

enum {
	VID_VIDH_INTERLACED			= (1 << 0),
	
};

enum {
	VID_YUVCONV_HIGHPRECISION	= (1 << 0),
	
};


typedef void* (*VIDAllocator)(unsigned int size);
typedef void (*VIDDeallocator)(void* ptr);

typedef void* (*VAUDAllocator)(unsigned int size);
typedef void (*VAUDDeallocator)(void* ptr);
typedef int (*AudioDecodeCb)(u32 numChannels, const s16 **samples, u32 sampleNum, void* userData);



struct Vaud {
	u8* headerInfo;
	int headerInfoBytes;
	int frq;
	int numChannels;
	int maxHeap;
	int preAlloc;
};

struct Vaudex {
	int totalSampleCount;
	int frameCount;
	int maxBufferSize;
	int version;
	int size;
	int frameTimeMs;
	
};

struct PcmInfo {
	int frq;
	int numChannels;
	
};

struct VidAUDH {
	Vaud vaud;
	Vaudex vaudex;
	PcmInfo pcm16;
	u32 audioID;
	
};

struct VidAUDHVAUD {
	
};

struct Vid {
	int versionMajor, versionMinor;
	int vidConvMajor, vidConvMinor, vidConvBuild;
	
};

struct VidVIDH {
	u32 frameRateScale;
	u32 flags;
	
};

struct VidChunk {
	u32 id;
	u32 version;
	int len;
	Vid vid;
	VidVIDH vidh;
	
};

struct VidDecoder {
	
};

struct VidAUDDVAUD {
	u32 size = 0;
	
};

struct VIDImage {
	u8* y;
	u8* u;
	u8* v;
	int width;
	int height;
	int texWidth;
	int texHeight;
	
};

struct VidInfo {
	int maxFrameCount;
	int maxBufferSize;
	int width, height;
	int frameRate;
	int frameRateScale;
	u32 flags;
	
	
};

struct AdPcmInfo {
	
};

struct VidBuffer {
	u8* ptr;
	int frameNumber;
	int size;
	int valid;
	
};

struct VidSimplePlayer {
	VidBuffer readBuffer[VID_NUM_READ_BUFFERS];
	u8* readBufferBaseMem;
	u8* audioHeaderChunk;
	u8* image;
	VidDecoder* decoder;
	char fileName[MAX_FNAME_LEN];
	
	DVDFileInfo fileHandle;
	VidAUDH audioInfo;
	VidInfo videoInfo;
	AdPcmInfo adpcmInfo;
	VIDAllocator cbAlloc;
	VIDAllocator cbLockedCache;
	VIDDeallocator cbFree;
	int firstFrameOffset;
	int firstFrameSize;
	int nextFrameOffset;
	int nextFrameSize;
	int currentFrameCount;
	int readIndex;
	int preFetchState;
	int decodeIndex;
	int error;
	int fps;
	int frameRate;
	int lastDecodedFrame;
	bool asyncDvdRunning;
	bool open;
	bool loopMode;
	
};

struct VIDDecoderSetup {
	VIDAllocator cbMemAlloc;
	VIDAllocator cbMemAllocLockedCache;
	VIDDeallocator cbMemFree;
	Vaud audio;
	int size;
	int width, height;
	u32 flags;
	
};



typedef VidDecoder* VAUDDecoder;




#endif
