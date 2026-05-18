#ifndef ANOMALY_CONFIG_H
#define ANOMALY_CONFIG_H

namespace AnomalyConfig
{
    const int MAX_DEVICES_PER_HOUR = 3;
    const int MAX_FAILED_LOGIN_STREAK = 5;
    const int MAX_RESOURCES_PER_30MIN = 20;
    const int WORK_HOUR_START = 8;  // UTC
    const int WORK_HOUR_END = 18;   // UTC
    const int IMPOSSIBLE_TRAVEL_MINS = 120;
    const int MAX_LOCATION_CHANGES = 3;
    const int MAX_SESSION_HOURS = 8;
    const int MAX_SESSIONS_PER_HOUR = 5;
    const int DANGEROUS_SEQ_MINS = 10;
    const int SESSION_TIMEOUT_MINS = 30;
    const int DORMANT_DAYS = 7;
    const int BURST_EVENTS_PER_HOUR = 20;
}

#endif // ANOMALY_CONFIG_H
