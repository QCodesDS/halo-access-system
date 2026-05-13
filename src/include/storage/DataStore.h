#pragma once

#include "models/LogRecord.h"

// Dynamic array storage for LogRecords
struct DataStore
{
    LogRecord **records; // Array of LogRecord pointers
    int capacity;        // Current capacity
    int count;           // Current number of records
};

// Initialize DataStore with initial capacity
void initDataStore(DataStore &store, int initialCapacity = 1000);

// Add a LogRecord to the store (doubles capacity if needed)
void push(DataStore &store, LogRecord *r);

// Get LogRecord at index
LogRecord *get(const DataStore &store, int i);

// Get number of records
int size(const DataStore &store);

// Clear all records and free memory
void clear(DataStore &store);

// Get array of all records (for sorting/iterating)
LogRecord **getAll(const DataStore &store);
