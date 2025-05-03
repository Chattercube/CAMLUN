#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "treeset_testtools.h"

bool treesetnode_red(TreeSetNode *node) {
    return node == NULL ? false : node->color == RED;
}

bool treeset_is_black_balanced_helper(TreeSetNode *node, int num_black) {
    if(node == NULL) {
        return num_black == 0;
    }
    if(node->color != RED) {
        num_black--;
    }
    return treeset_is_black_balanced_helper(node->left, num_black) && treeset_is_black_balanced_helper(node->right, num_black);
}

bool treeset_is_black_balanced(TreeSet *this) {
    int num_black = 0;
    TreeSetNode *node = this->root;
    while(node->left) {
        if (node->color != RED) {
            num_black++;
        }
        node = node->left;
    }
    return treeset_is_black_balanced_helper(this->root, num_black);
}

bool treeset_is_bst_helper(TreeSet *this, TreeSetNode *node) {
    if (node == NULL) {
        return true;
    }
    if (node->left && this->data_methods->cmp(node->data, node->left->data) < 0) {
        return false;
    }
    if (node->right && this->data_methods->cmp(node->data, node->right->data) > 0) {
        return false;
    }
    return treeset_is_bst_helper(this, node->left) && treeset_is_bst_helper(this, node->right);
}

bool treeset_is_bst(TreeSet *this) {
    return treeset_is_bst_helper(this, this->root);
}

bool treeset_is_234_helper(TreeSetNode *node) {
    if (node == NULL) {
        return true;
    }

    if (node->color == RED) {
        if (treesetnode_red(node->left) || treesetnode_red(node->right)) {
            return false;
        }
    }
    return treeset_is_234_helper(node->left) && treeset_is_234_helper(node->right);
}

bool treeset_is_234(TreeSet *this) {
    return treeset_is_234_helper(this->root);
}