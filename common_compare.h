#ifndef __COMMON_COMPARE_H__
#define __COMMON_COMPARE_H__

#define max(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); \
		_a > _b ? (_a) : (_b); })

#define min(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); \
		_a > _b ? (_b) : (_a); })

// this type of function simply do `someop(e1) - someop(e2)`
typedef int (*cmpfn)(void *e1, void *e2);

#endif
