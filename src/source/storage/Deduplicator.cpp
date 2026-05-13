#include "storage/Deduplicator.h"
#include "storage/DataStore.h"
#include "models/LogRecord.h"
#include "utils/StringUtils.h"

bool recordsEqual(const LogRecord &r1, const LogRecord &r2)
{
    // Compare all 7 fields
    if (!strEquals(r1.user_id, r2.user_id))
        return false;
    if (!strEquals(r1.device_id, r2.device_id))
        return false;
    if (!strEquals(r1.app_id, r2.app_id))
        return false;
    if (!strEquals(r1.resource_id, r2.resource_id))
        return false;
    if (r1.event_type != r2.event_type)
        return false;
    if (r1.location != r2.location)
        return false;
    if (r1.timestamp != r2.timestamp)
        return false;
    return true;
}

int deduplicate(DataStore &store)
{
    if (store.count <= 1)
    {
        return 0;
    }

    int duplicatesRemoved = 0;

    // Create a new temporary store for unique records
    DataStore tempStore;
    initDataStore(tempStore, store.capacity);

    // Add first record
    LogRecord *firstCopy = new LogRecord(*store.records[0]);
    push(tempStore, firstCopy);

    // Check each subsequent record
    for (int i = 1; i < store.count; ++i)
    {
        bool isDuplicate = false;

        // Compare with all records already in tempStore
        for (int j = 0; j < tempStore.count; ++j)
        {
            if (recordsEqual(*store.records[i], *tempStore.records[j]))
            {
                isDuplicate = true;
                duplicatesRemoved++;
                break;
            }
        }

        // If not a duplicate, add to tempStore
        if (!isDuplicate)
        {
            LogRecord *copy = new LogRecord(*store.records[i]);
            push(tempStore, copy);
        }
    }

    // Clear original store records (but not the array that will be replaced)
    for (int i = 0; i < store.count; ++i)
    {
        delete store.records[i];
    }
    delete[] store.records;

    // Replace with deduplicated data
    store.records = tempStore.records;
    store.count = tempStore.count;
    store.capacity = tempStore.capacity;

    return duplicatesRemoved;
}
