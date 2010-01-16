#include "util/list.h"

#ifdef LIST_TEST
# include <stdlib.h>
# define kmalloc malloc
# define kfree free
#else
# include "mm/kmalloc.h"
#endif

/**
 *  list_empty(list)
 *
 * Returns true if the list is empty.
 */
bool list_empty(list_t *list)
{
	return (!list) || (list->front == NULL);
}

/**
 *  list_size(list)
 *
 * Returns the size of the list.
 */
size_t list_size(list_t *list)
{
	if (list_empty(list))
		return 0;
	return list->size;
}

static inline list_entry_t *_list_create_entry(void *data)
{
	list_entry_t *entry = kmalloc(sizeof(list_entry_t));
	entry->data = data;
	entry->prev = NULL;
	entry->next = NULL;

	return entry;
}

static inline void *_list_delete_entry(list_entry_t *e)
{
	void *data = e->data;
	kfree(e);
	return data;
}

/**
 *  list_add_front(list, data)
 *
 * Adds data in the front of the list.
 */
list_t *list_add_front(list_t *list, void *data)
{
	list_entry_t *e = _list_create_entry(data);
	e->next = list->front;

	if (list->front)
		list->front->prev = e;
	else if (!list->back)
		list->back = e;

	list->front = e;
	list->size++;

	return list;
}

/**
 *  list_add_back(list, data)
 *
 * Adds data in the back of the list.
 */
list_t *list_add_back(list_t *list, void *data)
{
	list_entry_t *e = _list_create_entry(data);
	e->prev = list->back;

	if (list->back)
		list->back->next = e;
	else if (!list->front)
		list->front = e;

	list->back = e;
	list->size++;

	return list;
}

list_t *list_add_after(list_t *list, list_entry_t *entry, void *data)
{
	list_entry_t *ne = _list_create_entry(data);

	if (entry->next) {
		ne->next = entry->next;
		entry->next->prev = ne;
	}
	entry->next = ne;
	ne->prev = entry;

	if (entry == list->back) {
		list->back = ne;
	}

	list->size++;

	return list;
}

/**
 *  list_del_front(list)
 *
 * Deletes the first entry in the list and returns its data.
 */
void *list_del_front(list_t *list)
{
	if (list_empty(list))
		return NULL;

	list_entry_t *e = list->front;
	list->front = e->next;
	if (list->back == e)
		list->back = NULL;

	list->size--;

	return _list_delete_entry(e);
}

/**
 *  list_del_back(list)
 *
 * Deletes the last entry in the list and returns its data.
 */
void *list_del_back(list_t *list)
{
	if (list_empty(list))
		return NULL;

	list_entry_t *e = list->back;
	list->back = e->prev;
	if (list->front == e)
		list->front = NULL;

	list->size--;

	return _list_delete_entry(e);
}

/**
 *
 *
 *
 */
void *list_del_entry(list_t *list, list_entry_t *entry)
{
	if (entry->next)
		entry->next->prev = entry->prev;
	if (entry->prev)
		entry->prev->next = entry->next;

	if (list->front == entry)
		list->front = entry->next;
	if (list->back == entry)
		list->back = entry->prev;

	return _list_delete_entry(entry);
}

/**
 *  list_front(list)
 *
 * Returns the first value in the list
 */
void *list_front(list_t *list)
{
	if (list_empty(list))
		return NULL;
	return list->front->data;
}

/**
 *  list_back(list)
 *
 * Returns the last value in the list
 */
void *list_back(list_t *list)
{
	if (list_empty(list))
		return NULL;
	return list->back->data;
}

/**
 *  list_create(data)
 *
 * Creats a new list entry.
 */
list_t *list_create()
{
	list_t *l = kmalloc(sizeof(list_t));
	//l->front = NULL;
	//l->back  = NULL;
	//l->size  = 0;
	list_init(l);

	return l;
}

/**
 *  list_destroy(list)
 *
 * Destroys a list and all entries.
 */
void list_destroy(list_t *list)
{
	while (list_del_front(list))
		;

	kfree(list);
}
