#ifndef _DLLIST_H_
#define _DLLIST_H_

#include <stddef.h>

// Doubly linked list

typedef struct {
    void* next;
    void* prev;
    void* data;
} dll_node_t;

typedef struct {
    dll_node_t* head;
    dll_node_t* tail;
    size_t count;
} doubly_linked_list_t;

/*
 * Insert node to the end of the list (new tail) 
 * (adds node to the list)
 */
extern void dll_insert_node_to_tail(doubly_linked_list_t* list, dll_node_t* new_node);

/*
 * Insert node to the start of the list (new head)
 * (adds node to the list)
 */
extern void dll_insert_node_to_head(doubly_linked_list_t* list, dll_node_t* new_node);

/*
 * Insert node after the node in the list
 * (adds node to the list)
 */
extern void dll_insert_node_after_node(doubly_linked_list_t* list, dll_node_t* node_in_list, dll_node_t* new_node);

/*
 * Insert node before the node in the list
 * (adds node to the list)
 */
extern void dll_insert_node_before_node(doubly_linked_list_t* list, dll_node_t* node_in_list, dll_node_t* new_node);

/*
 * Remove node from the list
 */
extern void dll_remove_node(doubly_linked_list_t* list, dll_node_t* node);

#endif
