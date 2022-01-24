#ifndef __RBUFFER_H__
#define __RBUFFER_H__

struct rbuffer {
	void **buf;
	int size, in, out;
};

typedef void (*rbuffer_fmt)(void *);

void rbuffer_fmt_raw(void *any);

struct rbuffer *rbuffer_new(int size);

void rbuffer_del(struct rbuffer *buf);

int rbuffer_count(struct rbuffer *buf);

int rbuffer_rest(struct rbuffer *buf);

int rbuffer_put(struct rbuffer *buf, void **data, int count);

int rbuffer_get(struct rbuffer *buf, void **data, int count);

void rbuffer_print(struct rbuffer *buf, rbuffer_fmt fmt);

void rbuffer_inspect(struct rbuffer *buf);

#endif
