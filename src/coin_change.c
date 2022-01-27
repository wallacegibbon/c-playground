#include <stdio.h>
#include "common_compare.h"

#define DENOMINATION_NUM 7

#define MAX_NUM 100000

int denominations[] = {1, 2, 5, 10, 20, 50, 100};

int coin_change_1(int num, int *denos, int size) {
	if (num == 0)
		return 0;

	if (num < 0)
		return -1;

	int res = MAX_NUM;
	for (int i = 0; i < size; i++) {
		int sub =  coin_change_1(num - denos[i], denos, size);
		if (sub == -1)
			continue;
		res = min_int(res, 1 + sub);
	}

	if (res != MAX_NUM)
		return res;
	else
		return -1;
}

int main(int argc, const char **argv) {
	int r = coin_change_1(12, denominations, DENOMINATION_NUM);
	printf("r: %d\n", r);
	return 0;
}
