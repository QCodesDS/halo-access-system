#ifndef SORTED_INDEX_H
#define SORTED_INDEX_H

#include "models/LogRecord.h"
#include "storage/DataStore.h"

// Sorted index by timestamp
struct SortedIndex
{
    LogRecord **records; // Array of LogRecord pointers sorted by timestamp
    int count;           // Number of records
};

// Build sorted index from DataStore (using merge sort)
void buildSortedIndex(SortedIndex &idx, DataStore &store);

// Merge sort implementation - sorts records by timestamp (ascending)
void mergeSort(LogRecord **arr, int left, int right);

// Merge helper function
void merge(LogRecord **arr, int left, int mid, int right);

// Binary search - find first record with timestamp >= t
// Returns index of first record >= t, or count if all records < t
int binarySearchStart(const SortedIndex &idx, long long t);

// Binary search - find last record with timestamp <= t
// Returns index of last record <= t, or -1 if all records > t
int binarySearchEnd(const SortedIndex &idx, long long t);

// Clear sorted index (only delete the array, NOT the individual LogRecord pointers)
// LogRecord ownership remains with DataStore
void clearSortedIndex(SortedIndex &idx);

#endif // SORTED_INDEX_H
