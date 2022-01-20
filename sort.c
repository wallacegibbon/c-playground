#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_compare.h"

typedef void (*sortfn)(void **arr, int size, cmpfn cmp);

static inline void swap_e(void **e1, void **e2) {
	void *tmp = *e1;
	*e1 = *e2;
	*e2 = tmp;
}

void bubble_sort(void **arr, int size, cmpfn cmp) {
	for (int i = 0; i < size - 1; i++)
		for (int j = 0, k = 1; j < size - i - 1; j++, k++)
			if (cmp(arr[j], arr[k]) > 0)
				swap_e(&arr[j], &arr[k]);
}

void selection_sort(void **arr, int size, cmpfn cmp) {
	for (int i = 0; i < size - 1; i++) {
		int k = i;
		for (int j = i + 1; j < size; j++)
			if (cmp(arr[k], arr[j]) > 0)
				k = j;
		if (k != i)
			swap_e(&arr[i], &arr[k]);
	}
}

static void __insertion_sort(void **arr, int size, cmpfn cmp, int delta) {
	for (int i = delta; i < size; i++) {
		void *tmp = arr[i];
		int j = i;
		while (j >= delta && cmp(arr[j - delta], tmp) > 0) {
			arr[j] = arr[j - delta];
			j -= delta;
		}
		if (j != i)
			arr[j] = tmp;
	}
}

void insertion_sort(void **arr, int size, cmpfn cmp) {
	__insertion_sort(arr, size, cmp, 1);
}

void shell_sort(void **arr, int size, cmpfn cmp) {
	int delta = 1;
	while (delta < size / 3)
		delta = delta * 3 + 1; // Knuth, 1973

	for (; delta >= 1; delta /= 3)
		__insertion_sort(arr, size, cmp, delta);
}

/* merge 2 sorted array */
static void __merge(void **arr, int start, int mid, int end, cmpfn cmp) {
	void **buf = alloca(sizeof(void *) * (end - start));
	int s1 = start, s2 = mid, i = 0;

	while (s1 < mid && s2 < end)
		if (cmp(arr[s1], arr[s2]) < 0)
			buf[i++] = arr[s1++];
		else
			buf[i++] = arr[s2++];

	while (s1 < mid)
		buf[i++] = arr[s1++];

	while (s2 < end)
		buf[i++] = arr[s2++];

	for (int j = 0; j < i; j++)
		arr[start + j] = buf[j];
}

static void __merge_sort_recur(void **arr, int start, int end, cmpfn cmp) {
	if (start >= end - 1)
		return;

	int mid = (start + end) / 2;

	__merge_sort_recur(arr, start, mid, cmp);
	__merge_sort_recur(arr, mid, end, cmp);

	__merge(arr, start, mid, end, cmp);
}

void merge_sort_recur(void **arr, int size, cmpfn cmp) {
	__merge_sort_recur(arr, 0, size, cmp);
}

static void __merge_sort_step(void *arr, int size, int step, cmpfn cmp) {
	int unit = step * 2;
	for (int i = 0; i < size; i += unit)
		__merge(arr, i, min(i + step, size), min(i + unit, size), cmp);
}

void merge_sort(void **arr, int size, cmpfn cmp) {
	for (int step = 1; step < size; step *= 2)
		__merge_sort_step(arr, size, step, cmp);
}

static int __divide(void **arr, int start, int end, cmpfn cmp) {
	void *mid = arr[end - 1];
	int left = start, right = end - 2;

	while (left < right) {
		while (cmp(arr[left], mid) <= 0 && left < right)
			left++;

		while (cmp(arr[right], mid) > 0 && left < right)
			right--;

		if (left != right)
			swap_e(&arr[left], &arr[right]);
	}

	if (cmp(arr[left], mid) > 0)
		swap_e(&arr[left], &arr[end - 1]);
	else
		left++;

	return left;
}

static void __quick_sort_recur(void **arr, int start, int end, cmpfn cmp) {
	if (start >= end - 1)
		return;

	int mid = __divide(arr, start, end, cmp);

	__quick_sort_recur(arr, start, mid, cmp);
	__quick_sort_recur(arr, mid, end, cmp);
}

