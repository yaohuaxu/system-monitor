#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <iostream>
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();

    return 0.0f; 
}