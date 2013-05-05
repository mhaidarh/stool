/*
	SMTI06, 54411850, M Haidar Hanif
	GIoT, Tomer Elmalem
	Stulink | Simple student database with doubly linked list
*/

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct lnode
{
	void* data;
	struct lnode* prev;
	struct lnode* next;
} node;

list* create_list(void)
{
	list *l = malloc(sizeof(list));
	l->head = NULL;
	l->size = 0;
	return l;
}

static node* create_node(void* data);

static node* create_node(void* data)
{
	node *n = malloc(sizeof(node));
	n->data = data;
	n->prev = NULL;
	n->next = NULL;
	return n;
}

void push_front(list* llist, void* data)
{
	node *n = create_node(data);

	if (!llist->size) {
		n->next = n;
		n->prev = n;
	} else {
		node *head = llist->head;
		node *prev = head->prev;
		n->next = head;
		n->prev = head->prev;
		head->prev = n;
		prev->next = n;
	}

	llist->head = n;
	llist->size++;
}

void push_rear(list* llist, void* data)
{
	node *n = create_node(data);

	if (!llist->size) {
		n->next = n;
		n->prev = n;
		llist->head = n;
	} else {
		node *head = llist->head;
		node *prev = head->prev;
		n->next = head;
		n->prev = head->prev;
		head->prev = n;
		prev->next = n;
	}

	llist->size++;
}

int remove_front(list* llist, list_op free_func)
{
	if (!llist->size) return -1;

	node *head = llist->head;
	
	if (llist->size == 1) {
		llist->head = NULL;
	} else {
		node *next = head->next;
		node *prev = head->prev;

		llist->head = next;

		next->prev = prev;
		prev->next = next;
	}

	free_func(head->data);
	free(head);

	llist->size--;
	return 0;
}

int remove_index(list* llist, int index, list_op free_func)
{
	if (!llist->size) return -1;

	node *current = llist->head;

	for (int i=0; i<index; i++) {
		current = current->next;
	}

	if (llist->size == 1) {
		llist->head = NULL;
	} else {
		node *next = current->next;
		node *prev = current->prev;
		prev->next = next;
		next->prev = prev;
	}

	free_func(current->data);
	free(current);

	llist->size--;

	return 0;
}

int remove_rear(list* llist, list_op free_func)
{
	if (!llist->size) return -1;

	node *head = llist->head;
	node *tbr = head->prev;
	node *nb = tbr->prev;

	if (llist->size == 1) {
		llist->head = NULL;
	} else {
		head->prev = nb;
		nb->next = head;
	}

	free_func(tbr->data);
	free(tbr);
	
	llist->size--;

	return 0;
}

int remove_data(list* llist, const void* data, equal_op compare_func, list_op free_func)
{
	int removed = 0;

	if (!llist->size) return removed;

	node *current = llist->head;
	node *next = current->next;
	node *prev = current->prev;
	int is_head = 1;

	for (int i=0; i<llist->size; i++) {
		if (compare_func(data, current->data)) {
			if (is_head) llist->head = next;

			next->prev = prev;
			prev->next = next;
			free_func(current->data);
			free(current);
			current = next;
			removed++;
		} else {
			is_head = 0;
			current = current->next;
		}

		if (llist->size > 1) {
			next = current->next;
			prev = current->prev;
		}
	}

	llist->size -= removed;
	if (!llist->size) llist->head = NULL;
	return removed;
}

void* front(list* llist)
{
	if (llist->size) {
		return llist->head->data;
	} else {
		return NULL;
	}
}

void* get_index(list* llist, int index)
{
	if (!llist->size || index >= llist->size) {
		return NULL;
	}

	node *current = llist->head;
	for (int i=0; i<index; i++) {
		current = current->next;
	}

	return current->data;
}

void* rear(list* llist)
{
	if (!llist->size) return NULL;

	node *end = llist->head->prev;
	return end->data;
}

int is_empty(list* llist)
{
	if (llist->size == 0 && llist->head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

int size(list* llist)
{
	return llist->size;
}

int find_occurrence(list* llist, const void* search, equal_op compare_func)
{
	node *current = llist->head;
	for (int i=0; i<llist->size; i++) {
		if (compare_func(search, current->data)) return 1;
		current = current->next;
	}

	return 0;
}

void empty_list(list* llist, list_op free_func)
{
	if (!llist->size) return;

	node *current = llist->head;
	node *next = current->next;

	for (int i=0; i<llist->size; i++) {
		free_func(current->data);
		free(current);
		current = next;
		if (i < llist->size-1) next = current->next;
	}

	llist->head=NULL;
	llist->size=0;
}

void traverse(list* llist, list_op do_func)
{
	node *current = llist->head;
	for (int i=0; i<llist->size; i++) {
		do_func(current->data);
		current = current->next;
	}
}

