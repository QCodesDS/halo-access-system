#include "anomaly/SessionDetector.h"
#include "anomaly/AnomalyConfig.h"

// A07: Long session
// Phiên kéo dài > 8 tiếng (MAX_SESSION_HOURS)
void detectLongSession(AnomalyList &results, const SessionList &sessionList)
{
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        if (session->endTime - session->startTime > (AnomalyConfig::MAX_SESSION_HOURS * 3600))
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::LONG_SESSION;
            rec->userId = session->userId;
            rec->timestamp = session->startTime;
            rec->detail = "Session lasted " + std::to_string((session->endTime - session->startTime) / 3600) + " hours";
            pushAnomaly(results, rec);
        }
    }
}

// A08: Session flood
// User tạo > 5 phiên trong 60 phút
void detectSessionFlood(AnomalyList &results, const SessionList &sessionList)
{
    // Vì SessionBuilder duyệt từng user rồi build sessions, 
    // các session của cùng một user sẽ nằm liên tiếp nhau trong mảng sessionList.
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *currentSession = sessionList.sessions[i];
        int sessionCount = 0;

        // Nhìn lại các session TRƯỚC ĐÓ của CÙNG user trong vòng 60 phút
        for (int j = i; j >= 0; j--)
        {
            UserSession *pastSession = sessionList.sessions[j];
            if (pastSession->userId != currentSession->userId)
            {
                break; // Hết session của user này
            }

            if (currentSession->startTime - pastSession->startTime > 3600)
            {
                break; // Quá 60 phút
            }

            sessionCount++;
        }

        if (sessionCount > AnomalyConfig::MAX_SESSIONS_PER_HOUR)
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::SESSION_FLOOD;
            rec->userId = currentSession->userId;
            rec->timestamp = currentSession->startTime;
            rec->detail = std::to_string(sessionCount) + " sessions started in 60 min";
            pushAnomaly(results, rec);
        }
    }
}

// A09: Dangerous sequence
// Trong 1 phiên: ADMIN_ACTION -> DOWNLOAD (trong vòng 10 phút)
void detectDangerousSequence(AnomalyList &results, const SessionList &sessionList)
{
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        
        for (int j = 0; j < session->eventCount; j++)
        {
            LogRecord *event = session->events[j];
            
            if (event->event_type == EventType::ADMIN_ACTION)
            {
                // Tìm kiếm tiếp DOWNLOAD trong vòng 10 phút sau đó
                for (int k = j + 1; k < session->eventCount; k++)
                {
                    LogRecord *futureEvent = session->events[k];
                    
                    if (futureEvent->timestamp - event->timestamp > (AnomalyConfig::DANGEROUS_SEQ_MINS * 60))
                    {
                        break; // Quá 10 phút, không tính
                    }
                    
                    if (futureEvent->event_type == EventType::DOWNLOAD)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::DANGEROUS_SEQUENCE;
                        rec->userId = session->userId;
                        rec->timestamp = futureEvent->timestamp;
                        rec->detail = "ADMIN_ACTION -> DOWNLOAD in " + std::to_string((futureEvent->timestamp - event->timestamp) / 60) + " min";
                        pushAnomaly(results, rec);
                        break; // Phát hiện 1 lần cho cụm này là đủ
                    }
                }
            }
        }
    }
}
