#include <stdio.h>
#include <stdlib.h>
#include "common_compare.h"

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

void range_init(struct range *rng, int s, int e) {
	rng->s = s;
	rng->e = e;
}

void quick_sort(void **arr, int size, cmpfn cmp) {
	struct range *r_stack = alloca(sizeof(struct range) * log2_int(size));
	// i always points to the next empty stack slot
	int i = 0;

	range_init(&r_stack[i++], 0, size);
	while (i) {
		struct range r = r_stack[--i];
		if (r.s >= r.e - 1)
			continue;

		int mid = __divide(arr, r.s, r.e, cmp);

		range_init(&r_stack[i++], r.s, mid);
		range_init(&r_stack[i++], mid, r.e);
	}
}

static void heap_adjust(void **arr, int size, int parent, cmpfn cmp) {
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
		heap_adjust(arr, size, i, cmp);

	for (int i = size - 1; i > 0; i--) {
		swap_e(&arr[0], &arr[i]);
		heap_adjust(arr, i, 0, cmp);
	}
}

