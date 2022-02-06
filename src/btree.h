#ifndef __BTREE_H__
#define __BTREE_H__

struct btree_node {
	int type;
	// the size of children should be the size of data + 1
	struct tree_node **children;
	void **data;
};

struct btree_handle {
	struct btree_node *root;
	int order;
	cmpfn cmp;
};

struct btree_node *btree_node_new(void *data);

struct btree_handle *btree_new(int order, cmpfn cmp);

#endif
