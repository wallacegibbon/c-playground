#include <stdio.h>

struct blah {
	struct {
		char *name;
		int age;
	};
	union {
		int degree_i;
		float degree_f;
	};
};

int main(int argc, const char **argv) {
	struct blah s = {.name = "wallace", .age = 30, .degree_f = 3.14};
	printf("%s:%d (%f, %d)\n", s.name, s.age, s.degree_f, s.degree_i);

	return 0;
}
