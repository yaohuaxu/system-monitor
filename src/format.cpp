#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {
    string time_string;
    const auto hour = seconds / 3600;
    const auto minute = (seconds % 3600) / 60;
    const auto second = (seconds % 3600) % 60;

    time_string = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);

    return time_string;

}