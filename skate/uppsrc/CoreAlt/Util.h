#ifndef _Core_Util_h_
#define _Core_Util_h_

NAMESPACE_SDK_BEGIN

void Panic(String s);
void Assert(bool b, String s="Assertion failed");
void AssertFalse(bool b, String s="Assertion failed");

template <class T>
T& PtrRef(T* o, String throw_msg) {
	if (!o) throw throw_msg;
	return *o;
}

Stream& Log();
Stream& Cout();
Stream& Cerr();
Stream& Cin();

inline String ReadStdIn() {return Cin().GetLine();}

void SetExeFilePath(String s);
String GetExeFilePath();
String GetHomeDir();
String ConfigFile(String file_name);
String GetFileName(String path);
String GetFileTitle(String path);
String GetFileDirectory(String path);
String AppendFileName(String a, String b);
String GetParentDirectory(String path, int steps=1);
String GetFileExt(String path);
void RealizeDirectory(String dir);
void DeleteFile(String path);
void RenameFile(String oldpath, String newpath);



struct StringParser {
	String s;
	int cursor = 0;

	StringParser(String s) : s(s) {}

	void PassChar(int chr, bool do_pass_white = true);
	bool Char(int chr);
	int ReadInt();
	String ReadId();
	int GetChar();
	int PeekChar();
	bool IsChar(int chr);
	void PassWhite();
};


int GetCachedStringId(String path);
String GetCachedString(int id);

struct FileLocation : Moveable<FileLocation> {
	int file = -1;
	int line = -1, col = -1;
	
	void SetFileLocation(const FileLocation& loc) {*this = loc;}
	IMMEDIATE_TYPES_EQUAL(FileLocation);
};

enum { PROCMSG_UNKNOWN, PROCMSG_SKIP, PROCMSG_INFO, PROCMSG_WARNING, PROCMSG_ERROR };

struct ProcMsg : Moveable<ProcMsg>, public FileLocation {
	int severity = 0;
	int src = -1;
	int msg = -1;

	ProcMsg() {}
	ProcMsg(const ProcMsg& e) { *this = e; }
	ProcMsg(String source, FileLocation loc, int severity, String msg) {
		this->src = GetCachedStringId(source);
		SetFileLocation(loc);
		this->severity = severity;
		this->msg = GetCachedStringId(msg);
	}
	
	IMMEDIATE_TYPES_EQUAL(ProcMsg);
	
	void operator=(const ProcMsg& e) {
		msg = e.msg;
		file = e.file;
		src = e.src;
		line = e.line;
		col = e.col;
		severity = e.severity;
	}
	
	void Set(String source, FileLocation loc, int severity, String msg) {
		this->src = GetCachedStringId(source);
		SetFileLocation(loc);
		this->severity = severity;
		this->msg = GetCachedStringId(msg);
	}
	
	String ToString() const {
		String s;
		s	<< ToUpper(src)
			<< (src >= 0 ? ": " + GetCachedString(src) : "")
			<< ": " << GetCachedString(file)
			<< ":" << line
			<< ":" << col
			<< ": " << GetSeverityString()
			<< ": " << GetCachedString(msg);
		return s;
	}
	
	String GetSeverityString() const {
		switch (severity) {
		case PROCMSG_SKIP: return "skip";
		case PROCMSG_INFO: return "info";
		case PROCMSG_WARNING: return "warning";
		case PROCMSG_ERROR: return "error";
		default: return "<invalid severity>";
		}
	}
};



class ErrorSource {
	String error_path, src;
	bool fail = false;

protected:
	void PrintHeader(String file = "", int line = -1, int col = -1);
	void Internal(String msg);
	void InternalWarning(String msg);
	void Error(String msg, int line, int col);
	void Error(String msg, String file, int line, int col);
	void Error(String msg);
	void Warning(String msg, int line, int col);
	void Warning(String msg, String file, int line, int col);
	void Warning(String msg);
	void Info(String msg);


public:
	ErrorSource() {}

	void SetErrorPath(String path) { error_path = path; }
	void SetSource(String s) { src = s; }

	//Callback1<struct Error> WhenError;
};


void ParseCommandLine(int argc, const char** argv);
void ParseCommandLine(const char* cmdline);
const Vector<String>& CommandLine();
int GetExitCode();
void SetExitCode(int i);
void SetCoreArg(String key, String value);
String GetExeDirFile(String filename);
String GetEnv(String id);
void ReadCoreCmdlineArgs();
void SetDataPath(String path);
String GetDataDirectory();
String GetDataFile(String filename);
bool FileExists(String path);
bool DirectoryExists(String path);
String FindLocalFile(String filename);
void AddLocalFileDirectory(String dir);
bool IsVerbose();
void SetVerbose(bool b=true);

#if defined flagWIN32 && defined flagGUI
void SetWin32Instances(HINSTANCE inst, HINSTANCE prev, bool nCmdShow);
HINSTANCE GetWin32Instance();
HINSTANCE GetWin32PrevInstance();
bool GetWin32CmdShow();
#endif

