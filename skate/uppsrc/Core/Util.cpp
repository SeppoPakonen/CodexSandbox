#include "Core.h"

#ifdef flagWIN32
#include <shlwapi.h>
#endif

#ifdef flagPOSIX
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#endif

NAMESPACE_SDK_BEGIN

int64 usecs(int64 prev)
{
	auto p2 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(p2.time_since_epoch()).count() - prev;
}

int msecs(int prev)
{
	auto p2 = std::chrono::steady_clock::now();
	return (int)std::chrono::duration_cast<std::chrono::milliseconds>(p2.time_since_epoch()).count() - prev;
}








void Uuid::New() {
	Random64(v, 2);
}

String Uuid::ToString() const {
	String s;
	union {
		uint64 u64;
		byte u8[8];
	};
	int c = 0;
	for(int i = 0; i < 2; i++) {
		u64 = v[i];
		for(int j = 0; j < 8; j++) {
			byte b = u8[j];
			if (c == 8 || c == 12 || c == 16 || c == 20)
				s.Cat('-');
			s.Cat(HexDigitLower(b >> 4));
			s.Cat(HexDigitLower(0xF & b));
			c+=2;
		}
	}
	return s;
}















Vector<String> __cmdline;
void ParseCommandLine(const char* cmdline) {
	__cmdline = Split(cmdline, " ");
}

void ParseCommandLine(int argc, const char** argv) {
	__cmdline.Clear();
	for(int i = 1; i < argc; i++)
		__cmdline.Add(argv[i]);
}

const Vector<String>& CommandLine() {return __cmdline;}

int __exitcode;
int GetExitCode() {return __exitcode;}
void SetExitCode(int i) {__exitcode = i;}



String sDataPath;
String config_path;

void SetCoreArg(String key, String value) {
	if (key.IsEmpty())
		return;
	
	LOG("core-arg: \"" << key << "\" -> \"" << value << "\"");
	if (key == "datapath") {
		ASSERT(DirectoryExists(value));
		sDataPath = value;
	}
	else if (key == "config") {
		ASSERT(DirectoryExists(value));
		config_path = value;
	}
}






String GetExeDirFile(String filename) {
	return AppendFileName(GetFileDirectory(GetExeFilePath()), filename);
}

String GetEnv(String id)
{
	if (id.IsEmpty())
		return String();
	#ifdef flagWIN32
	size_t len = 0;
	char homedir[2048];
	getenv_s(&len, &homedir[0], 2048, id.Begin());
	return String(homedir, len);
	#else
	return getenv(id.Begin());
	#endif
}





void ReadCoreCmdlineArgs() {
	for(String arg : __cmdline) {
		int i = arg.Find("=");
		if (i >= 0) {
			String key = arg.Left(i);
			String value = arg.Mid(i+1);
			if (key.Left(2) == "-E") {
				key = key.Mid(2);
				SetCoreArg(key, value);
			}
		}
	}
}






void SetDataPath(String path) {
	sDataPath = path;
}

String GetDataDirectory() {
	if(sDataPath.GetCount())
		return sDataPath;
	return GetEnv("UPP_MAIN__");
}

String GetDataFile(String filename) {
	if(sDataPath.GetCount())
		return AppendFileName(sDataPath, filename);
	String s = GetEnv("UPP_MAIN__");
	return s.GetCount() ? AppendFileName(s, filename) : GetExeDirFile(filename);
}

bool FileExists(String path) {
	#ifdef flagWIN32
	#ifndef flagUWP
	return PathFileExistsA(path.Begin());
	#else
	struct stat stat_info;
	if (stat(path.Begin(), &stat_info) != 0)
		return false;
	return true;
	#endif
	#else
	return access( path.Begin(), F_OK ) != -1;
	#endif
}

bool DirectoryExists(String path) {
	#ifdef flagWIN32
	#ifndef flagUWP
	DWORD dwAttrib = GetFileAttributes(path.Begin());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	#else
	#error TODO
	#endif
	#else
	DIR* dir = opendir(path.Begin());
	if (dir) {
	    closedir(dir);
	    return true;
	}
	return false;
	#endif
}

