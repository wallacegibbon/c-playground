#ifndef __AVLTREE_H__
#define __AVLTREE_H__

struct tree_node {
	struct tree_node *left, *right;
	int height;
	void *data;
};

struct tree_handle {
	struct tree_node *root;
	cmpfn cmp;
};

struct tree_handle *avltree_new(cmpfn cmp);

struct tree_node *avltree_min(struct tree_handle *handle);

struct tree_node *avltree_max(struct tree_handle *handle);

struct tree_node *avltree_insert(struct tree_handle *handle, void *data);

struct tree_node *avltree_remove(struct tree_handle *handle, void *data);

int avltree_search(struct tree_handle *handle, void **p_data);

#endif
