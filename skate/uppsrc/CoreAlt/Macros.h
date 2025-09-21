#ifndef _Core_Macros_h_
#define _Core_Macros_h_

NAMESPACE_SDK_BEGIN


#ifndef NULL
	#define NULL 0
#endif

// TODO translate
#define t_(x) x

#if !defined SCHAR_MIN
#  define SCHAR_MIN		(-128)
#  define SCHAR_MAX		127
#  define UCHAR_MAX		255
#  define SHRT_MIN		(-32768)
#  define SHRT_MAX		32767
#  define USHRT_MAX		65535
#  define INT_MIN		(-INT_MAX - 1)
#  define INT_MAX		2147483647
#  define UINT_MAX		4294967295U
# ifndef INT64_MAX
#  define INT64_MAX		9223372036854775807LL
# endif
# ifndef INT64_MIN
#  define INT64_MIN		(-LLONG_MAX - 1LL)
# endif
# ifndef UINT64_MAX
#  define UINT64_MAX	18446744073709551615ULL
# endif
#endif


#ifndef M_E
	#define M_E        2.71828182845904523536   // e
	#define M_LOG2E    1.44269504088896340736   // log2(e)
	#define M_LOG10E   0.434294481903251827651  // log10(e)
	#define M_LN2      0.693147180559945309417  // ln(2)
	#define M_LN10     2.30258509299404568402   // ln(10)
	#define M_PI       3.14159265358979323846   // pi
	#define M_PI_2     1.57079632679489661923   // pi/2
	#define M_PI_4     0.785398163397448309616  // pi/4
	#define M_1_PI     0.318309886183790671538  // 1/pi
	#define M_2_PI     0.636619772367581343076  // 2/pi
	#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
	#define M_SQRT2    1.41421356237309504880   // sqrt(2)
	#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif


#undef TODO

#define MACROSTR(x) #x
#define COUT(x) {::SDK::String ____s; ____s << x; Cout() << ____s << "\n";}
#define COUTLOG(x) {::SDK::String ____s; ____s << x; LOG(____s); Cout() << ____s << "\n";}
#define TODO {Panic("TODO " __FILE__ ":" + SDK::IntStr(__LINE__)); throw SDK::Exc("TODO");}
#define SYNON_UNION_2(type, name0, name1) union {type name0; type name1;};


#define LOG(x) {SDK::Log() << x << ENDL; SDK::Log().Flush();}

#define DUMP(x) {LOG( #x  " = " << ToString(x));}

#define DUMPC(c) {\
LOG(#c << " {"); \
for (int i = 0; i < c.GetCount(); i++) \
	LOG("\t[" << i << "]: " << ToString(c[i])); \
LOG("}"); \
}

#define DUMPM(m) {\
LOG(#m << " {"); \
for (int i = 0; i < m.GetCount(); i++) \
	LOG("\t[" << i << "]: " << ToString(m.GetKey(i)) << ": " << ToString(m[i])); \
LOG("}"); \
}


#ifdef flagGUI
	#if defined(flagWIN32)
		#define GUI_APP_MAIN \
			void AppMain(); \
			\
			int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cmdline, int show) {\
				char chr[512]; GetModuleFileNameA(NULL, chr, 512); \
				::SetExeFilePath(chr); \
				::SeedRandom(); \
				::SetWin32Instances(hinst, hprev, show); \
				::ParseCommandLine(cmdline); \
				::ReadCoreCmdlineArgs(); \
				::RunInitBlocks(); \
				AppMain(); \
				Thread::ShutdownThreads(); \
				::RunExitBlocks(); \
				return ::GetExitCode(); \
			} \
			\
			void AppMain()
	#else
		#define GUI_APP_MAIN \
			void AppMain(); \
			\
			int main(int argc, const char** argv) {\
				::SetExeFilePath(argv[0]); \
				::SeedRandom(); \
				::ParseCommandLine(argc, argv); \
				::ReadCoreCmdlineArgs(); \
				::RunInitBlocks(); \
				AppMain(); \
				Thread::ShutdownThreads(); \
				::RunExitBlocks(); \
				return ::GetExitCode(); \
			} \
			\
			void AppMain()
		#define MAIN_FN
	#endif
#else
	#define CONSOLE_APP_MAIN \
	void AppMain(); \
	\
	extern "C" int main(int argc, const char** argv) {\
		::SDK::SetExeFilePath(argv[0]); \
		::SDK::SeedRandom(); \
		::SDK::ParseCommandLine(argc, argv); \
		::SDK::ReadCoreCmdlineArgs(); \
		::SDK::RunInitBlocks(); \
		AppMain(); \
		::SDK::Thread::ShutdownThreads(); \
		::SDK::RunExitBlocks(); \
		return ::SDK::GetExitCode(); \
	} \
	\
	void AppMain()
#endif



#if defined flagGCC || flagCLANG
	#define MemoryCompare __builtin_memcmp
	#define MemoryCopy    __builtin_memcpy
	#define MemoryMove    __builtin_memmove
#elif defined flagMSC
	#define MemoryCompare memcmp
	#define MemoryCopy    memcpy
	#define MemoryMove    memmove
#endif


#define IMMEDIATE_TYPES_EQUAL(T) bool operator==(const T& t) const {return MemoryCompare(this, &t, sizeof(T)) == 0;}

#define COMBINE2(a, b) a##b
#define COMBINE(a, b) COMBINE2(a, b)

#define LINEID(pre, x) COMBINE(COMBINE(pre, x), __LINE__)
#define INITBLOCK(x) void LINEID(Init,x) (); static Callinit LINEID(initcb,x) (LINEID(Init,x)); void LINEID(Init,x) ()
#define EXITBLOCK(x) void LINEID(Exit,x) (); static Callexit LINEID(exitcb,x) (LINEID(Exit,x)); void LINEID(Exit,x) ()
#define ONCELOCK static AtomicInt __once; if (__once++ == 0)

NAMESPACE_SDK_END

#endif
