#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include "models/LogRecord.h"

// Node in the linked list for separate chaining
struct HashNode
{
    std::string key;    // Hash key
    LogRecord **values; // Dynamic array of LogRecord pointers
    int count;          // Current number of records
    int capacity;       // Capacity of values array
    HashNode *next;     // Next node in chain (for collision handling)
};

// Hash table using separate chaining
struct HashTable
{
    HashNode **buckets; // Array of bucket pointers (each bucket is a linked list)
    int tableSize;      // Number of buckets
    int totalKeys;      // Total unique keys
};

// Initialize hash table with specified size
void initHashTable(HashTable &ht, int tableSize);

// Insert a LogRecord into the hash table with given key
void insertHash(HashTable &ht, const std::string &key, LogRecord *record);

// Look up all records for a given key
// Returns array of LogRecord pointers, sets outCount to number of records found
// Returns nullptr if key not found
LogRecord **lookupHash(const HashTable &ht, const std::string &key, int &outCount);

// Clear and free all memory from hash table
void clearHashTable(HashTable &ht);

// Polynomial rolling hash function
int hashFunction(const std::string &key, int tableSize);

#endif // HASH_TABLE_H
