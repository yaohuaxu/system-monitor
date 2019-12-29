#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <iostream>
#include <vector>

using std::stof;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();
    // std::cout << "cpu_utilization[kUser_]" << cpu_utilization[LinuxParser::kUser_] << std::endl;
    // std::cout << "cpu_utilization[kNice_]" << cpu_utilization[LinuxParser::kNice_] << std::endl;
    // std::cout << "cpu_utilization[kSystem_]" << cpu_utilization[LinuxParser::kSystem_] << std::endl;
    // std::cout << "cpu_utilization[kIdle_]" << cpu_utilization[LinuxParser::kIdle_] << std::endl;
    static std::vector<std::string> cpu_utilization_last = cpu_utilization;

    const auto idles_waits = cpu_utilization[LinuxParser::kIdle_] + cpu_utilization[LinuxParser::kIOwait_];
    const auto prev_idles_waits = cpu_utilization_last[LinuxParser::kIdle_] + cpu_utilization_last[LinuxParser::kIOwait_];

    const auto nonidle = cpu_utilization[LinuxParser::kUser_] + cpu_utilization[LinuxParser::kNice_] + cpu_utilization[LinuxParser::kSystem_] + cpu_utilization[LinuxParser::kIRQ_] + cpu_utilization[LinuxParser::kSoftIRQ_] + cpu_utilization[LinuxParser::kSteal_];
    const auto prev_nonidle = cpu_utilization_last[LinuxParser::kUser_] + cpu_utilization_last[LinuxParser::kNice_] + cpu_utilization_last[LinuxParser::kSystem_] + cpu_utilization_last[LinuxParser::kIRQ_] + cpu_utilization_last[LinuxParser::kSoftIRQ_] + cpu_utilization_last[LinuxParser::kSteal_];

    const auto total = idles_waits + nonidle;
    const auto prev_total = prev_idles_waits + prev_nonidle;

    const auto totald = total + prev_total;
    const auto idled = idles_waits + prev_idles_waits;

    return (stof(totald) - stof(idled)) / stof(totald);
}