#ifndef _Core_Algo_h_
#define _Core_Algo_h_

#include <algorithm>

NAMESPACE_SDK_BEGIN

#undef min
#undef max
template <class T> const T& min(const T& a, const T& b) { return a < b ? a : b; }
template <class T> const T& max(const T& a, const T& b) { return a > b ? a : b; }


uint64 sNext(uint64 *s);
void sSeed(uint64 *s);

class RNG {
	uint64 state[4];
	
public:
	RNG() {sSeed(state);}
	double Randomf() {return (sNext(state) >> 11) * (1. / (UINT64_C(1) << 53));}
	uint64 Random(uint64 max) {return Get() % max;}
	uint64 Get() {return sNext(state);}
	operator uint64 () {return Get();}
	
	static uint64_t s_rotl(const uint64_t x, int k) {
		return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
	}
};

inline RNG& GetRNG() {static thread_local RNG r; return r;}


dword Random();
void Random64(uint64* t, int n);
dword Random(dword n);
uint64 Random64(uint64 n);
double Randomf();
void SeedRandom();
void SeedRandom(dword seed);


template <class I, class T>
I FindIf(I begin, I end, std::function<T> fn) {
	while (begin != end) {
		if (fn(*begin))
			return begin;
		begin++;
	}
	return end;
}



template <class T, class K>
inline bool IsInheritedBy(const K& o) {return dynamic_cast<const T*>(&o);}

template <class T, class K>
inline T& Cast(K& o) {return dynamic_cast<T&>(o);}









class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
    void Enter() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void Leave() {
        locked.clear(std::memory_order_release);
    }
};

struct AtomicBool {
	std::atomic<bool> value;
	AtomicBool() {value = 0;}
	AtomicBool(bool b) {value = b;}
	AtomicBool(const AtomicBool& ai) {value = (bool)ai.value;}
	operator bool() {return value;}
	bool operator = (bool b) {value = b; return b;}
	//int operator++(int) {return value++;}
	//int operator--(int) {return value--;}
};

struct AtomicInt {
	std::atomic<int> value;
	AtomicInt() {value = 0;}
	AtomicInt(int i) {value = i;}
	AtomicInt(const AtomicInt& ai) {value = (int)ai.value;}
	operator int() const {return value;}
	int operator = (int i) {value = i; return i;}
	int operator++() {return ++value;}
	int operator--() {return --value;}
	int operator++(int) {return value++;}
	int operator--(int) {return value--;}
};

struct FakeAtomicInt : Moveable<FakeAtomicInt> {
	SpinLock lock;
	int value;
	FakeAtomicInt() : value(0) {}
	FakeAtomicInt(int i) : value(i) {}
	FakeAtomicInt(const AtomicInt& ai) : value(ai.value) {}
	operator int() {return value;}
	int operator = (int i) {
		lock.Enter();
		value = i;
		lock.Leave();
		return i;
	}
	int operator++(int) {
		lock.Enter();
		int i = value++;
		lock.Leave();
		return i;
	}
	int operator--(int) {
		lock.Enter();
		int i = value--;
		lock.Leave();
		return i;
	}
	int Get() const {return value;}
};

typedef AtomicInt Atomic;

NAMESPACE_SDK_END

#endif
