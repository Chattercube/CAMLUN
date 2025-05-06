#ifndef VECTOR_EXT_H
#define VECTOR_EXT_H

#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

// ==== Methods Overview ====

// Access and iteration :



// Modifiers :

// -- Heap utilities

void vector_heapify(Vector *this, size_t pos, bool is_max);
void vector_build_heap(Vector *this, bool is_max);
void *heap_poll(Vector *this, bool is_max);
void heap_offer(Vector *this, void *data, bool is_max);
void heap_update(Vector *this, size_t pos, void *data, bool is_max);

// -- Sorting

void vector_qsort(Vector *this, bool descending);
void vector_sort(Vector *this, bool descending);


// Copy constructors or creators : 



// ==== End of Methods Overview ====




#endif