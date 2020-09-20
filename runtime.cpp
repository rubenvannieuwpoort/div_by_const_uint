#define N 16
#include "bits.h"
#include <assert.h>
#include <stdio.h>

struct fast_div_t {
	uint mul, add, shift;
};

fast_div_t precalc_div(uint d) {
	fast_div_t div;
	assert(d > 0);
	if (d == 1) {
		div.mul = MAX;
		div.add = MAX;
		div.shift = 0;
	}
	else {
		uint mod = (uint)(~d + 1) % d;
		uint minus_mod = mod ? d - mod : 0;
		uint k = loop_k(d, minus_mod, N);
		uint bits = k + 1 - ceil_log2(d);
		
		if (bits <= N) {
			div.mul = calc_m(d, k) + (mod > 0);
			div.add = 0;
			div.shift = k - N;
		}
		else {
			k = loop_k(d, mod, N);
			bits = k + 1 - ceil_log2(d);
			assert(bits <= N);
			div.mul = div.add = calc_m(d, k);
			div.shift = k - N;
		}
	}
	return div;
}

inline uint fast_divide(uint n, fast_div_t d) {
	uint hiword = ((big_uint)n * d.mul + d.add) >> N;
	return (uint)(hiword >> d.shift);
}

int main() {
	for (uint d = 1; true; d++) {
		fast_div_t divdata = precalc_div(d);
		for (uint n = 0; true; n++) {
			if (fast_divide(n, divdata) != n / d) {
				printf("mul: %u\nadd: %u\nk:   %u\n", divdata.mul, divdata.add, divdata.shift);
				printf("Fail: %u / %u = %u (got %u)\n", n, d, n / d, fast_divide(n, divdata));
				return 1;
			}
			if (n == MAX) break;
		}
		if (d == MAX) break;
	}
	printf("Exhaustively tested all %u-bit integers\n", N);
	return 0;
}
