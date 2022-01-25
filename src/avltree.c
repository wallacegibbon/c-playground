#include <stdio.h>
#include <stdlib.h>
#include "common_compare.h"
#include "sample_data.h"
#include "sysutil.h"
#include "rbuffer.h"

struct tree_node {
	struct tree_node *left, *right;
	int height;
	void *data;
};

struct tree_handle {
	struct tree_node *root;
	cmpfn cmp;
};

struct tree_handle *avltree_new(cmpfn cmp) {
	struct tree_handle *new_handle = malloc(sizeof(struct tree_handle));
	if (!new_handle)
		return NULL;

	new_handle->cmp = cmp;

	return new_handle;
}

static struct tree_node *tree_node_new(void *data) {
	struct tree_node *new_node = malloc(sizeof(struct tree_node));
	if (!new_node)
		return NULL;

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 0;
	new_node->data = data;

	return new_node;
}

static inline int calc_height(struct tree_node *node) {
	if (node == NULL)
		return -1;

	return max(calc_height(node->left), calc_height(node->right)) + 1;
}

static inline int balance_factor(struct tree_node *node) {
	if (node == NULL)
		return 0;

	int h1 = 0, h2 = 0;
	if (node->left)
		h1 = node->left->height;

	if (node->right)
		h2 = node->right->height;

	return h1 - h2;
}

static inline int is_balanced(struct tree_node *node) {
	return abs(balance_factor(node)) <= 1;
}

static struct tree_node *rotate_right(struct tree_node *node) {
	struct tree_node *left = node->left;

	node->left = left->right;
	left->right = node;

	left->height = calc_height(left);
	node->height = calc_height(node);

	return left;
}

static struct tree_node *rotate_left(struct tree_node *node) {
	struct tree_node *right = node->right;

	node->right = right->left;
	right->left = node;

	right->height = calc_height(right);
	node->height = calc_height(node);

	return right;
}

static struct tree_node *rebalance(struct tree_node *node) {
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

static int __insert(struct tree_node **node, void *data, cmpfn cmp) {
	if (*node == NULL) {
		*node = tree_node_new(data);
		return *node != NULL;
	}

	int cmp_result = cmp((*node)->data, data);
	int r = 1;

	if (cmp_result < 0) {
		r = __insert(&(*node)->right, data, cmp);
	} else if (cmp_result > 0) {
		r = __insert(&(*node)->left, data, cmp);
	} else {
		(*node)->data = data;
	}

	*node = rebalance(*node);
	return r;
}

static int __remove(struct tree_node **node, void *data, cmpfn cmp) {
	if (*node == NULL)
		return 0;

	int cmp_result = cmp((*node)->data, data);
	int r = 1;

	if (cmp_result < 0) {
		r = __remove(&(*node)->right, data, cmp);
	} else if (cmp_result > 0) {
		r = __remove(&(*node)->left, data, cmp);
	} else {
		if ((*node)->right == NULL) {
			*node = (*node)->left;
			free(*node);
		} else {
			//
		}
	}

	*node = rebalance(*node);
	return r;
}

int avltree_insert(struct tree_handle *handle, void *data) {
	return __insert(&handle->root, data, handle->cmp);
}

int avltree_remove(struct tree_handle *handle, void *data) {
	return __remove(&handle->root, data, handle->cmp);
}

void avltree_print(struct tree_handle *handle) {
	if (handle == NULL)
		return;

	struct rbuffer *q = rbuffer_new(1 << handle->root->height);
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
			printf("* ");
		}
	}

	rbuffer_del(q);

	printf("\n");
}

void avltree_test() {
	struct tree_handle *handle = avltree_new((cmpfn) person_name_cmp);

	for (int i = 0; i < SAMPLE_DATA_SIZE; i++) {
		int r = avltree_insert(handle, (void *) &person_db[i]);

		//printf("alvtree insert result: %d\n", r);
		avltree_print(handle);
	}
}

int main(int argc, const char **argv) {
	avltree_test();
	return 0;
}

