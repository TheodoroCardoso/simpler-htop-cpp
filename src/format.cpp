#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
    string time;
    int hours, mins;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    mins = seconds / 60;
    seconds = seconds % 60;

    if (hours < 10) time = "0";
    time += std::to_string(hours) + ":";
    
    if (mins < 10) time += "0";
    time += std::to_string(mins) + ":";
    
    if (seconds < 10)time += "0";
    time += std::to_string(seconds);

    return time;
}