#ifndef _Core_Stream_h_
#define _Core_Stream_h_

#ifdef flagPOSIX
inline int fopen_s(FILE **f, const char *name, const char *mode) {
    int ret = 0;
    ASSERT(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}
#endif

NAMESPACE_SDK_BEGIN

class Stream {

public:

	virtual String Get(int size) { return ""; }
	virtual bool IsOpen() const { return false; }

	virtual bool IsLoading() { return false; }
	virtual bool IsStoring() { return false; }
	virtual bool IsEof() { return false; }

	virtual Stream& operator << (String s) { return *this; }
	virtual Stream& operator << (int i) { return *this; }
	virtual int Put(const void* mem, int size) { return 0; }
	virtual int Put(char c) { return Put(&c, 1); }
	virtual int Get(void* mem, int size) { return 0; }
	virtual int64 GetCursor() { return 0; }
	virtual void Seek(int64 pos) {}
	virtual int64 GetSize() const {return 0;}
	
	virtual void Flush() {};
	
	String GetLine(int max = UINT16_MAX) {
		String out;
		while (!IsEof() && out.GetCount() < max) {
			char chr;
			Get(&chr, 1);
			if (chr == '\n')
				break;
			out.Cat(chr);
		}
		return out;
	}
	
	template <class T>
	Stream& operator%(T& o) {o.Serialize(*this); return *this;}
	

};

#define DEFAULT_SERIALIZER(type) template <> inline Stream& Stream::operator%(type& o) {\
	if (IsLoading()) Get(&o, sizeof(type)); \
	else Put(&o, sizeof(type)); \
	return *this; \
}

DEFAULT_SERIALIZER(bool)
DEFAULT_SERIALIZER(int8)
DEFAULT_SERIALIZER(uint8)
DEFAULT_SERIALIZER(int16)
DEFAULT_SERIALIZER(uint16)
DEFAULT_SERIALIZER(int32)
DEFAULT_SERIALIZER(uint32)
DEFAULT_SERIALIZER(int64)
DEFAULT_SERIALIZER(uint64)
DEFAULT_SERIALIZER(float)
DEFAULT_SERIALIZER(double)

class FileIn : public Stream {
	mutable FILE* s = NULL;

public:
	FileIn() {}
	FileIn(FILE* s) : s(s) {}
	FileIn(String path) {Open(path);}
	~FileIn() { Close(); }

	virtual bool IsLoading() { return true; }

	virtual bool IsOpen() const { return s != NULL; }
	virtual bool IsEof() { if (!s) return true; return feof(s); }

	bool Open(String path) {
		if (s) fclose(s);
		fopen_s(&s, path.Begin(), "rb");
		return s != NULL;
	}

	void Close() {
		if (s) fclose(s); s = NULL;
	}

	virtual String Get(int size) {
		if (!s || !size) return String();
		ASSERT(size > 0);
		int pos = ftell(s);
		Vector<char> v;
		v.SetCount(size + 1, 0);
		char* it = v.Begin();
		int left = size;
		int total_read = 0;
		while (left > 0) {
			int read_size = (int)fread(it, 1, left, s);
			left -= read_size;
			it += read_size;
			total_read += read_size;
			if (!read_size)
				break;
		}
		String out;
		out.SetData(v.Begin(), total_read+1);
		return out;
	}

	String GetText(int size) {
		if (!s) return "";
		Vector<char> v;
		v.SetCount(size+1);
		v[size] = 0;
		size_t read_size = fread(v.Begin(), 1, size, s);
		return v.Get();
	}

	virtual int Get(void* mem, int size) {
		if (!s) return 0;
		int64 ret = fread(mem, size, 1, s) * size;
		ASSERT(ret < INT_MAX);
		return (int)ret;
	}

	virtual int64 GetCursor() { if (!s) return 0; return ftell(s); }

	virtual int64 GetSize() const {
		if (!s) return 0;
		long pos = ftell(s);
		fseek(s, 0, SEEK_END);
		int size = ftell(s);
		fseek(s, pos, SEEK_SET);
		return size;
	}

	virtual void Seek(int64 pos) { fseek(s, (long)pos, SEEK_SET); }

};

String LoadFile(String path);


class FileOut : public Stream {
	mutable FILE* s = NULL;

public:
	FileOut() {}
	FileOut(FILE* s) : s(s) {}
	FileOut(String path) {Open(path);}
	~FileOut() { Close(); }

	virtual bool IsStoring() { return true; }

	virtual bool IsOpen() const { return s != NULL; }
	virtual bool IsEof() { if (!s) return true; return feof(s); }

	bool Open(String path) {
		if (s) fclose(s);
		#ifdef flagMSC
		s = _fsopen(path.Begin(), "w+b", _SH_DENYWR);
		#else
		s = fopen(path.Begin(), "w+b");
		#endif
		return s != NULL;
	}

	void Close() {
		if (s) { fflush(s); fclose(s); s = NULL; }
	}