void quick_sort_recur(void **arr, int size, cmpfn cmp) {
	__quick_sort_recur(arr, 0, size, cmp);
}

int log2_int(int num) {
	int i = 0;
	for (; num > 1; num /= 2)
		i++;

	if (num % 2 == 0)
		return i;
	else
		return i + 1;
}

struct range {
	int s, e;
};

void init_range(struct range *rng, int s, int e) {
	rng->s = s;
	rng->e = e;
}

void quick_sort(void **arr, int size, cmpfn cmp) {
	struct range *r_stack = alloca(sizeof(struct range) * log2_int(size));
	// i always points to the next empty stack slot
	int i = 0;

	init_range(&r_stack[i++], 0, size);
	while (i) {
		struct range r = r_stack[--i];
		if (r.s >= r.e - 1)
			continue;

		int mid = __divide(arr, r.s, r.e, cmp);

		init_range(&r_stack[i++], r.s, mid);
		init_range(&r_stack[i++], mid, r.e);
	}
}

static void adjust_heap(void **arr, int size, int parent, cmpfn cmp) {
	void *top = arr[parent];
	int p = parent;

	for (int i = p * 2 + 1; i < size; i = i * 2 + 1) {
		// i always represents the child of p
		if (cmp(arr[i], arr[i + 1]) < 0 && i + 1 < size)
			i++;

		if (cmp(top, arr[i]) > 0)
			break;

		arr[p] = arr[i];
		p = i;
	}

	if (p != parent)
		arr[p] = top;
}

void heap_sort(void **arr, int size, cmpfn cmp) {
	// create the heap from bottom. (start from the first non-leaf node)
	for (int i = size / 2 - 1; i >= 0; i--)
		adjust_heap(arr, size, i, cmp);

	for (int i = size - 1; i > 0; i--) {
		swap_e(&arr[0], &arr[i]);
		adjust_heap(arr, i, 0, cmp);
	}
}

struct person {
	int id;
	char name[20];
};

#define SAMPLE_SIZE 15

struct person person_db[SAMPLE_SIZE] =
{
{7, "Wally"}, {2, "Harry"}, {10, "Bruce"}, {4, "Ada"}, {1, "Jerry"}, {3, "Q"},
{25, "Ron"}, {18, "Judy"}, {5, "Hans"}, {20, "Anna"}, {4, "Elsa"}, {9, "Sven"},
{32, "Nick"}, {30, "Amy"}, {34, "Flash"}
};

static void init_test_data(struct person **buf) {
	for (int i = 0; i < SAMPLE_SIZE; i++)
		buf[i] = &person_db[i];
}

static int person_id_cmp(struct person *p1, struct person *p2) {
	return p1->id - p2->id;
}

static int person_name_cmp(struct person *p1, struct person *p2) {
	return strcmp(p1->name, p2->name);
}

static inline void print_one_person(struct person *p) {
	printf("{%d, %s} ", p->id, p->name);
}

static int person_arr_print(struct person **p, int size) {
	for (int i = 0; i < size; i++)
		print_one_person(p[i]);
}

static void test_sort(sortfn fn, char *prefix) {
	printf("%s:\n", prefix);

	struct person *t[SAMPLE_SIZE];
	init_test_data(t);

	fn((void **) t, SAMPLE_SIZE, (cmpfn) person_id_cmp);
	printf("\t");
	person_arr_print(t, SAMPLE_SIZE);
	printf("\n");

	fn((void **) t, SAMPLE_SIZE, (cmpfn) person_name_cmp);
	printf("\t");
	person_arr_print(t, SAMPLE_SIZE);

	printf("\n\n");
}

int main(int argc, char **argv) {
	test_sort(bubble_sort, "bubble sort");
	test_sort(selection_sort, "selection sort");
	test_sort(insertion_sort, "insertion sort");
	test_sort(shell_sort, "shell sort");
	test_sort(merge_sort_recur, "recursive merge sort");
	test_sort(merge_sort, "merge sort");
	test_sort(quick_sort_recur, "recursive quick sort");
	test_sort(quick_sort, "quick sort");
	test_sort(heap_sort, "heap sort");
	return 0;
}
