#ifndef _Core_Hash_h_
#define _Core_Hash_h_

NAMESPACE_SDK_BEGIN

class CombineHash : Moveable<CombineHash> {
	uint32 hash = 123456789;

public:
	CombineHash() {}
	CombineHash(int v0) { Put(v0); }
	CombineHash(int v0, int v1) { Put(v0); Put(v1); }

	operator uint32() const { return hash; }
	uint32 Get() const { return hash; }

	CombineHash& Put(int value) { hash = ((hash << 5) + hash) + value; return *this; }
	CombineHash& Put64(int64 value) {Put((int)(value >> 32)); Put((int)(value & 0xFFFFFFFFULL)); return *this; }
	
	CombineHash& operator << (int value) { Put(value); return *this; }
};





template <class K> inline uint32 GetHashValue(const K& k) { return k.GetHashValue(); }
template <> inline uint32 GetHashValue(const char& k) { return k; }
template <> inline uint32 GetHashValue(const int& k) { return k; }
template <> inline uint32 GetHashValue(const uint32& k) { return k; }
template <> inline uint32 GetHashValue(const uint64& k) {union {uint64 u64; dword dw[2];}; u64 = k; return dw[0] ^ dw[1];}
template <> inline uint32 GetHashValue(const float& k) { return static_cast<uint32>(k); }
template <> inline uint32 GetHashValue(const double& k) {return GetHashValue(*(uint64*)&k);}
template <> inline uint32 GetHashValue(const VOID_PTR& k) {
	#ifdef CPU_64
	return GetHashValue((uint64)k);
	#else
	return GetHashValue((uint32)k);
	#endif
}

NAMESPACE_SDK_END

#endif
