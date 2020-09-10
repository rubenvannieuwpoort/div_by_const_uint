#include "utils.h"


uint powers_of_two(uint x) {
	uint n = 0;
	while ((x & 1) == 0) {
		x >>= 1;
		n++;
	}
	return n;
}

bool is_power_of_two(uint x) {
	return x && !(x & (x - 1));
}

uint floor_log2(uint x) {
	assert(x > 0);
	uint count = 0;
	if (N >= 64 && x & 0xffffffff00000000) { x >>= 32; count += 32; }
	if (N >= 32 && x & 0x00000000ffff0000) { x >>= 16; count += 16; }
	if (N >= 16 && x & 0x000000000000ff00) { x >>= 8; count += 8; }
	if (N >= 8 && x & 0x00000000000000f0) { x >>= 4; count += 4; }
	if (x & 0x000000000000000c) { x >>= 2; count += 2; }
	if (x & 0x0000000000000002) { x >>= 1; count += 1; }
	return count;
}

uint ceil_log2(uint x) {
	uint floor = floor_log2(x);
	assert(x >= ((uint)1 << floor));
	if (x > ((uint)1 << floor)) floor++;
	return floor;
}