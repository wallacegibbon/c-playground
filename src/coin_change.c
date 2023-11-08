#include "common_compare.h"
#include "sysutil.h"
#include <stdio.h>
#include <stdlib.h>

#define DENOMINATION_NUM 7

#define MAX_NUM 100000

int denominations[] = {1, 2, 5, 10, 20, 50, 100};

int coin_change_1(int num, int *denos, int deno_n) {
	if (num == 0)
		return 0;
	if (num < 0)
		return -1;

	int res = MAX_NUM;
	for (int i = 0; i < deno_n; i++) {
		int sub = coin_change_1(num - denos[i], denos, deno_n);
		if (sub == -1)
			continue;
		res = min_int(res, 1 + sub);
	}

	if (res != MAX_NUM)
		return res;
	else
		return -1;
}

int coin_change_2(int num, int *denos, int deno_n, int *memorized) {
	if (num == 0)
		return 0;
	if (num < 0)
		return -1;

	int res = MAX_NUM;
	for (int i = 0; i < deno_n; i++) {
		int next_num = num - denos[i];
		// memorized is accessed as array, index should be checked.
		if (next_num < 0)
			continue;
		int sub = memorized[next_num];
		if (sub == 0) {
			sub = coin_change_2(next_num, denos, deno_n, memorized);
			memorized[next_num] = sub;
		}
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
	// int num = 29;
	int num = 259;

	int *memorized = calloc(sizeof(int), num);
	if (memorized == NULL)
		exit_info(1, "failed alloc memory for memorized data");

	// int r = coin_change_1(num, denominations, DENOMINATION_NUM);
	int r = coin_change_2(num, denominations, DENOMINATION_NUM, memorized);
	printf("r: %d\n", r);

	free(memorized);

	return 0;
}
