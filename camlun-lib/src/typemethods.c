#include "typemethods.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "murmur3.h"



void *numerical_default_constructor(size_t type_size) {
    return calloc(1, type_size);
}

void numerical_destructor(size_t type_size, void *ptr) {
    if (ptr) free(ptr);
}

void *numerical_copy_constructor(size_t type_size, void *ptr) {
    if (!ptr) return NULL;
    void *new_ptr = malloc(type_size);
    memcpy(new_ptr, ptr, type_size);
    return new_ptr;
}

int numerical_unsigned_comparator(size_t type_size, void *first, void *second) {
    if (!first && !second) return 0;
    if (!first) return -1;
    if (!second) return 1;
    return memcmp(first, second, type_size);
}

int numerical_signed_comparator(size_t type_size, void *first, void *second) {
    if (!first && !second) return 0;
    if (!first) return -1;
    if (!second) return 1;
    unsigned char first_leading_bit = ((unsigned char *)first)[0] & 0x80;
    unsigned char second_leading_bit = ((unsigned char *)second)[0] & 0x80;
    if (first_leading_bit != second_leading_bit) {  // One of them is negative
        return (first_leading_bit) ? -1 : 1;
    }
    int cmp = memcmp(first, second, type_size);
    if (cmp > 0) {
        return 1;
    } else if (cmp < 0) {
        return -1;
    } else
        return 0;
}

#define NUMERICAL_VALUE_COMPARATOR(type, first, second)                               \
    ((*(type *)first > *(type *)second) ? 1 : (*(type *)first < *(type *)second) ? -1 \
                                                                                 : 0)

size_t numerical_hash_function(size_t type_size, void *ptr) {
    uint64_t hash_output[2];
    MurmurHash3_x64_128(ptr, type_size, HASH_SEED, hash_output);
    return (size_t)(hash_output[0] ^ hash_output[1]);
}

void *shallow_default_constructor() {
    return NULL;
}

void shallow_destructor(void *ptr) {
    return;  // No action needed for shallow copy
}

void *shallow_copy_constructor(void *ptr) {
    return ptr;
}

int shallow_comparator(void *first, void *second) {
    return (first > second) - (first < second);  // Returns 1 if first > second, -1 if first < second, 0 if equal
}

size_t shallow_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(void *), ptr);  // Simple hash function for pointers
}

void *string_default_constructor() {
    char *str = calloc(1, sizeof(char));
    return (void *)str;
}

void string_destructor(void *ptr) {
    if (ptr) free(ptr);
}

void *string_copy_constructor(void *ptr) {
    char *dst = malloc(strlen (ptr) + 1);  // Space for length plus nul
    if (dst == NULL) return NULL;          // No memory
    strcpy(dst, ptr);                      // Copy the characters
    return dst;                            // Return the new string
}

int string_comparator(void *first, void *second) {
    char *str1 = (char *)first;
    char *str2 = (char *)second;
    int cmp = strcmp(str1, str2);
    if (cmp > 0) {
        return 1;
    } else if (cmp < 0) {
        return -1;
    } else
        return 0;
}

size_t string_hash_function(void *ptr) {
    uint64_t hash_output[2];
    int len = strlen((char *)ptr);
    MurmurHash3_x64_128(ptr, len, HASH_SEED, hash_output);
    return (size_t)(hash_output[0] ^ hash_output[1]);
}

void *bool_default_constructor() { return numerical_default_constructor(sizeof(bool)); }
void *char_default_constructor() { return numerical_default_constructor(sizeof(char)); }
void *signed_char_default_constructor() { return numerical_default_constructor(sizeof(signed char)); }
void *unsigned_char_default_constructor() { return numerical_default_constructor(sizeof(unsigned char)); }
void *short_default_constructor() { return numerical_default_constructor(sizeof(short)); }
void *unsigned_short_default_constructor() { return numerical_default_constructor(sizeof(unsigned short)); }
void *int_default_constructor() { return numerical_default_constructor(sizeof(int)); }
void *unsigned_int_default_constructor() { return numerical_default_constructor(sizeof(unsigned int)); }
void *long_default_constructor() { return numerical_default_constructor(sizeof(long)); }
void *unsigned_long_default_constructor() { return numerical_default_constructor(sizeof(unsigned long)); }
void *long_long_default_constructor() { return numerical_default_constructor(sizeof(long long)); }
void *unsigned_long_long_default_constructor() { return numerical_default_constructor(sizeof(unsigned long long)); }
void *float_default_constructor() { return numerical_default_constructor(sizeof(float)); }
void *double_default_constructor() { return numerical_default_constructor(sizeof(double)); }
void *long_double_default_constructor() { return numerical_default_constructor(sizeof(long double)); }

