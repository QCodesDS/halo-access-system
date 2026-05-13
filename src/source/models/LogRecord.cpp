#include "models/LogRecord.h"
#include <iostream>

// Default constructor
LogRecord::LogRecord()
    : user_id(""), device_id(""), app_id(""), resource_id(""),
      event_type(UNKNOWN_EVENT), location(UNKNOWN_LOCATION), timestamp(0)
{
}

// Constructor with all fields
LogRecord::LogRecord(const std::string &uid, const std::string &did, const std::string &aid,
                     const std::string &rid, EventType et, Location loc, long long ts)
    : user_id(uid), device_id(did), app_id(aid), resource_id(rid),
      event_type(et), location(loc), timestamp(ts)
{
}

// Print LogRecord for debugging
void printRecord(const LogRecord &r)
{
  std::cout << "[" << r.timestamp << "] "
            << r.user_id << " | "
            << r.device_id << " | "
            << r.app_id << " | "
            << r.resource_id << " | "
            << eventTypeToString(r.event_type) << " | "
            << locationToString(r.location)
            << "\n";
}
