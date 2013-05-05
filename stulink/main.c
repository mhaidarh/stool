/*
	SMTI06, 54411850, M Haidar Hanif
	GIoT, Tomer Elmalem
	Stulink | Simple student database with doubly linked list
*/

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Student
{
	unsigned int npm;
	char* first_name;
	char* last_name;
	int score;
} Student;

Student* create_student(unsigned int npm, const char* first_name, const char* last_name, int score)
{
	Student *s = malloc(sizeof(Student));

	char *f = malloc((strlen(first_name)+1) * sizeof(char));
	char *l = malloc((strlen(last_name)+1) * sizeof(char));

	strncpy(f, first_name, strlen(first_name)+1);
	strncpy(l, last_name, strlen(last_name)+1);

	s->npm = npm;
	s->first_name = f;
	s->last_name = l;
	s->score = score;

	return s;
}

void print_student_ver(void* data)
{
	Student *s = data;
	printf("NPM: %d\n", s->npm);
	printf("Full Name: %s %s\n",
		s->first_name, s->last_name);
	printf("Score: %d\n", s->score);
}

void print_student(void* data)
{
	Student *s = data;
	printf("[%d] %s %s : %d\n",
		s->npm, s->first_name, s->last_name, s->score);
}

void free_student(void* data)
{
	Student *s = data;

	free(s->first_name);
	free(s->last_name);
	free(s);
}

int student_eq(const void* a, const void* b)
{
	const Student *s1 = a;
	const Student *s2 = b;

	if (!strncmp(s1->first_name, s2->first_name, strlen(s1->first_name)) &&
		!strncmp(s2->last_name,	s2->last_name,	strlen(s1->last_name)) &&
		s1->score == s2->score &&
		s1->npm == s2->npm) {
		return 1;
	} else {
		return 0;
	}
}

