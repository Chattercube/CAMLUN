#ifndef TYPEMETHODS_H
#define TYPEMETHODS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Default constructor function pointer type.
 */
typedef void *(*default_constructor)(void);

/**
 * Destructor
 * Note : Must be able to handle NULL pointers.
 */
typedef void (*destructor)(void *);

/**
 * Copy constructor function pointer type.
 */
typedef void *(*copy_constructor)(void *);

/**
 * Comparator function pointer type.
 * @return 0 if equal, < 0 if first < second, > 0 if first > second
 */
typedef int (*comparator)(void *, void *);

/**
 * Hash function pointer type.
 */
typedef size_t (*hash_function)(void *);

static const uint32_t HASH_SEED = 0x12345678;

/**
 * @brief Type methods structure.
 * Contains function pointers for managing a specific type.
 */
typedef struct {
    default_constructor crt;
    destructor del;
    copy_constructor dup;
    comparator cmp;
    hash_function hash;
} type_methods;

size_t numerical_hash_function(size_t type_size, void *ptr);

/**
 * Constructors
 */

void *bool_default_constructor();
void *char_default_constructor();
void *signed_char_default_constructor();
void *unsigned_char_default_constructor();
void *short_default_constructor();
void *unsigned_short_default_constructor();
void *int_default_constructor();
void *unsigned_int_default_constructor();
void *long_default_constructor();
void *unsigned_long_default_constructor();
void *long_long_default_constructor();
void *unsigned_long_long_default_constructor();
void *float_default_constructor();
void *double_default_constructor();
void *long_double_default_constructor();

void *int8_t_default_constructor();
void *int16_t_default_constructor();
void *int32_t_default_constructor();
void *int64_t_default_constructor();
void *intmax_t_default_constructor();
void *intptr_t_default_constructor();
void *uint8_t_default_constructor();
void *uint16_t_default_constructor();
void *uint32_t_default_constructor();
void *uint64_t_default_constructor();
void *uintmax_t_default_constructor();
void *uintptr_t_default_constructor();

void *size_t_default_constructor();
void *ptrdiff_t_default_constructor();

void *string_default_constructor();
void *shallow_default_constructor();

/**
 * Destructors
 */

void bool_destructor(void *ptr);
void char_destructor(void *ptr);
void signed_char_destructor(void *ptr);
void unsigned_char_destructor(void *ptr);
void short_destructor(void *ptr);
void unsigned_short_destructor(void *ptr);
void int_destructor(void *ptr);
void unsigned_int_destructor(void *ptr);
void long_destructor(void *ptr);
void unsigned_long_destructor(void *ptr);
void long_long_destructor(void *ptr);
void unsigned_long_long_destructor(void *ptr);
void float_destructor(void *ptr);
void double_destructor(void *ptr);
void long_double_destructor(void *ptr);

void int8_t_destructor(void *ptr);
void int16_t_destructor(void *ptr);
void int32_t_destructor(void *ptr);
void int64_t_destructor(void *ptr);
void intmax_t_destructor(void *ptr);
void intptr_t_destructor(void *ptr);
void uint8_t_destructor(void *ptr);
void uint16_t_destructor(void *ptr);
void uint32_t_destructor(void *ptr);
void uint64_t_destructor(void *ptr);
void uintmax_t_destructor(void *ptr);
void uintptr_t_destructor(void *ptr);

void size_t_destructor(void *ptr);
void ptrdiff_t_destructor(void *ptr);

void string_destructor(void *ptr);
void shallow_destructor(void *ptr);

/**
 * Copy Constructors
 */

void *bool_copy_constructor(void *ptr);
void *char_copy_constructor(void *ptr);
void *signed_char_copy_constructor(void *ptr);
void *unsigned_char_copy_constructor(void *ptr);
void *short_copy_constructor(void *ptr);
void *unsigned_short_copy_constructor(void *ptr);
void *int_copy_constructor(void *ptr);
void *unsigned_int_copy_constructor(void *ptr);
void *long_copy_constructor(void *ptr);
void *unsigned_long_copy_constructor(void *ptr);
void *long_long_copy_constructor(void *ptr);
void *unsigned_long_long_copy_constructor(void *ptr);
void *float_copy_constructor(void *ptr);
void *double_copy_constructor(void *ptr);
void *long_double_copy_constructor(void *ptr);

