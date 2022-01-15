#ifndef __UNUSED_H__
#define __UNUSED_H__

//#define offsetof(type, member) __builtin_offsetof(type, member)
#define offsetof(type, member) ((size_t) &((type *) 0)->member)

/*
#define container_of(ptr, type, member) ({ \
	const typeof( ((type *) 0)-> member ) *__mptr = (ptr); \
	(type *) ((char *) __mptr - offsetof(type, member)); })
*/

#define container_of(ptr, type, member) \
	((type *) ((char *) (ptr) - offsetof(type, member)))

#endif
