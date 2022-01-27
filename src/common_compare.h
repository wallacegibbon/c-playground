#ifndef __COMMON_COMPARE_H__
#define __COMMON_COMPARE_H__

static inline int min_int(int a, int b) {
	return a < b ? a : b;
}

static inline int max_int(int a, int b) {
	return a > b ? a : b;
}

static inline int abs_int(int a) {
	return a > 0 ? a : -a;
}

// this type of function simply do `someop(e1) - someop(e2)`
typedef int (*cmpfn)(void *e1, void *e2);

static inline void swap_e(void **e1, void **e2) {
	void *tmp = *e1;
	*e1 = *e2;
	*e2 = tmp;
}

#endif