Vector<String> local_file_dirs;

String FindLocalFile(String filename) {
	String exe_dir_file = GetExeDirFile(filename);
	if (FileExists(exe_dir_file))
		return exe_dir_file;
	
	for(int i = 0; i < local_file_dirs.GetCount(); i++) {
		String dir_file = AppendFileName(local_file_dirs[i], filename);
		if (FileExists(dir_file) || DirectoryExists(dir_file))
			return dir_file;
	}
	
	return ConfigFile(filename);
}

void AddLocalFileDirectory(String dir) {
	local_file_dirs.Add(dir);
}

bool __is_verbose;
bool IsVerbose() {return __is_verbose;}
void SetVerbose(bool b) {__is_verbose = b;}











#if defined flagWIN32 && defined flagGUI
HINSTANCE __hinst_cur, __hinst_prev;
bool __nCmdShow;
void SetWin32Instances(HINSTANCE inst, HINSTANCE prev, bool nCmdShow) {
	__hinst_cur = inst;
	__hinst_prev = prev;
	__nCmdShow = nCmdShow;
}
HINSTANCE GetWin32Instance() {return __hinst_cur;}
HINSTANCE GetWin32PrevInstance() {return __hinst_prev;}
bool GetWin32CmdShow() {return __nCmdShow;}
#endif












bool IsDotString(String s) {return s == "." || s == "..";}

void GetDirectoryFiles(String dir, Index<String>& files) {
	files.Clear();
	
	
	#ifdef flagPOSIX
	DIR* dirp = opendir(dir.Begin());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (!IsDotString(dp->d_name))
            files.Add(dp->d_name);
    }
    closedir(dirp);
    #endif
    
    #ifdef flagWIN32
	String pattern = AppendFileName(dir, "*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.Begin(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            if (!IsDotString(data.cFileName))
                files.Add(data.cFileName);
        }
        while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
    #endif
}



FindFile::FindFile() {
	
}

bool FindFile::Search(String path) {
	TODO
}

bool FindFile::Next() {
	TODO
}

bool FindFile::IsDirectory() const {
	TODO
}

String FindFile::GetPath() const {
	TODO
}

String FindFile::GetName() const {
	TODO
}








Index<String> string_cache;
RWMutex string_cache_lock;

int GetCachedStringId(String path) {
	string_cache_lock.EnterRead();
	int i = string_cache.Find(path);
	string_cache_lock.LeaveRead();
	if (i < 0) {
		string_cache_lock.EnterWrite();
		i = string_cache.FindAdd(path);
		string_cache_lock.LeaveWrite();
	}
	return i;
}

String GetCachedString(int id) {
	if (id < 0) return String();
	string_cache_lock.EnterRead();
	String s = string_cache[id];
	string_cache_lock.LeaveRead();
	return s;
}












void CommandLineArguments::AddArg(char key, const char* desc, bool has_value) {
	Arg& a = args.Add();
	a.key = key;
	a.desc = desc;
	a.has_value = has_value;
}

bool CommandLineArguments::Parse() {
	const Vector<String>& args = CommandLine();
	
	for(int i = 0; i < args.GetCount(); i++) {
		String arg = args[i];
		
		if (arg.GetCount() < 2) return false;
		
		bool found = false;
		
		if (arg[0] == '-') {
			char key = arg[1];
			if (key) {
				for(int j = 0; j < this->args.GetCount(); j++) {
					Arg& a = this->args[j];
					if (a.key == key) {
						found = true;
						Input& in = inputs.Add();
						in.key = key;
						if (a.has_value) {
							if (i+1 >= args.GetCount()) {
								cerr << "No value provided: " << arg << endl;
								return false;
							}
							in.value = args[i+1];
							i++;
						}
						break;
					}
				}
			}
		}
		
		if (!found) {cerr << "Invalid argument: " << arg << endl; return false;}
	}
	
	return true;
}

