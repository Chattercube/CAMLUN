#ifndef TREESET_TESTTOOLS_H
#define TREESET_TESTTOOLS_H

#include <stdio.h>
#include "treeset.h"

// ==== Method Overview ====

// Private :
bool treeset_is_black_balanced_helper(TreeSetNode *node, int num_black);

// Public :
size_t treeset_height(TreeSet *this);
size_t treeset_width(TreeSet *this);

bool treeset_is_bst(TreeSet *this);
bool treeset_is_234(TreeSet *this);
bool treeset_is_black_balanced(TreeSet *this);

// ==== End of Method Overview ====

#endif