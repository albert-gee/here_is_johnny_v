#ifndef HERE_IS_JOHNNY_V_DB_H
#define HERE_IS_JOHNNY_V_DB_H

#include <stddef.h>

/**
 * @struct HashEntry
 * @brief Represents an entry in the hash table.
 *
 * The HashEntry structure represents a single entry in the hash table, containing
 * a key-value pair and a pointer to the next entry in case of collisions (linked list).
 */
typedef struct HashEntry {
    char *key;             /**< The key of the entry */
    char *value;           /**< The value associated with the key */
    struct HashEntry *next;/**< Pointer to the next entry in the list (for collisions) */
} HashEntry;

/**
 * @brief Computes the hash value for a given string.
 *
 * This function takes a string and computes a hash value which is used to
 * determine the index in the hash table where the key-value pair should be stored.
 *
 * @param str The input string for which the hash value is to be computed.
 * @return The computed hash value.
 */
unsigned int hash(const char *str);

/**
 * @brief Inserts a key-value pair into the hash table.
 *
 * This function inserts a key-value pair into the hash table. If the key already exists,
 * its value is updated with the new value.
 *
 * @param key The key to be inserted.
 * @param value The value to be associated with the key.
 * @param valueMaxLength The maximum length of the value.
 * @return The value associated with the key.
 */
const char* hash_table_insert(const char *key, const char *value, size_t valueMaxLength);

/**
 * @brief Looks up a value by its key in the hash table.
 *
 * This function searches for a key in the hash table and returns the associated value.
 * If the key is not found, the function returns NULL.
 *
 * @param key The key to be looked up.
 * @return The value associated with the key, or NULL if the key is not found.
 */
const char* hash_table_lookup(const char *key);

/**
 * @brief Cleans up the hash table by freeing all memory.
 *
 * This function iterates over all entries in the hash table and frees the memory
 * allocated for each key-value pair.
 */
void clear_hash_table();

#endif //HERE_IS_JOHNNY_V_DB_H
