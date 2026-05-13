#pragma once

#include <string>

// Print informational message with [INFO] prefix
void printInfo(const std::string &msg);

// Print error message with [ERROR] prefix to stderr
void printError(const std::string &msg);

// Print warning message with [WARNING] prefix to stderr
void printWarning(const std::string &msg);
