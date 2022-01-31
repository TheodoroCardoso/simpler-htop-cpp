#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    int hours, mins;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    mins = seconds / 60;
    seconds = seconds % 60;
    return std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(seconds); 
}