template <class T>
inline T& Single() {
	static T o;
	return o;
}


int64 usecs(int64 prev = 0);
int msecs(int prev = 0);




struct Uuid {
	uint64 v[2];

	Uuid() {}

	unsigned GetHashValue() const { CombineHash ch; ch.Put64(v[0]); ch.Put64(v[1]); return ch;}

	void     New();
	
	String ToString() const;
	static Uuid Create() { Uuid uuid; uuid.New(); return uuid; }
};




void GetDirectoryFiles(String dir, Index<String>& files);

class FindFile {
	
	
public:
	typedef FindFile CLASSNAME;
	FindFile();
	
	bool Search(String path);
	bool Next();
	
	bool IsDirectory() const;
	String GetPath() const;
	String GetName() const;
};




template <class I>
inline void IterSwap(I a, I b) { if(a != b) Swap(*a, *b); }

template <class I, class Less>
inline void OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

template <class I, class Less>
inline void FinalSort__(I begin, I end, const Less& less) {
	if(begin == end)
		return;
	I last = end;
	--last;
	while(!(begin == last)) {
		I best = last;
		I next = last;
		I ptr = last;
		for(;;) {
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is the final minimum
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
			}
			else
			if(ptr == begin) { // begin is the final minimum
				begin = next;  // and everything is sorted up to next
				break;
			}
			best = ptr;
		}
	NEXT_ITEM:
		;
	}
}


template <class I, class Less>
void Sort__(I l, I h, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSort__(l, h, less);
			return;
		}
		int pass = 4;
		for(;;) {
			I middle = l + (count >> 1);        // get the middle element
			OrderIter2__(l, middle, less);      // sort l, middle, h-1 to find median of 3
			OrderIter2__(middle, h - 1, less);
			OrderIter2__(l, middle, less);      // median is now in middle
			IterSwap(l + 1, middle);            // move median pivot to l + 1
			I ii = l + 1;
			for(I i = l + 2; i != h - 1; ++i)   // do partitioning; already l <= pivot <= h - 1
				if(less(*i, *(l + 1)))
					IterSwap(++ii, i);
			IterSwap(ii, l + 1);                // put pivot back in between partitions
			I iih = ii;
			while(iih + 1 != h && !less(*ii, *(iih + 1))) // Find middle range of elements equal to pivot
				++iih;
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) { // partition sizes ok or we have done max attempts
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					Sort__(l, ii, less);
					l = iih + 1;
				}
				else {
					Sort__(iih + 1, h, less);
					h = ii;
				}
				break;
			}
			IterSwap(l, l + (int)Random(count));     // try some other random elements for median pivot
			IterSwap(middle, l + (int)Random(count));
			IterSwap(h - 1, l + (int)Random(count));
			pass++;
		}
	}
}

template <class V, class S>
void Sort(V& vector, const S& sorter) {
	if (vector.GetCount() < 2)
		return;
	Sort__(vector.Begin().Get(), vector.End().Get(), sorter);
}



template <class T> struct StdLess {bool operator()(const T& a, const T& b) const {return a < b;}};
template <class T> struct StdGreater {bool operator()(const T& a, const T& b) const {return a > b;}};









class CommandLineArguments {
	static const int max_inputs = 300;
	static const int max_args = 100;
	
	
	struct Arg {
		char key;
		String desc;
		bool has_value;
	};
	
	struct Input {
		char key;
		String value;
	};
	
	Array<Arg> args;
	Array<Input> inputs;
	
public:
	CommandLineArguments() {}
	void AddArg(char key, const char* desc, bool has_value);
	bool Parse();
	void PrintHelp();
	int GetInputCount() const {return inputs.GetCount();}
	const Array<Input>& GetInputs() const {return inputs;}
	
};
















inline bool operator==(const Vector<byte>& a, const Vector<byte>& b) {
	if (a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++)
		if (a[i] != b[i])
			return false;
	return true;
}

typedef Vector<double>			BetSize;
typedef Vector<bool>			BoolVec;



/*
	Heapless Copying
	- normal vector copying "dst_vector <<= src_vector;" causes memory clearing and allocation
		- it is slow and useless if the allocated memory could be reused
	- to copy data with minimum reallocation, use SetCount and just copy values (or memcpy)
*/
template <class T>
inline void CopyHeapless(const T& from, T& to) {
	to.SetCount(from.GetCount());
	for(int i = 0; i < to.GetCount(); i++)
		to[i] = from[i];
}

template <class T>
inline void CopyHeaplessMat(const T& from, T& to) {
	to.SetCount(from.GetCount());
	for(int i = 0; i < to.GetCount(); i++) {
		const auto& from0 = from[i];
		auto& to0 = to[i];
		to0.SetCount(from0.GetCount());
		for(int i = 0; i < to0.GetCount(); i++)
			to0[i] = from0[i];
	}
}

inline void VectorAdd(const Vector<double>& from, Vector<double>& to) {
	ASSERT(from.GetCount() == to.GetCount());
	double* t = to.Begin();
	for(double f : from)
		*(t++) += f;
}





