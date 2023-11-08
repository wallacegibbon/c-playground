#include "common_compare.h"
#include "sample_data.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

static void sort_test(sortfn fn, char *prefix) {
	struct person *t[SAMPLE_DATA_SIZE];

	printf("%s:\n", prefix);
	test_data_init(t);

	fn((void **)t, SAMPLE_DATA_SIZE, (cmpfn)person_id_cmp);
	printf("\t");
	person_arr_print(t, SAMPLE_DATA_SIZE);
	printf("\n");

	fn((void **)t, SAMPLE_DATA_SIZE, (cmpfn)person_name_cmp);
	printf("\t");
	person_arr_print(t, SAMPLE_DATA_SIZE);

	printf("\n\n");
}

int main(int argc, const char **argv) {
	sort_test(bubble_sort, "bubble sort");
	sort_test(selection_sort, "selection sort");
	sort_test(insertion_sort, "insertion sort");
	sort_test(shell_sort, "shell sort");
	sort_test(merge_sort_recur, "recursive merge sort");
	sort_test(merge_sort, "merge sort");
	sort_test(quick_sort_recur, "recursive quick sort");
	sort_test(quick_sort, "quick sort");
	sort_test(heap_sort, "heap sort");
	return 0;
}
