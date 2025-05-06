#include "vector_ext.h"

#include "vector.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "typemethods.h"
#include <assert.h>

#define HEAP_PARENT(pos) ((pos - 1) / 2)
#define HEAP_LCHILD(pos) (2 * pos + 1)
#define HEAP_RCHILD(pos) (2 * pos + 2)

void vector_heapify(Vector *this, size_t pos, bool is_max) {
    if(is_max) {
        size_t largest = pos;
        size_t left = HEAP_LCHILD(pos);
        size_t right = HEAP_RCHILD(pos);

        if(left < this->size && USE_CMP(this->data_methods, vector_get(this, left), vector_get(this, largest)) > 0) {
            largest = left;
        }

        if(right < this->size && USE_CMP(this->data_methods, vector_get(this, right), vector_get(this, largest)) > 0) {
            largest = right;
        }

        if(largest != pos) {
            vector_swap(this, pos, largest);
            vector_heapify(this, largest, is_max);
        }
    } else {
        size_t smallest = pos;
        size_t left = HEAP_LCHILD(pos);
        size_t right = HEAP_RCHILD(pos);

        if(left < this->size && USE_CMP(this->data_methods, vector_get(this, left), vector_get(this, smallest)) < 0) {
            smallest = left;
        }

        if(right < this->size && USE_CMP(this->data_methods, vector_get(this, right), vector_get(this, smallest)) < 0) {
            smallest = right;
        }

        if(smallest != pos) {
            vector_swap(this, pos, smallest);
            vector_heapify(this, smallest, is_max);
        }
    }
}

void vector_build_heap(Vector *this, bool is_max) {
    for(int i = this->size / 2 - 1; i >= 0; i--) {
        vector_heapify(this, i, is_max);
    }
}

static void heap_sift_up(Vector *this, size_t pos, bool is_max) {
    if(is_max) {
        while(pos > 0 && USE_CMP(this->data_methods, vector_get(this, pos), vector_get(this, HEAP_PARENT(pos))) > 0) {
            vector_swap(this, pos, HEAP_PARENT(pos));
            pos = HEAP_PARENT(pos);
        }
    } else {
        while(pos > 0 && USE_CMP(this->data_methods, vector_get(this, pos), vector_get(this, HEAP_PARENT(pos))) < 0) {
            vector_swap(this, pos, HEAP_PARENT(pos));
            pos = HEAP_PARENT(pos);
        }
    }
}

void heap_update(Vector *this, size_t pos, void *data, bool is_max) {
    assert(pos < this->size);
    vector_set(this, pos, data);
    heap_sift_up(this, pos, is_max);
    vector_heapify(this, pos, is_max);
}

void *heap_poll(Vector *this, bool is_max) {
    if(this->size == 0){
        return NULL;
    }
    VectorNode *first_node = vector_begin(this);
    void *polled_data = *first_node;
    *first_node = NULL;
    vector_swap(this, 0, this->size - 1);
    this->size--;
    vector_heapify(this, 0, is_max);
    return polled_data;
}

void heap_offer(Vector *this, void *data, bool is_max) {
    vector_push_back(this, data);
    heap_sift_up(this, this->size - 1, is_max);
}

static void vector_qsort_helper(Vector *this, size_t start, size_t end, bool desc) {
    if(start >= end) {
        return;
    }

    size_t pivot = start + (end - start) / 2;
    void *pivot_value = vector_get(this, pivot);
    size_t left = start;
    size_t right = end;

    while(left <= right) {
        if(desc) {
            while(left < end && USE_CMP(this->data_methods, vector_get(this, left), pivot_value) > 0) {
                left++;
            }
            while(right > start && USE_CMP(this->data_methods, vector_get(this, right), pivot_value) < 0) {
                right--;
            }
        } else {
            while(left < end && USE_CMP(this->data_methods, vector_get(this, left), pivot_value) < 0) {
                left++;
            }
            while(right > start && USE_CMP(this->data_methods, vector_get(this, right), pivot_value) > 0) {
                right--;
            }
        }
    }

    if(start < right) {
        vector_qsort_helper(this, start, right, desc);
    }
    if(left < end) {
        vector_qsort_helper(this, left, end, desc);
    }
}

void vector_qsort(Vector *this, bool desc) {
    if (this->size < 2) {
        return;
    }
}