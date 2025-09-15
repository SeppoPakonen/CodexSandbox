#ifndef _Core_Defs_h_
#define _Core_Defs_h_

#include <stdint.h>

NAMESPACE_SDK_BEGIN



#if defined __linux__ || defined __freebsd__ || defined flagPOSIX
	#define DIR_SEPS "/"
	#define DIR_SEP  '/'
	#define ENDL "\n"
	#define endl "\n"
	#define PLATFORM_POSIX
	#ifdef __x86_64__
		#define CPU_64	1
	#endif
	#ifdef __GNUC__
		#define COMPILER_GNU
	#endif
	#ifdef __clang__
		#define COMPILER_CLANG
	#endif
#elif defined _WIN32 || defined WINAPI_FAMILY
	#undef  flagWIN32
	#define flagWIN32
	#define ENDL "\r\n"
	#define endl "\r\n"
	#define DIR_SEP  '\\'
	#define DIR_SEPS "\\"
	#ifdef _MSC_VER
		#define COMPILER_MSC
		#ifdef _WIN64
			#define CPU_64	1
		#endif
	#endif
#endif

#ifdef flagWIN32
	#define __BREAK__		__debugbreak()
#elif defined flagPOSIX
	#define __BREAK__        raise(SIGTRAP);
#else
	#define __BREAK__        (*(volatile int *)0 = 0) // kill(getpid(), SIGTRAP)
#endif

#undef ASSERT
#undef ASSERT_
#define ASSERT(x) {if (!(x)) {__BREAK__;}}
#define ASSERT_(x, msg) {if (!(x)) {LOG(SDK::String("Assertion failed: ") + msg); __BREAK__;}}
#define PANIC(msg) ASSERT_(false, msg)



typedef unsigned char	uint8;
typedef char			int8;
typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned int	uint32;
typedef int				int32;
typedef uint64_t		uint64;
typedef int64_t			int64;

typedef unsigned char	Byte;
typedef unsigned char	byte;
typedef uint16			word;
typedef uint32			dword;
typedef uint64			qword;

typedef unsigned long	DWORD;



typedef const char*		CString;

template <class T>
inline void IGNORE_RESULT(const T&) {}


typedef void* VOID_PTR;
typedef const void* CONST_VOID_PTR;

class Nuller {};

NAMESPACE_SDK_END


#endif
