#include "btree.h"
#include "common_compare.h"
#include "rbuffer.h"
#include "sample_data.h"
#include "sysutil.h"
#include <stdio.h>

void btree_print(struct btree_handle *handle) {
	if (handle == NULL || handle->root == NULL)
		return;

	printf("\n");
}

void btree_test() {
}

int main(int argc, const char **argv) {
	btree_test();
	return 0;
}
