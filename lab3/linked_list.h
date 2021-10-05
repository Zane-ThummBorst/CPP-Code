/*******************************************************************************
 * Name        : lab3
 * Author      : Zane ThummBorst
 * Date        : 2/26/2021
 * Description :
 * Pledge        :I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <string.h>
#include "node.h"

typedef struct linked_list {
    node *head;
    node *tail;
    size_t num_nodes;
} linked_list;

linked_list* create_linked_list() {
    return (linked_list *)calloc(1, sizeof(linked_list));
}

/**
 * Inserts a non-NULL node into a non-NULL linked list.
 * The node is inserted so that the data in the linked list is in
 * non-decreasing order.
 * A node with a value already in the list is inserted AFTER the node(s)
 * already in the list.
 */
void insert_in_order(linked_list *list, node *n, int (*cmp)(const void*, const void*)) {
    // TODO
	if(!list->head){
	    list->head = list->tail = n;
	    list->num_nodes++;
	}
	else{
	for (node *cur = list->head; cur != NULL; cur = cur->next){
		if(cmp(cur->data,n->data) > 0 && !cur->prev){
			cur->prev = n;
			n->next = cur;
			list->num_nodes++;
			list->head = n;
			return;
		}
		else if(cmp(cur->data,n->data) <= 0){
			if(!cur->next){
		        cur->next = n;
		        n->prev = cur;
		        n->next = NULL;
		        list->num_nodes++;
		        list->tail = n;

		        return;
			}
			else if((cmp(cur->next->data,n->data) > 0)){
				node *temp = NULL;
				temp = cur->next;
				cur->next = n;
				n->prev = cur;
				temp->prev = n;
				n->next = temp;
				list->num_nodes++;
				return;
			}
		}
	}
	}

}

void print_list(linked_list *list, void (*print_function)(void*)) {
    putchar('[');
    node *cur = list->head;
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    printf("]\n{length: %lu, head->data: ", list->num_nodes);
    list->head != NULL ? print_function(list->head->data) :
                         (void)printf("NULL");
    printf(", tail->data: ");
    list->tail != NULL ? print_function(list->tail->data) :
                         (void)printf("NULL");
    printf("}\n\n");
}

/**
 * Frees a list starting from the tail.
 * This will check if your previous pointers have been set up correctly.
 */
void free_list(linked_list *list, void (*free_data)(void *)) {
    while (list->tail != NULL) {
        node *prev = list->tail->prev;
        free_node(list->tail, free_data);
        list->tail = prev;
    }
}

#endif
