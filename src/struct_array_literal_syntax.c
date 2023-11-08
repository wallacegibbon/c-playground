#include <stdio.h>

/// you can always use "{}" to create array or struct literals on stack if
/// you give the right type information.

struct blah {
	int a, b, c;
};

void print_blah(struct blah *v) {
	printf("struct blah { %d, %d, %d }\n", v->a, v->b, v->c);
}

int main() {
	printf("%s\n", (char[]){97, 98, 99, 0});
	print_blah(&(struct blah){1, 2, 3});
	return 0;
}
