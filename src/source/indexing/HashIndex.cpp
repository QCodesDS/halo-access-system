#include "indexing/HashIndex.h"

// Build hash indexes from DataStore
void buildHashIndex(HashIndex &idx, DataStore &store, int tableSize)
{
    idx.tableSize = tableSize;

    // Initialize all three hash tables
    initHashTable(idx.byUser, tableSize);
    initHashTable(idx.byDevice, tableSize);
    initHashTable(idx.byResource, tableSize);

    // Iterate through all records and insert into each index
    for (int i = 0; i < store.count; i++)
    {
        LogRecord *record = get(store, i);

        if (record != nullptr)
        {
            insertHash(idx.byUser, record->user_id, record);
            insertHash(idx.byDevice, record->device_id, record);
            insertHash(idx.byResource, record->resource_id, record);
        }
    }
}

// Get records by user ID
LogRecord **getByUser(const HashIndex &idx, const std::string &userId, int &outCount)
{
    return lookupHash(idx.byUser, userId, outCount);
}

// Get records by device ID
LogRecord **getByDevice(const HashIndex &idx, const std::string &deviceId, int &outCount)
{
    return lookupHash(idx.byDevice, deviceId, outCount);
}

// Get records by resource ID
LogRecord **getByResource(const HashIndex &idx, const std::string &resourceId, int &outCount)
{
    return lookupHash(idx.byResource, resourceId, outCount);
}

// Clear hash indexes
void clearHashIndex(HashIndex &idx)
{
    clearHashTable(idx.byUser);
    clearHashTable(idx.byDevice);
    clearHashTable(idx.byResource);
    idx.tableSize = 0;
}
