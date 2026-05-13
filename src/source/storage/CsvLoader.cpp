#include "storage/CsvLoader.h"
#include "storage/DataStore.h"
#include "storage/Validator.h"
#include "utils/StringUtils.h"
#include "models/EventType.h"
#include "models/Location.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

LoadResult load(const std::string &filepath, DataStore &store)
{
    LoadResult result = {0, 0};

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] Cannot open file: " << filepath << std::endl;
        return result;
    }

    std::string line;
    int lineNum = 0;

    // Skip header
    if (!std::getline(file, line))
    {
        std::cerr << "[ERROR] Empty file: " << filepath << std::endl;
        file.close();
        return result;
    }
    lineNum++;

    // Read data lines
    while (std::getline(file, line))
    {
        lineNum++;

        // Trim the line
        line = trim(line);

        // Skip empty lines
        if (line.empty())
        {
            result.skipped++;
            continue;
        }

        // Split by comma
        int tokenCount = 0;
        std::string *tokens = split(line, ',', tokenCount);

        // Check token count
        if (tokenCount != 7)
        {
            std::cerr << "[WARNING] Line " << lineNum << ": Expected 7 fields, got " << tokenCount << std::endl;
            result.skipped++;
            if (tokens != nullptr)
            {
                delete[] tokens;
            }
            continue;
        }

        // Trim and parse tokens
        std::string user_id = trim(tokens[0]);
        std::string device_id = trim(tokens[1]);
        std::string app_id = trim(tokens[2]);
        std::string resource_id = trim(tokens[3]);
        std::string event_type_str = trim(tokens[4]);
        std::string location_str = trim(tokens[5]);
        std::string timestamp_str = trim(tokens[6]);

        // Parse timestamp
        long long timestamp = 0;
        try
        {
            timestamp = std::stoll(timestamp_str);
        }
        catch (...)
        {
            std::cerr << "[WARNING] Line " << lineNum << ": Invalid timestamp: " << timestamp_str << std::endl;
            result.skipped++;
            delete[] tokens;
            continue;
        }

        // Create LogRecord
        LogRecord *record = new LogRecord(
            user_id,
            device_id,
            app_id,
            resource_id,
            parseEventType(event_type_str),
            parseLocation(location_str),
            timestamp);

        // Validate
        if (!isValid(*record))
        {
            result.skipped++;
            delete record;
            delete[] tokens;
            continue;
        }

        // Add to store
        push(store, record);
        result.loaded++;

        delete[] tokens;
    }

    file.close();
    return result;
}
