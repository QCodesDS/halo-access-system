/**
 * @file        Deduplicator.cpp
 * @brief       Implementation của module loại bỏ bản ghi trùng lặp.
 * @author      QCodesDS
 * @date        2026-05-13
 */

#include "storage/Deduplicator.h"
#include "storage/DataStore.h"
#include "utils/StringUtils.h"

// ================================================================================
//  Helper functions
// ================================================================================

bool recordsEqual(const LogRecord &r1, const LogRecord &r2)
{
    return strEquals(r1.user_id, r2.user_id) &&
           strEquals(r1.device_id, r2.device_id) &&
           strEquals(r1.app_id, r2.app_id) &&
           strEquals(r1.resource_id, r2.resource_id) &&
           r1.event_type == r2.event_type &&
           r1.location == r2.location &&
           r1.timestamp == r2.timestamp;
}

// ================================================================================
//  Main function
// ================================================================================

int deduplicate(DataStore &store)
{
    if (store.count <= 1)
        return 0;

    int duplicatesRemoved = 0;

    // Tạo DataStore tạm để chứa các bản ghi unique
    DataStore tempStore;
    initDataStore(tempStore, store.capacity);

    // Thêm bản ghi đầu tiên
    LogRecord *firstCopy = new LogRecord(*store.records[0]);
    push(tempStore, firstCopy);

    // Kiểm tra từng bản ghi còn lại
    for (int i = 1; i < store.count; ++i)
    {
        bool isDuplicate = false;

        // So sánh với tất cả các bản ghi đã có trong tempStore
        for (int j = 0; j < tempStore.count; ++j)
        {
            if (recordsEqual(*store.records[i], *tempStore.records[j]))
            {
                isDuplicate = true;
                ++duplicatesRemoved;
                break;
            }
        }

        // Nếu chưa tồn tại thì thêm vào
        if (!isDuplicate)
        {
            LogRecord *copy = new LogRecord(*store.records[i]);
            push(tempStore, copy);
        }
    }

    // Giải phóng bộ nhớ của store cũ
    for (int i = 0; i < store.count; ++i)
    {
        delete store.records[i];
    }
    delete[] store.records;

    // Thay thế bằng dữ liệu đã deduplicate
    store.records = tempStore.records;
    store.count = tempStore.count;
    store.capacity = tempStore.capacity;

    return duplicatesRemoved;
}