#ifndef VECTOR_H
#define VECTOR_H

// ==== Includes ====

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "typemethods.h"

// ==== End of Includes ====

// ==== Constants ====

#define VECTOR_INITIAL_CAPACITY 16
#define VECTOR_GROWTH_FACTOR 2

// ==== End of Constants ====

// ==== Type Definitions ====

typedef void *VectorNode;

typedef struct {
    VectorNode *nodes;           // Array of nodes
    size_t size;                 // Current number of elements
    size_t capacity;             // Current capacity of the vector
    type_methods *data_methods;  // Methods for managing the data type
} Vector;

// ==== End of Type Definitions ====

// ==== Method Overview ====

// Private Methods :

size_t get_new_capacity(size_t target_capacity);
void vector_node_init(Vector *vector, VectorNode *node, void *data);

// Constructors and destructors :

Vector *vector_create(type_methods *data_methods);
void vector_destroy(Vector *this);

// Access and iteration :

void *vector_get(Vector *this, size_t pos);
void *vector_first(Vector *this);
void *vector_last(Vector *this);

VectorNode *vector_at(Vector *this, size_t pos);
VectorNode *vector_begin(Vector *this);
VectorNode *vector_end(Vector *this);
void vector_increment(VectorNode **this);
size_t vector_pos(Vector *this, VectorNode *node);

// Capacity :

bool vector_empty(Vector *this);
size_t vector_size(Vector *this);
size_t vector_capacity(Vector *this);

void vector_reserve(Vector *this, size_t new_capacity);
void vector_compact(Vector *this);

// Modifiers :

void vector_set(Vector *this, size_t pos, void *data);
void vector_insert(Vector *this, size_t pos, void *data);
void vector_erase(Vector *this, size_t pos);
void vector_push_back(Vector *this, void *data);
void vector_pop_back(Vector *this);

VectorNode *vector_insert_at(Vector *this, VectorNode *node, void *data);
VectorNode *vector_erase_at(Vector *this, VectorNode *node);
void vector_replace(Vector *this, VectorNode *node, void *data);

void vector_truncate(Vector *this, size_t new_size);
void vector_expand(Vector *this, size_t new_size);
void vector_resize(Vector *this, size_t new_size);
void vector_clear(Vector *this);

// Copy constructors or creators :

Vector *vector_clone(Vector *this, type_methods *new_data_methods);
Vector vector_slice(Vector *this, size_t offset, size_t count);

// === End of Method Overview ===

// ==== Macros ====

#define VECTOR_FPRINTF(stream, vector, varname, ...)   \
    do {                                               \
        fprintf(stream, "[");                          \
        if (vector_size(vector) > 0) {                 \
            VectorNode *node = vector_begin(vector);   \
            while (1) {                                \
                varname = *_node;                      \
                fprintf(stream, __VA_ARGS__);          \
                if (_node == vector_end(vector) - 1) { \
                    break;                             \
                }                                      \
                _node++;                               \
                fprintf(stream, ", ");                 \
            }                                          \
        }                                              \
        printf("]\n");                                 \
    } while (0)

#define VECTOR_SPRINTF(buffer, vector, varname, ...)           \
    do {                                                       \
        sprintf(buffer, "[");                                  \
        if (vector_size(vector) > 0) {                         \
            VectorNode *_node = vector_begin(vector);          \
            while (1) {                                        \
                varname = *_node;                              \
                sprintf(buffer + strlen(buffer), __VA_ARGS__); \
                if (_node == vector_end(vector) - 1) {         \
                    break;                                     \
                }                                              \
                _node++;                                       \
                sprintf(buffer + strlen(buffer), ", ");        \
            }                                                  \
        }                                                      \
        sprintf(buffer + strlen(buffer), "]\n");               \
    } while (0)

#define VECTOR_PRINTF(vector, varname, ...)            \
    do {                                               \
        printf("[");                                   \
        if (vector_size(vector) > 0) {                 \
            VectorNode *_node = vector_begin(vector);  \
            while (1) {                                \
                varname = *_node;                      \
                printf(__VA_ARGS__);                   \
                if (_node == vector_end(vector) - 1) { \
                    break;                             \
                }                                      \
                _node++;                               \
                printf(", ");                          \
            }                                          \
        }                                              \
        printf("]\n");                                 \
    } while (0)

// === End of Macros ====

#endif