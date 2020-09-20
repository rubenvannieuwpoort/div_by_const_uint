#ifndef BITS_H
#define BITS_H

#include <stdint.h>
#include <assert.h>

#if N == 8
#define UINT uint8_t
#define BIG_UINT uint16_t
#elif N == 16
#define UINT uint16_t
#define BIG_UINT uint32_t
#elif N == 32
#define UINT uint32_t
#define BIG_UINT uint64_t
#else
#error This bit size is not supported.
#endif

typedef UINT uint;
typedef BIG_UINT big_uint;

#define MAX ((uint)-1)

// Double 'init' modulo d until it is less than 2^(k - M), return k
int loop_k(uint d, uint init, uint M) {
	int k = N;
	for (uint mod = init, pow = 1 << (k - M); mod > pow && pow; pow <<= 1, k++) {
		// overflow-safe way of doubling 'mod' modulo d
		if (mod < d - mod) mod += mod;
		else mod -= d - mod;
	}
	return k;
}

// Returns 2^k / d
uint calc_m(uint d, int k) {
	return (uint)(((uint64_t)1 << k) / d);
}

// Returns the number of factors of two. In other
// words, returns p when x = q * (1 << p) with q odd
uint powers_of_two(uint x) {
	uint n = 0;
	while ((x & 1) == 0) {
		x >>= 1;
		n++;
	}
	return n;
}

// Returns true if x is a power of two
bool is_power_of_two(uint x) {
	return x && !(x & (x - 1));
}

// Compute the log2 of x, rounded down to the next integer
uint floor_log2(uint x) {
	assert(x > 0);
	uint count = 0;
#if N >= 64
	if (x & 0xffffffff00000000) { x >>= 32; count += 32; }
#endif
#if N >= 32
	if (N >= 32 && x & 0x00000000ffff0000) { x >>= 16; count += 16; }
#endif
#if N >= 16
	if (N >= 16 && x & 0x000000000000ff00) { x >>= 8; count += 8; }
#endif
	if (N >= 8 && x & 0x00000000000000f0) { x >>= 4; count += 4; }
	if (x & 0x000000000000000c) { x >>= 2; count += 2; }
	if (x & 0x0000000000000002) { x >>= 1; count += 1; }
	return count;
}

// Compute the log2 of x, rounded up to the next integer
uint ceil_log2(uint x) {
	uint floor = floor_log2(x);
	assert(x >= ((uint)1 << floor));
	if (x > ((uint)1 << floor)) floor++;
	return floor;
}

#endif
