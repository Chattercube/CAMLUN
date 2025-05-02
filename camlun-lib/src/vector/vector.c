
#include "vector.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "typemethods.h"

// Private methods

size_t get_new_capacity(size_t target_capacity) {
    size_t new_capacity = VECTOR_INITIAL_CAPACITY;
    while (new_capacity < target_capacity)
        new_capacity *= VECTOR_GROWTH_FACTOR;
    return new_capacity;
}

void vector_node_init(Vector *vector, VectorNode *node, void *data) {
    if (node == NULL) {
        return;
    }
    *node = vector->data_methods->dup(data);
    if (*node == NULL) {
        return;
    }
}

// End of private methods

Vector *vector_create(type_methods data_methods) {
    Vector *vector = malloc(sizeof(Vector));
    if (vector == NULL) {
        return NULL;
    }
    vector->size = 0;
    vector->data_methods = data_methods;

    vector->capacity = VECTOR_INITIAL_CAPACITY;
    vector->nodes = calloc(vector->capacity, sizeof(VectorNode));
    if (vector->nodes == NULL) {
        free(vector);
        return NULL;
    }
    return vector;
}

void vector_destroy(Vector *this) {
    for (size_t i = 0; i < this->size; i++) {
        this->data_methods->del(this->nodes[i]);
    }
}

void *vector_get(Vector *this, size_t pos) {
    return this->data_methods->dup(*(void *)vector_at(this, pos));
}

void *vector_first(Vector *this) {
    if (vector_empty(this)) return NULL;
    return this->data_methods->dup(*(void *)vector_begin(this));
}

void *vector_last(Vector *this) {
    if (vector_empty(this)) return NULL;
    return this->data_methods->dup(*(void *)(vector_end(this) - 1));
}

VectorNode *vector_at(Vector *this, size_t pos) {
    assert(pos < this->size);
    return this->nodes + pos;
}

VectorNode *vector_begin(Vector *this) {
    return this->nodes;
}

VectorNode *vector_end(Vector *this) {
    return this->nodes + this->size;
}

void vector_increment(VectorNode **this) {
    assert(*this != NULL);
    *this += 1;
}

size_t vector_pos(Vector *this, VectorNode *node) {
    assert(node >= this->nodes && node <= this->nodes + this->size);
    return (size_t)(node - this->nodes);
}

bool vector_empty(Vector *this) {
    return this->size == 0;
}

size_t vector_size(Vector *this) {
    return this->size;
}

size_t vector_capacity(Vector *this) {
    return this->capacity;
}

void vector_reserve(Vector *this, size_t new_capacity) {
    new_capacity = get_new_capacity(new_capacity);

    if (new_capacity <= this->capacity) {
        return this;
    }

    VectorNode *new_nodes = realloc(this->nodes, new_capacity * sizeof(VectorNode));
    if (new_nodes == NULL) {
        return;
    }

    this->nodes = new_nodes;
    this->capacity = new_capacity;
    return;
}

void vector_compact(Vector *this) {
    size_t new_capacity = get_new_capacity(this->size);

    VectorNode *new_nodes = realloc(this->nodes, new_capacity * sizeof(VectorNode));
    if (new_nodes == NULL) {
        return;
    }

    this->nodes = new_nodes;
    this->capacity = new_capacity;
    return;
}

// Note : vector_set assumes there is existing data at pos, use vector_node_init to initialize new nodes
void vector_set(Vector *this, size_t pos, void *data) {
    assert(pos < this->size);
    this->data_methods->del((void*)(this->nodes[pos]));
    this->nodes[pos] = this->data_methods->dup(data);
    return;
}

void vector_insert(Vector *this, size_t pos, void *data) {
    assert(pos <= this->size);
    if (this->size >= this->capacity) {
        vector_reserve(this, this->size + 1);
    }

    for (size_t i = this->size; i > pos; i--) {
        this->nodes[i] = this->nodes[i - 1];
    }

    this->nodes[pos] = NULL;
    vector_node_init(this, this->nodes + pos, data);
}

void vector_erase(Vector *this, size_t pos) {
    assert(pos < this->size);
    this->data_methods->del(this->nodes[pos]);

    for (size_t i = pos; i < this->size - 1; i++) {
        this->nodes[i] = this->nodes[i + 1];
    }

    this->nodes[this->size - 1] = NULL;
    this->size--;
}

// Return the iterator for continue to iterate
VectorNode *vector_insert_at(Vector *this, VectorNode *node, void *data) {
    assert(node >= this->nodes && node <= this->nodes + this->size);
    size_t pos = vector_pos(this, node);
    vector_insert(this, pos, data);
    return this->nodes + pos;
}

VectorNode *vector_erase_at(Vector *this, VectorNode *node) {
    assert(node >= this->nodes && node < this->nodes + this->size);
    size_t pos = vector_pos(this, node);
    vector_erase(this, pos);
    return this->nodes + pos;
}

void vector_replace(Vector *this, VectorNode *node, void *data) {
    assert(node >= this->nodes && node < this->nodes + this->size);
    size_t pos = vector_pos(this, node);
    vector_set(this, pos, data);
    return;
}

void vector_push_back(Vector *this, void *data) {
    if (this->size >= this->capacity) {
        vector_reserve(this, this->size + 1);
    }
    vector_node_init(this, this->nodes + this->size, data);
    this->size++;
}

void vector_pop_back(Vector *this, void *data) {
    assert(this->size > 0);
    this->size--;
    this->data_methods->del(this->nodes[this->size]);
    this->nodes[this->size] = NULL;
}
