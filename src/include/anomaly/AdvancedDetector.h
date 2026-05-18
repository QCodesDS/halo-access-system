#ifndef ADVANCED_DETECTOR_H
#define ADVANCED_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"

// A10: Brute force detection
void detectBruteForce(AnomalyList &results, const HashIndex &hashIdx);

// A11: Dormant user activation
void detectDormantActivation(AnomalyList &results, const HashIndex &hashIdx);

#endif // ADVANCED_DETECTOR_H
