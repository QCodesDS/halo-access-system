#ifndef USER_JOURNEY_QUERY_H
#define USER_JOURNEY_QUERY_H

#include <string>
#include "models/LogRecord.h"
#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

struct UserJourneyQuery
{
    std::string userId;
    long long timeStart;
    long long timeEnd;
};

// Execute user journey query and print results
void executeUserJourney(const UserJourneyQuery &q, const HashIndex &hashIdx, const SortedIndex &sortedIdx);

#endif // USER_JOURNEY_QUERY_H
