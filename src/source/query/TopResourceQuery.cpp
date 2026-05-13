#include "query/TopResourceQuery.h"
#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"
#include <cstdio>
#include <cstring>

// Helper: find if resource exists in count map, return index or -1
static int findCountEntry(CountEntry *entries, int count, const std::string &resourceId)
{
    for (int i = 0; i < count; i++)
    {
        if (entries[i].resourceId == resourceId)
        {
            return i;
        }
    }
    return -1;
}

// Helper: selection sort to get top N entries (sorts in descending order by count)
static void selectionSortTopN(CountEntry *entries, int count, int topN)
{
    int limit = (topN < count) ? topN : count;

    for (int i = 0; i < limit; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++)
        {
            if (entries[j].count > entries[maxIdx].count)
            {
                maxIdx = j;
            }
        }
        // Swap
        CountEntry temp = entries[i];
        entries[i] = entries[maxIdx];
        entries[maxIdx] = temp;
    }
}

void executeTopResources(long long timeStart, long long timeEnd, const SortedIndex &sortedIdx, int topN)
{
    // Find range in sorted index
    int startIdx = binarySearchStart(sortedIdx, timeStart);
    int endIdx = binarySearchEnd(sortedIdx, timeEnd);

    // Count resources in time range
    CountEntry *countMap = nullptr;
    int mapSize = 0;
    int mapCapacity = 100; // Initial capacity

    if (startIdx <= endIdx && startIdx < sortedIdx.count && endIdx >= 0)
    {
        countMap = new CountEntry[mapCapacity];

        for (int i = startIdx; i <= endIdx; i++)
        {
            LogRecord *record = sortedIdx.records[i];
            int entryIdx = findCountEntry(countMap, mapSize, record->resource_id);

            if (entryIdx >= 0)
            {
                // Increment existing entry
                countMap[entryIdx].count++;
            }
            else
            {
                // Add new entry
                if (mapSize >= mapCapacity)
                {
                    // Expand capacity
                    CountEntry *newMap = new CountEntry[mapCapacity * 2];
                    for (int k = 0; k < mapSize; k++)
                    {
                        newMap[k] = countMap[k];
                    }
                    delete[] countMap;
                    countMap = newMap;
                    mapCapacity *= 2;
                }
                countMap[mapSize].resourceId = record->resource_id;
                countMap[mapSize].count = 1;
                mapSize++;
            }
        }
    }

    // Sort to get top N
    if (mapSize > 0)
    {
        selectionSortTopN(countMap, mapSize, topN);
    }

    // Print header with date range
    std::string startDate = epochToReadable(timeStart);
    std::string endDate = epochToReadable(timeEnd);

    // Extract just the date part (YYYY-MM-DD)
    std::string startDateOnly = startDate.substr(0, 10);
    std::string endDateOnly = endDate.substr(0, 10);

    printf("Top %d Resources | %s to %s\n", topN, startDateOnly.c_str(), endDateOnly.c_str());
    printf("──────────────────────────────────────────\n");
    printf("Rank  Resource    Count\n");
    printf("──────────────────────────────────────────\n");

    if (mapSize == 0)
    {
        printf("[INFO] No resources found in time range\n");
    }
    else
    {
        int displayCount = (topN < mapSize) ? topN : mapSize;
        for (int i = 0; i < displayCount; i++)
        {
            printf("%-5d %-12s %d\n", i + 1, countMap[i].resourceId.c_str(), countMap[i].count);
        }
    }

    printf("──────────────────────────────────────────\n");

    // Cleanup
    if (countMap != nullptr)
    {
        delete[] countMap;
        countMap = nullptr;
    }
}
