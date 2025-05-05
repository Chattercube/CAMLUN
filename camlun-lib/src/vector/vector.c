
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "typemethods.h"
#include "vector.h"

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

Vector *vector_create(type_methods *data_methods) {
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
        USE_DEL(this->data_methods, this->nodes[i]);
    }
}

void *vector_get(Vector *this, size_t pos) {
    return *(void **)(this->nodes + pos);
}

void *vector_first(Vector *this) {
    if (this->size == 0) return NULL;
    return *(void **)(this->nodes);
}

void *vector_last(Vector *this) {
    if (this->size == 0) return NULL;
    return *(void **)(this->nodes + this->size - 1);
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
        return;
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
    USE_DEL(this->data_methods, (void*)(this->nodes[pos]));
    this->nodes[pos] = USE_DUP(this->data_methods, data);
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
    this->size++;
}

void vector_erase(Vector *this, size_t pos) {
    assert(pos < this->size);
    USE_DEL(this->data_methods, this->nodes[pos]);

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

void vector_pop_back(Vector *this) {
    assert(this->size > 0);
    this->size--;
    USE_DEL(this->data_methods, this->nodes[this->size]);
    this->nodes[this->size] = NULL;
}

void vector_truncate(Vector *this, size_t new_size) {
    for (size_t i = new_size; i < this->size; i++) {
        USE_DEL(this->data_methods, this->nodes[i]);
    }
    this->size = new_size;
}

void vector_expand(Vector *this, size_t new_size) {
    if (new_size > this->capacity) {
        vector_reserve(this, new_size);
    }
    for (size_t i = this->size; i < new_size; i++) {
        this->nodes[i] = USE_CRT(this->data_methods);
    }
    this->size = new_size;
}

void vector_resize(Vector *this, size_t new_size) {
    if(this->size < new_size) {
        vector_expand(this, new_size);
    } else {
        vector_truncate(this, new_size);
    }
}

void vector_clear(Vector *this) {
    for (size_t i = 0; i < this->size; i++) {
        USE_DEL(this->data_methods, this->nodes[i]);
    }
    this->size = 0;
}

Vector *vector_clone(Vector *this, type_methods *new_data_methods) {
    Vector *clone = vector_create(new_data_methods);
    if (clone == NULL) {
        return NULL;
    }
    vector_reserve(clone, this->size);
    for (size_t i = 0; i < this->size; i++) {
        clone->nodes[i] = USE_DUP(this->data_methods, this->nodes[i]);
    }
    clone->size = this->size;
    return clone;
}

Vector vector_slice(Vector *this, size_t offset, size_t count) {
    Vector slice;
    slice.data_methods = this->data_methods;
    slice.nodes = this->nodes + offset;
    slice.size = count;
    slice.capacity = count;
    return slice;
}