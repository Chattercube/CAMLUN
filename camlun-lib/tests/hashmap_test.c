#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "typemethods.h"

// Helper functions for testing
static type_methods TYPE_INT = TYPE_METHODS(int);
static type_methods TYPE_STRING = TYPE_METHODS(string);

// Test functions
void test_hashmap_creation() {
    printf("Testing HashMap creation...\n");
    HashMap *map = hashmap_create(&TYPE_STRING, &TYPE_INT);
    if (map) {
        printf("HashMap created successfully.\n");
    } else {
        printf("Failed to create HashMap.\n");
    }
    hashmap_destroy(map);
}

void test_hashmap_insertion_and_retrieval() {
    printf("Testing HashMap insertion and retrieval...\n");
    HashMap *map = hashmap_create(&TYPE_STRING, &TYPE_INT);

    char *key1 = "key1";
    char *key2 = "key2";
    int value1 = 10;
    int value2 = 20;

    hashmap_set(map, key1, &value1);
    hashmap_set(map, key2, &value2);

    int *retrieved_value1 = hashmap_get(map, key1);
    int *retrieved_value2 = hashmap_get(map, key2);

    if (retrieved_value1 && *retrieved_value1 == value1) {
        printf("Retrieved value for key1 is correct: %d\n", *retrieved_value1);
    } else {
        printf("Retrieved value for key1 is incorrect.\n");
    }

    if (retrieved_value2 && *retrieved_value2 == value2) {
        printf("Retrieved value for key2 is correct: %d\n", *retrieved_value2);
    } else {
        printf("Retrieved value for key2 is incorrect.\n");
    }

    hashmap_destroy(map);
}

void test_hashmap_iteration() {
    printf("Testing HashMap iteration...\n");
    HashMap *map = hashmap_create(&TYPE_STRING, &TYPE_INT);

    char *key1 = "key1";
    char *key2 = "key2";
    int value1 = 10;
    int value2 = 20;

    hashmap_set(map, key1, &value1);
    hashmap_set(map, key2, &value2);

    printf("Iterating over keys:\n");
    HASHMAP_KEYS_FOREACH(map, char *key, {
        printf("Key: %s\n", key);
    });

    printf("Iterating over values:\n");
    HASHMAP_VALUES_FOREACH(map, int *value, {
        printf("Value: %d\n", *value);
    });

    printf("Iterating over key-value pairs:\n");
    HASHMAP_PAIRS_FOREACH(map, char *key, int *value, {
        printf("Key: %s, Value: %d\n", key, *value);
    });

    hashmap_destroy(map);
}

void test_hashmap_deletion() {
    printf("Testing HashMap deletion...\n");
    HashMap *map = hashmap_create(&TYPE_INT, &TYPE_STRING);

    char *key1 = "key1";
    char *key2 = "key2";
    int value1 = 10;
    int value2 = 20;

    hashmap_set(map, key1, &value1);
    hashmap_set(map, key2, &value2);

    hashmap_remove(map, key1);

    if (hashmap_contains(map, key1)) {
        printf("Key1 was not removed successfully.\n");
    } else {
        printf("Key1 was removed successfully.\n");
    }

    if (hashmap_contains(map, key2)) {
        printf("Key2 is still present.\n");
    } else {
        printf("Key2 was removed incorrectly.\n");
    }

    hashmap_destroy(map);
}

void test_hashmap_printf() {
    printf("Testing HashMap printf functions...\n");
    HashMap *map = hashmap_create(&TYPE_STRING, &TYPE_INT);

    char *key1 = "key1";
    char *key2 = "key2";
    int value1 = 10;
    int value2 = 20;

    hashmap_set(map, key1, &value1);
    hashmap_set(map, key2, &value2);

    printf("HashMap contents using HASHMAP_PRINTF:\n");
    HASHMAP_PRINTF(map, char *key, int *value, "%s: %d", key, *value);

    hashmap_destroy(map);
}

int main() {
    test_hashmap_creation();
    test_hashmap_insertion_and_retrieval();
    test_hashmap_iteration();
    test_hashmap_deletion();
    test_hashmap_printf();
    return 0;
}