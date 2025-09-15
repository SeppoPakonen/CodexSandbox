#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Core.h"

NAMESPACE_SDK_BEGIN

uint64_t s_rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
}


uint64 sNext(uint64 *s)
{
	const uint64_t result_starstar = s_rotl(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = s_rotl(s[3], 45);

	return result_starstar;
}


void sSeed(uint64 *s)
{
	for(int pass = 0; pass < 4; pass++) {
		for(int i = 0; i < 4; i++) {
			CombineHash h;
			Time t = GetSysTime();
			h.Put64(t.Get());
			h.Put64(usecs());
			h.Put64(msecs());
			s[i] ^= h;
		}
	}
}

uint64 *sState()
{
	thread_local uint64 *s;
	if(!s) {
		thread_local uint64 state[4];
		s = state;
		sSeed(s);
	}
	return s;
}

dword Random()
{
	return (dword)sNext(sState());
}

uint64 Random64()
{
	return sNext(sState());
}

void Random64(uint64 *t, int n) {
	uint64 *s = sState();
	while(n-- > 0)
		*t++ = sNext(s);
}

dword Random(dword n) {
	ASSERT(n);
	uint64 *s = sState();
	dword mask = (1 << SignificantBits(n)) - 1;
	dword r;
	do
		r = (dword)sNext(s) & mask;
	while(r >= n);
	return r;
}

uint64 Random64(uint64 n) {
	ASSERT(n);
	uint64 *s = sState();
	uint64 mask = ((uint64)1 << SignificantBits64(n)) - 1;
	uint64 r;
	do
		r = sNext(s) & mask;
	while(r >= n);
	return r;
}

double Randomf()
{
	return (sNext(sState()) >> 11) * (1. / (UINT64_C(1) << 53));
}

void SeedRandom()
{
	sSeed(sState());
}

void SeedRandom(dword seed) {
	uint64 *s = sState();
	for(int i = 0; i < 4; i++)
		s[i] = 12345678 + seed + i; // xoshiro does not work well if all is zero
}

NAMESPACE_SDK_END