template <class Stream>
void CopyStreamCont(Array<Stream>& dest, Array<Stream>& src) {
	dest.Clear();
	for(int i = 0; i < src.GetCount(); i++) {
		StreamCopy(dest.Add(), src[i]);
	}
}

bool StreamCopy(Stream& dest, Stream& src);



void FillVector(Vector<double>& v);
String UpperBegin(String s);

inline int PopCount64(uint64 i) {
	#ifdef flagMSC
	#if CPU_64
	return (int)__popcnt64(i);
	#elif CPU_32
	return __popcnt(i) + __popcnt(i >> 32);
	#endif
	#else
	return (int)__builtin_popcountll(i);
	#endif
}

inline int PopCount32(dword i) {
	#ifdef flagMSC
	return (int)__popcnt64(i);
	#else
	return (int)__builtin_popcountl(i);
	#endif
}

typedef std::atomic<uint64> Atomic64;



inline int HammingDistance32(int count, const dword* a, const dword* b) {
	if (count <= 0) return 0;
	const dword* end = a + count;
	int distance = 0;
	while(a != end)
		distance += PopCount32(*a++ ^ *b++);
	return distance;
}

inline int HammingDistance64(int count, const uint64* a, const uint64* b) {
	if (count <= 0) return 0;
	const uint64* end = a + count;
	int distance = 0;
	while(a != end)
		distance += PopCount64(*a++ ^ *b++);
	return distance;
}



inline String GetDataDirectoryFile(String file) {return AppendFileName(GetDataDirectory(), file);}
void SetSharedDirectory(String d);
String GetSharedDirectory();
String SharedFile(String file);

inline String AppendPosixFilename(const String& a, const String& b) {
	if (a.Right(1) == "/") return a + b;
	else return a + "/" + b;
}







template <class T, int I>
struct DtorValueSetter {
	T& obj;
	DtorValueSetter(T& obj) : obj(obj) {}
	~DtorValueSetter() {obj = I;}
};

typedef DtorValueSetter<bool, 0> FlagDisabler;





bool IsClose(double a, double b);
inline bool IsFin(float f) {return ::isfinite(f);}



inline float RandomFloat32() {
	float f;
	do {
		float base = 1.0f + (float)Randomf();
		dword bf = (Random(2) << 31) | (Random(0x100) << 23) | (*(dword*)&base & (0xFFFFFFFF >> (32 - 23)));
		f = *(float*)&bf;
	}
	while (!IsFin(f));
	return f;
}

double Multiply(double prev_norm, int64 count, double multiplier);

inline int msb(dword d) {
    int res = 0;
	#ifdef flagGCC
    asm("bsrl %1,%0" : "=r"(res) : "r"(d));
    #else
	while (d >>= 1)
	    res++;
	#endif
	return res;
}










inline void DumpProbVector(const Vector<double>& probs, int steps, int indent=0) {
	double step = 1.0 / (steps + 1);
	String s;
	for(int i = 0; i < steps; i++) {
		double offset = (steps - i) * step;
		s = "";
		for(int j = 0; j < indent; j++)
			s.Cat('\t');
		for(double d : probs)
			s.Cat(d >= offset ? '#' : ' ');
		LOG(s);
	}
}










inline int64 GetCpuTicks() {
	#ifdef flagWIN32
    return __rdtsc();
    #else
    return clock();
    #endif
}

inline int64 Delay(int64 cpu_ticks) {
	int64 end = GetCpuTicks() + cpu_ticks;
	int64 iters = 0;
	while (GetCpuTicks() < end)
		iters++;
	return iters;
}












template <class T>
inline const T& IndexAdd(Index<T>& idx, const T& o) {
	int id = idx.GetCount();
	idx.Add(o);
	return idx[id];
}

template <class T>
inline const T& IndexFindAdd(Index<T>& idx, const T& o) {
	int id = idx.FindAdd(o);
	return idx[id];
}





// https://software.intel.com/sites/landingpage/IntrinsicsGuide

struct Trans8x16 {
	union {
		uint16 u16[8];
		uint8 u8[16];
		__m128i m;
		uint64 u64[2];
	};
	
	void TransFrom16x8() {
		__m128i x = m;
	    for (int i = 0; i < 8; ++i) {
	        u16[7-i] = _mm_movemask_epi8(x);
	        x = _mm_slli_epi64(x,1);
	    }
	}
	
	void Zero() {u64[0] = 0; u64[1] = 0;}
};

#ifdef flagAVX2
struct Trans8x32 {
	union {
		uint32 u32[8];
		uint8 u8[32];
		__m256i m;
	};
	
	void TransFrom32x8() {
		__m256i x = m;
	    for (int i = 0; i < 8; ++i) {
	        u32[7-i] = _mm256_movemask_epi8(x);
	        x = _mm256_slli_epi64(x,1);
	    }
	}
};
#endif




inline void* MemoryAlloc(size_t size) {return malloc(size);}
inline void MemoryFree(void* ptr) {free(ptr);}

NAMESPACE_SDK_END

#endif
