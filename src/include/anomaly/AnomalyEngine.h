#ifndef ANOMALY_ENGINE_H
#define ANOMALY_ENGINE_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"
#include "storage/DataStore.h"

// Chạy tất cả các detector và trả về danh sách các anomaly đã được lọc trùng lặp
void runAnomalyDetection(AnomalyList &results, const DataStore &store, const HashIndex &hashIdx);

// In báo cáo anomaly ra màn hình theo format quy định
void printAnomalyReport(const AnomalyList &results);

#endif // ANOMALY_ENGINE_H
