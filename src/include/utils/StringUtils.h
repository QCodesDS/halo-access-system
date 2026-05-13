#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

// Split string by delimiter, returns dynamic array of strings
// outCount is set to the number of tokens
// Caller must delete[] the returned array
std::string *split(const std::string &s, char delim, int &outCount);

// Trim leading and trailing whitespace (space and tab)
std::string trim(const std::string &s);

// Case-sensitive string equality
bool strEquals(const std::string &a, const std::string &b);

#endif // STRING_UTILS_H