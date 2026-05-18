#include "anomaly/ThresholdDetector.h"
#include "anomaly/AnomalyConfig.h"
#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"

// Helper to check if a string exists in array
static bool existsInArray(const std::string *arr, int count, const std::string &val)
{
    for (int i = 0; i < count; i++)
    {
        if (strEquals(arr[i], val))
            return true;
    }
    return false;
}

// A01: Multi-device login
void detectMultiDeviceLogin(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                // Keep track of unique devices in the 60-min window ending at currentEvent
                std::string uniqueDevices[AnomalyConfig::MAX_DEVICES_PER_HOUR + 1];
                int uniqueCount = 0;

                for (int j = i; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 3600)
                    {
                        break;
                    }

                    if (!existsInArray(uniqueDevices, uniqueCount, pastEvent->device_id))
                    {
                        if (uniqueCount < AnomalyConfig::MAX_DEVICES_PER_HOUR + 1)
                        {
                            uniqueDevices[uniqueCount++] = pastEvent->device_id;
                        }
                    }
                }

                if (uniqueCount > AnomalyConfig::MAX_DEVICES_PER_HOUR)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::MULTI_DEVICE_LOGIN;
                    rec->userId = node->key;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "User used > " + std::to_string(AnomalyConfig::MAX_DEVICES_PER_HOUR) + " devices in 60 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}

// A02: Consecutive failed login
void detectConsecutiveFailedLogin(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            int failedStreak = 0;
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *event = node->values[i];
                if (event->event_type == EventType::FAILED_LOGIN)
                {
                    failedStreak++;
                    if (failedStreak > AnomalyConfig::MAX_FAILED_LOGIN_STREAK)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::CONSECUTIVE_FAILED_LOGIN;
                        rec->userId = node->key;
                        rec->timestamp = event->timestamp;
                        rec->detail = "Consecutive failed logins > " + std::to_string(AnomalyConfig::MAX_FAILED_LOGIN_STREAK);
                        pushAnomaly(results, rec);
                        // Prevent flooding by resetting or continuing, wait, dedup will handle it later.
                    }
                }
                else
                {
                    // Any other event breaks the failed login streak
                    failedStreak = 0;
                }
            }
            node = node->next;
        }
    }
}

// A03: Resource flood per device
void detectResourceFlood(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byDevice.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byDevice.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                std::string uniqueResources[AnomalyConfig::MAX_RESOURCES_PER_30MIN + 1];
                int uniqueCount = 0;

                for (int j = i; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 1800) // 30 mins
                    {
                        break;
                    }

                    if (!existsInArray(uniqueResources, uniqueCount, pastEvent->resource_id))
                    {
                        if (uniqueCount < AnomalyConfig::MAX_RESOURCES_PER_30MIN + 1)
                        {
                            uniqueResources[uniqueCount++] = pastEvent->resource_id;
                        }
                    }
                }

                if (uniqueCount > AnomalyConfig::MAX_RESOURCES_PER_30MIN)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::RESOURCE_FLOOD;
                    rec->userId = currentEvent->user_id; // Tag the user who caused the flood
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "Device " + node->key + " accessed > " + std::to_string(AnomalyConfig::MAX_RESOURCES_PER_30MIN) + " unique resources in 30 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}

// A04: Off-hours access
void detectOffHoursAccess(AnomalyList &results, const DataStore &store)
{
    for (int i = 0; i < store.count; i++)
    {
        LogRecord *event = store.records[i];
        if (isOffHours(event->timestamp))
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::OFF_HOURS_ACCESS;
            rec->userId = event->user_id;
            rec->timestamp = event->timestamp;
            rec->detail = "Access during off-hours";
            pushAnomaly(results, rec);
        }
    }
}
