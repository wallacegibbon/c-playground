#ifndef __SAMPLE_DATA_H__
#define __SAMPLE_DATA_H__

#include <string.h>

struct person {
	int id;
	char name[20];
};

#define SAMPLE_DATA_SIZE 15

struct person person_db[SAMPLE_DATA_SIZE] =
{
{7, "Wally"}, {2, "Harry"}, {10, "Bruce"}, {4, "Ada"}, {1, "Jerry"}, {3, "Q"},
{25, "Ron"}, {18, "Judy"}, {5, "Hans"}, {20, "Anna"}, {4, "Elsa"}, {9, "Sven"},
{32, "Nick"}, {30, "Amy"}, {34, "Flash"}
};

static void test_data_init(struct person **buf) {
	for (int i = 0; i < SAMPLE_DATA_SIZE; i++)
		buf[i] = &person_db[i];
}

static int person_id_cmp(struct person *p1, struct person *p2) {
	return p1->id - p2->id;
}

static int person_name_cmp(struct person *p1, struct person *p2) {
	return strcmp(p1->name, p2->name);
}

static inline void person_print(struct person *p, char *prefix, char *suffix) {
	printf("%s{%d, %s}%s", prefix, p->id, p->name, suffix);
}

static int person_arr_print(struct person **p, int size) {
	for (int i = 0; i < size; i++)
		person_print(p[i], "", " ");
}

#endif
