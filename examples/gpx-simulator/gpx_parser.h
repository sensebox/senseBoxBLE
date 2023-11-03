
#ifndef GPX_PARSER_H
#define GPX_PARSER_H

#include <cstddef>

// Function prototypes
bool parseWaypoint(const char* gpxData, size_t& pos, double& latitude, double& longitude, double& elevation, char* timestamp);

#endif // GPX_PARSER_H