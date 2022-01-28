#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
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
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float mem_total, mem_free;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") mem_total = std::stof(value);
        if (key == "MemFree:") {
          mem_free = std::stof(value);
          break;
        }
      }
    }
  }
  return 100 * (mem_total - mem_free) / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line, value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> value;        
      }
  }
  return stol(value);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  // Acess files to parse the data
  string line, value;
  vector<string> res;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> value;
        while (linestream >> value) res.emplace_back(value);
      }
  }
  return res; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  int line_counter = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line_counter++ > 5) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "processes") return std::stof(value);
        }
      }
    }
  }
  return -1; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int line_counter = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line_counter++ > 6) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "procs_running") return std::stof(value);
        }
      }
    }
  }
  return -1;
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  string pid_dir = "/" + std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pid_dir + kCmdlineFilename);
  if (filestream.is_open()) std::getline(filestream, line);
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string pid_dir = "/" + std::to_string(pid) + "/";
  string line, key, value;
  int line_counter = 0;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line_counter++ > 16) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "VmSize:") return value;
        }
      }
    }
  }
  return "-1";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string pid_dir = "/" + std::to_string(pid) + "/";
  string line, key, value;
  int line_counter = 0;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line_counter++ > 7) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "Uid:") return value;
        }
      }
    }
  }
  return "-1";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, x, id;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> id) {
        if (id == uid) return user;
      }
    }
  }
  return "-1";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string pid_dir = "/" + std::to_string(pid) + "/";
  string line, value;
  int counter = 0;
  std::ifstream filestream(kProcDirectory + pid_dir + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (counter++ > 20) return LinuxParser::UpTime() - std::stol(value);
      }
    }
  }
  return -1;
}
