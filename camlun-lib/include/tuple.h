#ifndef TUPLE_H
#define TUPLE_H

#include "typemethods.h"

#define TUPLE_INIT(varname, struct_def, struct_name, comparator_code) \
    typedef struct struct_name {                                      \
        struct_def                                                    \
    } struct_name;                                                    \
                                                                      \
    static void *struct_name##_default_constructor() {                \
        return calloc(1, sizeof(struct_name));                        \
    }                                                                 \
                                                                      \
    static void struct_name##_destructor(void *ptr) {                 \
        free(ptr);                                                    \
    }                                                                 \
                                                                      \
    static void *struct_name##_copy_constructor(void *ptr) {          \
        struct_name *copy = malloc(sizeof(struct_name));              \
        memcpy(copy, ptr, sizeof(struct_name));                       \
        return copy;                                                  \
    }                                                                 \
                                                                      \
    static size_t struct_name##_hash_function(void *ptr) {            \
        return numerical_hash_function(sizeof(struct_name), ptr);     \
    }                                                                 \
                                                                      \
    static int struct_name##_comparator(void *first, void *second) {  \
        struct_name *a = (struct_name *)first;                        \
        struct_name *b = (struct_name *)second;                       \
        comparator_code                                               \
    }                                                                 \
                                                                      \
    type_methods varname = {                                          \
        .crt = struct_name##_default_constructor,                     \
        .del = struct_name##_destructor,                              \
        .dup = struct_name##_copy_constructor,                        \
        .cmp = struct_name##_comparator,                              \
        .hash = struct_name##_hash_function};

#endif