#include "cli/Printer.h"
#include <iostream>

void printInfo(const std::string &msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void printError(const std::string &msg)
{
    std::cerr << "[ERROR] " << msg << std::endl;
}

void printWarning(const std::string &msg)
{
    std::cerr << "[WARNING] " << msg << std::endl;
}
