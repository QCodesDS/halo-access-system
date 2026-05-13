#ifndef HASH_INDEX_H
#define HASH_INDEX_H

#include "indexing/HashTable.h"
#include "storage/DataStore.h"

// Hash index structure containing 3 hash tables
// One for each dimension: user_id, device_id, resource_id
struct HashIndex
{
    HashTable byUser;     // Index by user_id
    HashTable byDevice;   // Index by device_id
    HashTable byResource; // Index by resource_id
    int tableSize;        // Table size used for all 3 tables
};

// Build hash indexes from DataStore
void buildHashIndex(HashIndex &idx, DataStore &store, int tableSize);

// Get records by user ID
// Returns array of LogRecord pointers, sets outCount to number found
LogRecord **getByUser(const HashIndex &idx, const std::string &userId, int &outCount);

// Get records by device ID
LogRecord **getByDevice(const HashIndex &idx, const std::string &deviceId, int &outCount);

// Get records by resource ID
LogRecord **getByResource(const HashIndex &idx, const std::string &resourceId, int &outCount);

// Clear and free all hash indexes
void clearHashIndex(HashIndex &idx);

#endif // HASH_INDEX_H
