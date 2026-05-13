#include "indexing/HashTable.h"
#include <cstring>

// Polynomial rolling hash function using prime 31
int hashFunction(const std::string &key, int tableSize)
{
    if (tableSize <= 0)
        return 0;

    long long hash = 0;
    const long long prime = 31;
    const long long modulo = 1e9 + 7;

    for (char c : key)
    {
        hash = (hash * prime + (int)c) % modulo;
    }

    return (int)(hash % tableSize);
}

// Initialize hash table
void initHashTable(HashTable &ht, int tableSize)
{
    ht.tableSize = tableSize;
    ht.totalKeys = 0;
    ht.buckets = new HashNode *[tableSize];

    for (int i = 0; i < tableSize; i++)
    {
        ht.buckets[i] = nullptr;
    }
}

// Insert LogRecord into hash table
void insertHash(HashTable &ht, const std::string &key, LogRecord *record)
{
    if (record == nullptr)
        return;

    int bucket = hashFunction(key, ht.tableSize);
    HashNode *node = ht.buckets[bucket];
    HashNode *prevNode = nullptr;

    // Find existing node with this key or end of chain
    while (node != nullptr)
    {
        if (node->key == key)
        {
            // Found existing key - append record to its values array
            if (node->count == node->capacity)
            {
                // Resize values array
                int newCapacity = node->capacity * 2;
                LogRecord **newValues = new LogRecord *[newCapacity];

                for (int i = 0; i < node->count; i++)
                {
                    newValues[i] = node->values[i];
                }

                delete[] node->values;
                node->values = newValues;
                node->capacity = newCapacity;
            }

            node->values[node->count] = record;
            node->count++;
            return;
        }

        prevNode = node;
        node = node->next;
    }

    // Key not found - create new node
    HashNode *newNode = new HashNode();
    newNode->key = key;
    newNode->capacity = 10;
    newNode->count = 1;
    newNode->values = new LogRecord *[newNode->capacity];
    newNode->values[0] = record;
    newNode->next = nullptr;

    if (prevNode == nullptr)
    {
        // Empty bucket
        ht.buckets[bucket] = newNode;
    }
    else
    {
        // Append to end of chain
        prevNode->next = newNode;
    }

    ht.totalKeys++;
}

// Look up records by key
LogRecord **lookupHash(const HashTable &ht, const std::string &key, int &outCount)
{
    outCount = 0;

    int bucket = hashFunction(key, ht.tableSize);
    HashNode *node = ht.buckets[bucket];

    // Traverse chain to find key
    while (node != nullptr)
    {
        if (node->key == key)
        {
            outCount = node->count;
            return node->values;
        }

        node = node->next;
    }

    // Key not found
    return nullptr;
}

// Clear hash table and free all memory
void clearHashTable(HashTable &ht)
{
    for (int i = 0; i < ht.tableSize; i++)
    {
        HashNode *node = ht.buckets[i];

        while (node != nullptr)
        {
            HashNode *temp = node;
            node = node->next;

            delete[] temp->values;
            delete temp;
        }
    }

    delete[] ht.buckets;
    ht.buckets = nullptr;
    ht.tableSize = 0;
    ht.totalKeys = 0;
}
