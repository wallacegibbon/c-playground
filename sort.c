#include <stdio.h>
#include <string.h>

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

void __insert_sort(void **arr, int size, cmpfn cmp, int delta) {
	for (int i = delta; i < size; i += delta) {
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

void heap_sort(void **arr, int size, cmpfn cmp) {
}

void quick_sort(void **arr, int size, cmpfn cmp) {
}

struct person {
	int id;
	char name[20];
};

#define SAMPLE_SIZE 6

struct person person_db[SAMPLE_SIZE] =
{{7, "Wally"}, {2, "Harry"}, {10, "Bruce"}, {4, "Ada"}, {1, "Jerry"}, {3, "Q"}};

void init_test_data(struct person **buf) {
	for (int i = 0; i < SAMPLE_SIZE; i++)
		buf[i] = &person_db[i];
}

int person_id_cmp(struct person *p1, struct person *p2) {
	return p1->id - p2->id;
}

int person_name_cmp(struct person *p1, struct person *p2) {
	return strcmp(p1->name, p2->name);
}

static inline void print_one_person(struct person *p) {
	printf("{%d, %s} ", p->id, p->name);
}

int person_arr_print(struct person **p, int size) {
	for (int i = 0; i < size; i++)
		print_one_person(p[i]);
}

void test_sort(sortfn fn, char *prefix) {
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
	return 0;
}
