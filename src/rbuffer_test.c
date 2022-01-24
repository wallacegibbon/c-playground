#include <stdio.h>
#include "rbuffer.h"

int main(int argc, const char **argv) {
	struct rbuffer *b = rbuffer_new(5);
	int r, t;

	long a[] = {10, 20, 30, 40, 50};

	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, rbuffer_fmt_raw);

	//r = rbuffer_put(b, (void **) a, 3);
	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, rbuffer_fmt_raw);

	r = rbuffer_get(b, (void **) &t, 1);
	printf("get from buffer: %d\n", t);

	for (int i = 111; i < 141; i++) {
		r = rbuffer_put(b, (void **) &i, 1);
		//rbuffer_inspect(b);
		rbuffer_print(b, rbuffer_fmt_raw);
		rbuffer_get(b, (void **) &t, 1);
	}

	return 0;
}
