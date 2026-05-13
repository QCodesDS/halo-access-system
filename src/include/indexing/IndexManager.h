#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H

#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"
#include "storage/DataStore.h"

// Manager for all indexes
struct IndexManager
{
    HashIndex hashIdx;     // Hash indexes for user/device/resource
    SortedIndex sortedIdx; // Sorted index by timestamp
    bool isBuilt;          // Flag indicating if indexes are built
};

// Build all indexes from DataStore
// Automatically selects appropriate table size based on record count
// Prints build time in format: "[INFO] Indexes built in Xs"
void buildAllIndexes(IndexManager &mgr, DataStore &store);

// Clear all indexes and free memory
void clearAllIndexes(IndexManager &mgr);

#endif // INDEX_MANAGER_H
