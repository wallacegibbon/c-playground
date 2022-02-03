#include <stdio.h>
#include "rbuffer.h"

// put one data into the buffer and then take one out.
void shift_data(struct rbuffer *b, void *data) {
	int r;
	r = rbuffer_put(b, &data, 1);
	//printf("r is %d\n", r);
	//rbuffer_inspect(b);
	rbuffer_print(b, rbuffer_fmt_raw);

	long t;
	rbuffer_get(b, (void **) &t, 1);
}

int main(int argc, const char **argv) {
	long a[] = {0x10, 0x20, 0x30, 0x40, 0x50};
	struct rbuffer *b = rbuffer_new(4);

	int r;
	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, rbuffer_fmt_raw);

	//r = rbuffer_put(b, (void **) a, 3);
	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, rbuffer_fmt_raw);

	long t;
	r = rbuffer_get(b, (void **) &t, 1);
	printf("get from buffer: 0x%lx\n", t);
	rbuffer_print(b, rbuffer_fmt_raw);

	for (long i = 0x111; i < 0x121; i++)
		shift_data(b, (void *) i);

	return 0;
}

