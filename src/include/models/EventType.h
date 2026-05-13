#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include <string>

// Enum representing different types of security events
enum EventType
{
    LOGIN,
    LOGOUT,
    TOKEN_REFRESH,
    ACCESS,
    FAILED_LOGIN,
    OPEN_APP,
    DOWNLOAD,
    ADMIN_ACTION,
    UNKNOWN_EVENT
};

// Convert string to EventType enum
EventType parseEventType(const std::string &s);

// Convert EventType enum to string
std::string eventTypeToString(EventType type);

#endif // EVENT_TYPE_H