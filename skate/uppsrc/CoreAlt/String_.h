#ifndef _Core_String_h_
#define _Core_String_h_

#include "Defs.h"
#include <memory>

NAMESPACE_SDK_BEGIN

class Stream;


inline bool IsOctDigit(char c) { return c >= '0' && c <= '7'; }
inline bool IsHexDigit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
inline bool IsDigit(char c) { return c >= '0' && c <= '9'; }
inline bool IsSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
inline bool IsAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
inline int GetHexDigit(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	return 0;
}

inline void MemoryCopySlow(void* dst, const void* src, int len) {
	uint32* dst4 = (uint32*)dst;
	uint32* src4 = (uint32*)src;
	while (len >= 4) {
		uint32 value = *src4++;

		*dst4++ = value;

		len -= 4;
	}
	uint8* dst1 = (uint8*)dst4;
	uint8* src1 = (uint8*)src4;
	while (len > 0) {
		*dst1++ = *src1++;
		len -= 1;
	}
}

inline void MemorySet(void* dst, int value, int len) {
	byte bvalue = value & 0xFF;
	byte* b = (byte*)dst;
	for (int i = 0; i < len; i++) {
		*b++ = bvalue;
	}
}

void MemoryMoveSlow(void* dst, const void* src, int len);

/*int CharInt(const char* s);
int64 CharInt64(const char* s);
uint64 CharIntU64(const char* s);
const char* IntChar(char* p, int bufsize, int x);
const char* IntChar64(char* p, int bufsize, int64 x);
const char* IntCharU64(char* p, int bufsize, uint64 x);*/


template <class T>
class String0 {
	T* data = NULL;
	AtomicInt refs;
	int reserved = 0;

public:
	String0(T* str, int reserved) : data(str), refs(1), reserved(reserved) {}
	String0(String0&& s) = delete;
	~String0() {Clear();}

	void Clear() {if (data) {free(data); data = 0; refs = 0; reserved = 0;}}
	
	void IncreaseReserved() {
		uint64 new_reserved = 1;
		while (new_reserved <= reserved)
			new_reserved <<= 1;
		if (new_reserved >= INT_MAX)
			new_reserved = INT_MAX;
		Reserve((int)new_reserved);
	}
	void IncreaseReserved(int min_reserved) {
		uint64 new_reserved = 1;
		while (new_reserved <= min_reserved)
			new_reserved <<= 1;
		if (new_reserved >= INT_MAX)
			new_reserved = INT_MAX;
		Reserve((int)new_reserved);
	}
	void Reserve(int new_reserved) {
		if (new_reserved <= reserved)
			return;
		T* new_data = (T*)malloc(new_reserved * sizeof(T));
		if (data) {
			if (reserved > 0)
				MemoryCopy(new_data, data, sizeof(T) * reserved);
			free(data);
		}
		data = new_data;
		reserved = new_reserved;
	}

	const T* Get() const { return data; }
	T* Begin() { return data; }
	operator const T* () const { return data; }

	int GetReserved() const { return reserved; }
	int GetRefs() const { return refs; }
	void Inc() { refs++; }
	void Dec() { refs--; if (refs <= 0) delete this; }
};


template <class T>
class StringT : Moveable<StringT<T>> {
	typedef String0<T> String0T;
	static const int buf_size = 8;
	static const int buf_bytes = buf_size * sizeof(T);
#if 0
	String0T* s = NULL;
	T buf[buf_size];
	#define BIG_FIELD s
#else
	static_assert(buf_bytes % sizeof(void*) == 0, "buffer size modulus ptr size must be zero");
	static const int ptr_count = buf_bytes / sizeof(void*);
	union {
		T buf[buf_size];
		String0T* s[ptr_count];
	};
	#define BIG_FIELD s[0]
	void Zero() {memset(this, 0, sizeof(StringT));}
#endif
	
	#define BIG (this->BIG_FIELD)
	
	bool is_big = false;
	int count = 0;
	
	
public:
	StringT() {Zero();}
	StringT(const T* c) {Zero(); *this = c;}
	StringT(const T* c, int len) {Zero(); Set(c, len);}
	StringT(const StringT& s) {Zero(); *this = s;}
	StringT(StringT&& s) {Zero(); Swap(*this, s);}
	~StringT() {
		Clear();
	}

