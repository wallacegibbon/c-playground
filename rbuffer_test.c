#include <stdio.h>
#include "rbuffer.h"

void print_num(int num) {
	printf("%d ", num);
}

int main(int argc, const char **argv) {
	struct rbuffer *b = rbuffer_new(5);
	int r, t;

	long a[] = {10, 20, 30, 40, 50};

	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, (rbuffer_fmt) print_num);

	r = rbuffer_put(b, (void **) a, 2);
	printf("r is %d\n", r);

	rbuffer_inspect(b);
	rbuffer_print(b, (rbuffer_fmt) print_num);

	r = rbuffer_get(b, (void **) &t, 1);
	printf("get from buffer: %d\n", t);

	for (int i = 111; i < 141; i++) {
		r = rbuffer_put(b, (void **) &i, 1);
		//rbuffer_inspect(b);
		rbuffer_print(b, (rbuffer_fmt) print_num);
		rbuffer_get(b, (void **) &t, 1);
	}

	return 0;
}
