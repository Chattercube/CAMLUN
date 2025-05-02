#include "typemethods.h"
#include <stdio.h>

const static type_methods TYPE_DOUBLE = TYPE_INIT(double);
const static type_methods TYPE_STRING = TYPE_INIT(string);

int main() {
    double my_double[] = {1.0, 2.0, 3.0, 923.21, 2};
    char *my_str[] = {"hello", "world", "worl2d", "Merry", "hello"};

    // Get their hash values
    for(size_t i = 0; i < 5; i++){
        printf("%08lX\n", TYPE_STRING.hash(my_str[i]));
    }

    for(size_t i = 0; i < 5; i++){
        printf("%08lX\n", TYPE_DOUBLE.hash(&my_double[i]));
    }
}