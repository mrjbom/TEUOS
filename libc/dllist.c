#include "dllist.h"

void dll_insert_node_to_tail(doubly_linked_list_t* list, dll_node_t* new_node)
{
    if (list == NULL || new_node == NULL) {
        return;
    }
    if (list->count == 0) {
        list->count++;
        list->head = new_node;
        list->tail = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
    }
    else {
        list->count++;
        dll_node_t* old_tail = list->tail;
        old_tail->next = new_node;
        new_node->prev = old_tail;
        new_node->next = NULL;
        list->tail = new_node;
        list->tail->next = NULL;
    }
}

void dll_insert_node_to_head(doubly_linked_list_t* list, dll_node_t* new_node)
{
    if (list == NULL || new_node == NULL) {
        return;
    }
    if (list->count == 0) {
        dll_insert_node_to_tail(list, new_node);
    }
    else {
        list->count++;
        dll_node_t* old_head = list->head;
        new_node->next = old_head;
        old_head->prev = new_node;
        new_node->prev = NULL;
        list->head = new_node;
        list->head->prev = NULL;
    }
}

void dll_insert_node_after_node(doubly_linked_list_t* list, dll_node_t* node_in_list, dll_node_t* new_node)
{
    if (list == NULL || node_in_list == NULL || new_node == NULL || list->count == 0) {
        return;
    }
    if (list->count == 1) {
        list->count++;
        list->head->next = new_node;
        new_node->prev = list->head;
        list->tail = new_node;
        list->tail->next = NULL;
    }
    else /* count > 1 */ {
        list->count++;
        if (node_in_list == list->tail) {
            list->tail->next = new_node;
            new_node->prev = list->tail;
            list->tail = new_node;
            list->tail->next = NULL;
        }
        else {
            dll_node_t* node_in_list_next = node_in_list->next;
            node_in_list->next = new_node;
            new_node->prev = node_in_list;
            new_node->next = node_in_list_next;
            node_in_list_next->prev = new_node;
        }
    }
}

void dll_insert_node_before_node(doubly_linked_list_t* list, dll_node_t* node_in_list, dll_node_t* new_node)
{
    if (list == NULL || node_in_list == NULL || new_node == NULL || list->count == 0) {
        return;
    }
    if (list->count == 1) {
        list->count++;
        new_node->next = node_in_list;
        node_in_list->prev = new_node;
        list->head = new_node;
        list->head->prev = NULL;
    }
    else /* count > 1 */ {
        list->count++;
        if (node_in_list == list->head) {
            new_node->next = list->head;
            list->head->prev = new_node;
            list->head = new_node;
            list->head->prev = NULL;
        }
        else {
            dll_node_t* node_in_list_prev = node_in_list->prev;
            node_in_list_prev->next = new_node;
            new_node->prev = node_in_list_prev;
            new_node->next = node_in_list;
            node_in_list->prev = new_node;
        }
    }
}

void dll_remove_node(doubly_linked_list_t* list, dll_node_t* node)
{
    if (list == NULL || node == NULL || list->count == 0) {
        return;
    }
    if (list->count == 1) {
        list->count--;
        list->head = NULL;
        list->tail = NULL;
    }
    else /* count > 1 */ {
        list->count--;
        if (node == list->head) {
            list->head = list->head->next;
            list->head->prev = NULL;
        }
        else if (node == list->tail) {
            list->tail = list->tail->prev;
            list->tail->next = NULL;
        }
        else {
            dll_node_t* node_prev = node->prev;
            dll_node_t* node_next = node->next;
            node_prev->next = node_next;
            node_next->prev = node_prev;
        }
    }
}
