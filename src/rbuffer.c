#include "rbuffer.h"
#include <stdio.h>
#include <stdlib.h>

struct rbuffer *rbuffer_new(int size) {
	struct rbuffer *buf = malloc(sizeof(struct rbuffer));
	if (buf == NULL)
		return NULL;

	size += 1;
	if (size < 2)
		size = 2;

	void **b = malloc(sizeof(void **) * size);
	if (b == NULL)
		return NULL;

	buf->buf = b;
	buf->size = size;
	buf->in = 0;
	buf->out = 0;
	return buf;
}

void rbuffer_del(struct rbuffer *buf) {
	free(buf->buf);
	free(buf);
}

static inline int rbuffer_count(struct rbuffer *buf) {
	if (buf->in >= buf->out)
		return buf->in - buf->out;
	else
		return buf->size + buf->in - buf->out;
}

static inline int rbuffer_rest(struct rbuffer *buf) {
	return buf->size - 1 - rbuffer_count(buf);
}

int rbuffer_put(struct rbuffer *buf, void **data, int count) {
	if (rbuffer_rest(buf) < count)
		return 0;

	int i = 0;
	while (i < count && buf->in < buf->size)
		buf->buf[buf->in++] = data[i++];

	if (buf->in == buf->size)
		buf->in = 0;

	while (i < count)
		buf->buf[buf->in++] = data[i++];

	return count;
}

int rbuffer_get(struct rbuffer *buf, void **data, int count) {
	if (rbuffer_count(buf) < count)
		return 0;

	int i = 0;
	while (i < count && buf->out < buf->size)
		data[i++] = buf->buf[buf->out++];

	if (buf->out == buf->size)
		buf->out = 0;

	while (i < count)
		data[i++] = buf->buf[buf->out++];

	return count;
}

void rbuffer_print(struct rbuffer *buf, rbuffer_fmt fmt) {
	int i = buf->out;
	if (i > buf->in) {
		while (i < buf->size)
			fmt(buf->buf[i++]);
		i = 0;
	}
	while (i < buf->in)
		fmt(buf->buf[i++]);
	printf("\n");
}

void rbuffer_inspect(struct rbuffer *buf) {
	printf("<ring buffer> size: %d, in: %d, out: %d\n",
	       buf->size - 1, buf->in, buf->out);
}

void rbuffer_fmt_raw(void *any) {
	printf("%p ", any);
}