	void Clear() {
		if (is_big) { BIG->Dec(); BIG = NULL; is_big = false;} else {buf[0] = 0;} count = 0;
	}
	
	const String0T* GetString0() const {return BIG;}
	
	void Serialize(Stream& s);
	
	StringT& operator=(const T* c) {
		Clear();
		if (!c) return *this;
		count = Length(c);
		if (count < buf_size) {
			Copy(buf, c);
		}
		else {
			T* buf = (T*)malloc(sizeof(T) * (count + 1));
			MemoryCopy(buf, c, sizeof(T) * count);
			buf[count] = 0;
			BIG = new String0T(buf, count + 1);
			is_big = true;
		}
		return *this;
	}
	StringT& Set(const T* c, int len) {
		Clear();
		if (!c) return *this;
		count = min(Length(c, len), len);
		if (count < buf_size) {
			MemoryCopy(buf, c, sizeof(T) * count);
			buf[count] = 0;
		}
		else {
			T* buf = (T*)malloc(sizeof(T) * (count + 1));
			MemoryCopy(buf, c, sizeof(T) * count);
			buf[count] = 0;
			BIG = new String0T(buf, count + 1);
			is_big = true;
		}
		return *this;
	}
	StringT& SetData(const T* c, int len) {
		Clear();
		if (!c) return *this;
		ASSERT(len > 0);
		count = len;
		if (count < buf_size) {
			MemoryCopy(buf, c, sizeof(T) * count);
		}
		else {
			T* buf = (T*)malloc(sizeof(T) * count);
			MemoryCopy(buf, c, sizeof(T) * count);
			BIG = new String0T(buf, count);
			is_big = true;
		}
		ASSERT(c[len-1] == 0);
		return *this;
	}
	StringT& operator=(const StringT& str) {
		if (this == &str)
			return *this;
		if (!str.is_big) {
			Clear();
			MemoryCopy(buf, str.buf, sizeof(buf));
			is_big = false;
		}
		else {
			if (BIG == str.BIG_FIELD)
				return *this;
			Clear();
			is_big = true;
			BIG = str.BIG_FIELD;
			BIG->Inc();
		}
		count = str.count;
		return *this;
	}
	StringT& Cat(T c, int count) {
		for(int i = 0; i < count; i++) Cat(c);
		return *this;
	}
	StringT& Cat(T i) {
		if (!is_big) {
			is_big = count + 1 >= buf_size;
			if (!is_big) {
				buf[count] = i;
				count++;
				buf[count] = 0;
			}
			else {
				T* buf = (T*)malloc(sizeof(T) * (count + 1 + 1));
				MemoryCopy(buf, this->buf, sizeof(T) * count);
				buf[count] = i;
				count++;
				buf[count] = 0;
				BIG = new String0T(buf, count + 1);
				is_big = true;
			}
		}
		else {
			if (BIG->GetRefs() == 1) {
				int new_count = count + 1 + 1;
				if (new_count > BIG->GetReserved())
					BIG->IncreaseReserved();
				T* c = BIG->Begin();
				c[count] = i;
				count++;
				c[count] = 0;
			}
			else {
				T* buf = (T*)malloc(sizeof(T) * (count + 1 + 1));
				MemoryCopy(buf, BIG->Get(), sizeof(T) * count);
				buf[count] = i;
				count++;
				buf[count] = 0;
				BIG->Dec();
				BIG = new String0T(buf, count + 1);
			}
		}
		return *this;
	}
	StringT& Cat(const StringT& str) {
		if (!is_big) {
			is_big = count + str.GetCount() >= buf_size;
			if (!is_big) {
				MemoryCopy(&buf[count], str.Begin(), sizeof(T) * (str.GetCount() + 1));
				count += str.GetCount();
			}
			else {
				int reserved = count + str.GetCount() + 1;
				T* buf = (T*)malloc(sizeof(T) * (reserved));
				MemoryCopy(buf, this->buf, sizeof(T) * count);
				MemoryCopy(&buf[count], str.Begin(), sizeof(T) * (str.GetCount() + 1));
				count += str.GetCount();
				BIG = new String0T(buf, reserved);
				is_big = true;
			}
		}
		else {
			if (BIG->GetRefs() == 1) {
				int new_count = count + str.GetCount() + 1;
				if (new_count > BIG->GetReserved())
					BIG->IncreaseReserved(new_count);
				T* c = BIG->Begin();
				MemoryCopy(c + count, str.Begin(), sizeof(T) * str.GetCount());
				count += str.GetCount();
				c[count] = 0;
			}
			else {
				int reserved = count + str.GetCount() + 1;
				T* buf = (T*)malloc(sizeof(T) * (reserved));
				MemoryCopy(buf, BIG->Get(), sizeof(T) * count);
				MemoryCopy(&buf[count], str.Begin(), sizeof(T) * (str.GetCount() + 1));
				count += str.GetCount();
				buf[count] = 0;
				BIG->Dec();
				BIG = new String0T(buf, reserved);
			}
		}
		return *this;
	}

