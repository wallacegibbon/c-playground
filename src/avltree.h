#ifndef __AVLTREE_H__
#define __AVLTREE_H__

struct avltree_node {
	struct avltree_node *left, *right;
	int height;
	void *data;
};

struct avltree_handle {
	struct avltree_node *root;
	cmpfn cmp;
};

struct avltree_handle *avltree_new(cmpfn cmp);

struct avltree_node *avltree_min(struct avltree_handle *handle);

struct avltree_node *avltree_max(struct avltree_handle *handle);

struct avltree_node *avltree_insert(struct avltree_handle *handle,
		void *data);

struct avltree_node *avltree_remove(struct avltree_handle *handle,
		void *data);

int avltree_search(struct avltree_handle *handle, void **p_data);

#endif
