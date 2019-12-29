#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE(ok): An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }

  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        // std::cout << "pid: " << pid << std::endl;
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO(ok): Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memory_total;
  float memory_free;
  string key;
  float value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memory_total = value;
        } else if (key == "MemFree") {
          memory_free = value;
          return (float)(memory_total - memory_free) / memory_total;
        }
      }
    }
  }

  return 0.0f;
}

// TODO(ok): Read and return the system uptime
long LinuxParser::UpTime() {
  long up_time = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
    // std::cout << "up_time: " << up_time << std::endl;
  }

  return up_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<long> LinuxParser::ActiveJiffies(int pid) { 
    string line;
    string value;
    vector<long> time;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);\
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        int i = 1;
        while(linestream >> value) {
            if ((i >=14 && i <= 17) || (22 == i)) {
                time.emplace_back(std::stol(value));
            }
            i++;
        }

        return time;
    }
    return {};
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key;
        if ("cpu" == key) {
            while(linestream >> key) {
                cpu_utilization.emplace_back(key);
                // std::cout << "cpu_utilization.back(): " << cpu_utilization.back() << std::endl;
            }
        }
      }
  }

  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if ("processes" == key) {
                return value;
            }
        }
    }

    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line;
    string key;
    int value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if ("procs_running" == key) {
                return value;
            }
        }
    }

    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    // string command;
    string value;
    string line;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) {
        std::getline(stream, line);

        return line;
        // std::replace(line.begin(), line.end(), '\0', ' ');
        // std::istringstream linestream(line);
        // while (linestream >> value) {
        //     command += value;
        // }
        // return command;
    }

    return "Invalid";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string key;
    int value;
    string line;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while(linestream >> key >> value) {
                // std::cout << value << std::endl;
                if ("VmSize:" == key) {
                    // std::cout << "vmsize: " << value << std::endl;
                    return std::to_string(value / 1024); //MB
                }
            }
        }
    }

    return "inf";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string key;
    string value;
    string line;

    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if ("Uid:" == key) {
                    return value;
                     // return std::stoi(value);
                }
            }
        }
    }

    return "inf";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string user;
    string value;
    string line;
    string skip;

    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            // std::replace(line.begin(), line.end(), 'x', ' ');
            std::istringstream linestream(line);
            while (linestream >> user >> skip >> value) {
                if (Uid(pid) == value) {
                    return user;
                }
            }
        }
    }

    return "Unname";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
    string value;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        for (int i = 0; i <= 21; i++) {
            linestream >> value;
        }
        std::cout << "---UpTime: " << std::stol(value) / sysconf(_SC_CLK_TCK) << ", value: " << value;
        return std::stol(value) / sysconf(_SC_CLK_TCK);
    }
    return 0;
}