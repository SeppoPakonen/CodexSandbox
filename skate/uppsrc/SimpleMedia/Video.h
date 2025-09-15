#ifndef _SimpleMedia_Video_h_
#define _SimpleMedia_Video_h_



int			VidChunkLen(u8*);

VAUDDecoder	VAUDCreateDecoder(VAUDAllocator alloc, VAUDDeallocator free, int max_heap);
void		VAUDDestroyDecoder(VAUDDecoder dec);
bool		VAUDInitDecoder(VAUDDecoder dec, const u8* data, int len, int prealloc);
int			VAUDDecode(VAUDDecoder dec, const u8* begin, int size, u32 channelSelectMask, AudioDecodeCb ptr, void* unknown);

BOOL		VIDSimpleAudioDecode(const u8* bitstream, u32 bitstreamLen);
BOOL		VIDVideoDecode(VidDecoder* dec, const u8* chunk, int chunk_size, u8** dst);

void		VIDDrawGXYuv2RgbSetup(GXRenderModeObj *rmode);
void		VIDDrawGXYuv2RgbDraw(s16 x, s16 y, s16 w, s16 h, const u8* img);
void		VIDDrawGXRestore();
void		VIDXFBDraw(const u8* img, void* buf, int w, int h, void* mem);
int			VIDSetupTEV(int);
int			VIDAudioDecode(VAUDDecoder dec, const u8* buf, int size, int, AudioDecodeCb, void* arg);
void		VIDSimpleAudioChangePlayback(const u32 *playMaskArray, u32 numMasks);
int			VIDXFBGetLCSize();
void		VIDSimpleAudioStartPlayback(const u32* mask, int);

VAUDDecoder	VIDCreateDecoder(VIDDecoderSetup* setup);
void		VIDDestroyDecoder(VAUDDecoder dec);

void GQRSetup6(int, int, int, int);
void GQRSetup7(int, int, int, int);

#endif
