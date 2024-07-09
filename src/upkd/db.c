#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/upkd/db.h"

#define HASH_SIZE 256

// Hash table entry
static HashEntry *hash_table[HASH_SIZE];

unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % HASH_SIZE;
}

const char* hash_table_insert(const char *key, const char *value, size_t valueMaxLength) {
    // Compute hash index for the given key
    unsigned int index = hash(key);

    // Check if key already exists
    HashEntry *entry = hash_table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            // Key already exists, concatenate the new value to the existing one
            size_t current_value_len = strlen(entry->value);
            size_t new_value_len = current_value_len + strlen(value);

            if (new_value_len > valueMaxLength) {
                // Calculate how much to cut from the beginning of the existing value
                size_t cut_length = new_value_len - valueMaxLength;
                size_t new_start = strlen(value) - cut_length;

                // Update the existing value by cutting from the beginning
                memmove(entry->value, entry->value + cut_length, current_value_len - cut_length + 1); // +1 for the null terminator

                // Update the length of the value
                current_value_len = valueMaxLength;
            }

            // Append new value to existing value
            strncat(entry->value, value, valueMaxLength - current_value_len);

            return entry->value; // Return the concatenated value
        }
        entry = entry->next;
    }

    // Key does not exist, create a new entry
    HashEntry *new_entry = malloc(sizeof(HashEntry));
    if (new_entry == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL; // Handle memory allocation failure
    }
    new_entry->key = strdup(key);
    if (new_entry->key == NULL) {
        free(new_entry);
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL; // Handle memory allocation failure
    }

    // Allocate memory for the value
    new_entry->value = malloc(valueMaxLength + 1); // +1 for null terminator
    if (new_entry->value == NULL) {
        free(new_entry->key);
        free(new_entry);
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL; // Handle memory allocation failure
    }

    // Copy the value ensuring it doesn't exceed valueMaxLength
    strncpy(new_entry->value, value, valueMaxLength);
    new_entry->value[valueMaxLength] = '\0'; // Ensure null terminator

    new_entry->next = hash_table[index];
    hash_table[index] = new_entry;

    return new_entry->value; // Return the newly inserted value
}

const char* hash_table_lookup(const char *key) {
    unsigned int index = hash(key);
    HashEntry *entry = hash_table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void clear_hash_table() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *entry = hash_table[i];
        while (entry != NULL) {
            HashEntry *temp = entry;
            entry = entry->next;

            // Free allocated memory for key and value
            free(temp->key);
            free(temp->value);

            // Now free the HashEntry structure itself
            free(temp);
        }
        hash_table[i] = NULL; // Ensure the hash table slot is cleared
    }
}