void *int8_t_copy_constructor(void *ptr);
void *int16_t_copy_constructor(void *ptr);
void *int32_t_copy_constructor(void *ptr);
void *int64_t_copy_constructor(void *ptr);
void *intmax_t_copy_constructor(void *ptr);
void *intptr_t_copy_constructor(void *ptr);
void *uint8_t_copy_constructor(void *ptr);
void *uint16_t_copy_constructor(void *ptr);
void *uint32_t_copy_constructor(void *ptr);
void *uint64_t_copy_constructor(void *ptr);
void *uintmax_t_copy_constructor(void *ptr);
void *uintptr_t_copy_constructor(void *ptr);

void *size_t_copy_constructor(void *ptr);
void *ptrdiff_t_copy_constructor(void *ptr);

void *string_copy_constructor(void *ptr);
void *shallow_copy_constructor(void *ptr);

/**
 * Comparators
 */

int bool_comparator(void *first, void *second);
int char_comparator(void *first, void *second);
int signed_char_comparator(void *first, void *second);
int unsigned_char_comparator(void *first, void *second);
int short_comparator(void *first, void *second);
int unsigned_short_comparator(void *first, void *second);
int int_comparator(void *first, void *second);
int unsigned_int_comparator(void *first, void *second);
int long_comparator(void *first, void *second);
int unsigned_long_comparator(void *first, void *second);
int long_long_comparator(void *first, void *second);
int unsigned_long_long_comparator(void *first, void *second);
int float_comparator(void *first, void *second);
int double_comparator(void *first, void *second);
int long_double_comparator(void *first, void *second);

int int8_t_comparator(void *first, void *second);
int int16_t_comparator(void *first, void *second);
int int32_t_comparator(void *first, void *second);
int int64_t_comparator(void *first, void *second);
int intmax_t_comparator(void *first, void *second);
int intptr_t_comparator(void *first, void *second);
int uint8_t_comparator(void *first, void *second);
int uint16_t_comparator(void *first, void *second);
int uint32_t_comparator(void *first, void *second);
int uint64_t_comparator(void *first, void *second);
int uintmax_t_comparator(void *first, void *second);
int uintptr_t_comparator(void *first, void *second);

int size_t_comparator(void *first, void *second);
int ptrdiff_t_comparator(void *first, void *second);

int string_comparator(void *first, void *second);
int shallow_comparator(void *first, void *second);

/**
 * Hash Functions
 */

size_t bool_hash_function(void *ptr);
size_t char_hash_function(void *ptr);
size_t signed_char_hash_function(void *ptr);
size_t unsigned_char_hash_function(void *ptr);
size_t short_hash_function(void *ptr);
size_t unsigned_short_hash_function(void *ptr);
size_t int_hash_function(void *ptr);
size_t unsigned_int_hash_function(void *ptr);
size_t long_hash_function(void *ptr);
size_t unsigned_long_hash_function(void *ptr);
size_t long_long_hash_function(void *ptr);
size_t unsigned_long_long_hash_function(void *ptr);
size_t float_hash_function(void *ptr);
size_t double_hash_function(void *ptr);
size_t long_double_hash_function(void *ptr);

size_t int8_t_hash_function(void *ptr);
size_t int16_t_hash_function(void *ptr);
size_t int32_t_hash_function(void *ptr);
size_t int64_t_hash_function(void *ptr);
size_t intmax_t_hash_function(void *ptr);
size_t intptr_t_hash_function(void *ptr);
size_t uint8_t_hash_function(void *ptr);
size_t uint16_t_hash_function(void *ptr);
size_t uint32_t_hash_function(void *ptr);
size_t uint64_t_hash_function(void *ptr);
size_t uintmax_t_hash_function(void *ptr);
size_t uintptr_t_hash_function(void *ptr);

size_t size_t_hash_function(void *ptr);
size_t ptrdiff_t_hash_function(void *ptr);

size_t string_hash_function(void *ptr);
size_t shallow_hash_function(void *ptr);

/**
 * @brief Macro to initialize a type_methods structure for a given type.
 */
#define TYPE_INIT(varname, type)           \
    varname = (type_methods) {             \
        .crt = type##_default_constructor, \
        .del = type##_destructor,          \
        .dup = type##_copy_constructor,    \
        .cmp = type##_comparator,          \
        .hash = type##_hash_function       \
    }

