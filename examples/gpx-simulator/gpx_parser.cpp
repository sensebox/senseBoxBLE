
#include <cstring>
#include <cmath>
#include <algorithm> // Include the <algorithm> header


// Function to parse a single waypoint with elevation
bool parseWaypoint(const char* gpxData, size_t& pos, double& latitude, double& longitude, double& elevation, char* timestamp) {
    // Find the next occurrence of '<trkpt'
    const char* start = strstr(gpxData + pos, "<trkpt");
    if (start == nullptr) {
        return false; // No more waypoints
    }

    // Find the latitude and longitude attributes
    const char* latAttr = strstr(start, "lat=\"");
    const char* lonAttr = strstr(start, "lon=\"");
    if (latAttr == nullptr || lonAttr == nullptr) {
        return false; // Malformed waypoint
    }

    // Extract latitude and longitude values
    latitude = atof(latAttr + 5); // Skip "lat=\""
    longitude = atof(lonAttr + 5); // Skip "lon=\""

    // Find the elevation (altitude) element
    const char* eleStart = strstr(start, "<ele>");
    const char* eleEnd = strstr(start, "</ele>");
    if (eleStart != nullptr && eleEnd != nullptr && eleStart < eleEnd) {
        elevation = atof(eleStart + 5);
    } else {
        elevation = 0.0; // Default to 0 elevation if not found
    }

    // Find the timestamp within the waypoint
    const char* timeStart = strstr(start, "<time>");
    const char* timeEnd = strstr(start, "</time>");
    if (timeStart != nullptr && timeEnd != nullptr && timeStart < timeEnd) {
        size_t len = std::min<size_t>(timeEnd - timeStart - 6, 19); // 19 characters maximum for timestamp
        strncpy(timestamp, timeStart + 6, len);
        timestamp[len] = '\0';
    } else {
        timestamp[0] = '\0'; // No timestamp found
    }

    // Update the position for the next search
    pos = start - gpxData + 1;
    return true;
}