/*
 * Restricted Calculator
 *
 * Examples:
 *
 * > 346010
 * 555 * 577 + 25775 = 346010
 *
 * > 6432
 * 25 * 27 + 5757 = 6432
 * 22 * 55 + 5222 = 6432
 */

#include "restricted_calculator.h"

#include <inttypes.h>
#include <stdio.h>

int
main(void) {
	uint32_t k;
	scanf("%" SCNu32, &k);
	struct results *r = solve(k);
	struct results *c = r;
	while (c) {
		if (c->solution.n != 0)
			printf("%d * %d + %d = %d\n", c->solution.x, c->solution.y, c->solution.n, k);
		else
			printf("%d * %d = %d\n", c->solution.x, c->solution.y, k);
		c = c->next;
	}
	results_free(r);
	return 0;
}
