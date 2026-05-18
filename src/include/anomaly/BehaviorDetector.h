#ifndef BEHAVIOR_DETECTOR_H
#define BEHAVIOR_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"

// A05: Impossible travel
void detectImpossibleTravel(AnomalyList &results, const HashIndex &hashIdx);

// A06: Location churning
void detectLocationChurning(AnomalyList &results, const HashIndex &hashIdx);

#endif // BEHAVIOR_DETECTOR_H
