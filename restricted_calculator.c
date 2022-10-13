#include "restricted_calculator.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static uint8_t alphabet[] = {2, 5, 7};

static uint8_t
count_digits(uint32_t n) {
	uint8_t c = 0;
	for (; n > 0; n /= 10)
		c++;
	return c;
}

static uint32_t
power(uint32_t b, uint8_t n) {
	uint32_t p = 1;
	for (uint8_t i = 0; i < n; i++)
		p *= b;
	return p;
}

static uint32_t
sum_of_powers(uint32_t b, uint8_t n) {
	uint32_t s = 0;
	for (uint8_t i = 1; i <= n; i++)
		s += power(b, i);
	return s;
}

static bool
in_alphabet(uint8_t digit) {
	for (size_t i = 0; i < ARRAY_SIZE(alphabet); i++)
		if (digit == alphabet[i])
			return true;
	return false;
}

static bool
valid_number(uint32_t n) {
	while (n > 0) {
		if (!in_alphabet(n % 10))
			return false;
		n /= 10;
	}
	return true;
}

static bool
valid_solution(struct solution s, uint32_t answer) {
	return valid_number(s.x)
	    && valid_number(s.y)
	    && valid_number(s.n)
	    && s.x * s.y + s.n == answer;
}

static uint32_t *
permutations(uint8_t n) {
	uint8_t base = ARRAY_SIZE(alphabet);
	uint32_t *p = malloc(sizeof(uint32_t[sum_of_powers(base, n)]));
	for (size_t i = 0; i < base; i++)
		p[i] = alphabet[i];
	for (size_t i = 1, r = 0, w = base; i < n; i++)
		for (size_t j = 0; j < power(base, i); j++, r++)
			for (size_t k = 0; k < base; k++, w++)
				p[w] = p[r] * 10 + alphabet[k];
	return p;
}

static struct result *
result_new(struct solution s, struct result *next) {
	struct result *r = malloc(sizeof(struct result));
	r->solution = s;
	r->next = next;
	return r;
}

static struct result *
result_reverse(struct result *r) {
	struct result *prev = NULL, *next;
	while (r) {
		next = r->next;
		r->next = prev;
		prev = r;
		r = next;
	}
	return prev;
}

void
result_free(struct result *r) {
	struct result *prev;
	while (r) {
		prev = r;
		r = r->next;
		free(prev);
	}
}

struct result *
solve(uint32_t k) {
	uint8_t c = count_digits(k);
	uint32_t *p = permutations(c);
	size_t pn = sum_of_powers(ARRAY_SIZE(alphabet), c);

	struct result *r = NULL;
	for (size_t i = 0; i < pn; i++) {
		for (size_t j = i; j < pn; j++) {
			if (p[i] * p[j] > k)
				continue;
			struct solution s = {p[i], p[j], k - p[i] * p[j]};
			if (valid_solution(s, k))
				r = result_new(s, r);
		}
	}
	free(p);
	return result_reverse(r);
}
