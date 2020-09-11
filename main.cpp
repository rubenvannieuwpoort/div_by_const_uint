#include "compiler.h"
#include "utils.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

// Double 'init' modulo d until it is less than 2^(k - M), return k
int loop_k(uint d, uint init, uint M) {
	int k = N;
	for (uint mod = init, pow = 1 << (k - M); mod > pow; pow <<= 1, k++) {
		// overflow-safe way of doubling 'mod' modulo d
		if (mod < d - mod) mod += mod;
		else mod -= d - mod;
	}
	return k;
}

uint calc_m(uint d, int k) {
	return (uint)(((uint64_t)1 << k) / d);
}

expression_t mul_based_div_by_const_uint(uint d, expression_t n) {
	uint mod = (uint)(~d + 1) % d; // 2^N mod d
	uint minus_mod = mod ? d - mod : 0;
	uint k = loop_k(d, minus_mod, N);
	uint bits = k + 1 - ceil_log2(d);
	if (bits <= N) {
		// Algorithm B: cooperative divisors
		uint m = calc_m(d, k) + 1;
		expression_t hiword = umulhi(n, constant(m));
		if (k == N) return hiword;
		else return shr(hiword, constant(k - N));
	}
	if (d % 2 == 0) {
		// Algorithm C: even uncooperative divisors
 		uint p = powers_of_two(d);
		uint q = d >> p;
		mod = (uint)(~q + 1) % q; // 2^N mod d
		uint minus_mod = mod ? q - mod : 0;
		k = loop_k(q, minus_mod, N - p);
		uint bits = k + 1 - ceil_log2(q);
		assert(bits <= N);
		uint m = calc_m(q, k) + 1;
		expression_t n_odd = shr(n, constant(p));
		expression_t hiword = umulhi(n_odd, constant(m));
		if (k == N) return hiword;
		else return shr(hiword, constant(k - N));
	}
	else {
		// Algorithm D: odd uncooperative divisors
		k = loop_k(d, mod, N);
		bits = k + 1 - ceil_log2(d);
		assert(bits <= N);
		uint m = calc_m(d, k);
		expression_t n_inc = sbb(add(n, constant(1)), constant(0));
		expression_t hiword = umulhi(n_inc, constant(m));
		if (k == N) return hiword;
		else return shr(hiword, constant(k - N));
	}
}

expression_t div_by_const_uint(const uint d, expression_t n) {
	if (is_power_of_two(d)) {
		if (d == 1) return n;
		else return shr(n, constant(floor_log2(d)));
	}
	else {
		if (d > MAX / 2) return gte(n, constant(d));
		else return mul_based_div_by_const_uint(d, n);
	}
}

void examples() {
	variable_t input = variable(0);
	uint inputs[] = { 1, 16, 2147483649, 3, 641, 14, 28, 7 };

	for (uint d : inputs) {
		expression_t e = div_by_const_uint(d, input);
		printf("d = %u\ndivide:\n%s\tret\n\n", d, e->code().c_str());
	}
}

void test_exhaustive() {
	variable_t input = variable(0);
	for (uint d = 1; true; d++) {
		expression_t e = div_by_const_uint(d, input);

		for (uint n = 0; true; n++) {
			input->assign(n);
			if (e->evaluate() != n / d) {
				printf("%u / %u = %u (got %u)\n", n, d, n / d, e->evaluate());
			}
			if (n == MAX) break;
		}

		if (d == MAX) break;
	}
}

int main(void) {
	examples();
 }




























