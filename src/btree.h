#ifndef __BTREE_H__
#define __BTREE_H__

struct btree_node {
	int type;
	union {
		struct tree_node **children;
		void *data;
	} d;
};

struct btree_handle {
	struct btree_node *root;
	int order;
	cmpfn cmp;
};

struct btree_node *btree_node_new(void *data);

struct btree_handle *btree_new(int order, cmpfn cmp);

#endif
