#include "anomaly/AnomalyEngine.h"
#include "anomaly/ThresholdDetector.h"
#include "anomaly/BehaviorDetector.h"
#include "anomaly/SessionDetector.h"
#include "anomaly/AdvancedDetector.h"
#include "session/SessionBuilder.h"
#include "utils/TimeUtils.h"
#include <iostream>
#include <iomanip>

static std::string getAnomalyTypeName(AnomalyType type)
{
    switch (type)
    {
    case AnomalyType::MULTI_DEVICE_LOGIN: return "A01_MULTI_DEVICE";
    case AnomalyType::CONSECUTIVE_FAILED_LOGIN: return "A02_FAILED_LOGIN";
    case AnomalyType::RESOURCE_FLOOD: return "A03_RESOURCE_FLOOD";
    case AnomalyType::OFF_HOURS_ACCESS: return "A04_OFF_HOURS";
    case AnomalyType::IMPOSSIBLE_TRAVEL: return "A05_IMPOSSIBLE_TRAVEL";
    case AnomalyType::LOCATION_CHURNING: return "A06_LOCATION_CHURNING";
    case AnomalyType::LONG_SESSION: return "A07_LONG_SESSION";
    case AnomalyType::SESSION_FLOOD: return "A08_SESSION_FLOOD";
    case AnomalyType::DANGEROUS_SEQUENCE: return "A09_DANGEROUS_SEQUENCE";
    case AnomalyType::BRUTE_FORCE: return "A10_BRUTE_FORCE";
    case AnomalyType::DORMANT_ACTIVATION: return "A11_DORMANT_ACTIVATION";
    default: return "UNKNOWN";
    }
}

// Merge Sort for AnomalyRecords by timestamp
static void mergeAnomalies(AnomalyRecord **arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    AnomalyRecord **L = new AnomalyRecord *[n1];
    AnomalyRecord **R = new AnomalyRecord *[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i]->timestamp <= R[j]->timestamp)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

static void sortAnomalies(AnomalyRecord **arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        sortAnomalies(arr, left, mid);
        sortAnomalies(arr, mid + 1, right);
        mergeAnomalies(arr, left, mid, right);
    }
}

void runAnomalyDetection(AnomalyList &results, const DataStore &store, const HashIndex &hashIdx)
{
    AnomalyList rawResults;
    initAnomalyList(rawResults);

    std::cout << "[DEBUG] Running basic detectors..." << std::endl;
    // 1. Chạy các detector cơ bản
    detectMultiDeviceLogin(rawResults, hashIdx);
    detectConsecutiveFailedLogin(rawResults, hashIdx);
    detectResourceFlood(rawResults, hashIdx);
    detectOffHoursAccess(rawResults, store);
    detectImpossibleTravel(rawResults, hashIdx);
    detectLocationChurning(rawResults, hashIdx);
    
    std::cout << "[DEBUG] Building sessions..." << std::endl;
    // 2. Build sessions và chạy các detector dựa trên session
    SessionList sessions;
    buildSessions(sessions, hashIdx);
    
    std::cout << "[DEBUG] Running session detectors..." << std::endl;
    detectLongSession(rawResults, sessions);
    detectSessionFlood(rawResults, sessions);
    detectDangerousSequence(rawResults, sessions);
    
    clearSessions(sessions);

    std::cout << "[DEBUG] Running advanced detectors..." << std::endl;
    // 3. Chạy các detector nâng cao
    detectBruteForce(rawResults, hashIdx);
    detectDormantActivation(rawResults, hashIdx);

    std::cout << "[DEBUG] Sorting " << rawResults.count << " anomalies..." << std::endl;
    // 4. Sắp xếp rawResults theo thời gian
    if (rawResults.count > 1)
    {
        sortAnomalies(rawResults.records, 0, rawResults.count - 1);
    }

    std::cout << "[DEBUG] Deduping anomalies..." << std::endl;
    // 5. Loại bỏ trùng lặp (Dedup)
    // Nếu cùng user + cùng type + cách nhau < 1 giờ (3600s) -> bỏ qua
    for (int i = 0; i < rawResults.count; i++)
    {
        AnomalyRecord *curr = rawResults.records[i];
        bool isDuplicate = false;

        // Chỉ cần check lùi lại vài record trong kết quả đã lọc
        for (int j = results.count - 1; j >= 0; j--)
        {
            AnomalyRecord *prev = results.records[j];
            if (curr->timestamp - prev->timestamp > 3600)
                break; // Quá 1 giờ, không thể trùng

            if (curr->userId == prev->userId && curr->type == prev->type)
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
        {
            AnomalyRecord *copy = new AnomalyRecord(*curr);
            pushAnomaly(results, copy);
        }
    }

    clearAnomalyList(rawResults);
}

void printAnomalyReport(const AnomalyList &results)
{
    std::cout << "\n===============================================================================\n";
    std::cout << "                         ANOMALY DETECTION REPORT\n";
    std::cout << "===============================================================================\n";
    std::cout << "Total anomalies found: " << results.count << "\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    
    if (results.count == 0)
    {
        std::cout << "No anomalies detected. System is safe.\n";
    }
    else
    {
        std::cout << std::left 
                  << std::setw(20) << "TIMESTAMP" 
                  << std::setw(12) << "USER ID" 
                  << std::setw(25) << "ANOMALY TYPE" 
                  << "DETAILS\n";
        std::cout << "-------------------------------------------------------------------------------\n";
        
        for (int i = 0; i < results.count; i++)
        {
            AnomalyRecord *rec = results.records[i];
            std::cout << std::left 
                      << std::setw(20) << epochToReadable(rec->timestamp)
                      << std::setw(12) << rec->userId
                      << std::setw(25) << getAnomalyTypeName(rec->type)
                      << rec->detail << "\n";
        }
    }
    std::cout << "===============================================================================\n";
}
