#ifndef TREESET_H
#define TREESET_H

// ==== Includes ====

#include <stdbool.h>
#include <stddef.h>

#include "typemethods.h"

// ==== End of Includes ====

// ==== Constants ====

typedef enum { RED = 0,
               BLACK = 1 } color;
typedef enum { LEFT = 0,
               RIGHT = 1 } direction;

// ==== End of Constants ====

// ==== Type Definitions ====

typedef struct TreeSetNode {
    void *data;
    union {
        struct {
            struct TreeSetNode *left;
            struct TreeSetNode *right;
        };
        struct TreeSetNode *child[2];
    };
    color color;
} TreeSetNode;

typedef struct TreeSet {
    TreeSetNode *root;
    size_t size;
    type_methods *data_methods;
} TreeSet;

// ==== End of Type Definitions ====

// ==== Method Overview ====

// Private methods :

// TreeSetNode *treesetnode_create(void *data, color color);
// void treesetnode_destroy(TreeSetNode *node);
// TreeSetNode *treesetnode_get_max(TreeSetNode *node);
// TreeSetNode *treesetnode_delete_fix_up(TreeSetNode *node, direction dir, bool *balanced);
// TreeSetNode *treesetnode_insert_fix_up(TreeSetNode *node, direction dir);
// TreeSetNode *treesetnode_rotate(TreeSetNode *node, direction dir);
// TreeSetNode *treesetnode_double_rotate(TreeSetNode *node, direction dir);
// bool treesetnode_red(TreeSetNode *node);
// void treesetnode_color_flip(TreeSetNode *node);
// TreeSetNode *treeset_delete_helper(TreeSet *set, TreeSetNode *node, void *data, bool *balanced);
// TreeSetNode *treeset_insert_helper(TreeSet *set, TreeSetNode *node, void *data);
// void treeset_destroy_helper(TreeSet *set, TreeSetNode *node);
// void treeset_insert(TreeSet *set, void *data);
// void treeset_delete(TreeSet *set, void *data);

// Constructors and destructors :

TreeSet *treeset_create(type_methods *data_methods);
void treeset_destroy(TreeSet *this);

// Access and iteration :

void *treeset_minimum(TreeSet *this);
void *treeset_maximum(TreeSet *this);

bool treeset_contains(TreeSet *this, void *data);

void *treeset_get_key(TreeSet *this, void *data);
// TreeSetNode *treeset_find(TreeSet *this, void *data);

// Size and capacity :

bool treeset_empty(TreeSet *this);
size_t treeset_size(TreeSet *this);

// Modifiers :

void treeset_add(TreeSet *this, void *data);
void treeset_remove(TreeSet *this, void *data);
void treeset_clear(TreeSet *this);

// Copy constructors or creators :

TreeSet *treeset_clone(TreeSet *this, type_methods *new_data_methods);

// === End of Method Overview ====

// ==== Macros ====

#define TREESET_FOREACH(set, varname, callback)                         \
    do {                                                                \
        TreeSetNode *_curr = (set)->root;                               \
        while (_curr != NULL) {                                         \
            if (_curr->left == NULL) {                                  \
                varname = _curr->data;                                  \
                callback;                                               \
                _curr = _curr->right;                                   \
            } else {                                                    \
                TreeSetNode *_prev = _curr->left;                       \
                while (_prev->right != NULL && _prev->right != _curr) { \
                    _prev = _prev->right;                               \
                }                                                       \
                if (_prev->right == NULL) {                             \
                    _prev->right = _curr;                               \
                    _curr = _curr->left;                                \
                } else {                                                \
                    _prev->right = NULL;                                \
                    varname = _curr->data;                              \
                    callback;                                           \
                    _curr = _curr->right;                               \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (0)

#define TREESET_PRINTF(set, varname, ...)       \
    do {                                        \
        printf("{");                            \
        size_t _counter = 0;                    \
        TREESET_FOREACH(set, varname, {         \
            _counter++;                         \
            printf(__VA_ARGS__);                \
            if (_counter < treeset_size(set)) { \
                printf(", ");                   \
            }                                   \
        });                                     \
        printf("}");                            \
    } while (0)

#define TREESET_FPRINTF(stream, set, varname, ...) \
    do {                                           \
        fprintf(stream, "{");                      \
        size_t _counter = 0;                       \
        TREESET_FOREACH(set, varname, {            \
            _counter++;                            \
            fprintf(stream, __VA_ARGS__);          \
            if (_counter < treeset_size(set)) {    \
                fprintf(stream, ", ");             \
            }                                      \
        });                                        \
        fprintf(stream, "}");                      \
    } while (0)

#define TREESET_SPRINTF(buffer, set, varname, ...)         \
    do {                                                   \
        sprintf(buffer, "{");                              \
        size_t _counter = 0;                               \
        TREESET_FOREACH(set, varname, {                    \
            _counter++;                                    \
            sprintf(buffer + strlen(buffer), __VA_ARGS__); \
            if (_counter < treeset_size(set)) {            \
                sprintf(buffer + strlen(buffer), ", ");    \
            }                                              \
        });                                                \
        sprintf(buffer + strlen(buffer), "}");             \
    } while (0)
// === End of Macros ====

#endif
