#ifndef LOCATION_H
#define LOCATION_H

#include <string>

// Enum representing geographic locations
enum Location
{
    US,
    VN,
    JP,
    KR,
    SG,
    CN,
    DE,
    FR,
    UK,
    AU,
    CA,
    IN,
    BR,
    RU,
    TH,
    UNKNOWN_LOCATION
};

// Convert string to Location enum
Location parseLocation(const std::string &s);

// Convert Location enum to string
std::string locationToString(Location loc);

#endif // LOCATION_H