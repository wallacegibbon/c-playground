#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point { double x, y; };

struct point random_point(double size) {
	// the maximum value of random() is (2 ^ 31) - 1;
	double x = size * random() / 0x7fffffff;
	double y = size * random() / 0x7fffffff;
	struct point r = {x, y};
	return r;
}

double calc_pi_once() {
	static const double radius = 100.0;

	// this 2 counters could overflow
	static unsigned long long match_cnt = 0, total_cnt = 0;

	struct point p = random_point(radius);

	if (sqrt(pow(p.x, 2) + pow(p.y, 2)) <= radius)
		match_cnt++;

	total_cnt++;

	return (double) match_cnt / total_cnt * 4;
}

double calc_pi() {
	double pi;
	for (int i = 0; i < 10000; i++)
		pi = calc_pi_once();
	return pi;
}

int main(int argc, const char **argv) {
	while (1) {
		double pi = calc_pi();
		printf("pi = %1.40lf\r", pi);
		//usleep(5000);
	}
	return 0;
}
