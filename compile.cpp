#define N 32
#include "compiler.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

expression_t mul_based_div_by_const_uint(uint d, expression_t n);

// Returns an expression that efficiently computes
// a division by the constant value d.
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

// Returns an expression that efficiently computes a division by the
// constant value d, where d is not a power of two (this is important
// because in this case we can just add one to m to round up).
expression_t mul_based_div_by_const_uint(uint d, expression_t n) {
	// compute minus_mod = -2^N mod d
	uint mod = (uint)(~d + 1) % d;
	uint minus_mod = mod ? d - mod : 0;
	uint k = loop_k(d, minus_mod, N);
	uint bits = k + 1 - ceil_log2(d);
	if (bits <= N) {
		// Algorithm B: cooperative divisors
		
		// we can round up by adding one, since we assume that
		// d is not a power of two (otherwise we could add (mod > 0))
		uint m = calc_m(d, k) + 1;
		expression_t hiword = umulhi(n, constant(m));
		if (k == N) return hiword;
		else return shr(hiword, constant(k - N));
	}
	if (d % 2 == 0) {
		// Algorithm C: even uncooperative divisors
		
		uint p = powers_of_two(d);
		uint q = d >> p;
		// compute minus_mod = -2^N mod q
		mod = (uint)(~q + 1) % q;
		uint minus_mod = mod ? q - mod : 0;
		k = loop_k(q, minus_mod, N - p);
		// we can round up by adding one, since we assume that
		// d is not a power of two (otherwise we could add (mod > 0))
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
	return 0;
}
