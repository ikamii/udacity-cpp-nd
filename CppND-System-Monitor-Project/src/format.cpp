#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    
    // Calculate the hours, minutes and remaining seconds
    long hours, mins, secs;
    hours = seconds / 3600;
    mins = (seconds - hours * 3600) / 60;
    secs = (seconds - hours * 3600) % 60;

    string HH, MM, SS;
    // Convert the values to string
    HH = hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours);
    MM = mins < 10 ? "0" + std::to_string(mins) : std::to_string(mins);
    SS = secs < 10 ? "0" + std::to_string(secs) : std::to_string(secs);

    return HH + ":" + MM + ":" + SS;
}