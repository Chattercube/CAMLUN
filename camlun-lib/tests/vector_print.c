#include <stdio.h>
#include "vector.h"
#include "typemethods.h"

void print_integers(Vector *vector) {
    printf("[ ");
    if(vector_size(vector) > 0) {
        VectorNode *node = vector_begin(vector);
        while(1) {
            int *data = *node;
            printf("%d", *data);
            if (node == vector_end(vector) - 1) {
                break;
            }
            printf(", ");
        }
    }
    printf("]\n");
}

static const TYPE_INT = TYPE_INI

int main() {

    return 0;
}