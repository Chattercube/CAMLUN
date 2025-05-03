#include "treeset_ext.h"

// this = this ∪ another
void treeset_add_set(TreeSet *this, TreeSet *another) {

    TREESET_FOREACH(another, void *data, {
        treeset_add(this, data);
    });
    return;
}


// this = this \ another
void treeset_remove_set(TreeSet *this, TreeSet *another) {

    TREESET_FOREACH(another, void *data, {
        treeset_remove(this, data);
    });
    return;
}

TreeSet *treeset_union(TreeSet *first, TreeSet *second) {
    TreeSet *new_set = treeset_create(first->data_methods);
    if (new_set == NULL) {
        return NULL;
    }
    TREESET_FOREACH(first, void *data, {
        treeset_add(new_set, data);
    });
    TREESET_FOREACH(second, void *data, {
        treeset_add(new_set, data);
    });
    return new_set;
};

TreeSet *treeset_intersection(TreeSet *first, TreeSet *second) {
    TreeSet *new_set = treeset_create(first->data_methods);
    if (new_set == NULL) {
        return NULL;
    }
    TreeSet *smaller_set = first->size < second->size ? first : second;
    TreeSet *larger_set = first->size < second->size ? second : first;
    TREESET_FOREACH(smaller_set, void *data, {
        if(treeset_contains(larger_set, data)) {
            treeset_add(new_set, data);
        }
    });
    return new_set;
};

TreeSet *treeset_complement(TreeSet *first, TreeSet *second) {
    TreeSet *new_set = treeset_create(first->data_methods);
    if (new_set == NULL) {
        return NULL;
    }
    TREESET_FOREACH(first, void *data, {
        if(!treeset_contains(second, data)) {
            treeset_add(new_set, data);
        }
    });
    return new_set;
};

// true if another ⊆ this
bool treeset_subset(TreeSet *this, TreeSet *another) {
    if (this->size > another->size) {
        return false;
    }
    TREESET_FOREACH(another, void *data, {
        if(!treeset_contains(this, data)) {
            return false;
        }
    });
    return true;
}

inline bool treeset_equals(TreeSet *this, TreeSet *another) {
    return this->size == another->size && treeset_subset(this, another);
}