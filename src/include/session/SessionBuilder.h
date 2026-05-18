#ifndef SESSION_BUILDER_H
#define SESSION_BUILDER_H

#include <string>
#include "models/LogRecord.h"
#include "indexing/HashIndex.h"

// Repesents a single continuous session of a user
struct UserSession
{
    std::string userId;
    long long startTime;
    long long endTime;
    
    LogRecord **events; // Array of event pointers in this session
    int eventCount;
    int eventCapacity;
};

// Represents a list of all sessions across all users
struct SessionList
{
    UserSession **sessions; // Array of pointers to UserSession
    int count;
    int capacity;
};

// Builds all sessions from the existing HashIndex
// It extracts events per user, sorts them, and splits them into sessions
void buildSessions(SessionList &sessionList, const HashIndex &hashIdx);

// Frees all memory allocated for sessions
void clearSessions(SessionList &sessionList);

#endif // SESSION_BUILDER_H
