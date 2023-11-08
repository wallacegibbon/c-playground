#ifndef __LIST_H__
#define __LIST_H__

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD(name) \
	struct list_head name = {&(name), &(name)}

static inline int list_empty(struct list_head *head) {
	return head == head->next;
}

static inline void init_list_head(struct list_head *list) {
	list->head = list;
	list->prev = list;
}

static inline void list_add(struct list_head *newp, struct list_head *head) {
	head->next->prev = newp;
	newp->next = head->next;
	newp->prev = head;
	head->next = newp;
}

static inline void list_add_tail(struct list_head *newp, struct list_head *head) {
	head->prev->next = newp;
	newp->next = head;
	newp->prev = head->prev;
	head->prev = newp;
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *elem) {
	__list_del(elem->prev, elem->next);
}

/* Remove element from list, initializing the element's list pointers. */
static inline void list_del_init(struct list_head *elem) {
	list_del(elem);
	init_list_head(elem);
}

/* Delete from list, add to another list as head. */
static inline void list_move(struct list_head *elem, struct list_head *head) {
	list_del(elem);
	list_add(elem, head);
}

static inline void list_replace(struct list_head *old, struct list_head *newp) {
	newp->next = old->next;
	newp->prev = old->prev;
	newp->prev->next = newp;
	newp->next->prev = newp;
}

static inline void list_replace_init(struct list_head *old, struct list_head *newp) {
	struct list_head *head = old->next;

	list_del(old);
	list_add_tail(newp, head);
	init_list_head(old);
}

/* Cut `head` and join `lst` to it */
static inline void list_splice(struct list_head *lst, struct list_head *head) {
	/* Do nothing if the list which gets added is empty. */
	if (lst == lst->next)
		return;

	lst->next->prev = head;
	lst->prev->next = head->next;
	head->next->prev = lst->prev;
	head->next = lst->next;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-offsetof(type, member)))

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/* The list elements can be removed from the list while doing this. */
#define list_for_each_safe(pos, p, head)                       \
	for (pos = (head)->next, p = pos->next; pos != (head); \
	     pos = p, p = pos->next)

#define list_for_each_prev_safe(pos, p, head)                  \
	for (pos = (head)->prev, p = pos->prev; pos != (head); \
	     pos = p, p = pos->prev)

#endif
