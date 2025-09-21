#ifndef _Dolphin_Defs_h_
#define _Dolphin_Defs_h_

#include <stdint.h>

#if defined __x86_64__ || defined _____LP64_____
	#define CPU64
#else
	#define CPU32
#endif


//#define Dbg_Assert(x) if (!(x)) {PANIC("Addertion failed: " #x);}
//#define TODO Dbg_Assert(0); throw 0;
#define Dbg_Assert(x) ASSERT(x)


#define CONCAT13(a, b) (((u32)a << (3*8)) | (u32)(b))
#define CONCAT12(a, b) (((u32)a << (2*8)) | (u32)(b))
#define CONCAT11(a, b) (((u32)a << (1*8)) | (u32)(b))


#ifndef M_PI
	#define M_PI		(3.14159265358979323846)
#endif

#define M_PI_MUL2	(2.0 * M_PI)


typedef unsigned char	uint8;
typedef char			int8;
typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned int	uint32;
typedef int				int32;
typedef unsigned char	byte;

#if 0


#ifdef CPU64
typedef unsigned long long	uint64;
typedef long long			int64;
static_assert(sizeof(uint64) == 8);
#else
typedef unsigned long long	uint64;
typedef long long			int64;
//static_assert(sizeof(uint64) == 8);
#endif

typedef uint16			word;
typedef uint32			dword;
typedef uint64			qword;
typedef uint32			uint32_t;
typedef int32			int32_t;
typedef uint64			uint64_t;
typedef int64			int64_t;
typedef unsigned long	DWORD;
typedef const char*		CString;

#else

typedef unsigned long long	uint64;
typedef long long			int64;
static_assert(sizeof(uint64) == 8);

#endif


//constexpr double POW2_1024 = 1024.0 * 1024.0;
#define POW2_1024 (1024.0 * 1024.0)

typedef unsigned char    byte;
typedef unsigned char    uchar;
typedef unsigned int     uint;
typedef unsigned short   ushort;




typedef float f32;
typedef uint64_t u64;
typedef int64_t  s64;
typedef void * Ptr;
typedef int s32;
typedef ushort u16;
typedef uint u32;
typedef short s16;
typedef uchar u8;
typedef double f64;
typedef char s8;





#undef __BREAK__
#ifdef flagWIN32
	#define __BREAK__		__debugbreak()
#elif defined flagPOSIX
	#define __BREAK__        raise(SIGTRAP);
#else
	#define __BREAK__        (*(volatile int *)0 = 0) // kill(getpid(), SIGTRAP)
#endif


#include "platform.h"

#ifndef PANIC
	#define PANIC(x) {std::cout << x << std::endl; volatile int* i = 0; *i = 0;}
#endif

#endif