	void Replace(const StringT& s, const StringT& value) {
		if (s == value) return;
		int i = Find(s);
		while (i >= 0) {
			Replace(i, s.GetCount(), value);
			i = Find(s, i + value.GetCount());
		}
	}

	void Replace(int i, int len, const StringT& value) {
		int new_len = this->count - len + value.GetCount();
		const T* begin = Begin();
		int reserved = new_len + 1;
		T* buf = (T*)malloc(sizeof(T) * (reserved));
		int cursor = 0;
		if (i > 0) {
			MemoryCopy(buf, begin, sizeof(T) * i);
			cursor += i;
		}
		if (value.GetCount() > 0) {
			MemoryCopy(buf + cursor, value.Begin(), sizeof(T) * value.GetCount());
			cursor += value.GetCount();
		}
		int c = i + len;
		int end_len = this->count - c;
		if (end_len > 0) {
			MemoryCopy(buf + cursor, begin + c, sizeof(T) * end_len);
			cursor += end_len;
		}
		buf[cursor++] = 0;

		Clear();
		this->is_big = true;
		this->count = new_len;
		BIG = new String0T(buf, reserved);
		ASSERT(cursor == new_len+1);
	}

	int GetCount() const { return count; }
	bool IsEmpty() const { return count == 0; }
	int Find(const StringT& str, int pos = 0) const {
		if (GetCount() == 0) return -1;
		if (pos == count)
			return -1;
		ASSERT(pos >= 0 && pos < GetCount());
		const T* cur = Begin();
		const T* cmp = str.Begin();
		int len = str.GetCount();
		for (int i = pos; i < count; i++) {
			if (Compare(cur + i, cmp, len) == 0)
				return i;
		}
		return -1;
	}
	int ReverseFind(const StringT& str) const { return ReverseFind(str, GetCount() - 1); }
	int ReverseFind(const StringT& str, int pos) const {
		if (GetCount() == 0) return -1;
		ASSERT(pos >= 0 && pos < GetCount());
		const T* cur = Begin();
		const T* cmp = str.Begin();
		int len = str.GetCount();
		for (int i = pos; i >= 0; i--) {
			if (Compare(cur + i, cmp, len) == 0)
				return i;
		}
		return -1;
	}
	int FindFirstNotOf(const T* str) const {
		if (GetCount() <= 0 || !str) return -1;
		const T* it  = Begin();
		const T* end = End();
		int i = 0;
		while (it != end) {
			T chr = *it++;
			const T* cmp_it = str;
			bool match = false;
			while (1) {
				T cmp_chr = *cmp_it++;
				if (!cmp_chr)
					break;
				if (chr == cmp_chr) {
					match = true;
					break;
				}
			}
			if (!match)
				return i;
			i++;
		}
		return -1;
	}
	int ReverseFindFirstNotOf(const T* str) const {
		if (GetCount() <= 0 || !str) return -1;
		const T* begin = Begin();
		const T* it = End();
		int i = GetCount();
		while (it != begin) {
			T chr = *--it;
			const T* cmp_it = str;
			bool match = false;
			while (1) {
				T cmp_chr = *cmp_it++;
				if (!cmp_chr)
					break;
				if (chr == cmp_chr) {
					match = true;
					break;
				}
			}
			i--;
			if (!match)
				return i;
		}
		return -1;
	}
	StringT Mid(int i) const { if (i >= GetCount()) return ""; if (i < 0) i = 0; return Mid(i, GetCount() - i); }
	StringT Mid(int i, int size) const {
		size = min(size, count - i);
		if (size <= 0) return StringT();
		ASSERT(i >= 0 && i + size <= count);
		return StringT(Begin() + i, size);
	}
	StringT Left(int i) const { return Mid(0, i); }
	StringT Right(int i) const { return Mid(GetCount() - i, i); }
	
