#ifndef TOP_RESOURCE_QUERY_H
#define TOP_RESOURCE_QUERY_H

#include <string>
#include "indexing/SortedIndex.h"

// Entry in the count map for top resources
struct CountEntry
{
    std::string resourceId;
    int count;
};

// Execute top resources query and print results
void executeTopResources(long long timeStart, long long timeEnd, const SortedIndex &sortedIdx, int topN = 10);

#endif // TOP_RESOURCE_QUERY_H
