#ifndef THRESHOLD_DETECTOR_H
#define THRESHOLD_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"
#include "storage/DataStore.h"

// A01: Multi-device login
void detectMultiDeviceLogin(AnomalyList &results, const HashIndex &hashIdx);

// A02: Consecutive failed login
void detectConsecutiveFailedLogin(AnomalyList &results, const HashIndex &hashIdx);

// A03: Resource flood per device
void detectResourceFlood(AnomalyList &results, const HashIndex &hashIdx);

// A04: Off-hours access
void detectOffHoursAccess(AnomalyList &results, const DataStore &store);

#endif // THRESHOLD_DETECTOR_H