	void Remove(int begin, int count) {
		ASSERT(begin >= 0 && count >= 0);
		begin = max(0, begin);
		count = max(0, count);
		int end = begin + count;
		int c = GetCount();
		ASSERT(begin <= c && end <= c);
		end = min(c, end);
		begin = min(c, begin);
		int l = begin;
		int r = c - end;
		if (l && r)
			*this = Left(l) + Right(r);
		else if (l)
			*this = Left(l);
		else if (r)
			*this = Right(r);
		else
			Clear();
	}
	bool Insert(int begin, const T* str, int n) {
		if (begin < 0 || begin > GetCount() || !str || n <= 0)
			return false;
		ASSERT(begin >= 0 && begin <= GetCount());
		begin = max(0, min(begin, GetCount()));
		StringT part(str, n);
		int l = begin;
		int r = GetCount() - begin;
		if (l && r)
			*this = Left(l) + part + Right(r);
		else if (l)
			*this = Left(l) + part;
		else if (r)
			*this = part + Right(r);
		else
			*this = part;
		return true;
	}
	const T* Begin() const { return is_big ? BIG->Get() : buf; }
	const T* End() const { return Begin() + GetCount(); }

	bool operator==(const StringT& s) const { if (s.GetCount() != GetCount()) return false; return Compare(s.Begin(), Begin()) == 0; }
	bool operator!=(const StringT& s) const { return !(*this == s); }

	T operator[] (int i) const {return At(i);}
	T At(int i) const { ASSERT(i >= 0 && i < count); return *(Begin() + i); }

	StringT& operator += (const StringT& s) { Cat(s); return *this; }
	StringT& operator << (const StringT& s) { Cat(s); return *this; }
	StringT& operator << (int i) { Cat(IntStr(i)); return *this; }
	StringT operator + (const StringT& s) { StringT out(*this); out.Cat(s); return out; }

	static int OctInt(const StringT& s) { return CharOctInt(s.Begin()); }
	static int HexInt(const StringT& s) { return CharHexInt(s.Begin()); }
	static int StrInt(const StringT& s) { return CharInt(s.Begin()); }
	static int64 StrInt64(const StringT& s) { return CharInt64(s.Begin()); }
	static double StrDbl(const StringT& s) { return CharDbl(s.Begin()); }

	static StringT IntStr(int i) {
		T buf[10];
		const T* value = IntChar(buf, 10, i);
		return StringT(value);
	}

	static StringT IntStr64(int64 i) {
		T buf[30];
		const T* value = IntChar64(buf, 30, i);
		return StringT(value);
	}

	static StringT DblStr(double d) {
		char output[50];
		snprintf(output, 50, "%g", d);
		printf("%s", output);
		return StringT(output);
	}

	StringT ToString() const { return *this; }

	uint32 GetHashValue() const {
		CombineHash ch;
		const T* buf = Begin();
		for (int i = 0; i < count; i++)
			ch.Put(*buf++);
		return ch;
	}
	
	#include "StringInline.inl"
};

typedef StringT<char> String;
typedef StringT<wchar_t> WString;

inline String operator+ (const char* c, const String& l) { String s(c); s.Cat(l); return s; }
inline String operator+ (const String& c, const String& l) { String s(c); s.Cat(l); return s; }

struct Exc : public String {
	Exc() {}
	Exc(const String& s) : String(s) {}
};
typedef Exc MemoryLimitExc;
typedef Exc NeverExc;
typedef Exc TodoExc;
typedef Exc SystemExc;

#define NEVER() throw NeverExc()

