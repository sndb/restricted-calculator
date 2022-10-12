#include "restricted_calculator.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t alphabet[] = {2, 5, 7};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static uint8_t
count_digits(uint32_t n) {
	uint8_t l = 0;
	for (; n > 0; n /= 10)
		l++;
	return l;
}

static uint32_t
power(uint32_t n, uint8_t p) {
	uint32_t r = 1;
	for (uint8_t i = 0; i < p; i++)
		r *= n;
	return r;
}

static uint32_t
sum_of_powers(uint32_t n, uint8_t p) {
	uint32_t r = 0;
	for (uint8_t i = 1; i <= p; i++)
		r += power(n, i);
	return r;
}

static bool
in_alphabet(uint8_t digit) {
	for (size_t i = 0; i < ARRAY_SIZE(alphabet); i++)
		if (digit == alphabet[i])
			return true;
	return false;
}

static bool
verify_number(uint32_t n) {
	while (n > 0) {
		if (!in_alphabet(n % 10))
			return false;
		n /= 10;
	}
	return true;
}

static bool
verify_solution(struct solution s, uint32_t answer) {
	return verify_number(s.x)
	    && verify_number(s.y)
	    && verify_number(s.n)
	    && s.x * s.y + s.n == answer;
}

static uint32_t *
permutations(uint8_t len) {
	uint32_t *r = malloc(sizeof(uint32_t[sum_of_powers(ARRAY_SIZE(alphabet), len)]));
	for (size_t i = 0; i < ARRAY_SIZE(alphabet); i++)
		r[i] = alphabet[i];
	uint32_t offset = ARRAY_SIZE(alphabet);
	uint32_t prev = power(ARRAY_SIZE(alphabet), 1);
	for (size_t i = 2; i <= len; i++) {
		for (size_t j = 0; j < prev; j++)
			for (size_t k = 0; k < ARRAY_SIZE(alphabet); k++)
				r[offset + j * ARRAY_SIZE(alphabet) + k]
					= r[offset - prev + j] * 10 + alphabet[k];
		prev = power(ARRAY_SIZE(alphabet), i);
		offset += prev;
	}
	return r;
}

static struct results *
results_new(struct solution s, struct results *next) {
	struct results *n = malloc(sizeof(struct results));
	n->solution = s;
	n->next = next;
	return n;
}

void
results_free(struct results *r) {
	struct results *p;
	while (r) {
		p = r;
		r = r->next;
		free(p);
	}
}

struct results *
solve(uint32_t k) {
	uint8_t d = count_digits(k);
	uint32_t *p = permutations(d);
	size_t p_len = sum_of_powers(ARRAY_SIZE(alphabet), d);

	struct results *r = NULL;
	for (size_t i = 0; i < p_len; i++) {
		for (size_t j = i; j < p_len; j++) {
			if (p[i] * p[j] > k)
				continue;
			struct solution s = {p[i], p[j], k - p[i] * p[j]};
			if (verify_solution(s, k))
				r = results_new(s, r);
		}
	}
	free(p);
	return r;
}
