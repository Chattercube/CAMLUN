#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "treeset.h"
#include "typemethods.h"
#include "treeset_testtools.h"

// Helper functions for testing
int int_comparator(void *first, void *second) {
    return (*(int *)first - *(int *)second);
}

void *int_copy_constructor(void *ptr) {
    int *copy = malloc(sizeof(int));
    *copy = *(int *)ptr;
    return copy;
}

void int_destructor(void *ptr) {
    free(ptr);
}

type_methods int_methods = {
    .crt = int_copy_constructor,
    .del = int_destructor,
    .dup = int_copy_constructor,
    .cmp = int_comparator,
    .hash = NULL // Not needed for TreeSet
};

// Function to validate red-black tree properties
bool validate_red_black_tree(TreeSetNode *node, int *black_height) {
    if (node == NULL) {
        *black_height = 1; // Null nodes are considered black
        return true;
    }

    int left_black_height = 0, right_black_height = 0;

    // Validate left and right subtrees
    if (!validate_red_black_tree(node->left, &left_black_height) ||
        !validate_red_black_tree(node->right, &right_black_height)) {
        return false;
    }

    // Check if black heights are equal
    if (left_black_height != right_black_height) {
        return false;
    }

    // Check red-black tree properties
    if (node->color == RED) {
        // Red nodes cannot have red children
        if ((node->left && node->left->color == RED) ||
            (node->right && node->right->color == RED)) {
            return false;
        }
    }

    // Update black height
    *black_height = left_black_height + (node->color == BLACK ? 1 : 0);
    return true;
}

bool validate_treeset(TreeSet *set) {
    int black_height = 0;
    return validate_red_black_tree(set->root, &black_height);
}

// Test functions
void test_treeset_creation() {
    printf("Testing TreeSet creation...\n");
    TreeSet *set = treeset_create(&int_methods);
    if (set) {
        printf("TreeSet created successfully.\n");
    } else {
        printf("Failed to create TreeSet.\n");
    }
    treeset_destroy(set);
}

void test_treeset_insertion_and_validation() {
    printf("Testing TreeSet insertion and validation...\n");
    TreeSet *set = treeset_create(&int_methods);

    int values[] = {10, 20, 30, 15, 25, 5, 1, 8, 12, 18};
    for (int i = 0; i < 10; i++) {
        treeset_add(set, &values[i]);
    }

    if (validate_treeset(set)) {
        printf("TreeSet is valid after insertion.\n");
    } else {
        printf("TreeSet is invalid after insertion.\n");
    }

    treeset_destroy(set);
}

void test_treeset_iteration() {
    printf("Testing TreeSet iteration...\n");
    TreeSet *set = treeset_create(&int_methods);

    int values[] = {10, 20, 30, 15, 25, 5, 1, 8, 12, 18};
    for (int i = 0; i < 10; i++) {
        treeset_add(set, &values[i]);
    }

    printf("Iterating over TreeSet:\n");
    TREESET_FOREACH(set, int *value, {
        printf("%d ", *value);
    });
    printf("\n");

    treeset_destroy(set);
}

void test_treeset_deletion_and_validation() {
    printf("Testing TreeSet deletion and validation...\n");
    TreeSet *set = treeset_create(&int_methods);

    int values[] = {10, 20, 30, 15, 25, 5, 1, 8, 12, 18};
    for (int i = 0; i < 10; i++) {
        treeset_add(set, &values[i]);
    }

    // Delete a few nodes
    treeset_remove(set, &values[3]); // Remove 15
    treeset_remove(set, &values[6]); // Remove 1

    if (validate_treeset(set)) {
        printf("TreeSet is valid after deletion.\n");
    } else {
        printf("TreeSet is invalid after deletion.\n");
    }

    treeset_destroy(set);
}

void test_treeset_printf() {
    printf("Testing TreeSet printf functions...\n");
    TreeSet *set = treeset_create(&int_methods);

    int values1[] = {10, 20, 30, 15, 25, 5, 1, 8, 12, 18};
    for (int i = 0; i < 10; i++) {
        treeset_add(set, &values1[i]);
    }

    int values2[] = {15, 25, 18};
    for (int i = 0; i < 3; i++) {
        treeset_remove(set, &values2[i]);
    }

    

    printf("TreeSet contents using TREESET_PRINTF:\n");
    TREESET_PRINTF(set, int *value, "%d", *value);

    

    if(treeset_is_234(set)) {
        printf("TreeSet is a valid 2-3-4 tree.\n");
    } else {
        printf("TreeSet is not a valid 2-3-4 tree.\n");
    }

    if(treeset_is_black_balanced(set)) {
        printf("TreeSet is black balanced.\n");
    } else {
        printf("TreeSet is not black balanced.\n");
    }

    if(treeset_is_bst(set)) {
        printf("TreeSet is a valid binary search tree.\n");
    } else {
        printf("TreeSet is not a valid binary search tree.\n");
    }

    treeset_destroy(set);
}

int main() {
    test_treeset_creation();
    test_treeset_insertion_and_validation();
    test_treeset_iteration();
    test_treeset_deletion_and_validation();
    test_treeset_printf();
    return 0;
}