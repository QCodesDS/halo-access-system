#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include "models/EventType.h"
#include "models/Location.h"

// Struct representing a single security log record
struct LogRecord
{
    std::string user_id;     // e.g., "U007"
    std::string device_id;   // e.g., "D018"
    std::string app_id;      // e.g., "APP003"
    std::string resource_id; // e.g., "R025"
    EventType event_type;
    Location location;
    long long timestamp; // Unix epoch

    // Default constructor
    LogRecord();

    // Constructor with all fields
    LogRecord(const std::string &uid, const std::string &did, const std::string &aid,
              const std::string &rid, EventType et, Location loc, long long ts);
};

// Print LogRecord for debugging
void printRecord(const LogRecord &r);

#endif // LOG_RECORD_H