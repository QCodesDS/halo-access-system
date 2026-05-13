#include "indexing/IndexManager.h"
#include <ctime>
#include <iostream>
#include <iomanip>

// Helper function to select appropriate table size based on record count
static int selectTableSize(int recordCount)
{
    if (recordCount <= 0)
        return 20011; // Default minimum prime
    else if (recordCount <= 10000)
        return 20011; // Prime number suitable for ~10k records
    else if (recordCount <= 1000000)
        return 1999993; // Large prime suitable for ~1M records
    else
        return 1999993 + (recordCount / 100000); // Scale up if needed
}

// Build all indexes
void buildAllIndexes(IndexManager &mgr, DataStore &store)
{
    if (store.count == 0)
    {
        mgr.isBuilt = false;
        return;
    }

    // Record start time
    clock_t start = clock();

    // Select table size based on record count
    int tableSize = selectTableSize(store.count);

    // Build hash index
    buildHashIndex(mgr.hashIdx, store, tableSize);

    // Build sorted index
    buildSortedIndex(mgr.sortedIdx, store);

    // Calculate elapsed time
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    mgr.isBuilt = true;

    // Print build completion message with time
    std::cout << "[INFO] Indexes built in " << std::fixed << std::setprecision(2) << elapsed << "s" << std::endl;
}

// Clear all indexes
void clearAllIndexes(IndexManager &mgr)
{
    clearHashIndex(mgr.hashIdx);
    clearSortedIndex(mgr.sortedIdx);
    mgr.isBuilt = false;
}
