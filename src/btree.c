#include <stdio.h>
#include <stdlib.h>
#include "common_compare.h"
#include "btree.h"

struct btree_node *btree_node_new(void *data) {
	struct btree_node *new_node = malloc(sizeof(struct btree_node));
	if (new_node == NULL)
		return NULL;

	new_node->data = data;
	return new_node;
}

struct btree_handle *btree_new(int order, cmpfn cmp) {
	struct btree_handle *new_handle = malloc(sizeof(struct btree_handle));
	if (new_handle == NULL)
		return NULL;

	new_handle->order = order;
	new_handle->cmp = cmp;
	return new_handle;
}