int main(void) 
{
	printf("Stulink Tests");
	list* llist = create_list();

	printf("TEST CASE | An Empty list should print nothing here:\n");
	traverse(llist, print_student);
	printf("\n");

 	push_front(llist, create_student(12345678, "Nick", "Fury", 88));
 	printf("TEST CASE | A List with one student should print that student:\n");
 	traverse(llist, print_student);
 	printf("\n");

 	remove_front(llist, free_student);
 	printf("TEST CASE | Another Empty list should print nothing here:\n");
 	traverse(llist, print_student);
 	printf("\n");

 	push_front(llist, create_student(123456789, "Tony", "Stark", 92));
 	push_front(llist, create_student(902313373, "Nikola", "Tesla", 65));
 	printf("TEST CASE | A List with two students should print those two students:\n");
 	traverse(llist, print_student);
 	printf("\n"); 	

	Student* p = create_student(131313137, "Mother", "Earth", 3);
	printf("TEST CASE | Finding occurence when a node is not in the list should fail:\n");
	printf("%d\n", find_occurrence(llist, p, student_eq));
	free_student(p);
 	printf("\n");

	empty_list(llist, free_student);
 	printf("TEST CASE | Empty. After freeing all nodes the list should be empty:\n");
 	traverse(llist, print_student);
 	printf("\n"); 	
 	
	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	push_front(llist, create_student(10101010, "Iron", "Man", 42));
	push_front(llist, create_student(54545454, "Bruce", "Banner", 200));
	push_rear(llist, create_student(00112233, "Thomas", "Edison", 0));
	printf("TEST CASE | Push to rear. Order should be Bruce, Iron Man, Tony, Thomas:\n");
	traverse(llist, print_student);
	printf("\n");

	remove_front(llist, free_student);
	printf("TEST CASE | Remove front. Bruce should no longer be in the list:\n");
	traverse(llist, print_student);
	printf("\n");

	printf("TEST CASE | Remove rear. Thomas should no longer by in the list:\n");
	remove_rear(llist, free_student);
	traverse(llist, print_student);
	printf("\n");

	remove_index(llist, 1, free_student);
	printf("TEST CASE | Remove by index. Remove the student at index 1, Tony should no longer be in the list:\n");
	traverse(llist, print_student);
	printf("\n");

	empty_list(llist, free_student);
	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	push_front(llist, create_student(12345678, "Iron", "Man", 42));
	push_front(llist, create_student(12345678, "Bruce", "Banner", 200));
	push_rear(llist, create_student(12345678, "Thomas", "Edison", 0));

	printf("TEST CASE | Front. get the front of the list. Should be Bruce:\n");
	print_student(front(llist));
	printf("\n");

	printf("TEST CASE | Get index. get the student at index 1. Should be Iron Man:\n");
	print_student(get_index(llist, 1));
	printf("\n");

	printf("TEST CASE | Rear. Let's get the student at the rear of the list. Should be Tony:\n");
	print_student(rear(llist));
	printf("\n");

	printf("TEST CASE | Is empty. Is the list empty? (Should be no)\n");
	printf(is_empty(llist) ? "Yes\n" : "No\n");
	printf("\n");

	printf("TEST CASE | Size. What is the size of the list? (Should be 3)\n");
	printf("%d\n", size(llist));
	printf("\n");

	p = create_student(12345678, "Tony", "Stark", 92);
	printf("TEST CASE | Find occurence. Does the student Tony Stark exist in the list? (Should be yes)\n");
	printf(find_occurrence(llist, p, student_eq) ? "Yes\n" : "No\n");
	printf("\n");
	free_student(p);

	empty_list(llist, free_student);

	push_rear(llist, create_student(12345678, "Tony", "Stark", 92));
	printf("TEST CASE | Push rear on empty list:\n");
	traverse(llist, print_student);
	printf("\n");
	empty_list(llist, free_student);

	printf("TEST CASE | Remove fromt on empty list: (Should fail and return -1)\n");
	printf("%d\n", remove_front(llist, free_student));
	printf("\n");

	printf("TEST CASE | Remove index on empty list: (Should fail and return -1)\n");
	printf("%d\n", remove_index(llist, 0, free_student));
	printf("\n");

	printf("TEST CASE | Remove rear on empty list: (Should fail and return -1)\n");
	printf("%d\n", remove_rear(llist, free_student));
	printf("\n");

	p = create_student(12345678, "Tony", "Stark", 92);
	printf("TEST CASE | Remove data on empty list: (nothing should happen, should return 0)\n");
	printf("%d\n", remove_data(llist, p, student_eq, free_student));
	printf("\n");
	free_student(p);

	printf("TEST CASE | Is empty on empty list:\n");
	printf("%d\n", is_empty(llist));
	printf("\n");

	p = create_student(12345678, "Tony", "Stark", 92);
	printf("TEST CASE | Find occurrence on empty list:\n");
	printf("%d\n", find_occurrence(llist, p, student_eq));
	printf("\n");
	free_student(p);

	empty_list(llist, free_student);

	push_front(llist, create_student(12345678, "Tony", "Stark", 92));

	push_rear(llist, create_student(12345678, "Iron", "Man", 42));
	printf("TEST CASE | Push rear on list of size 1. Should be Tony then Iron Man:\n");
	traverse(llist, print_student);
	printf("\n");
	empty_list(llist, free_student);

	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	printf("TEST CASE | Remove rear on list of size 1. Should be empty:\n");
	remove_rear(llist, free_student);
	printf("\n");

	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	printf("TEST CASE | Remove index on list of size 1. Should be empty:\n");
	remove_index(llist, 0, free_student);
	traverse(llist, print_student);
	printf("\n");

	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	p = create_student(12345678, "Tony", "Stark", 92);
	printf("TEST CASE | Remove data on list of size 1: (Should return 1)\n");
	printf("%d\n", remove_data(llist, p, student_eq, free_student));
	printf("\n");
	free_student(p);

	push_front(llist, create_student(12345678, "Tony", "Stark", 92));
	printf("TEST CASE | Front of a list of size 1: (Should be Tony)\n");
	print_student(front(llist));
	printf("\n");

	printf("TEST CASE | Rear of a list of size 1: (Should be Tony)\n");
	print_student(rear(llist));
	printf("\n");

	printf("TEST CASE | Get index of a list of size 1: (Should be Tony)\n");
	print_student(get_index(llist, 0));
	printf("\n");				
	empty_list(llist, free_student);

	empty_list(llist, free_student);
 	free(llist);

	printf("Stulink Tests End");
	return 0;
}

