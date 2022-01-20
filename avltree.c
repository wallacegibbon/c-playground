#include <stdio.h>
#include "common_compare.h"
#include "sysutil.h"

struct tree_node {
	struct tree_node *left, *right;
	void *data;
	int height;
};

struct tree_node *tree_node_new(void *data) {
	struct tree_node *new_node = malloc(sizeof struct tree_node);
	if (!new_node)
		exit_info("failed alloc memory for tree_node");

	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

static inline int tree_height(struct tree_node *root) {
	if (root == NULL)
		return 0;

	return max(tree_height(root->left), tree_height(root->right)) + 1;
}

static inline int balance_factor(struct tree_node *root) {
	if (root == NULL)
		return 0;

	return root->left->height - root->right->height;
}

static struct tree_node *rotate_right(struct tree_node *root) {
	struct tree_node *left = root->left;

	root->left = left->right;
	left->right = root;

	left->height = tree_height(left);
	root->height = tree_height(root);

	return left;
}

static struct tree_node *rotate_left(struct tree_node *root) {
	struct tree_node *right = root->right;

	root->right = right->left;
	right->left = root;

	right->height = tree_height(right);
	root->height = tree_height(root);

	return right;
}

struct tree_node *rebalance(struct tree_node *root) {
	int factor = balance_factor(root);
	if (factor > 1 && balance_factor(root->left) > 0) // LL
		return rotate_right(root);

	if (factor < -1 && balance_factor(root->right) <= 0) // RR
		return rotate_left(root);

	if (factor > 1 && balance_factor(root->left) <= 0) { // LR
		root->left = rotate_left(root->left);
		return rotate_right(root);
	}

	if (factor < -1 && balance_factor(root->right) > 0) { // RL
		root->right = rotate_right(root->right);
		return rotate_left(root);
	}

	return root;
}

void avltree_insert(struct tree_node **root, void *data, cmpfn cmp) {
	if (*root == NULL) {
		*root = tree_node_new(data);
		return;
	}

	int cmp_result = cmp((*root)->data, data);

	if (cmp_result == 0) {
		(*root)->data = data;
		return;
	}

	if (cmp_result < 0)
		avltree_insert(&(*root)->right, value);
	else
		avltree_insert(&(*root)->left, value);

	*root = rebalance(*root);
}

void avltree_remove(struct tree_node **root, void *data, cmpfn cmp) {
	if (*root == NULL)
		return;

	int cmp_result = cmp((*root)->data, data);

	if (cmp_result == 0) {
		if ((*root)->right == NULL) {
			*root = (*root)->left;
			free(*root);
		} else {
			//
		}
	}

	if (cmp_result < 0)
		avltree_remove(&(*root)->right, data);
	else
		avltree_remove(&(*root)->left, data);

	*root = rebalance(*root);
}