#define TYPE_METHODS(type)                 \
    (type_methods) {                       \
        .crt = type##_default_constructor, \
        .del = type##_destructor,          \
        .dup = type##_copy_constructor,    \
        .cmp = type##_comparator,          \
        .hash = type##_hash_function       \
    }

#define TYPE_INIT_CMP_OVERRIDE(varname, base_type, custom_id, comparison_code)     \
    static int _##base_type##_##custom_id##_comparator(void *first, void *second){ \
        comparison_code}                                                           \
                                                                                   \
    varname = {                                                                    \
        .crt = base_type##_default_constructor,                                    \
        .del = base_type##_destructor,                                             \
        .dup = base_type##_copy_constructor,                                       \
        .cmp = _##base_type##_##custom_id##_comparator,                            \
        .hash = base_type##_hash_function};

#define BOXED_ARR(type, ...) \
    (type[]) { __VA_ARGS__ }
#define BOXED(data) (&(__typeof__(data)){data})
#define UNBOX(type, ptr) (*(type *)(ptr))
#define RAW(data) ((union { __typeof__(data) _raw; void* p; }){._raw = (data)}).p
#define LIT(type, ptr) (((union { type _raw; void* p; }){.p = (ptr)})._raw)

// vector_insert(my_vec, BOXED(int, 23), 2)
// int my_var = UNBOX(int, vector_get(my_vec, 2))

// Fallback to
#define USE_CRT(type_methods_ptr) \
    ((type_methods_ptr) && (type_methods_ptr)->crt ? (type_methods_ptr)->crt() : NULL)

// Use the destructor or fallback to shallow_destructor
#define USE_DEL(type_methods_ptr, ptr)                       \
    do {                                                     \
        if ((type_methods_ptr) && (type_methods_ptr)->del) { \
            (type_methods_ptr)->del(ptr);                    \
        }                                                    \
    } while (0)

// Use the copy constructor or fallback to shallow_copy_constructor
#define USE_DUP(type_methods_ptr, ptr) \
    ((type_methods_ptr) && (type_methods_ptr)->dup ? (type_methods_ptr)->dup(ptr) : ptr)

// Use the comparator or fallback to shallow_comparator
#define USE_CMP(type_methods_ptr, first, second) \
    ((type_methods_ptr) && (type_methods_ptr)->cmp ? (type_methods_ptr)->cmp(first, second) : (first > second) - (first < second))

// Use the hash function or fallback to shallow_hash_function
#define USE_HASH(type_methods_ptr, ptr) \
    ((type_methods_ptr) && (type_methods_ptr)->hash ? (type_methods_ptr)->hash(ptr) : numerical_hash_function(sizeof(void *), ptr))

#define DEFINE_DEFAULT_CONSTRUCTOR(type_name, type, code) \
    static void *type_name##_default_constructor(void) {  \
        do {                                              \
            code;                                         \
        } while (0);                                      \
        return calloc(1, sizeof(type));                   \
    }

#define DEFINE_DESTRUCTOR(type_name, type, code)    \
    static void type_name##_destructor(void *ptr) { \
        do {                                        \
            code;                                   \
        } while (0);                                \
        if (ptr) free(ptr);                          \
    }

#define DEFINE_COPY_CONSTRUCTOR(type_name, type, code)             \
    static void *type_name##_copy_constructor(void *ptr) {         \
        do {                                                       \
            code;                                                  \
        } while (0);                                               \
        return memcpy(calloc(1, sizeof(type)), ptr, sizeof(type)); \
    }

#define DEFINE_COMPARATOR(type_name, type, code)                   \
    static int type_name##_comparator(void *first, void *second) { \
        type *_first = (type *)first;                              \
        type *_second = (type *)second;                            \
        do {                                                       \
            code;                                                  \
        } while (0);                                               \
        return memcmp(first, second, sizeof(type));                \
    }

#define DEFINE_HASH_FUNCTION(type_name, type, code)        \
    static size_t type_name##_hash_function(void *ptr) {   \
        do {                                               \
            code;                                          \
        } while (0);                                       \
        return numerical_hash_function(sizeof(type), ptr); \
    }

#define NUMERICAL_COMPARE(type, first, second) \
    (*(type *)(first) > *(type *)(second)) - (*(type *)(first) < *(type *)(second))

#define VALUE_COMPARE(type, first, second) \
    ((type)(first) > (type)(second)) - ((type)(first) < (type)(second))
#endif