void CommandLineArguments::PrintHelp() {
	String exe = GetExeFilePath();
	int i = exe.ReverseFind(DIR_SEPS);
	if (i >= 0)
		exe = exe.Mid(i+1);
	
	cout << exe << " [-";
	for(int i = 0; i < args.GetCount(); i++)
		if (args[i].has_value)
			cout << args[i].key;
	cout << " value]";
	
	cout << " [-";
	for(int i = 0; i < args.GetCount(); i++)
		if (!args[i].has_value)
			cout << args[i].key;
	cout << "]" << endl;
	
	for(int i = 0; i < args.GetCount(); i++) {
		Arg& a = args[i];
		cout << "\t-" << a.key;
		if (a.has_value)
			cout << " value";
		cout << ": " << a.desc << endl;
	}
}




















void FillVector(Vector<double>& v) {
	int first = -1;
	for(int i = 0; i < v.GetCount(); i++)
		if (v[i] != 0.0)
			{first = i; break;}
	if (first < 0)
		return;
	double left_pad = v[first];
	for(int i = first-1; i >= 0; i--)
		v[i] = left_pad;
	double prev = left_pad;
	for(int i = first+1; i < v.GetCount(); i++) {
		double& d = v[i];
		if (!d)
			d = prev;
		else
			prev = d;
	}
}

String UpperBegin(String s) {
	String o;
	int is_upper = true;
	for(int i = 0; i < s.GetCount(); i++) {
		int chr = s[i];
		if (is_upper) {
			o.Cat(ToUpper(chr));
			is_upper = false;
		}
		else
			o.Cat(ToLower(chr));
		if (chr == ' ')
			is_upper = true;
	}
	return o;
}

String GetTimeDurationString(Time begin, Time end) {
	String out;
	int64 total = end.Get() - begin.Get();
	bool neg = false;
	if (total < 0) {
		neg = true;
		total *= -1;
	}
	int sec = total % 60;
	total /= 60;
	int min = total % 60;
	total /= 60;
	int hours = total % 24;
	total /= 24;
	int days = total % 30;
	total /= 30;
	int months = total % 12;
	total /= 12;
	int64 years = total;
	if (years)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d years"), years);}
	if (months)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d months"), months);}
	if (days)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d days"), days);}
	if (hours)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d hours"), hours);}
	if (min)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d minutes"), min);}
	if (sec)		{if (out.GetCount()) out << ", ";	out << Format(t_("%d seconds"), sec);}
	
	if (neg) out = "-" + out;
	return out;
}


bool IsClose(double a, double b) {
	if (IsFin(a) && IsFin(b)) {
		double diff = fabs(a - b);
		return diff < 0.0001;
	}
	else {
		return *(uint64*)&a == *(uint64*)&b;
	}
}

double Multiply(double prev_norm, int64 count, double multiplier) {
	if (count <= 0)
		return multiplier;
	if (prev_norm == 0.0 || multiplier == 0.0)
		return 0.0;
	ASSERT(count < 300);
	return FastPow(FastPow(prev_norm, (double)count) * multiplier, 1.0 / (count + 1));
}



bool StreamCopy(Stream& dest, Stream& src) {
	dest.Seek(0);
	src.Seek(0);
	byte buf[1024];
	int64 size_left = src.GetSize();
	while (size_left > 0) {
		int block = size_left < 1024 ? (int)size_left : 1024;
		int got = src.Get(buf, block);
		if (got != block)
			return false;
		dest.Put(buf, block);
		size_left -= block;
	}
	return true;
}


String shared_dir;

void SetSharedDirectory(String d) {shared_dir = d;}
String GetSharedDirectory() {return shared_dir;}
String SharedFile(String file) {
	if (shared_dir.IsEmpty())
		return ConfigFile(file);
	else
		return AppendFileName(shared_dir, file);
}

NAMESPACE_SDK_END
