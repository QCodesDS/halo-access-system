#include "anomaly/BehaviorDetector.h"
#include "anomaly/AnomalyConfig.h"
#include "models/Location.h"

// A05: Impossible travel
// User appears in 2+ different locations in 120 minutes
void detectImpossibleTravel(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                if (currentEvent->location == Location::UNKNOWN_LOCATION) continue;

                // Check previous events within IMPOSSIBLE_TRAVEL_MINS window
                for (int j = i - 1; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (pastEvent->location == Location::UNKNOWN_LOCATION) continue;
                    
                    if (currentEvent->timestamp - pastEvent->timestamp > (AnomalyConfig::IMPOSSIBLE_TRAVEL_MINS * 60))
                    {
                        break;
                    }

                    if (currentEvent->location != pastEvent->location)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::IMPOSSIBLE_TRAVEL;
                        rec->userId = node->key;
                        rec->timestamp = currentEvent->timestamp;
                        rec->detail = locationToString(pastEvent->location) + " -> " + locationToString(currentEvent->location) + " in " + std::to_string((currentEvent->timestamp - pastEvent->timestamp) / 60) + " min";
                        pushAnomaly(results, rec);
                        break; // Only flag once for this event
                    }
                }
            }
            node = node->next;
        }
    }
}

// A06: Location churning
// User changes location > 3 times in 60 minutes
void detectLocationChurning(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                int locationChanges = 0;
                Location lastLoc = currentEvent->location;

                // Look back 60 minutes and count consecutive changes
                for (int j = i - 1; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 3600)
                    {
                        break;
                    }
                    
                    if (pastEvent->location != Location::UNKNOWN_LOCATION && 
                        pastEvent->location != lastLoc)
                    {
                        locationChanges++;
                        lastLoc = pastEvent->location;
                    }
                }

                if (locationChanges > AnomalyConfig::MAX_LOCATION_CHANGES)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::LOCATION_CHURNING;
                    rec->userId = node->key;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "Location changed " + std::to_string(locationChanges) + " times in 60 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}
