#include "utils/TimeUtils.h"
#include <ctime>
#include <cstdio>

std::string epochToReadable(long long epoch)
{
    time_t t = (time_t)epoch;
    struct tm *utcTime = gmtime(&t);

    char buffer[64]; // Sufficient space for datetime format
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             utcTime->tm_year + 1900,
             utcTime->tm_mon + 1,
             utcTime->tm_mday,
             utcTime->tm_hour,
             utcTime->tm_min,
             utcTime->tm_sec);

    return std::string(buffer);
}

int getHourUTC(long long epoch)
{
    time_t t = (time_t)epoch;
    struct tm *utcTime = gmtime(&t);
    return utcTime->tm_hour;
}

bool isOffHours(long long epoch)
{
    int hour = getHourUTC(epoch);
    // Off-hours if before 08:00 (hour < 8) or at/after 18:00 (hour >= 18)
    return (hour < 8 || hour >= 18);
}
