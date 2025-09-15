
#ifndef COMPILE_H
#define COMPILE_H

#if 0
#define WinX11
#define OsTypeIsUnix
#define Win31
#define OsTypeIsDos
#else
#define OsTypeIsPsx
#endif

#ifdef OsTypeIsDos
#include <process.h>
#endif

#endif

