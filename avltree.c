#include <stdio.h>
#include <stdlib.h>
#include "common_compare.h"
#include "sample_data.h"
#include "sysutil.h"

struct tree_node {
	struct tree_node *left, *right;
	int height;
	void *data;
};

static struct tree_node *tree_node_new(void *data) {
	struct tree_node *new_node = malloc(sizeof(struct tree_node));
	if (!new_node)
		exit_info(1, "failed alloc memory for tree_node");

	new_node->left = NULL;
	new_node->right = NULL;
	new_node->height = 0;
	new_node->data = data;

	return new_node;
}

static inline int calc_height(struct tree_node *node) {
	if (node == NULL)
		return 0;

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

struct tree_node *rebalance(struct tree_node *node) {
	int factor = balance_factor(node);
	if (factor > 1 && balance_factor(node->left) > 0) // LL
		return rotate_right(node);

	if (factor < -1 && balance_factor(node->right) <= 0) // RR
		return rotate_left(node);

	if (factor > 1 && balance_factor(node->left) <= 0) { // LR
		node->left = rotate_left(node->left);
		return rotate_right(node);
	}

	if (factor < -1 && balance_factor(node->right) > 0) { // RL
		node->right = rotate_right(node->right);
		return rotate_left(node);
	}

	node->height = calc_height(node);
	return node;
}

void avltree_insert(struct tree_node **node, void *data, cmpfn cmp) {
	if (*node == NULL) {
		*node = tree_node_new(data);
		return;
	}

	int cmp_result = cmp((*node)->data, data);

	if (cmp_result == 0) {
		(*node)->data = data;
		return;
	}

	if (cmp_result < 0)
		avltree_insert(&(*node)->right, data, cmp);
	else
		avltree_insert(&(*node)->left, data, cmp);

	*node = rebalance(*node);
}

void avltree_remove(struct tree_node **node, void *data, cmpfn cmp) {
	if (*node == NULL)
		return;

	int cmp_result = cmp((*node)->data, data);

	if (cmp_result == 0) {
		if ((*node)->right == NULL) {
			*node = (*node)->left;
			free(*node);
		} else {
			//
		}
	}

	if (cmp_result < 0)
		avltree_remove(&(*node)->right, data, cmp);
	else
		avltree_remove(&(*node)->left, data, cmp);

	*node = rebalance(*node);
}

void avltree_print(struct tree_node *node) {
	int depth = 1 << node->height;
	struct tree_node **queue = alloca(sizeof(struct tree_node *) * depth);
	int i = 0;

	queue[i++] = node;
	while (i) {
		struct tree_node *n = queue[--i];
		queue[i++] = n->left;
		queue[i++] = n->right;
		if (n != NULL)
			person_print((struct person *) n->data, " ", " ");
		else
			printf(" _ ");
	}
}

void avltree_test() {
	struct tree_node *root;

	for (int i = 0; i < SAMPLE_DATA_SIZE; i++)
		avltree_insert(&root, (void **) &person_db[i],
				(cmpfn) person_id_cmp);

	avltree_print(root);
}

int main(int argc, const char **argv) {
	avltree_test();
	return 0;
}
