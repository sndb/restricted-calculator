#include <stdint.h>

struct solution {
	uint32_t x, y, n;
};

struct results {
	struct solution solution;
	struct results *next;
};

struct results *solve(uint32_t k);
void results_free(struct results *r);
