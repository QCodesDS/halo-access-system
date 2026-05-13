#include "query/QueryEngine.h"
#include "query/UserJourneyQuery.h"
#include "query/ResourceJourneyQuery.h"
#include "query/TopResourceQuery.h"
#include "utils/StringUtils.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

void executeQuery(const std::string &input, const HashIndex &hashIdx, const SortedIndex &sortedIdx)
{
    // Parse input command
    // Supported formats:
    // "query user <uid> <t_start> <t_end>"
    // "query resource <rid> <t_start> <t_end>"
    // "top resources <t_start> <t_end>"

    std::string trimmed = trim(input);

    // Split by spaces
    char *inputCopy = new char[trimmed.length() + 1];
    strcpy(inputCopy, trimmed.c_str());

    const char *delim = " ";
    char *token = strtok(inputCopy, delim);

    int tokenCount = 0;
    const char *tokens[10] = {nullptr};

    while (token != nullptr && tokenCount < 10)
    {
        tokens[tokenCount++] = token;
        token = strtok(nullptr, delim);
    }

    // Dispatch based on command
    if (tokenCount < 1)
    {
        printf("[ERROR] Invalid query format\n");
        delete[] inputCopy;
        return;
    }

    std::string cmd = tokens[0];

    if (cmd == "query" && tokenCount >= 5)
    {
        std::string subCmd = tokens[1];

        if (subCmd == "user")
        {
            // query user <uid> <t_start> <t_end>
            std::string userId = tokens[2];
            long long timeStart = atoll(tokens[3]);
            long long timeEnd = atoll(tokens[4]);

            UserJourneyQuery q;
            q.userId = userId;
            q.timeStart = timeStart;
            q.timeEnd = timeEnd;

            executeUserJourney(q, hashIdx, sortedIdx);
        }
        else if (subCmd == "resource")
        {
            // query resource <rid> <t_start> <t_end>
            std::string resourceId = tokens[2];
            long long timeStart = atoll(tokens[3]);
            long long timeEnd = atoll(tokens[4]);

            ResourceJourneyQuery q;
            q.resourceId = resourceId;
            q.timeStart = timeStart;
            q.timeEnd = timeEnd;

            executeResourceJourney(q, hashIdx, sortedIdx);
        }
        else
        {
            printf("[ERROR] Unknown query subcommand: %s\n", subCmd.c_str());
        }
    }
    else if (cmd == "top" && tokenCount >= 4)
    {
        if (std::string(tokens[1]) == "resources")
        {
            // top resources <t_start> <t_end>
            long long timeStart = atoll(tokens[2]);
            long long timeEnd = atoll(tokens[3]);

            executeTopResources(timeStart, timeEnd, sortedIdx, 10);
        }
        else
        {
            printf("[ERROR] Unknown top subcommand: %s\n", tokens[1]);
        }
    }
    else
    {
        printf("[ERROR] Invalid query format. Expected:\n");
        printf("  query user <uid> <t_start> <t_end>\n");
        printf("  query resource <rid> <t_start> <t_end>\n");
        printf("  top resources <t_start> <t_end>\n");
    }

    delete[] inputCopy;
}
