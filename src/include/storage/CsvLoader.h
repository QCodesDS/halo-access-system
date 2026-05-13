#pragma once

#include <string>

// Result of loading CSV file
struct LoadResult
{
    int loaded;  // Number of records successfully loaded
    int skipped; // Number of records skipped
};

struct DataStore;

// Load CSV file into DataStore
LoadResult load(const std::string &filepath, DataStore &store);