	virtual Stream& operator << (String str) {
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	virtual Stream& operator << (int i) {
		String str = IntStr(i);
		Put(str.Begin(), str.GetCount());
		return *this;
	}

	virtual int Put(const void* mem, int size) {
		if (!s) return 0;
		int64 ret = fwrite(mem, size, 1, s) * size;
		ASSERT(ret < INT_MAX);
		return (int)ret;
	}
	virtual int Put(char c) { return Put(&c, 1); }

	virtual void Flush() { if (s) fflush(s); }
	
	virtual int64 GetCursor() { if (!s) return 0; return ftell(s); }

	virtual int64 GetSize() const {
		if (!s) return 0;
		long pos = ftell(s);
		fseek(s, 0, SEEK_END);
		int size = ftell(s);
		fseek(s, pos, SEEK_SET);
		return size;
	}

};

extern FileIn cin;
extern FileOut cout, cerr;

class StringStream : public Stream {
	Vector<char> s;
	int64 cursor = 0;
	bool is_storing = true;
	
public:
	StringStream() {}

	virtual bool IsLoading() { return !is_storing; }
	virtual bool IsStoring() { return is_storing; }
	virtual bool IsEof() { return cursor >= s.GetCount(); }
	
	void SetStoring() {is_storing = true;}
	void SetLoading() {is_storing = false;}
	
	virtual Stream& operator << (String str) {
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	virtual Stream& operator << (int i) {
		String str = IntStr(i);
		Put(str.Begin(), str.GetCount());
		return *this;
	}
	virtual int Put(const void* mem, int size) {
		int64 end = cursor + size;
		ASSERT(end < INT_MAX);
		if (end > s.GetCount())
			s.SetCount((int)end);
		char* dst = s.Begin() + cursor;
		const char* src = (const char*)mem;
		MemoryCopy(dst, src, size);
		cursor += size;
		return size;
	}
	virtual int Put(char c) { return Put(&c, 1); }
	virtual int Get(void* mem, int size) {
		int64 sz = min((int64)size, (int64)s.GetCount() - cursor);
		if (sz <= 0) return 0;
		ASSERT(sz < INT_MAX);
		char* b = (char*)mem;
		MemoryCopy(b, s.Begin() + cursor, (int)sz);
		cursor += sz;
		return (int)sz;
	}
	virtual int64 GetCursor() { return cursor; }
	virtual int64 GetSize() const {return s.GetCount();}

	void Seek(int64 i) { cursor = i; }
	String GetResult() { return String(s.Begin(), s.GetCount()); }
};

class MemReadStream : public Stream {
	const char* buf;
	int64 size = 0;
	int64 cursor = 0;
	
public:
	MemReadStream(const char* buf, int64 size) : buf(buf), size(size) {}

	virtual bool IsLoading() { return true; }
	virtual bool IsStoring() { return false; }
	virtual bool IsEof() { return cursor >= size; }
	
	virtual int Get(void* mem, int size) {
		int64 sz = min((int64)size, size - cursor);
		if (sz <= 0) return 0;
		ASSERT(sz < INT_MAX);
		char* b = (char*)mem;
		MemoryCopy(b, buf + cursor, (int)sz);
		cursor += sz;
		return (int)sz;
	}
	virtual int64 GetCursor() { return cursor; }
	void Seek(int64 i) { cursor = i; }
	virtual int64 GetSize() const {return size;}
};



template <class T>
void StringT<T>::Serialize(Stream& str) {
	uint32 len = 0;
	if (str.IsLoading()) {
		Clear();
		str.Get(&len, sizeof(len));
		count = len;
		if (count < buf_size) {
			str.Get(buf, len*sizeof(T));
			buf[count] = 0;
		}
		else {
			T* buf = (T*)malloc(sizeof(T) * (count + 1));
			str.Get(buf, len*sizeof(T));
			buf[count] = 0;
			BIG = new String0T(buf, count + 1);
			is_big = true;
		}
	}
	else {
		len = GetCount();
		str.Put(&len, sizeof(len));
		str.Put(Begin(), len*sizeof(T));
	}
}

template <class T>
void Vector<T>::Serialize(Stream& str) {
	uint32 len = 0;
	if (str.IsLoading()) {
		str.Get(&len, sizeof(len));
		SetCount(len);
	}
	else {
		len = GetCount();
		str.Put(&len, sizeof(len));
	}
	for(T& o : *this)
		str % o;
}

template <class T>
void LoadFromFile(T& o, String path) {
	FileIn fin(path);
	if (!fin.IsOpen()) return;
	fin % o;
}

template <class T>
void StoreToFile(T& o, String path) {
	FileOut fout(path);
	if (!fout.IsOpen()) return;
	fout % o;
}






class AudioFrame {
	
	
public:
	virtual ~AudioFrame() {}
	
};

class VideoFrame {
	
	
public:
	virtual ~VideoFrame() {}
	
};

class MediaStream {
	
	
public:
	
	virtual bool Initialize(AudioFrame& a, VideoFrame& v) = 0;
	virtual String GetLastError() const {return "unknown";}
	
};



NAMESPACE_SDK_END

#endif
