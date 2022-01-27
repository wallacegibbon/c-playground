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

static struct tree_node *__min(struct tree_node *node) {
	if (node == NULL)
		return NULL;

	while (node->left != NULL)
		node = node->left;

	return node;
}

static struct tree_node *__max(struct tree_node *node) {
	if (node == NULL)
		return NULL;

	while (node->right != NULL)
		node = node->right;

	return node;
}

struct tree_node *avltree_min(struct tree_handle *handle) {
	return __min(handle->root);
}

struct tree_node *avltree_max(struct tree_handle *handle) {
	return __max(handle->root);
}

static struct tree_node *__insert(struct tree_node **node, void *data,
					cmpfn cmp) {
	if (*node == NULL) {
		*node = tree_node_new(data);
		return *node;
	}

	int cmp_result = cmp((*node)->data, data);
	struct tree_node *ret = *node;

	if (cmp_result < 0) {
		ret = __insert(&(*node)->right, data, cmp);
	} else if (cmp_result > 0) {
		ret = __insert(&(*node)->left, data, cmp);
	} else {
		(*node)->data = data;
		ret = *node;
	}
	*node = rebalance(*node);
	return ret;
}

static struct tree_node *__remove(struct tree_node **node, void *data,
					cmpfn cmp) {
	if (*node == NULL)
		return NULL;

	int cmp_result = cmp((*node)->data, data);
	struct tree_node *ret = *node;

	if (cmp_result < 0) {
		ret = __remove(&(*node)->right, data, cmp);
	} else if (cmp_result > 0) {
		ret = __remove(&(*node)->left, data, cmp);
	} else {
		ret = *node;
		if (ret->left == NULL) {
			*node = ret->right;
		} else if (ret->right == NULL) {
			*node = ret->left;
		} else {
			struct tree_node *r = ret->right;
			struct tree_node *m = __min(r);
			__remove(&r, m->data, cmp);
			m->right = r;
			m->left = ret->left;
			*node = m;
		}
	}
	*node = rebalance(*node);
	return ret;
}

struct tree_node *avltree_insert(struct tree_handle *handle, void *data) {
	return __insert(&handle->root, data, handle->cmp);
}

struct tree_node *avltree_remove(struct tree_handle *handle, void *data) {
	return __remove(&handle->root, data, handle->cmp);
}

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

void avltree_test() {
	struct tree_handle *handle = avltree_new((cmpfn) person_name_cmp);

	for (int i = 0; i < SAMPLE_DATA_SIZE; i++) {
		printf("-->> inserting to avltree: ");
		person_print(&person_db[i], "", "");
		printf("\n");
		struct tree_node *r = avltree_insert(handle,
					(void *) &person_db[i]);
		//printf("alvtree insert result: %p\n", r);
		avltree_print(handle);
	}

	for (int i = 0; i < SAMPLE_DATA_SIZE; i++) {
		printf("--<< removing from avltree: ");
		person_print(&person_db[i], "", "");
		printf("\n");
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

