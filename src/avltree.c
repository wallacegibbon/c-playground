#include "avltree.h"
#include "common_compare.h"
#include <stdio.h>
#include <stdlib.h>

static struct avltree_node *avltree_node_new(void *data) {
	struct avltree_node *new_node = malloc(sizeof(struct avltree_node));
	if (new_node == NULL)
		return NULL;

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 0;
	new_node->data = data;
	return new_node;
}

struct avltree_handle *avltree_new(cmpfn cmp) {
	struct avltree_handle *new_handle =
		malloc(sizeof(struct avltree_handle));
	if (new_handle == NULL)
		return NULL;

	new_handle->cmp = cmp;
	return new_handle;
}

static inline int calc_height(struct avltree_node *node) {
	if (node == NULL)
		return -1;

	return max_int(calc_height(node->left), calc_height(node->right)) + 1;
}

static inline int balance_factor(struct avltree_node *node) {
	if (node == NULL)
		return 0;

	int h1 = 0, h2 = 0;
	if (node->left)
		h1 = node->left->height;

	if (node->right)
		h2 = node->right->height;

	return h1 - h2;
}

static inline int is_balanced(struct avltree_node *node) {
	return abs_int(balance_factor(node)) <= 1;
}

static struct avltree_node *rotate_right(struct avltree_node *node) {
	struct avltree_node *left = node->left;

	node->left = left->right;
	left->right = node;

	left->height = calc_height(left);
	node->height = calc_height(node);

	return left;
}

static struct avltree_node *rotate_left(struct avltree_node *node) {
	struct avltree_node *right = node->right;

	node->right = right->left;
	right->left = node;

	right->height = calc_height(right);
	node->height = calc_height(node);

	return right;
}

static struct avltree_node *rebalance(struct avltree_node *node) {
	if (node == NULL)
		return NULL;

	int factor = balance_factor(node);
	if (factor > 1) {
		if (balance_factor(node->left) > 0) { // LL
			return rotate_right(node);
		} else { // LR
			node->left = rotate_left(node->left);
			return rotate_right(node);
		}
	} else if (factor < -1) {
		if (balance_factor(node->right) > 0) { // RL
			node->right = rotate_right(node->right);
			return rotate_left(node);
		} else { // RR
			return rotate_left(node);
		}
	} else {
		node->height = calc_height(node);
		return node;
	}
}

static struct avltree_node *__min(struct avltree_node *node) {
	if (node == NULL)
		return NULL;

	while (node->left != NULL)
		node = node->left;

	return node;
}

static struct avltree_node *__max(struct avltree_node *node) {
	if (node == NULL)
		return NULL;

	while (node->right != NULL)
		node = node->right;

	return node;
}

struct avltree_node *avltree_min(struct avltree_handle *handle) {
	return __min(handle->root);
}

struct avltree_node *avltree_max(struct avltree_handle *handle) {
	return __max(handle->root);
}

static struct avltree_node *__insert(struct avltree_node **node, void *data, cmpfn cmp) {
	struct avltree_node *ret = *node;
	if (ret == NULL) {
		*node = avltree_node_new(data);
		return *node;
	}

	int cmp_result = cmp(ret->data, data);
	if (cmp_result < 0) {
		ret = __insert(&ret->right, data, cmp);
	} else if (cmp_result > 0) {
		ret = __insert(&ret->left, data, cmp);
	} else {
		ret->data = data;
	}
	*node = rebalance(*node);
	return ret;
}

static struct avltree_node *__remove(struct avltree_node **node, void *data, cmpfn cmp) {
	struct avltree_node *ret = *node;
	if (ret == NULL)
		return NULL;

	int cmp_result = cmp(ret->data, data);
	if (cmp_result < 0) {
		ret = __remove(&ret->right, data, cmp);
	} else if (cmp_result > 0) {
		ret = __remove(&ret->left, data, cmp);
	} else {
		if (ret->left == NULL) {
			*node = ret->right;
		} else if (ret->right == NULL) {
			*node = ret->left;
		} else {
			struct avltree_node *r = ret->right;
			struct avltree_node *m = __min(r);
			__remove(&r, m->data, cmp);
			m->right = r;
			m->left = ret->left;
			*node = m;
		}
	}
	*node = rebalance(*node);
	return ret;
}

struct avltree_node *avltree_insert(struct avltree_handle *handle, void *data) {
	return __insert(&handle->root, data, handle->cmp);
}

/* memory for tree node is freed in this function */
struct avltree_node *avltree_remove(struct avltree_handle *handle, void *data) {
	struct avltree_node *r = __remove(&handle->root, data, handle->cmp);
	free(r);
	return r;
}

static struct avltree_node *__search(struct avltree_node *node, void *data, cmpfn cmp) {
	if (node == NULL)
		return NULL;

	int cmp_result = cmp(node->data, data);
	if (cmp_result < 0) {
		return __search(node->right, data, cmp);
	} else if (cmp_result > 0) {
		return __search(node->left, data, cmp);
	} else {
		return node;
	}
}

/* the result is stored in *data */
int avltree_search(struct avltree_handle *handle, void **data) {
	struct avltree_node *r = __search(handle->root, *data, handle->cmp);
	if (r == NULL)
		return 0;

	*data = r->data;
	return 1;
}
