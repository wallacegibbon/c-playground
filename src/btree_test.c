#include <stdio.h>
#include "common_compare.h"
#include "sysutil.h"
#include "rbuffer.h"
#include "btree.h"
#include "sample_data.h"

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

