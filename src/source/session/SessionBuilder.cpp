#include "session/SessionBuilder.h"
#include "anomaly/AnomalyConfig.h"

// Helper to push a session to the session list
static void pushSession(SessionList &list, UserSession *session)
{
    if (list.count == list.capacity)
    {
        int newCapacity = list.capacity == 0 ? 1000 : list.capacity * 2;
        UserSession **newSessions = new UserSession *[newCapacity];
        for (int i = 0; i < list.count; i++)
        {
            newSessions[i] = list.sessions[i];
        }
        if (list.sessions != nullptr)
        {
            delete[] list.sessions;
        }
        list.sessions = newSessions;
        list.capacity = newCapacity;
    }
    list.sessions[list.count++] = session;
}

void buildSessions(SessionList &sessionList, const HashIndex &hashIdx)
{
    sessionList.count = 0;
    sessionList.capacity = 1000;
    sessionList.sessions = new UserSession *[sessionList.capacity];

    // Traverse all buckets in byUser hash table
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            int count = node->count;
            if (count == 0)
            {
                node = node->next;
                continue;
            }

            // Events are already sorted by timestamp (thanks to Deduplicator)
            UserSession *currentSession = nullptr;

            for (int i = 0; i < count; i++)
            {
                LogRecord *event = node->values[i];
                bool shouldStartNew = false;

                if (currentSession == nullptr)
                {
                    shouldStartNew = true;
                }
                else if (event->event_type == EventType::LOGIN)
                {
                    shouldStartNew = true;
                }
                else
                {
                    // Check if gap > 30 mins
                    long long lastEventTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                    if (event->timestamp - lastEventTime > (AnomalyConfig::SESSION_TIMEOUT_MINS * 60))
                    {
                        shouldStartNew = true;
                    }
                }

                if (shouldStartNew)
                {
                    // Finalize and save the old session
                    if (currentSession != nullptr)
                    {
                        currentSession->endTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                        pushSession(sessionList, currentSession);
                    }

                    // Start a new session
                    currentSession = new UserSession();
                    currentSession->userId = node->key;
                    currentSession->startTime = event->timestamp;
                    currentSession->eventCapacity = 10;
                    currentSession->eventCount = 0;
                    currentSession->events = new LogRecord *[currentSession->eventCapacity];
                }

                // Add event to current session
                if (currentSession->eventCount == currentSession->eventCapacity)
                {
                    int newCap = currentSession->eventCapacity * 2;
                    LogRecord **newArr = new LogRecord *[newCap];
                    for (int j = 0; j < currentSession->eventCount; ++j)
                    {
                        newArr[j] = currentSession->events[j];
                    }
                    delete[] currentSession->events;
                    currentSession->events = newArr;
                    currentSession->eventCapacity = newCap;
                }
                currentSession->events[currentSession->eventCount++] = event;

                // Condition to close session immediately
                if (event->event_type == EventType::LOGOUT)
                {
                    currentSession->endTime = event->timestamp;
                    pushSession(sessionList, currentSession);
                    currentSession = nullptr;
                }
            }

            // Push the last open session for this user
            if (currentSession != nullptr)
            {
                currentSession->endTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                pushSession(sessionList, currentSession);
            }

            node = node->next;
        }
    }
}

void clearSessions(SessionList &sessionList)
{
    if (sessionList.sessions == nullptr)
        return;

    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        if (session != nullptr)
        {
            if (session->events != nullptr)
            {
                delete[] session->events;
            }
            delete session;
        }
    }

    delete[] sessionList.sessions;
    sessionList.sessions = nullptr;
    sessionList.count = 0;
    sessionList.capacity = 0;
}