void *int8_t_default_constructor() { return numerical_default_constructor(sizeof(int8_t)); }
void *int16_t_default_constructor() { return numerical_default_constructor(sizeof(int16_t)); }
void *int32_t_default_constructor() { return numerical_default_constructor(sizeof(int32_t)); }
void *int64_t_default_constructor() { return numerical_default_constructor(sizeof(int64_t)); }
void *intmax_t_default_constructor() { return numerical_default_constructor(sizeof(intmax_t)); }
void *intptr_t_default_constructor() { return numerical_default_constructor(sizeof(intptr_t)); }
void *uint8_t_default_constructor() { return numerical_default_constructor(sizeof(uint8_t)); }
void *uint16_t_default_constructor() { return numerical_default_constructor(sizeof(uint16_t)); }
void *uint32_t_default_constructor() { return numerical_default_constructor(sizeof(uint32_t)); }
void *uint64_t_default_constructor() { return numerical_default_constructor(sizeof(uint64_t)); }
void *uintmax_t_default_constructor() { return numerical_default_constructor(sizeof(uintmax_t)); }
void *uintptr_t_default_constructor() { return numerical_default_constructor(sizeof(uintptr_t)); }

void *size_t_default_constructor() { return numerical_default_constructor(sizeof(size_t)); }
void *ptrdiff_t_default_constructor() { return numerical_default_constructor(sizeof(ptrdiff_t)); }

void bool_destructor(void *ptr) { if (ptr) free(ptr); }
void char_destructor(void *ptr) { if (ptr) free(ptr); }
void signed_char_destructor(void *ptr) { if (ptr) free(ptr); }
void unsigned_char_destructor(void *ptr) { if (ptr) free(ptr); }
void short_destructor(void *ptr) { if (ptr) free(ptr); }
void unsigned_short_destructor(void *ptr) { if (ptr) free(ptr); }
void int_destructor(void *ptr) { if (ptr) free(ptr); }
void unsigned_int_destructor(void *ptr) { if (ptr) free(ptr); }
void long_destructor(void *ptr) { if (ptr) free(ptr); }
void unsigned_long_destructor(void *ptr) { if (ptr) free(ptr); }
void long_long_destructor(void *ptr) { if (ptr) free(ptr); }
void unsigned_long_long_destructor(void *ptr) { if (ptr) free(ptr); }
void float_destructor(void *ptr) { if (ptr) free(ptr); }
void double_destructor(void *ptr) { if (ptr) free(ptr); }
void long_double_destructor(void *ptr) { if (ptr) free(ptr); }

void int8_t_destructor(void *ptr) { if (ptr) free(ptr); }
void int16_t_destructor(void *ptr) { if (ptr) free(ptr); }
void int32_t_destructor(void *ptr) { if (ptr) free(ptr); }
void int64_t_destructor(void *ptr) { if (ptr) free(ptr); }
void intmax_t_destructor(void *ptr) { if (ptr) free(ptr); }
void intptr_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uint8_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uint16_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uint32_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uint64_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uintmax_t_destructor(void *ptr) { if (ptr) free(ptr); }
void uintptr_t_destructor(void *ptr) { if (ptr) free(ptr); }

void size_t_destructor(void *ptr) { if (ptr) free(ptr); }
void ptrdiff_t_destructor(void *ptr) { if (ptr) free(ptr); }

void *bool_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(bool), ptr);
}

void *char_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(char), ptr);
}

void *signed_char_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(signed char), ptr);
}

void *unsigned_char_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(unsigned char), ptr);
}

void *short_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(short), ptr);
}

void *unsigned_short_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(unsigned short), ptr);
}

void *int_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(int), ptr);
}

void *unsigned_int_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(unsigned int), ptr);
}

