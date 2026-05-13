#include "utils/StringUtils.h"

std::string *split(const std::string &s, char delim, int &outCount)
{
    // First pass: count delimiters to determine number of tokens
    int count = 1; // At least one token if string is not empty
    if (s.empty())
    {
        count = 0;
    }
    else
    {
        for (size_t i = 0; i < s.length(); ++i)
        {
            if (s[i] == delim)
            {
                count++;
            }
        }
    }

    outCount = count;
    if (count == 0)
    {
        return nullptr;
    }

    // Allocate array for tokens
    std::string *tokens = new std::string[count];

    // Second pass: extract tokens
    int tokenIndex = 0;
    size_t start = 0;

    for (size_t i = 0; i <= s.length(); ++i)
    {
        if (i == s.length() || s[i] == delim)
        {
            tokens[tokenIndex] = s.substr(start, i - start);
            tokenIndex++;
            start = i + 1;
        }
    }

    return tokens;
}

std::string trim(const std::string &s)
{
    size_t start = 0;
    size_t end = s.length();

    // Find first non-whitespace character
    while (start < s.length() && (s[start] == ' ' || s[start] == '\t'))
    {
        start++;
    }

    // Find last non-whitespace character
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
    {
        end--;
    }

    return s.substr(start, end - start);
}

bool strEquals(const std::string &a, const std::string &b)
{
    if (a.length() != b.length())
    {
        return false;
    }
    for (size_t i = 0; i < a.length(); ++i)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}
