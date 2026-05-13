#pragma once

#include "models/LogRecord.h"

// Validate a LogRecord according to BRD requirements
bool isValid(const LogRecord &r);

// Check if a timestamp is valid (> 0)
bool isValidTimestamp(long long ts);

// Check if a string field is non-empty
bool isValidString(const std::string &s);