inline int ToUpper(int chr) {
	if (chr >= 'a' && chr <= 'z')
		return 'A' + (chr - 'a');
	return chr;
}

inline int ToLower(int chr) {
	if (chr >= 'A' && chr <= 'Z')
		return 'a' + (chr - 'A');
	return chr;
}

String ToUpper(const String& s);
String ToLower(const String& s);

inline String ToUpperFirst(const String& s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}

inline double StrDbl(String s) { return String::StrDbl(s); }
inline int StrInt(String s) { return String::StrInt(s); }
inline int OctInt(String s) { return String::OctInt(s); }
inline int HexInt(String s) { return String::HexInt(s); }
inline int64 StrInt64(String s) { return String::StrInt64(s); }
inline String IntStr(int i) { return String::IntStr(i); }
inline String IntStr64(int64 i) { return String::IntStr64(i); }
inline String DblStr(double d) { return String::DblStr(d); }

String EscapeString(String s);
String EscapeCharacter(String s);

String TrimBoth(String s);


template <class T> inline String ToString(const T& o) {return o.ToString();}
template<> inline String ToString(const bool& o) {return o ? "true" : "false";}
template<> inline String ToString(const int& o) {return IntStr(o);}
template<> inline String ToString(const uint32& o) {return IntStr64(o);}
template<> inline String ToString(const int64& o) {return IntStr64(o);}
template<> inline String ToString(const byte& o) {return IntStr(o);}
template<> inline String ToString(const char& o) {String s; s.Cat(o); return s;}
template<> inline String ToString(const float& o) {return DblStr(o);}
template<> inline String ToString(const double& o) {return DblStr(o);}
template<> inline String ToString(const String& o) {return o;}
template <class T> inline WString ToWString(const T& o) {return o.ToWString();}

#ifdef flagWIN32
/*template<> inline String ToString(const WString& o) {return WideCharToMultiByte(o.Begin());}
template<> inline WString ToWString(const CString& o) {return MultiByteToWideChar(std::string(o));}
template<> inline WString ToWString(const String& o) {return MultiByteToWideChar(std::string(o.Begin()));}*/
#else

#if 0
typedef std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> UnicodeConverter;
inline UnicodeConverter& GetUnicodeConverter() {static UnicodeConverter conv; return conv;}
template<> inline String ToString(const WString& o) {return GetUnicodeConverter().to_bytes(std::wstring(o.Begin())).c_str();}
template<> inline WString ToWString(const CString& o) {return GetUnicodeConverter().from_bytes(std::string(o)).c_str();}
template<> inline WString ToWString(const String& o) {return GetUnicodeConverter().from_bytes(std::string(o.Begin())).c_str();}
#else
template<> inline String ToString(const WString& o) {ASSERT(0); return String();}
template<> inline WString ToWString(const CString& o) {ASSERT(0); return WString();}
template<> inline WString ToWString(const String& o) {ASSERT(0); return WString();}
#endif
#endif

template <class T> inline int64 ToInt(const T& o) {return o.ToInt();}
template<> inline int64 ToInt(const bool& o) {return (int64)o;}
template<> inline int64 ToInt(const int& o) {return (int64)o;}
template<> inline int64 ToInt(const int64& o) {return (int64)o;}
template<> inline int64 ToInt(const byte& o) {return (int64)o;}
template<> inline int64 ToInt(const char& o) {return (int64)o;}
template<> inline int64 ToInt(const float& o) {return (int64)o;}
template<> inline int64 ToInt(const double& o) {return (int64)o;}
template<> inline int64 ToInt(const String& o) {return StrInt(o);}

inline int HexDigit(int c) {return "0123456789ABCDEF"[c & 15];}
inline int HexDigitLower(int c) {return "0123456789abcdef"[c & 15];}
inline String HexStr(uint64 p) {
	String o("0x");
	bool zero = true;
	uint64 i = 64;
	do {
		i--;
		byte b = (p >> i) & 15ULL;
		if (b || !zero) {
			zero = false;
			o.Cat(HexDigit(b));
		}
	}
	while (i);
	if (zero) o.Cat('0');
	return o;
}
inline String HexStr(void* p) {return HexStr((uint64)p);}

NAMESPACE_SDK_END

#endif
