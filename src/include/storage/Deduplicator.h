#pragma once

struct DataStore;

// Remove duplicate records from DataStore
// Returns number of records removed
int deduplicate(DataStore &store);

// Compare two LogRecords for equality (all 7 fields)
bool recordsEqual(const struct LogRecord &r1, const struct LogRecord &r2);