void *long_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(long), ptr);
}

void *unsigned_long_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(unsigned long), ptr);
}

void *long_long_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(long long), ptr);
}

void *unsigned_long_long_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(unsigned long long), ptr);
}

void *float_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(float), ptr);
}

void *double_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(double), ptr);
}

void *long_double_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(long double), ptr);
}

void *int8_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(int8_t), ptr);
}

void *int16_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(int16_t), ptr);
}

void *int32_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(int32_t), ptr);
}

void *int64_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(int64_t), ptr);
}

void *intmax_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(intmax_t), ptr);
}

void *intptr_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(intptr_t), ptr);
}

void *uint8_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uint8_t), ptr);
}

void *uint16_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uint16_t), ptr);
}

void *uint32_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uint32_t), ptr);
}

void *uint64_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uint64_t), ptr);
}

void *uintmax_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uintmax_t), ptr);
}

void *uintptr_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(uintptr_t), ptr);
}

void *size_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(size_t), ptr);
}

void *ptrdiff_t_copy_constructor(void *ptr) {
    return numerical_copy_constructor(sizeof(ptrdiff_t), ptr);
}

int bool_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(bool, first, second);
}

int char_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(char, first, second);
}

int signed_char_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(signed char, first, second);
}

int unsigned_char_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(unsigned char, first, second);
}

int short_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(short, first, second);
}

int unsigned_short_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(unsigned short, first, second);
}

int int_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(int, first, second);
}

int unsigned_int_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(unsigned int, first, second);
}

int long_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(long, first, second);
}

int unsigned_long_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(unsigned long, first, second);
}

int long_long_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(long long, first, second);
}

int unsigned_long_long_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(unsigned long long, first, second);
}

int float_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(float, first, second);
}

int double_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(double, first, second);
}

int long_double_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(long double, first, second);
}

int int8_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(int8_t, first, second);
}

int int16_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(int16_t, first, second);
}

int int32_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(int32_t, first, second);
}

int int64_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(int64_t, first, second);
}

int intmax_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(intmax_t, first, second);
}

int intptr_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(intptr_t, first, second);
}

int uint8_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uint8_t, first, second);
}

int uint16_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uint16_t, first, second);
}

int uint32_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uint32_t, first, second);
}

int uint64_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uint64_t, first, second);
}

int uintmax_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uintmax_t, first, second);
}

int uintptr_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(uintptr_t, first, second);
}

int size_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(size_t, first, second);
}

int ptrdiff_t_comparator(void *first, void *second) {
    return NUMERICAL_VALUE_COMPARATOR(ptrdiff_t, first, second);
}

size_t bool_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(bool), ptr);
}

size_t char_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(char), ptr);
}

size_t signed_char_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(signed char), ptr);
}

size_t unsigned_char_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(unsigned char), ptr);
}

size_t short_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(short), ptr);
}

size_t unsigned_short_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(unsigned short), ptr);
}

size_t int_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(int), ptr);
}

size_t unsigned_int_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(unsigned int), ptr);
}

size_t long_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(long), ptr);
}

size_t unsigned_long_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(unsigned long), ptr);
}

size_t long_long_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(long long), ptr);
}

size_t unsigned_long_long_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(unsigned long long), ptr);
}

size_t float_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(float), ptr);
}

size_t double_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(double), ptr);
}

size_t long_double_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(long double), ptr);
}

size_t int8_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(int8_t), ptr);
}

size_t int16_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(int16_t), ptr);
}

size_t int32_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(int32_t), ptr);
}

size_t int64_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(int64_t), ptr);
}

size_t intmax_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(intmax_t), ptr);
}

size_t intptr_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(intptr_t), ptr);
}

size_t uint8_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uint8_t), ptr);
}

size_t uint16_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uint16_t), ptr);
}

size_t uint32_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uint32_t), ptr);
}

size_t uint64_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uint64_t), ptr);
}

size_t uintmax_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uintmax_t), ptr);
}

size_t uintptr_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(uintptr_t), ptr);
}

size_t size_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(size_t), ptr);
}

size_t ptrdiff_t_hash_function(void *ptr) {
    return numerical_hash_function(sizeof(ptrdiff_t), ptr);
}
