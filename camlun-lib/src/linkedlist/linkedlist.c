#include "linkedlist.h"
#include <stdlib.h>
#include <stddef.h>

LinkedList *linkedlist_create() {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->head = NULL;
    return list;
}

void linkedlist_destroy(LinkedList *this) {
    LinkedListNode *current = this->head;
    while (current != NULL) {
        LinkedListNode *next = current->next;
        free(current);
        current = next;
    }
    free(this);
}

LinkedListNode *linkedlist_push_front(LinkedList *this, void *data) {
    LinkedListNode *new_node = malloc(sizeof(LinkedListNode));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = this->head;
    new_node->prev = NULL;

    if (this->head != NULL) {
        this->head->prev = new_node;
    }

    this->head = new_node;
    this->size++;
    return new_node;
}

void linkedlist_delete(LinkedList *this, LinkedListNode *node) {
    if (node == NULL) {
        return;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        this->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    free(node);
    this->size--;

}

void linkedlist_clear(LinkedList *this) {
    LinkedListNode *current = this->head;
    while (current != NULL) {
        LinkedListNode *next = current->next;
        free(current);
        current = next;
    }
    this->head = NULL;
    this->size = 0;
}

size_t linkedlist_size(LinkedList *this) {
    return this->size;
}

