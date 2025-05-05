#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

// ==== Type definition ====

typedef struct LinkedListNode {
    void *data;
    struct LinkedListNode *next;
    struct LinkedListNode *prev;
} LinkedListNode;

typedef struct LinkedList {
    size_t size;
    struct LinkedListNode *head;
} LinkedList;

// ==== End of Type definition ====

// ==== Method Overview ====

LinkedList *linkedlist_create();
void linkedlist_destroy(LinkedList *this);
LinkedListNode *linkedlist_push_front(LinkedList *this, void *data);
void linkedlist_delete(LinkedList *this, LinkedListNode *node);
void linkedlist_clear(LinkedList *this);
size_t linkedlist_size(LinkedList *this);

// ==== End of Method Overview ====

// ==== Macros ====

#define LINKEDLIST_FOREACH(linkedlist, varname, code)  \
    do {                                               \
        LinkedListNode *_current = (linkedlist)->head; \
        while (_current != NULL) {                     \
            varname = _current->data;                   \
            code;                                      \
            _current = _current->next;                  \
        }                                              \
    } while (0)

#endif