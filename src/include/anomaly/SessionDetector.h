#ifndef SESSION_DETECTOR_H
#define SESSION_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "session/SessionBuilder.h"

// A07: Long session
void detectLongSession(AnomalyList &results, const SessionList &sessionList);

// A08: Session flood
void detectSessionFlood(AnomalyList &results, const SessionList &sessionList);

// A09: Dangerous sequence (ADMIN_ACTION followed by DOWNLOAD)
void detectDangerousSequence(AnomalyList &results, const SessionList &sessionList);

#endif // SESSION_DETECTOR_H
