#include "storage/Validator.h"
#include <iostream>

bool isValidString(const std::string &s)
{
    return s.length() > 0;
}

bool isValidTimestamp(long long ts)
{
    return ts > 0;
}

bool isValid(const LogRecord &r)
{
    // Check all string fields are non-empty
    if (!isValidString(r.user_id))
    {
        std::cerr << "[WARNING] Invalid user_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.device_id))
    {
        std::cerr << "[WARNING] Invalid device_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.app_id))
    {
        std::cerr << "[WARNING] Invalid app_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.resource_id))
    {
        std::cerr << "[WARNING] Invalid resource_id (empty)" << std::endl;
        return false;
    }

    // Check event_type is valid (not UNKNOWN_EVENT)
    if (r.event_type == UNKNOWN_EVENT)
    {
        std::cerr << "[WARNING] Invalid event_type (UNKNOWN_EVENT)" << std::endl;
        return false;
    }

    // Check location is valid (not UNKNOWN_LOCATION)
    if (r.location == UNKNOWN_LOCATION)
    {
        std::cerr << "[WARNING] Invalid location (UNKNOWN_LOCATION)" << std::endl;
        return false;
    }

    // Check timestamp is valid (> 0)
    if (!isValidTimestamp(r.timestamp))
    {
        std::cerr << "[WARNING] Invalid timestamp (" << r.timestamp << ")" << std::endl;
        return false;
    }

    return true;
}
