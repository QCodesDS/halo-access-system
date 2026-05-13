#ifndef RESOURCE_JOURNEY_QUERY_H
#define RESOURCE_JOURNEY_QUERY_H

#include <string>
#include "models/LogRecord.h"
#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

struct ResourceJourneyQuery
{
    std::string resourceId;
    long long timeStart;
    long long timeEnd;
};

// Execute resource journey query and print results
void executeResourceJourney(const ResourceJourneyQuery &q, const HashIndex &hashIdx, const SortedIndex &sortedIdx);

#endif // RESOURCE_JOURNEY_QUERY_H
