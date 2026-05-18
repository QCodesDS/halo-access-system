#ifndef ANOMALY_RECORD_H
#define ANOMALY_RECORD_H

#include <string>

enum class AnomalyType
{
    MULTI_DEVICE_LOGIN, // A01
    CONSECUTIVE_FAILED_LOGIN, // A02
    RESOURCE_FLOOD, // A03
    OFF_HOURS_ACCESS, // A04
    IMPOSSIBLE_TRAVEL, // A05
    LOCATION_CHURNING, // A06
    LONG_SESSION, // A07
    SESSION_FLOOD, // A08
    DANGEROUS_SEQUENCE, // A09
    BRUTE_FORCE, // A10
    DORMANT_ACTIVATION // A11
};

struct AnomalyRecord
{
    AnomalyType type;
    std::string userId;
    long long timestamp;
    std::string detail;
};

// Dynamic array for AnomalyRecord pointers
struct AnomalyList
{
    AnomalyRecord **records;
    int count;
    int capacity;
};

void initAnomalyList(AnomalyList &list, int capacity = 100);
void pushAnomaly(AnomalyList &list, AnomalyRecord *record);
void clearAnomalyList(AnomalyList &list);

#endif // ANOMALY_RECORD_H
