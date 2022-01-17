#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define min(a, b) ((a) > (b) ? (b) : (a))

// this type of function simply do `someop(e1) - someop(e2)`
typedef int (*cmpfn)(void *e1, void *e2);
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

static void __insert_sort(void **arr, int size, cmpfn cmp, int delta) {
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

void insert_sort(void **arr, int size, cmpfn cmp) {
	__insert_sort(arr, size, cmp, 1);
}

void shell_sort(void **arr, int size, cmpfn cmp) {
	int delta = 1;
	while (delta < size / 3)
		delta = delta * 3 + 1; // Knuth, 1973

	for (; delta >= 1; delta /= 3)
		__insert_sort(arr, size, cmp, delta);
}

/* merge 2 sorted array */
static void __merge(void **arr, int start, int mid, int end, cmpfn cmp) {
	void **buf = alloca((end - start) * sizeof(void *));

	int s1 = start, s2 = mid;
	int i = 0;

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

static void __merge_sort_unit(void *arr, int size, int step, cmpfn cmp) {
	int unit = step * 2;
	for (int i = 0; i < size; i += unit)
		__merge(arr, i, min(i + step, size), min(i + unit, size), cmp);
}

void merge_sort(void **arr, int size, cmpfn cmp) {
	for (int step = 1; step < size; step *= 2)
		__merge_sort_unit(arr, size, step, cmp);
}

void quick_sort_recur(void **arr, int size, cmpfn cmp) {
}

void quick_sort(void **arr, int size, cmpfn cmp) {
}

void heap_sort(void **arr, int size, cmpfn cmp) {
}

struct person {
	int id;
	char name[20];
};

#define SAMPLE_SIZE 12

struct person person_db[SAMPLE_SIZE] =
{
{7, "Wally"}, {2, "Harry"}, {10, "Bruce"}, {4, "Ada"}, {1, "Jerry"}, {3, "Q"},
{25, "Ron"}, {18, "Judy"}, {5, "Hans"}, {20, "Anna"}, {4, "Elsa"}, {9, "Sven"}
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
	test_sort(insert_sort, "insert sort");
	test_sort(shell_sort, "shell sort");
	test_sort(merge_sort_recur, "recursive merge sort");
	test_sort(merge_sort, "merge sort");
	/*
	test_sort(quick_sort_recur, "recursive quick sort");
	test_sort(quick_sort, "quick sort");
	*/
	return 0;
}
