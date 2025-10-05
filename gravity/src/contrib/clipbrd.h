#include <dpmi.h>
#include <go32.h>
#include <dos.h>
#include <sys/movedata.h>

#define cb_text       1
#define cb_bitmap     2
#define cb_oemtext    7
#define cb_dsptext    0x81
#define cb_dspbitmap  0x82

#define bufsize       _go32_info_block.size_of_transfer_buffer

char ClipboardAvailable();
char CloseClipboard();
long int ClipboardCompact(int l);
char OpenClipboard();
char EmptyClipboard();
long int GetClipboardDataSize(char format);
char GetClipboardData(char format, char *data);
char SetClipboardData(char format, char *data, int l);
