#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "treeset.h"
#include "typemethods.h"

// TreeSetNode Methods, should be private

TreeSetNode *treesetnode_create(void *data, color color) {
    TreeSetNode *node = malloc(sizeof(TreeSetNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->color = color;
    return node;
}

bool treesetnode_red(TreeSetNode *node) {
    return node == NULL ? false : node->color == RED;
}

void treesetnode_color_flip(TreeSetNode *node) {
    node->color ^= 1;
    node->left->color ^= 1;
    node->right->color ^= 1;
}

TreeSetNode *treesetnode_rotate(TreeSetNode *node, direction dir) {
    TreeSetNode *temp = node->child[!dir];
    node->child[!dir] = temp->child[dir];
    temp->child[dir] = node;

    temp->color = node->color;
    node->color = RED;

    return temp;
}

TreeSetNode *treesetnode_double_rotate(TreeSetNode *node, direction dir) {
    node->child[!dir] = treesetnode_rotate(node->child[!dir], !dir);
    return treesetnode_rotate(node, dir);
}

TreeSetNode *treesetnode_insert_fix_up(TreeSetNode *node, direction dir) {
    if(treesetnode_red(node->child[dir])) {
        if(treesetnode_red(node->child[!dir])) {
            if(treesetnode_red(node->child[dir]->child[dir]) || treesetnode_red(node->child[dir]->child[!dir])) {
                treesetnode_color_flip(node);
            }
        } else {
            if(treesetnode_red(node->child[dir]->child[dir])) {
                node = treesetnode_rotate(node, !dir);
            } else if(treesetnode_red(node->child[dir]->child[!dir])) {
                node = treesetnode_double_rotate(node, !dir);
            }
        }
    }
    return node;
}

TreeSetNode *treeset_insert_helper(TreeSet *set, TreeSetNode *node, void *data) {
    if (node == NULL) {
        return treesetnode_create(data, RED);
    }

    bool dir = set->data_methods->cmp(data, node->data) > 0;
    node->child[dir] = treeset_insert_helper(set, node->child[dir], data);
    node = treesetnode_insert_fix_up(node, dir);

}

TreeSetNode *treesetnode_delete_fix_up(TreeSetNode *node, direction dir, bool *balanced) {
    TreeSetNode *parent = node;
    TreeSetNode *sibling = node->child[!dir];

    if(treesetnode_red(sibling)) {
        node = treesetnode_rotate(node, dir);
        sibling = parent->child[!dir];
    }

    if(sibling != NULL) {
        if(!treesetnode_red(sibling->child[LEFT]) && !treesetnode_red(sibling->child[RIGHT])) {
            if(treesetnode_red(parent)) {
                *balanced = true;
            }
            parent->color = BLACK;
            sibling->color = RED;
        } else {
            color initcol_parent = parent->color;
            bool is_red_sibling_reduction = !(node == parent);

            if(treesetnode_red(sibling->child[!dir])) {
                parent = treesetnode_rotate(parent, dir);
            } else {
                parent = treesetnode_double_rotate(parent, dir);
            }
            parent->color = initcol_parent;
            parent->child[LEFT]->color = BLACK;
            parent->child[RIGHT]->color = BLACK;
            if(is_red_sibling_reduction) {
                node->child[dir] = parent;
            } else {
                node = parent;
            }
            *balanced = true;
        }
    }
    return node;
}

void treesetnode_destroy(TreeSetNode *node) {
    free(node);
}

TreeSetNode *treesetnode_get_max(TreeSetNode *node) {
    while (node->child[RIGHT] != NULL) {
        node = node->child[RIGHT];
    }
    return node;
}

TreeSetNode *treeset_delete_helper(TreeSet *set, TreeSetNode *node, void *data, bool *balanced) {
    if (node == NULL) {
        *balanced = true;
        return NULL;
    }
    if (set->data_methods->cmp(node->data, data) == 0) {
        if (node->child[LEFT] == NULL || node->child[RIGHT] == NULL) {
            TreeSetNode *temp = NULL;
            if(node->child[LEFT]) {
                temp = node->child[LEFT];
            }
            if(node->child[RIGHT]) {
                temp = node->child[RIGHT];
            }

            if(treesetnode_red(node)) {
                *balanced = true;
            } else if (treesetnode_red(temp)) {
                temp->color = BLACK;
                *balanced = true;
            }
            
            #if DEBUG
            fprintf(stderr, "Deleting node %p\n", node->data);
            #endif
            set->data_methods->del(node->data);
            treesetnode_destroy(node);
            return temp;
        } else {
            TreeSetNode *temp = treesetnode_get_max(node->child[LEFT]);
            node->data = set->data_methods->dup(temp->data);
            data = temp->data;
        }
    }
    bool dir = set->data_methods->cmp(data, node->data) > 0;
    node->child[dir] = treeset_delete_helper(set, node->child[dir], data, balanced);
    return *balanced ? node : treesetnode_delete_fix_up(node, dir, balanced);
}

void treeset_insert(TreeSet *set, void *data) {
    void *new_data = set->data_methods->dup(data);
    set->root = treeset_insert_helper(set, set->root, new_data);
    set->root->color = BLACK;
}

void treeset_delete(TreeSet *set, void *data) {
    bool balanced = false;
    set->root = treeset_delete_helper(set, set->root, data, &balanced);
    if(set->root != NULL) {
        set->root->color = BLACK;
    }
}

void treeset_destroy_helper(TreeSet *set, TreeSetNode *node) {
    if(node == NULL) {
        return;
    }
    if(node->child[LEFT] != NULL) {
        treeset_destroy_helper(set, node->child[LEFT]);
    }
    if(node->child[RIGHT] != NULL) {
        treeset_destroy_helper(set, node->child[RIGHT]);
    }
    set->data_methods->del(node->data);
    treesetnode_destroy(node);
    return;
}

// end of private

TreeSet *treeset_create(type_methods data_methods) {
    TreeSet *set = malloc(sizeof(TreeSet));
    if (set == NULL) {
        return NULL;
    }
    set->size = 0;
    set->data_methods = data_methods;
    set->root = NULL;
    return set;
}

void treeset_destroy(TreeSet *this) {
    if (this == NULL) {
        return;
    }
    if(this->root != NULL) {
        treeset_destroy_helper(this, this->root);
    }
    free(this);
    return;
}

void *treeset_find(TreeSet *this, void *data) {
    TreeSetNode *current_node = this->root;
    while (current_node != NULL) {
        switch (this->data_methods->cmp(current_node->data, data)) {
            case 0:
                return current_node->data;
            case 1:
                current_node = current_node->child[RIGHT];
                break;
            default:
                current_node = current_node->child[LEFT];
                break;
        }
    }
    return NULL;
}

void *treeset_minimum(TreeSet *this) {
    if (this->root == NULL) {
        return NULL;
    }
    TreeSetNode *current_node = this->root;
    while (current_node->left != NULL) {
        current_node = current_node->child[LEFT];
    }
    return current_node->data;
}

void *treeset_maximum(TreeSet *this) {
    if (this->root == NULL) {
        return NULL;
    }
    TreeSetNode *current_node = this->root;
    while (current_node->right != NULL) {
        current_node = current_node->child[RIGHT];
    }
    return current_node->data;
}

bool treeset_contains(TreeSet *this, void *data) {
    TreeSetNode *current_node = this->root;
    while (current_node != NULL) {
        switch (this->data_methods->cmp(current_node->data, data)) {
            case 0:
                return true;
            case 1:
                current_node = current_node->child[LEFT];
                break;
            default:
                current_node = current_node->child[RIGHT];
                break;
        }
    }
    return false;
}

inline bool treeset_empty(TreeSet *this) {
    return this->size == 0;
}

inline size_t treeset_size(TreeSet *this) {
    return this->size;
}

void treeset_add(TreeSet *this, void *data) {
    if(treeset_contains(this, data)) return;
    // void *new_data = this->data_methods->dup(data);
    if (data == NULL) {
        return;
    }
    treeset_insert(this, data);
    this->size++;
    return;
}

void treeset_remove(TreeSet *this, void *data) {
    if (this->root == NULL) {
        return;
    }
    //if(!treeset_contains(this, data)) return;
    treeset_delete(this, data);
    this->size--;
    return;
}

void treeset_clear(TreeSet *this) {
    if (this == NULL || this->root == NULL) {
        return;
    }
    treeset_destroy_helper(this, this->root);
    this->size = 0;
    return;
}

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

TreeSet *treeset_clone(TreeSet *this, type_methods new_data_methods) {
    TreeSet *clone = treeset_create(new_data_methods);
    if (clone == NULL) {
        return NULL;
    }
    clone->size = this->size;
    TREESET_FOREACH(this, void *data, {
        treeset_add(clone, data);
    });
    return clone;
}

int treeset_compare(TreeSet *first, TreeSet *second) {
    if(first->size > second->size) {
        return 1;
    } else if(first->size < second->size) {
        return -1;
    } else {
        size_t compare_element_index_counter = 0;
        void *compare_element = NULL;
        TREESET_FOREACH(first, compare_element, {
            if(!treeset_contains(second, compare_element)) {
                goto endforeach;
            }
            compare_element_index_counter++;
        });
        endforeach:
        TREESET_FOREACH(second, void *data, {
            if(compare_element_index_counter == 0) {
                return first->data_methods->cmp(compare_element, data);
            }
            compare_element_index_counter--;
        });
        return 0;
    }
}
