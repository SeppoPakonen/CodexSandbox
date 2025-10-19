#include "clipbrd.h"

char ClipboardAvailable()
{
  __dpmi_regs r;

  r.x.ax = 0x1700;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0x1700;
}

char CloseClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1708;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

long int ClipboardCompact(int l)
{
  __dpmi_regs r;

  r.x.ax = 0x1709;
  r.x.si = l >> 16;
  r.x.cx = l;
  __dpmi_int(0x2f,&r);

  return r.x.ax + (r.x.dx << 16);
}

char OpenClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1701;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

char EmptyClipboard()
{
  __dpmi_regs r;

  r.x.ax = 0x1702;
  __dpmi_int(0x2f,&r);

  return r.x.ax != 0;
}

long int GetClipboardDataSize(char format)
{
  __dpmi_regs r;

  r.x.ax = 0x1704;
  r.x.dx = format;
  __dpmi_int(0x2f,&r);

  return r.x.ax + (r.x.dx << 16);
}

char GetClipboardData(char format, char *data)
{
  __dpmi_regs r;

  r.x.ax = 0x1705;
  r.x.dx = format;
  r.x.bx = __tb & 0x0f;
  r.x.es = (__tb >> 4) & 0xffff;
  __dpmi_int(0x2f,&r);

  dosmemget(__tb,bufsize,data);

  return r.x.ax != 0;
}

char SetClipboardData(char format, char *data, int l)
{
  __dpmi_regs r;
  int OpenClipboard;

  a = OpenClipboard;

  if ( (data) && (l > 0) && (l < bufsize) && (EmptyClipboard) &&
       (ClipboardCompact(l) >= l) )
  {
    dosmemput(data,l,__tb);

    r.x.ax = 0x1703;
    r.x.dx = format;
    r.x.bx = __tb & 0x0f;
    r.x.es = (__tb >> 4) & 0xffff;
    r.x.si = l >> 16;
    r.x.cx = l;
    __dpmi_int(0x2f,&r);

    return r.x.ax != 0;
  }

  return 0;
}

