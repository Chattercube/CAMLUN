#include <stdio.h>

#include "typemethods.h"
#include "vector.h"

void print_integers(Vector *vector) {
    printf("[");
    if (vector_size(vector) > 0) {
        VectorNode *node = vector_begin(vector);
        while (1) {
            int *data = *node;
            printf("%d", *data);
            if (node == vector_end(vector) - 1) {
                break;
            }
            node++;
            printf(", ");
        }
    }
    printf("]\n");
}

TYPE_INIT(static type_methods TYPE_INT, int);

int main() {
    Vector *vector = vector_create(&TYPE_INT);

    int values[] = {1, 20, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        vector_push_back(vector, &values[i]);
    }

    print_integers(vector);

    vector_erase(vector, 2);

    print_integers(vector);

    vector_destroy(vector);

    do {
        printf("[");
        if (vector_size(vector) > 0) {
            VectorNode *node = vector_begin(vector);
            while (1) {
                int *data = *node;
                printf("%d", *data);
                if (node == vector_end(vector) - 1) {
                    break;
                }
                node++;
                printf(", ");
            }
        }
        printf("]\n");
    } while (0);
}