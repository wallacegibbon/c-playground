#include <stdio.h>
#include "common_compare.h"
#include "sysutil.h"
#include "rbuffer.h"
#include "avltree.h"
#include "sample_data.h"

void avltree_print(struct tree_handle *handle) {
	if (handle == NULL || handle->root == NULL)
		return;

	struct rbuffer *q = rbuffer_new(1 << (handle->root->height + 1));
	if (q == NULL)
		exit_info(1, "failed alloc memory for avltree_print");

	rbuffer_put(q, (void **) &handle->root, 1);

	struct tree_node *n = NULL;
	while (rbuffer_get(q, (void **) &n, 1)) {
		if (n != NULL) {
			rbuffer_put(q, (void **) &n->left, 1);
			rbuffer_put(q, (void **) &n->right, 1);
			person_print((struct person *) n->data, "", " ");
		} else {
			printf("# ");
		}
	}

	rbuffer_del(q);
	printf("\n");
}

static inline void person_tree_print(char *prefix, struct person *p) {
	printf("%s: ", prefix);
	person_print(p, "", "");
	printf("\n");
}

void avltree_test() {
	//struct tree_handle *handle = avltree_new((cmpfn) person_name_cmp);
	struct tree_handle *handle = avltree_new((cmpfn) person_id_cmp);

	printf("\n\n-- testing insert --\n\n");
	for (int i = 0; i < SAMPLE_DATA_SIZE; i++) {
		person_tree_print("-->> inserting to avltree", &person_db[i]);
		struct tree_node *r = avltree_insert(handle,
					(void *) &person_db[i]);
		//printf("alvtree insert result: %p\n", r);
		avltree_print(handle);
	}

	printf("\n\n-- testing search --\n\n");
	struct person p = {7, "Q"}, *pp = &p;
	int r_search = avltree_search(handle, (void **) &pp);
	if (r_search) {
		person_print(pp, "", "");
		printf("\n");
	} else {
		printf("search failed\n");
	}

	printf("\n\n-- testing remove --\n\n");
	for (int i = 0; i < SAMPLE_DATA_SIZE; i++) {
		person_tree_print("--<< removing from avltree", &person_db[i]);
		struct tree_node *r = avltree_remove(handle,
					(void *) &person_db[i]);
		//printf("alvtree remove result: %p\n", r);
		avltree_print(handle);
	}
}

int main(int argc, const char **argv) {
	avltree_test();
	return 0;
}

