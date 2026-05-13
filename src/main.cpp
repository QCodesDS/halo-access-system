#include <iostream>
#include <cstring>
#include <sstream>
#include "models/EventType.h"
#include "models/Location.h"
#include "models/LogRecord.h"
#include "utils/StringUtils.h"
#include "utils/TimeUtils.h"
#include "storage/DataStore.h"
#include "storage/CsvLoader.h"
#include "storage/Validator.h"
#include "storage/Deduplicator.h"
#include "indexing/IndexManager.h"
#include "cli/Printer.h"
#include "query/QueryEngine.h"

using namespace std;

int main()
{
    cout << "=== Halo Cyber Access Engine ===" << endl;
    cout << "Type 'help' for available commands" << endl;
    cout << endl;

    DataStore store;
    initDataStore(store);

    IndexManager indexMgr;
    indexMgr.isBuilt = false;

    string line;
    while (true)
    {
        cout << "> ";
        if (!getline(cin, line))
        {
            break;
        }

        // Trim line
        line = trim(line);

        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        // Split command and arguments
        int argc = 0;
        string *argv = split(line, ' ', argc);

        if (argc == 0)
        {
            if (argv != nullptr)
            {
                delete[] argv;
            }
            continue;
        }

        string command = argv[0];

        // Load command
        if (strEquals(command, "load") && argc >= 2)
        {
            string filepath = argv[1];
            LoadResult loadResult = load(filepath, store);

            int numDuplicates = deduplicate(store);

            stringstream ss;
            ss << "Loaded " << loadResult.loaded << " records ("
               << loadResult.skipped << " skipped, "
               << numDuplicates << " duplicates removed)";
            printInfo(ss.str());

            // Build indexes
            if (store.count > 0)
            {
                buildAllIndexes(indexMgr, store);
            }
        }
        // Query commands
        else if (strEquals(command, "query") || strEquals(command, "top"))
        {
            // Reconstruct query string from remaining tokens
            string queryStr = line;

            if (!indexMgr.isBuilt)
            {
                printError("No data loaded. Use 'load <filepath>' first.");
            }
            else
            {
                executeQuery(queryStr, indexMgr.hashIdx, indexMgr.sortedIdx);
            }
        }
        // Help command
        else if (strEquals(command, "help"))
        {
            cout << "Available commands:" << endl;
            cout << "  load <filepath>                          - Load CSV file" << endl;
            cout << "  query user <uid> <t_start> <t_end>       - User journey query" << endl;
            cout << "  query resource <rid> <t_start> <t_end>   - Resource journey query" << endl;
            cout << "  top resources <t_start> <t_end>          - Top resources by count" << endl;
            cout << "  help                                     - Show this help message" << endl;
            cout << "  exit                                     - Exit program" << endl;
        }
        // Exit command
        else if (strEquals(command, "exit"))
        {
            printInfo("Memory freed. Goodbye.");
            if (argv != nullptr)
            {
                delete[] argv;
            }
            // Clear indexes
            if (indexMgr.isBuilt)
            {
                clearAllIndexes(indexMgr);
            }
            clear(store);
            break;
        }
        // Unknown command
        else
        {
            printError("Unknown command: " + command);
        }

        if (argv != nullptr)
        {
            delete[] argv;
        }
    }

    // Make sure store is cleaned up
    if (store.count > 0)
    {
        clear(store);
    }

    return 0;
}
