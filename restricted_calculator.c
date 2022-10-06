// Examples
// > 346010
// 555 * 577 + 25775 = 346010
// > 6432
// 25 * 27 + 5757 = 6432
// 22 * 55 + 5222 = 6432

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t alphabet[]    = {2, 5, 7};
size_t  alphabet_size = sizeof(alphabet) / sizeof(*alphabet);

struct solution {
	uint32_t x, y;
	uint32_t a;
};

bool
in_alphabet(uint8_t k) {
	for (size_t i = 0; i < alphabet_size; i++)
		if (k == alphabet[i])
			return true;
	return false;
}

bool
verify_number(uint32_t n) {
	while (n > 0) {
		if (!in_alphabet(n % 10))
			return false;
		n /= 10;
	}
	return true;
}

bool
verify_solution(struct solution s, uint32_t k) {
	return verify_number(s.x) && verify_number(s.y) && verify_number(s.a)
	    && s.x * s.y + s.a == k;
}

struct node {
	struct solution s;
	struct node    *next;
};

struct node *
new_node(struct solution s, struct node *next) {
	struct node *n = malloc(sizeof(struct node));
	n->s           = s;
	n->next        = next;
	return n;
}

void
free_node(struct node *n) {
	struct node *p;
	while (n) {
		p = n;
		n = n->next;
		free(p);
	}
}

uint32_t *
cartesian_product(uint8_t n, size_t *r_size) {
	assert(n != 0);
	if (n == 1) {
		uint32_t *r = malloc(sizeof(uint32_t[alphabet_size]));
		for (size_t i = 0; i < alphabet_size; i++)
			r[i] = alphabet[i];
		*r_size = alphabet_size;
		return r;
	}
	size_t    p_size;
	uint32_t *p = cartesian_product(n - 1, &p_size);
	size_t    s = alphabet_size * p_size;
	uint32_t *r = malloc(sizeof(uint32_t[s]));
	for (size_t i = 0; i < p_size; i++)
		for (size_t j = 0; j < alphabet_size; j++)
			r[i * alphabet_size + j] = p[i] * 10 + alphabet[j];
	free(p);
	*r_size = s;
	return r;
}

uint32_t *
cartesian_product_sum(uint8_t n, size_t *r_size) {
	size_t s = 0, a = 1;
	for (uint8_t i = 0; i < n; i++) {
		a *= alphabet_size;
		s += a;
	}
	uint32_t *r      = malloc(sizeof(uint32_t[s]));
	size_t    offset = 0;
	for (uint8_t i = 1; i <= n; i++) {
		size_t    p_size;
		uint32_t *p = cartesian_product(i, &p_size);
		memcpy(r + offset, p, sizeof(uint32_t[p_size]));
		free(p);
		offset += p_size;
	}
	*r_size = s;
	return r;
}

struct node *
solve(uint32_t k) {
	uint8_t n = 0;
	for (uint8_t j = k; j > 0; j /= 10)
		n++;
	size_t    p_size;
	uint32_t *p = cartesian_product_sum(n, &p_size);

	struct node *r = NULL;
	for (size_t i = 0; i < p_size; i++) {
		for (size_t j = i; j < p_size; j++) {
			struct solution s = {p[i], p[j], k - p[i] * p[j]};
			if (verify_solution(s, k))
				r = new_node(s, r);
		}
	}
	free(p);
	return r;
}

int
main(void) {
	uint32_t k;
	scanf("%" SCNu32, &k);
	struct node *s = solve(k);
	struct node *c = s;
	while (c) {
		printf("%d * %d + %d = %d\n", c->s.x, c->s.y, c->s.a, k);
		c = c->next;
	}
	free_node(s);
	return 0;
}
