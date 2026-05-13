#include "storage/DataStore.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

void initDataStore(DataStore &store, int initialCapacity)
{
    store.capacity = initialCapacity;
    store.count = 0;
    store.records = new LogRecord *[store.capacity];
    // No need to initialize pointers to nullptr, we track count
}

void push(DataStore &store, LogRecord *r)
{
    // Double capacity if full
    if (store.count >= store.capacity)
    {
        int newCapacity = store.capacity * 2;
        LogRecord **newRecords = new LogRecord *[newCapacity];

        // Copy existing records
        for (int i = 0; i < store.count; ++i)
        {
            newRecords[i] = store.records[i];
        }

        // Delete old array
        delete[] store.records;

        store.records = newRecords;
        store.capacity = newCapacity;
    }

    store.records[store.count++] = r;
}

LogRecord *get(const DataStore &store, int i)
{
    if (i < 0 || i >= store.count)
    {
        return nullptr;
    }
    return store.records[i];
}

int size(const DataStore &store)
{
    return store.count;
}

void clear(DataStore &store)
{
    // Delete each LogRecord
    for (int i = 0; i < store.count; ++i)
    {
        delete store.records[i];
    }

    // Delete array
    delete[] store.records;

    // Reset
    store.records = nullptr;
    store.count = 0;
    store.capacity = 0;
}

LogRecord **getAll(const DataStore &store)
{
    return store.records;
}
