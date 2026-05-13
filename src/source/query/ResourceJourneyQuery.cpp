#include "query/ResourceJourneyQuery.h"
#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"
#include <cstdio>
#include <cstring>

// Helper: insertion sort for small result sets
static void insertionSortByTimestamp(LogRecord **records, int count)
{
    for (int i = 1; i < count; i++)
    {
        LogRecord *key = records[i];
        int j = i - 1;
        while (j >= 0 && records[j]->timestamp > key->timestamp)
        {
            records[j + 1] = records[j];
            j--;
        }
        records[j + 1] = key;
    }
}

void executeResourceJourney(const ResourceJourneyQuery &q, const HashIndex &hashIdx, const SortedIndex &)
{
    // Get all records for this resource
    int resourceRecordCount = 0;
    LogRecord **resourceRecords = getByResource(hashIdx, q.resourceId, resourceRecordCount);

    // Filter records within time range
    LogRecord **filtered = nullptr;
    int filteredCount = 0;

    if (resourceRecordCount > 0)
    {
        // Allocate temporary array for filtered results
        filtered = new LogRecord *[resourceRecordCount];

        for (int i = 0; i < resourceRecordCount; i++)
        {
            if (resourceRecords[i]->timestamp >= q.timeStart && resourceRecords[i]->timestamp <= q.timeEnd)
            {
                filtered[filteredCount++] = resourceRecords[i];
            }
        }
    }

    // Sort filtered results by timestamp
    if (filteredCount > 0)
    {
        insertionSortByTimestamp(filtered, filteredCount);
    }

    // Print header with date range
    std::string startDate = epochToReadable(q.timeStart);
    std::string endDate = epochToReadable(q.timeEnd);

    // Extract just the date part (YYYY-MM-DD)
    std::string startDateOnly = startDate.substr(0, 10);
    std::string endDateOnly = endDate.substr(0, 10);

    printf("Resource Journey: %s | %s to %s\n", q.resourceId.c_str(), startDateOnly.c_str(), endDateOnly.c_str());
    printf("───────────────────────────────────────────────────────────────────────────────────\n");

    if (filteredCount == 0)
    {
        printf("[INFO] No events found for resource %s\n", q.resourceId.c_str());
    }
    else
    {
        // Print each record in format: [YYYY-MM-DD HH:MM:SS] user_id → device_id → app_id (event_type @ location)
        for (int i = 0; i < filteredCount; i++)
        {
            LogRecord *record = filtered[i];
            std::string timestamp = epochToReadable(record->timestamp);
            std::string eventStr = eventTypeToString(record->event_type);
            std::string locStr = locationToString(record->location);

            printf("[%s] %s → %s → %s (%s @ %s)\n",
                   timestamp.c_str(),
                   record->user_id.c_str(),
                   record->device_id.c_str(),
                   record->app_id.c_str(),
                   eventStr.c_str(),
                   locStr.c_str());
        }

        printf("───────────────────────────────────────────────────────────────────────────────────\n");
        printf("Total: %d events\n", filteredCount);
    }

    // Cleanup (note: resourceRecords is owned by HashIndex, do NOT delete it)
    if (filtered != nullptr)
    {
        delete[] filtered;
        filtered = nullptr;
    }
}
