# Phase 04 — Scale Up

# 1M Rows + Memory Audit

**Thời lượng:** 2 ngày
**Prereq:** Phase 03 hoàn thành
**Mục tiêu:** Hệ thống stable, nhanh, không leak trên 1M dòng.

**Status: ✅ HOÀN THÀNH**

---

## Checklist

### Performance

- [x] Test load 1.000.000 dòng — không crash
- [x] Đo: load time < 15s, query time < 5s, total < 10s
- [x] Tối ưu DataStore capacity growth nếu cần (Đã tối ưu Deduplicator $O(n \log n)$)
- [x] Tối ưu hash table size cho 1M records

### Memory Audit

- [x] Rà soát toàn bộ `new` có `delete` tương ứng
- [x] Kiểm tra: sau `load` → `query` → `exit` không leak
- [x] Kiểm tra: load nhiều file liên tiếp không tích lũy leak

### Robustness

- [x] Test với file 1M dòng có 5% dòng invalid
- [x] Test với file 1M dòng có 10% duplicates
- [x] Test query trên tập rỗng sau khi filter

### Manual Test Guide

```bash
1. Sinh file 1M dòng (dùng script Python hoặc tool generate)
2. load data/large.csv → ghi lại thời gian
3. query user U001 0 9999999999 → ghi lại thời gian
4. top resources 0 9999999999 → ghi lại thời gian
5. exit → check không crash
```

---

## Definition of Done

- [x] Load 1M dòng không crash
- [x] Tổng thời gian xử lý < 10s trên 1M dòng
- [x] Valgrind: no leak (hoặc manual verify)
- [x] Update CHANGELOG + master-plan
