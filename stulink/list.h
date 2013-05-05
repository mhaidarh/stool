#ifndef LIST_H
#define LIST_H

struct lnode;

typedef struct llist
{
	struct lnode* head;
	unsigned int size;
} list;

typedef void (*list_op)(void*);
typedef int (*list_pred)(const void*);
typedef int (*equal_op)(const void*, const void*);

// Creating
list* create_list(void);

// Adding
void push_front(list* llist, void* data);
void push_rear(list* llist, void* data);

// Removing
int remove_index(list* llist, int index, list_op free_func);
int remove_front(list* llist, list_op free_func);
int remove_rear(list* llist, list_op free_func);
int remove_data(list* llist, const void* data, equal_op compare_func, list_op free_func);
int remove_if(list* llist, list_pred pred_func, list_op free_func);

// Querying List
void* front(list* llist);
void* rear(list* llist);
void* get_index(list* llist, int index);
int is_empty(list* llist);
int size(list* llist);

// Searching
int find_occurrence(list* llist, const void* search, equal_op compare_func);

// Traversal
void traverse(list* llist, list_op do_func);

// Freeing
void empty_list(list* llist, list_op free_func);

#endif

