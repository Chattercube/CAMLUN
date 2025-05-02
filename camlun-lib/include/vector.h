#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "typemethods.h"

// Macro constants
#define VECTOR_INITIAL_CAPACITY 16
#define VECTOR_GROWTH_FACTOR 2

// Type definitions
typedef void *VectorNode;

typedef struct {
    VectorNode *nodes;          // Array of nodes
    size_t size;                // Current number of elements
    size_t capacity;            // Current capacity of the vector
    type_methods *data_methods; // Methods for managing the data type
} Vector;

// Public API

/**
 * @brief Creates a new vector.
 * 
 * @param data_methods Methods for managing the data type.
 * @return Pointer to the newly created vector, or NULL on failure.
 */
Vector *vector_create(type_methods data_methods);

/**
 * @brief Destroys the vector and frees all associated memory.
 * 
 * @param this Pointer to the vector to destroy.
 */
void vector_destroy(Vector *this);

/**
 * @brief Retrieves a copy of the element at the specified position.
 * 
 * @param this Pointer to the vector.
 * @param pos Position of the element to retrieve.
 * @return Pointer to the copied element, or NULL on failure.
 */
void *vector_get(Vector *this, size_t pos);

/**
 * @brief Retrieves a copy of the first element in the vector.
 * 
 * @param this Pointer to the vector.
 * @return Pointer to the copied first element, or NULL if the vector is empty.
 */
void *vector_first(Vector *this);

/**
 * @brief Retrieves a copy of the last element in the vector.
 * 
 * @param this Pointer to the vector.
 * @return Pointer to the copied last element, or NULL if the vector is empty.
 */
void *vector_last(Vector *this);

/**
 * @brief Retrieves a pointer to the element at the specified position.
 * 
 * @param this Pointer to the vector.
 * @param pos Position of the element to retrieve.
 * @return Pointer to the element at the specified position.
 */
VectorNode *vector_at(Vector *this, size_t pos);

/**
 * @brief Retrieves a pointer to the first element in the vector.
 * 
 * @param this Pointer to the vector.
 * @return Pointer to the first element in the vector.
 */
VectorNode *vector_begin(Vector *this);

/**
 * @brief Retrieves a pointer to the end of the vector (one past the last element).
 * 
 * @param this Pointer to the vector.
 * @return Pointer to the end of the vector.
 */
VectorNode *vector_end(Vector *this);

/**
 * @brief Increments the iterator to the next element.
 * 
 * @param this Pointer to the iterator to increment.
 */
void vector_increment(VectorNode **this);

/**
 * @brief Retrieves the position of a node in the vector.
 * 
 * @param this Pointer to the vector.
 * @param node Pointer to the node.
 * @return Position of the node in the vector.
 */
size_t vector_pos(Vector *this, VectorNode *node);

/**
 * @brief Checks if the vector is empty.
 * 
 * @param this Pointer to the vector.
 * @return true if the vector is empty, false otherwise.
 */
bool vector_empty(Vector *this);

/**
 * @brief Retrieves the size of the vector.
 * 
 * @param this Pointer to the vector.
 * @return The number of elements in the vector.
 */
size_t vector_size(Vector *this);

/**
 * @brief Retrieves the capacity of the vector.
 * 
 * @param this Pointer to the vector.
 * @return The capacity of the vector.
 */
size_t vector_capacity(Vector *this);

/**
 * @brief Reserves memory for at least the specified capacity.
 * 
 * @param this Pointer to the vector.
 * @param new_capacity The new capacity to reserve.
 */
void vector_reserve(Vector *this, size_t new_capacity);

/**
 * @brief Reduces the capacity of the vector to fit its size.
 * 
 * @param this Pointer to the vector.
 */
void vector_compact(Vector *this);

/**
 * @brief Sets the value of an existing element at the specified position.
 * 
 * @param this Pointer to the vector.
 * @param pos Position of the element to set.
 * @param data Pointer to the new data.
 */
void vector_set(Vector *this, size_t pos, void *data);

/**
 * @brief Inserts a new element at the specified position.
 * 
 * @param this Pointer to the vector.
 * @param pos Position to insert the new element.
 * @param data Pointer to the data to insert.
 */
void vector_insert(Vector *this, size_t pos, void *data);

/**
 * @brief Erases the element at the specified position.
 * 
 * @param this Pointer to the vector.
 * @param pos Position of the element to erase.
 */
void vector_erase(Vector *this, size_t pos);

/**
 * @brief Inserts a new element at the specified iterator position.
 * 
 * @param this Pointer to the vector.
 * @param node Iterator pointing to the position to insert the new element.
 * @param data Pointer to the data to insert.
 * @return Iterator pointing to the inserted element.
 */
VectorNode *vector_insert_at(Vector *this, VectorNode *node, void *data);

/**
 * @brief Erases the element at the specified iterator position.
 * 
 * @param this Pointer to the vector.
 * @param node Iterator pointing to the position to erase.
 * @return Iterator pointing to the next element after the erased one.
 */
VectorNode *vector_erase_at(Vector *this, VectorNode *node);

/**
 * @brief Replaces the value of an element at the specified iterator position.
 * 
 * @param this Pointer to the vector.
 * @param node Iterator pointing to the element to replace.
 * @param data Pointer to the new data.
 */
void vector_replace(Vector *this, VectorNode *node, void *data);

/**
 * @brief Appends a new element to the end of the vector.
 * 
 * @param this Pointer to the vector.
 * @param data Pointer to the data to append.
 */
void vector_push_back(Vector *this, void *data);

/**
 * @brief Removes the last element from the vector.
 * 
 * @param this Pointer to the vector.
 */
void vector_pop_back(Vector *this);