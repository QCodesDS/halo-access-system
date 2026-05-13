#include "indexing/SortedIndex.h"
#include <cstring>

// Merge helper function for merge sort
void merge(LogRecord **arr, int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Create temporary arrays
    LogRecord **leftArr = new LogRecord *[leftSize];
    LogRecord **rightArr = new LogRecord *[rightSize];

    // Copy data to temp arrays
    for (int i = 0; i < leftSize; i++)
    {
        leftArr[i] = arr[left + i];
    }

    for (int j = 0; j < rightSize; j++)
    {
        rightArr[j] = arr[mid + 1 + j];
    }

    // Merge the temp arrays back into arr
    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize)
    {
        // Use <= for stable sort (preserves original order on ties)
        if (leftArr[i]->timestamp <= rightArr[j]->timestamp)
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements from left array
    while (i < leftSize)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy remaining elements from right array
    while (j < rightSize)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Merge sort - recursive sort by timestamp
void mergeSort(LogRecord **arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Build sorted index
void buildSortedIndex(SortedIndex &idx, DataStore &store)
{
    idx.count = store.count;

    if (idx.count == 0)
    {
        idx.records = nullptr;
        return;
    }

    // Allocate and copy all record pointers
    idx.records = new LogRecord *[idx.count];

    for (int i = 0; i < idx.count; i++)
    {
        idx.records[i] = get(store, i);
    }

    // Sort by timestamp using merge sort
    if (idx.count > 1)
    {
        mergeSort(idx.records, 0, idx.count - 1);
    }
}

// Binary search - find first record with timestamp >= t
int binarySearchStart(const SortedIndex &idx, long long t)
{
    int left = 0, right = idx.count - 1;
    int result = idx.count; // Default: all records < t

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (idx.records[mid]->timestamp >= t)
        {
            result = mid;
            right = mid - 1; // Keep searching left
        }
        else
        {
            left = mid + 1; // Search right
        }
    }

    return result;
}

// Binary search - find last record with timestamp <= t
int binarySearchEnd(const SortedIndex &idx, long long t)
{
    int left = 0, right = idx.count - 1;
    int result = -1; // Default: all records > t

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (idx.records[mid]->timestamp <= t)
        {
            result = mid;
            left = mid + 1; // Keep searching right
        }
        else
        {
            right = mid - 1; // Search left
        }
    }

    return result;
}

// Clear sorted index - only delete array, NOT individual pointers
// LogRecord ownership remains with DataStore
void clearSortedIndex(SortedIndex &idx)
{
    if (idx.records != nullptr)
    {
        delete[] idx.records;
        idx.records = nullptr;
    }
    idx.count = 0;
}
