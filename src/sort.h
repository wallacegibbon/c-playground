#ifndef __SORT_H__
#define __SORT_H__

typedef void (*sortfn)(void **arr, int size, cmpfn cmp);

void bubble_sort(void **arr, int size, cmpfn cmp);
void selection_sort(void **arr, int size, cmpfn cmp);
void insertion_sort(void **arr, int size, cmpfn cmp);
void shell_sort(void **arr, int size, cmpfn cmp);
void merge_sort(void **arr, int size, cmpfn cmp);
void merge_sort_recur(void **arr, int size, cmpfn cmp);
void quick_sort(void **arr, int size, cmpfn cmp);
void quick_sort_recur(void **arr, int size, cmpfn cmp);
void heap_sort(void **arr, int size, cmpfn cmp);

#endif
