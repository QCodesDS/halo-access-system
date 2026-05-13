#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <string>

// Convert Unix epoch seconds to UTC readable format "YYYY-MM-DD HH:MM:SS"
std::string epochToReadable(long long epoch);

// Check if epoch time is outside work hours (08:00-18:00 UTC)
// Returns true if time is off-hours (before 08:00 or at/after 18:00)
bool isOffHours(long long epoch);

// Get hour (0-23) from epoch time in UTC
int getHourUTC(long long epoch);

#endif // TIME_UTILS_H