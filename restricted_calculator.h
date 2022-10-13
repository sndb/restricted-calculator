#include <stdint.h>

struct solution {
	uint32_t x, y, n;
};

struct result {
	struct solution solution;
	struct result *next;
};

struct result *solve(uint32_t k);
void result_free(struct result *r);
